#include <cmath>
#include <unistd.h>
#include "utils/colors.h"

#include "geometry_msgs/msg/twist.hpp"

#include "rclcpp/rclcpp.hpp"
#include "go2_interface/srv/go2_modes.hpp"
#include "go2_interface/srv/go2_light.hpp"
#include "go2_interface/srv/go2_volume.hpp"

#include "unitree/robot/go2/vui/vui_client.hpp"
#include "unitree/robot/go2/sport/sport_client.hpp"
#include "unitree/robot/b2/motion_switcher/motion_switcher_client.hpp"

using std::placeholders::_1;

class go2_highlevel_request : public rclcpp::Node
{
public:
    go2_highlevel_request(
        std::shared_ptr<unitree::robot::go2::SportClient> sport_client,
        std::shared_ptr<unitree::robot::b2::MotionSwitcherClient> switcher_client,
        std::shared_ptr<unitree::robot::go2::VuiClient> vui_client,
        //: Node("go2_highroscontrol"),
        const std::string & ns = "/go2_unit_49702")
        : Node("go2_highroscontrol", ns),
          sport_client_(sport_client),
          switcher_client_(switcher_client),
          vui_client_(vui_client)
    {
        // Initialization
        RCLCPP_INFO(get_logger(), BOLD(FGRN("Initializing GO2 ROS Hardware Communication")));
        RCLCPP_INFO(get_logger(), BOLD(FGRN("test")));
        // Subscribers
        // try {
        //     sub_euler_cmd = this->create_subscription<geometry_msgs::msg::Twist>(
        //         "euler_cmd", 10,
        //         std::bind(&go2_highlevel_request::callback_euler_cmd, this, std::placeholders::_1)
        //     );
        //     RCLCPP_INFO(get_logger(), "euler_cmd subscriber created: %s", sub_euler_cmd->get_topic_name());
        // } catch (const std::exception &e) {
        //     RCLCPP_ERROR(get_logger(), "FAILED to create euler_cmd subscriber: %s", e.what());
        // }

        sub_go2_cmd_vel = this->create_subscription<geometry_msgs::msg::Twist>(
            "hardware/cmd_vel", 11, std::bind(&go2_highlevel_request::callback_go2_cmd, this, _1));
        sub_euler_cmd = this->create_subscription<geometry_msgs::msg::Twist>(
            "euler_cmd", 1, std::bind(&go2_highlevel_request::callback_euler_cmd, this, _1));

        // Service
        srv_go2_modes = this->create_service<go2_interface::srv::Go2Modes>(
            "modes", std::bind(&go2_highlevel_request::callback_go2_modes, this, std::placeholders::_1, std::placeholders::_2));
        srv_go2_light = this->create_service<go2_interface::srv::Go2Light>(
            "light", std::bind(&go2_highlevel_request::callback_go2_light, this, std::placeholders::_1, std::placeholders::_2));
        srv_go2_volume = this->create_service<go2_interface::srv::Go2Volume>(
            "volume", std::bind(&go2_highlevel_request::callback_go2_volume, this, std::placeholders::_1, std::placeholders::_2));

        // Provide Mode Info
        printAllModes();
        printMotionStatus();
    };

private:
    int printMotionStatus()
    {
        std::string robotForm, motionName;
        int motionStatus;
        switcher_client_->CheckMode(robotForm, motionName);
        RCLCPP_INFO(get_logger(), FORA("Current robotform: %s"), (robotForm == "0" ? "GO2" : "GO2-W"));
        if (motionName.empty())
        {
            RCLCPP_INFO(get_logger(), FORA("sport_mode or ai_sport is deactivate"));
            motionStatus = 0;
        }
        else
        {
            RCLCPP_INFO(get_logger(), FORA("%s"), (motionName == "normal" ? "sport_mode is activate" : "ai_sport is activate"));
            motionStatus = 1;
        }
        return motionStatus;
    }

