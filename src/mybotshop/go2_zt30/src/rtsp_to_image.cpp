// SPDX-License-Identifier: BSD-3-Clause
// Author: Salman Omar Sohail <support@mybotshop.de>

#include <rclcpp/rclcpp.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>

#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>
#include <map>
#include <sstream>
#include <vector>

using namespace std::chrono_literals;

static std::string colorize(const std::string &text, const std::string &color) {
    const std::map<std::string, std::string> color_codes = {
        {"green","\033[92m"},{"purple","\033[95m"},{"cyan","\033[96m"},
        {"yellow","\033[93m"},{"orange","\033[38;5;208m"},
        {"blue","\033[94m"},{"red","\033[91m"}};
    auto it = color_codes.find(color);
    return it != color_codes.end() ? it->second + text + "\033[0m" : text;
}

class RTSPImagePublisher : public rclcpp::Node {
public:
    RTSPImagePublisher() : Node("rtsp_image_publisher") {
        // Parameters
        this->declare_parameter<std::string>("rtsp_url", "rtsp://192.168.123.25:8554/video1");
        this->declare_parameter<std::string>("rtsp_transport", "tcp");     // tcp|udp
        this->declare_parameter<int>("gst_latency_ms", 100);               // small buffer helps some servers
        this->declare_parameter<std::string>("codec", "auto");             // auto|h264|h265|uridecodebin
        this->declare_parameter<std::string>("decoder", "avdec");          // avdec|nv(h264/h265)dec|vaapi...
        this->declare_parameter<int>("reconnect_delay_ms", 1000);
        this->declare_parameter<int>("stall_empty_frames", 30);
        this->declare_parameter<int64_t>("watchdog_no_frame_ms", 2000);

        this->declare_parameter<std::string>("topic_name", "zt30/image_raw");
        this->declare_parameter<std::string>("compressed_topic_name", "zt30/image_raw/compressed");
        this->declare_parameter<std::string>("camera_frame_id", "zt30_camera_link");
        this->declare_parameter<bool>("publish_compressed", true);
        this->declare_parameter<int>("jpeg_quality", 80);

        // Read params
        rtsp_url_            = this->get_parameter("rtsp_url").as_string();
        rtsp_transport_      = this->get_parameter("rtsp_transport").as_string();
        gst_latency_ms_      = this->get_parameter("gst_latency_ms").as_int();
        codec_pref_          = this->get_parameter("codec").as_string();
        decoder_pref_        = this->get_parameter("decoder").as_string();
        reconnect_delay_ms_  = this->get_parameter("reconnect_delay_ms").as_int();
        stall_empty_frames_  = this->get_parameter("stall_empty_frames").as_int();
        watchdog_no_frame_ms_= this->get_parameter("watchdog_no_frame_ms").as_int();
        topic_name_          = this->get_parameter("topic_name").as_string();
        comp_topic_          = this->get_parameter("compressed_topic_name").as_string();
        frame_id_            = this->get_parameter("camera_frame_id").as_string();
        publish_compressed_  = this->get_parameter("publish_compressed").as_bool();
        jpeg_quality_        = this->get_parameter("jpeg_quality").as_int();

        // Publishers (best-effort latest-only)
        auto qos = rclcpp::SensorDataQoS().keep_last(1).best_effort().durability_volatile();
        raw_pub_ = this->create_publisher<sensor_msgs::msg::Image>(topic_name_, qos);
        if (publish_compressed_) {
            comp_pub_ = this->create_publisher<sensor_msgs::msg::CompressedImage>(comp_topic_, qos);
            RCLCPP_INFO(get_logger(), "Compressed publishing enabled on: %s (quality=%d)",
                        comp_topic_.c_str(), jpeg_quality_);
        }

        RCLCPP_INFO(get_logger(), "%s", colorize("Initializing GStreamer RTSP stream...", "cyan").c_str());
        open_capture();  // initial open (auto codec probing)

        running_.store(true);
        capture_thread_ = std::thread(&RTSPImagePublisher::capture_loop, this);

        if (publish_compressed_ && comp_pub_) {
            jpeg_thread_running_.store(true);
            jpeg_thread_ = std::thread(&RTSPImagePublisher::jpeg_loop, this);
        }

        RCLCPP_INFO(get_logger(), "%s", colorize("RTSP Image Publisher Node Ready.", "blue").c_str());
    }