    /**
     * @brief Callback for cmd_vel topic subscriber.
     *
     * Handles incoming messages from the cmd_vel topic and passes them to the Unitree GO2
     * robot's Move() function for execution.
     *
     * If the message contains only a linear or angular velocity, the other velocities
     * will be set to 0.0.
     *
     * If the message is empty, the callback does nothing.
     *
     * @param msg The Twist message containing the velocities.
     */
    void callback_go2_cmd(geometry_msgs::msg::Twist::SharedPtr msg)
    {
        //RCLCPP_INFO(this->get_logger(), "MOVE CALLBACK TRIGGERED");
        if (msg->linear.x == 0.0 && msg->linear.y == 0.0 && msg->angular.z == 0.0)
        {
            sport_client_->StopMove();
        }
        else
        {
            sport_client_->Move(
                msg->linear.x,
                msg->linear.y,
                msg->angular.z
            );
        }
        // float linear_x = 0.0;
        // float linear_y = 0.0;
        // float angular_z = 0.0;
        // // if (mode_ == NORMAL)
        // // {
        // if (msg->linear.x && msg->linear.y && msg->angular.z)
        // {
        //     linear_x = msg->linear.x;
        //     linear_y = msg->linear.y;
        //     angular_z = msg->angular.z;
        // }
        // else if (msg->linear.x && msg->linear.y)
        // {
        //     linear_x = msg->linear.x;
        //     linear_y = msg->linear.y;
        // }
        // else if (msg->linear.x && msg->angular.z)
        // {
        //     linear_x = msg->linear.x;
        //     angular_z = msg->angular.z;
        // }
        // else if (msg->linear.y && msg->angular.z)
        // {
        //     linear_y = msg->linear.y;
        //     angular_z = msg->angular.z;
        // }
        // else if (msg->linear.x)
        // {
        //     linear_x = msg->linear.x;
        // }
        // else if (msg->linear.y)
        // {
        //     linear_y = msg->linear.y;
        // }
        // else if (msg->angular.z)
        // {
        //     angular_z = msg->angular.z;
        // }
        // else
        // {   
        //     return;
        // }
        // sport_client_->Move(linear_x, linear_y, angular_z);
    }

    //callback for the Euler function
    void callback_euler_cmd(const geometry_msgs::msg::Twist::SharedPtr msg)
    {
        //mode_ = EULER;
        // RCLCPP_INFO(this->get_logger(), "EULER CALLBACK TRIGGERED");
        // RCLCPP_INFO(this->get_logger(), "Euler values - x: %f, y: %f, z: %f",
        //     msg->linear.x, msg->linear.y, msg->angular.z);

        //sport_client_->Euler(-0.5,0,0);
        sport_client_->BalanceStand();
        sport_client_->Euler(
            msg->linear.x,
            msg->linear.y,
            msg->angular.z
        );
    }

    // Callback for Subscribers and Services
    void callback_go2_volume(const std::shared_ptr<go2_interface::srv::Go2Volume::Request> request,
                             std::shared_ptr<go2_interface::srv::Go2Volume::Response> response)
    {
        try
        {
            RCLCPP_INFO(get_logger(), BOLD(FORA("Service request received for volume level: ")) FBLU("%d"), request->volume_level);
            bool success = go2_volume_configuration(request);

            response->success = success;

            if (success)
            {
                response->reason = "Volume configuration set successfully";
            }
            else
            {
                response->reason = "Failed to set Volume configuration";
            }
        }
        catch (const std::exception &e)
        {
            RCLCPP_ERROR(get_logger(), "Exception in callback_go2_volume: %s", e.what());
        }
    }

    void callback_go2_light(const std::shared_ptr<go2_interface::srv::Go2Light::Request> request,
                            std::shared_ptr<go2_interface::srv::Go2Light::Response> response)
    {
        try
        {
            RCLCPP_INFO(get_logger(), BOLD(FORA("Service request received for light level: ")) FBLU("%d"), request->light_level);
            bool success = go2_light_configuration(request);

            response->success = success;

            if (success)
            {
                response->reason = "Light configuration set successfully";
            }
            else
            {
                response->reason = "Failed to set light configuration";
            }
        }
        catch (const std::exception &e)
        {
            RCLCPP_ERROR(get_logger(), "Exception in callback_go2_light: %s", e.what());
        }
    }

    /**
     * @brief Callback for the go2_modes service, which sets the GO2 robot
     *        to a specific mode.
     *
     * @param request The request data containing the name of the mode to set.
     * @param response The response data containing the success status and reason.
     */
    void callback_go2_modes(const std::shared_ptr<go2_interface::srv::Go2Modes::Request> request,
                            std::shared_ptr<go2_interface::srv::Go2Modes::Response> response)
    {
        try
        {
            RCLCPP_INFO(get_logger(), BOLD(FORA("Service request received: ")) FBLU("%s"), request->request_data.c_str());

            bool success = go2_mode_selection(request);

            response->success = success;

            if (success)
            {
                response->reason = "GO2 mode set successfully";
            }
            else
            {
                response->reason = "Failed to set GO2 mode";
            }
        }
        catch (const std::exception &e)
        {
            RCLCPP_ERROR(get_logger(), "Exception in callback_go2_modes: %s", e.what());
        }
    }

    bool go2_volume_configuration(const std::shared_ptr<go2_interface::srv::Go2Volume::Request> request)
    {
        if (request->volume_level >= 0 && request->volume_level <= 10)
        {
            vui_client_->SetVolume(request->volume_level);
            
            // Add volume control client
            return true;
        }
        else
        {
            RCLCPP_WARN(this->get_logger(), "Unknown light configuration received: %d", request->volume_level);
            return false;
        }
        return false;
    }

    bool go2_light_configuration(const std::shared_ptr<go2_interface::srv::Go2Light::Request> request)
    {
        if (request->light_level >= 0 && request->light_level <= 10)
        {
            vui_client_->SetBrightness(request->light_level);

            // Add light control client
            return true;
        }
        else
        {
            RCLCPP_WARN(this->get_logger(), "Unknown light configuration received: %d", request->light_level);
            return false;
        }
    }

    /**
     * @brief Handles the selection of a GO2 mode
     *
     * @param request the request data
     * @return true if the mode was selected successfully, false otherwise
     */
    bool go2_mode_selection(const std::shared_ptr<go2_interface::srv::Go2Modes::Request> request)
    {

        if (request->request_data == "gait_type_classic")
        {
            sport_client_->ClassicWalk(true);

            return true;
        }
        else if (request->request_data == "gait_type_agile")
        {
            sport_client_->ClassicWalk(false);

            return true;
        }
        else if (request->request_data == "gait_type_trot_run")
        {
            sport_client_->TrotRun();

            return true;
        }
        else if (request->request_data == "gait_type_flat_terrain")
        {
            sport_client_->StaticWalk();

            return true;
        }
        else if (request->request_data == "gait_type_economic")
        {
            sport_client_->EconomicGait();

            return true;
        }
        else if (request->request_data == "balance_stand")
        {
            sport_client_->BalanceStand();

            return true;
        }
        else if (request->request_data == "euler")
        {
            RCLCPP_INFO(this->get_logger(), "Entering EULER mode");
            return true;
        }
        else if (request->request_data == "stand_down")
        {
            RCLCPP_INFO(this->get_logger(), "Entering down mode");
            //sport_client_->StandUp();
            //std::this_thread::sleep_for(std::chrono::seconds(2));
            sport_client_->StandDown();
            
            return true;
        }
        else if (request->request_data == "stand_up")
        {
            sport_client_->StandUp();
            std::this_thread::sleep_for(std::chrono::seconds(3));
            sport_client_->BalanceStand();
            return true;
        }
        else if (request->request_data == "damp")
        {
            sport_client_->Damp();

            return true;
        }
        else if (request->request_data == "recovery_stand")
        {
            sport_client_->RecoveryStand();

            return true;
        }
        else if (request->request_data == "sit")
        {
            sport_client_->Sit();

            return true;
        }
        else if (request->request_data == "rise_sit")
        {
            sport_client_->RiseSit();

            return true;
        }
        else if (request->request_data == "stretch")
        {
            sport_client_->Stretch();

            return true;
        }
        else if (request->request_data == "pose")
        {
            sport_client_->Pose(true);

            return true;
        }
        else if (request->request_data == "front_jump")
        {
            sport_client_->FrontJump();

            return true;
        }
        else if (request->request_data == "front_pounce")
        {
            sport_client_->FrontPounce();

            return true;
        }
        else if (request->request_data == "front_flip")
        {
            sport_client_->FrontFlip();

            return true;
        }
        else if (request->request_data == "scrape")
        {
            sport_client_->Scrape();

            return true;
        }
        else if (request->request_data == "stop_move")
        {
            sport_client_->StopMove();

            return true;
        }
        else if (request->request_data == "obstacle_avoidance")
        {
            sport_client_->SwitchAvoidMode();

            return true;
        }
        else if (request->request_data == "sports_mode")
        {
            int32_t ret = switcher_client_->SelectMode("sport_mode");
            RCLCPP_INFO(this->get_logger(), "Switching to sports_mode, ret: %d", ret);

            return true;
        }
        else if (request->request_data == "advanced_sports_mode")
        {
            int32_t ret = switcher_client_->SelectMode("advanced_sport");
            RCLCPP_INFO(this->get_logger(), "Switching to advanced_sport, ret: %d", ret);

            return true;
        }
        else if (request->request_data == "ai_sport")
        {
            int32_t ret = switcher_client_->SelectMode("ai_sport");
            RCLCPP_INFO(this->get_logger(), "Switching to ai_sport, ret: %d", ret);

            return true;
        }
        else if (request->request_data == "wheeled_sport")
        {
            int32_t ret = switcher_client_->SelectMode("wheeled_sport");
            RCLCPP_INFO(this->get_logger(), "Switching to wheeled_sport, ret: %d", ret);

            return true;
        }
        else
        {
            RCLCPP_WARN(this->get_logger(), "Unknown mode received: %s", request->request_data.c_str());
            sport_client_->StopMove();
            RCLCPP_INFO(this->get_logger(), "MY NODE RECEIVED REQUEST");
            RCLCPP_WARN(this->get_logger(), "Unknown mode received: %s", request->request_data.c_str());
            return false;
        }
    }