    ~RTSPImagePublisher() override {
        running_.store(false);
        jpeg_thread_running_.store(false);
        if (capture_thread_.joinable()) capture_thread_.join();
        if (jpeg_thread_.joinable())    jpeg_thread_.join();
        if (cap_.isOpened()) cap_.release();
    }

private:
    // Build candidate pipelines based on preference and try them in order
    bool open_capture() {
        if (cap_.isOpened()) cap_.release();

        const std::string proto = (rtsp_transport_ == "udp") ? "udp" : "tcp";
        auto mk_dec = [&](const std::string& codec) -> std::string {
            // decoder selection
            std::string dec;
            if (decoder_pref_ == "avdec") {
                dec = (codec == "h264") ? "avdec_h264" : "avdec_h265";
            } else {
                // allow explicit decoders (e.g., nvh264dec / nvh265dec, vaapih264dec / vaapih265dec)
                dec = decoder_pref_;
                // If user passed just "nv", expand minimally
                if (decoder_pref_ == "nv") dec = (codec == "h264") ? "nvh264dec" : "nvh265dec";
            }
            return dec;
        };

        auto h264 = [&]() {
            std::ostringstream p;
            p << "rtspsrc location=" << rtsp_url_
              << " protocols=" << proto
              << " latency=" << gst_latency_ms_
              << " drop-on-latency=true do-rtsp-keep-alive=true "
              << "! rtph264depay ! h264parse config-interval=1 "
              << "! " << mk_dec("h264") << " "
              << "! videoconvert "
              << "! appsink name=appsink emit-signals=false sync=false max-buffers=1 drop=true";
            return p.str();
        };

        auto h265 = [&]() {
            std::ostringstream p;
            p << "rtspsrc location=" << rtsp_url_
              << " protocols=" << proto
              << " latency=" << gst_latency_ms_
              << " drop-on-latency=true do-rtsp-keep-alive=true "
              << "! rtph265depay ! h265parse "
              << "! " << mk_dec("h265") << " "
              << "! videoconvert "
              << "! appsink name=appsink emit-signals=false sync=false max-buffers=1 drop=true";
            return p.str();
        };

        auto uri_dec = [&]() {
            // uridecodebin will autoplug depay/decoder for H264/H265/MJPEG
            std::ostringstream p;
            p << "uridecodebin uri=" << rtsp_url_
              << " ! videoconvert "
              << "! appsink name=appsink emit-signals=false sync=false max-buffers=1 drop=true";
            return p.str();
        };

        std::vector<std::string> candidates;
        if (codec_pref_ == "h264") {
            candidates = { h264() };
        } else if (codec_pref_ == "h265") {
            candidates = { h265() };
        } else if (codec_pref_ == "uridecodebin") {
            candidates = { uri_dec() };
        } else { // auto
            candidates = { h264(), h265(), uri_dec() };
        }

        for (const auto& pipe : candidates) {
            RCLCPP_INFO(get_logger(), "Trying pipeline: %s", pipe.c_str());
            if (cap_.open(pipe, cv::CAP_GSTREAMER)) {
                RCLCPP_INFO(get_logger(), "%s", colorize("RTSP opened via GStreamer.", "green").c_str());
                last_ok_ns_.store(this->now().nanoseconds(), std::memory_order_relaxed);
                return true;
            } else {
                RCLCPP_WARN(get_logger(), "Failed to open pipeline.");
            }
        }

        RCLCPP_ERROR(get_logger(), "%s", colorize("All GStreamer pipeline attempts failed!", "red").c_str());
        return false;
    }

    void capture_loop() {
        cv::Mat frame;
        int consecutive_empty = 0;
        last_ok_ns_.store(this->now().nanoseconds(), std::memory_order_relaxed);

        while (rclcpp::ok() && running_.load()) {
            if (!cap_.isOpened()) {
                RCLCPP_WARN_THROTTLE(get_logger(), *this->get_clock(), 5000,
                                     "Capture not opened. Retrying after %d ms...", reconnect_delay_ms_);
                std::this_thread::sleep_for(std::chrono::milliseconds(reconnect_delay_ms_));
                open_capture();
                continue;
            }

            if (!cap_.read(frame) || frame.empty()) {
                consecutive_empty++;
                if (consecutive_empty >= stall_empty_frames_) {
                    RCLCPP_WARN(get_logger(), "Stall detected (empty frames x%d). Reopening...",
                                consecutive_empty);
                    cap_.release();
                    std::this_thread::sleep_for(std::chrono::milliseconds(reconnect_delay_ms_));
                    open_capture();
                    consecutive_empty = 0;
                } else {
                    std::this_thread::sleep_for(5ms);
                }
                continue;
            }
            consecutive_empty = 0;

            // Timestamp now (latest)
            std_msgs::msg::Header header;
            header.stamp = this->now();
            header.frame_id = frame_id_;

            // Publish RAW
            auto raw_msg = cv_bridge::CvImage(header, "bgr8", frame).toImageMsg();
            raw_pub_->publish(*raw_msg);

            // Hand off to JPEG thread (latest-only overwrite)
            if (publish_compressed_ && comp_pub_) {
                std::lock_guard<std::mutex> lk(m_);
                latest_for_jpeg_ = frame.clone();
                latest_header_   = header;
            }

            // Watchdog bookkeeping
            last_ok_ns_.store(this->now().nanoseconds(), std::memory_order_relaxed);
        }
    }

    void jpeg_loop() {
        std::vector<uchar> buffer;
        buffer.reserve(128 * 1024);

        while (rclcpp::ok() && jpeg_thread_running_.load()) {
            cv::Mat frame;
            std_msgs::msg::Header header;

            {
                std::lock_guard<std::mutex> lk(m_);
                if (!latest_for_jpeg_.empty()) {
                    frame = latest_for_jpeg_.clone();
                    latest_for_jpeg_.release();
                    header = latest_header_;
                }
            }

            if (!frame.empty()) {
                std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, jpeg_quality_};
                cv::imencode(".jpg", frame, buffer, params);

                sensor_msgs::msg::CompressedImage msg;
                msg.header = header;
                msg.format = "jpeg";
                msg.data   = buffer;
                comp_pub_->publish(msg);
            } else {
                std::this_thread::sleep_for(2ms);
            }

            // Watchdog: force reopen if no frames seen for too long
            auto now_ns  = this->now().nanoseconds();
            auto last_ns = last_ok_ns_.load(std::memory_order_relaxed);
            if (now_ns - last_ns > static_cast<int64_t>(watchdog_no_frame_ms_) * 1'000'000LL) {
                RCLCPP_WARN(get_logger(), "Watchdog: no frames > %ld ms, reopening capture...",
                            static_cast<long>(watchdog_no_frame_ms_));
                if (cap_.isOpened()) cap_.release();
                std::this_thread::sleep_for(std::chrono::milliseconds(reconnect_delay_ms_));
                open_capture();
                last_ok_ns_.store(now_ns, std::memory_order_relaxed);
            }
        }
    }

    // --- Members ---
    std::string rtsp_url_, rtsp_transport_, codec_pref_, decoder_pref_;
    int gst_latency_ms_{100};
    int reconnect_delay_ms_{1000};
    int stall_empty_frames_{30};
    int64_t watchdog_no_frame_ms_{2000};

    std::string topic_name_, comp_topic_, frame_id_;
    bool publish_compressed_{true};
    int jpeg_quality_{80};

    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr raw_pub_;
    rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr comp_pub_;

    cv::VideoCapture cap_;
    std::atomic<bool> running_{false};
    std::thread capture_thread_;

    // JPEG worker
    std::mutex m_;
    cv::Mat latest_for_jpeg_;
    std_msgs::msg::Header latest_header_;
    std::atomic<bool> jpeg_thread_running_{false};
    std::thread jpeg_thread_;

    // Watchdog
    std::atomic<std::int64_t> last_ok_ns_{0};
};

int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RTSPImagePublisher>());
    rclcpp::shutdown();
    return 0;
}