    void printAllModes()
    /**
     * @brief Print all available GO2 modes and functionalities via ROS2 services.
     *
     * This function prints all available GO2 modes and functionalities that can be accessed via ROS2 services.
     * This includes:
     *  - GO2 modes: balance_stand, stand_down, stand_up, damp, recovery_stand, sit, rise_sit, stretch, pose, front_jump, front_pounce, front_flip, scrape, stop_move, obstacle_avoidance
     *  - GO2 functionalities: light level: 0-10, volume level: 0-10
     */
    {
        RCLCPP_INFO(get_logger(), BOLD(FBLU("Available GO2 Modes via ROS2 Services:")));

        std::vector<std::string> modes = {
            "gait_type_classic",
            "gait_type_flat_terrain",
            "gait_type_economic",
            "gait_type_agile",
            "balance_stand",
            "stand_down",
            "stand_up",
            "euler",
            "damp",
            "recovery_stand",
            "sit",
            "rise_sit",
            "stretch",
            "pose",
            "front_jump",
            "front_pounce",
            "front_flip",
            "scrape",
            "stop_move",
            "obstacle_avoidance",
        };

        for (const auto &mode : modes)
        {
            RCLCPP_INFO(get_logger(), FCYN("\t- %s"), mode.c_str());
        }

        RCLCPP_INFO(get_logger(), BOLD(FBLU("Available GO2 Functionalities via ROS2 Services:")));

        std::vector<std::string> functionalities = {
            "Light level:  0-10",
            "Volume level: 0-10",
        };

        for (const auto &functionalities : functionalities)
        {
            RCLCPP_INFO(get_logger(), FCYN("\t- %s"), functionalities.c_str());
        }
    }

    // Initialize Subscribers
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_go2_cmd_vel;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_euler_cmd;

    // Initialize Service
    rclcpp::Service<go2_interface::srv::Go2Modes>::SharedPtr srv_go2_modes;
    rclcpp::Service<go2_interface::srv::Go2Light>::SharedPtr srv_go2_light;
    rclcpp::Service<go2_interface::srv::Go2Volume>::SharedPtr srv_go2_volume;

    // Unitree client
    std::shared_ptr<unitree::robot::go2::VuiClient> vui_client_;
    std::shared_ptr<unitree::robot::go2::SportClient> sport_client_;
    std::shared_ptr<unitree::robot::b2::MotionSwitcherClient> switcher_client_;
};

int main(int argc, char *argv[])
{
    // Initialize Unitree Robot
    unitree::robot::ChannelFactory::Instance()->Init(0, "eth0");

    auto sport_client = std::make_shared<unitree::robot::go2::SportClient>();
    sport_client->SetTimeout(10.0f);
    sport_client->Init();

    // Initialize MotionSwitcherClient
    auto switcher_client = std::make_shared<unitree::robot::b2::MotionSwitcherClient>();
    switcher_client->Init();

    // Initialize VUI (Virtual User Interface)
    auto vui_client = std::make_shared<unitree::robot::go2::VuiClient>();
    vui_client->Init();

    // Initialize ROS 2
    rclcpp::init(argc, argv);
    auto node = std::make_shared<go2_highlevel_request>(sport_client, switcher_client, vui_client);
    rclcpp::spin(node);
    RCLCPP_INFO(node->get_logger(), "\x1B[31mTerminating Go2 high-level ROS2 hardware communication\x1B[0m");
    rclcpp::shutdown();
    return 0;
}
