#include <unitree/robot/channel/channel_publisher.hpp>
#include <unitree/robot/channel/channel_subscriber.hpp>
#include <unitree/common/time/time_tool.hpp>

#include "msg/ArmString_.hpp"
#include "msg/PubServoInfo_.hpp"
#include "msg/SetServoAngle_.hpp"
#include "msg/SetServoDumping_.hpp"

#include "CSerialPort/SerialPort.h"
#include "FashionStar/UServo/FashionStar_UartServoProtocol.h"
#include "FashionStar/UServo/FashionStar_UartServo.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include <iostream>
#include <thread>
#include <chrono>

#define PubArmFeedback_Topic "rt/arm_Feedback"
#define PubServoAngle_Topic "current_servo_angle"
#define SubServoAngle_Topic "set_servo_angle"
#define SubServoDumping_Topic "set_servo_dumping"
#define SERVO_PORT_NAME "/dev/ttyS4"

using namespace unitree::robot;
using namespace unitree::common;
using namespace fsuservo;
using namespace rapidjson;

ChannelPublisherPtr<unitree_arm::msg::dds_::ArmString_> pubArmFeedback_publisher;
ChannelPublisherPtr<unitree_arm::msg::dds_::PubServoInfo_> pubServoAngle_publisher;

FSUS_Protocol protocol(SERVO_PORT_NAME, FSUS_DEFAULT_BAUDRATE);
FSUS_Servo servo[7] = {
            FSUS_Servo(0,&protocol),
            FSUS_Servo(1,&protocol),
            FSUS_Servo(2,&protocol),
            FSUS_Servo(3,&protocol),
            FSUS_Servo(4,&protocol),
            FSUS_Servo(5,&protocol),
            FSUS_Servo(6,&protocol),
        };

class Timer_ {
    public:
        Timer_(std::function<void()> callback, int interval) : 
            callback_(callback), 
            interval_(interval), 
            running_(false) {}

        void start() {
            running_ = true;
            timer_thread_ = std::thread([this]() {
                while (running_) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(interval_));
                    if (callback_ != nullptr) {
                        callback_();
                    }
                }
            });
        }

        void stop() {
            running_ = false;
            if (timer_thread_.joinable()) {
                timer_thread_.join();
            }
        }

    private:
        std::function<void()> callback_;
        int interval_;
        bool running_;
        std::thread timer_thread_;
};

void execStateFeedback(int seq,int status)
{
    Document document;
    document.SetObject();

    Value data(kObjectType);
    Document::AllocatorType& allocator = document.GetAllocator();

    data.AddMember("exec_status", status, allocator);

    document.AddMember("seq",seq,allocator);
    document.AddMember("address",3,allocator);
    document.AddMember("funcode",2,allocator);
    document.AddMember("data", data, allocator);

    StringBuffer buffer; 
    Writer<StringBuffer> writer(buffer);

    document.Accept(writer);

    unitree_arm::msg::dds_::ArmString_ armFeedback_msg{};
    armFeedback_msg.data_() = buffer.GetString();
    pubArmFeedback_publisher->Write(armFeedback_msg,0);
}

void pubServoAngle_callback()
{
    float angle;

    Document document;
    document.SetObject();

    Value data(kObjectType);
    Document::AllocatorType& allocator = document.GetAllocator();

    unitree_arm::msg::dds_::PubServoInfo_ servoInfo_msg{};
    unitree_arm::msg::dds_::ArmString_ armFeedback_msg{};

    angle = servo[0].queryRawAngle();
    servoInfo_msg.servo0_data_() = angle;
    data.AddMember("angle0", angle, allocator);

    angle = servo[1].queryRawAngle();
    servoInfo_msg.servo1_data_() = angle;
    data.AddMember("angle1", angle, allocator);

    angle = servo[2].queryRawAngle();
    servoInfo_msg.servo2_data_() = angle;
    data.AddMember("angle2", angle, allocator);

    angle = servo[3].queryRawAngle();
    servoInfo_msg.servo3_data_() = angle;
    data.AddMember("angle3", angle, allocator);

    angle = servo[4].queryRawAngle();
    servoInfo_msg.servo4_data_() = angle;
    data.AddMember("angle4", angle, allocator);

    angle = servo[5].queryRawAngle();
    servoInfo_msg.servo5_data_() = angle;
    data.AddMember("angle5", angle, allocator);

    angle = servo[6].queryRawAngle();
    servoInfo_msg.servo6_data_() = angle;
    data.AddMember("angle6", angle, allocator);

    document.AddMember("seq",10,allocator);
    document.AddMember("address",2,allocator);
    document.AddMember("funcode",1,allocator);
    document.AddMember("data", data, allocator);

    StringBuffer buffer;
    Writer<StringBuffer> writer(buffer);

    document.Accept(writer);
    armFeedback_msg.data_() = buffer.GetString();

    pubServoAngle_publisher->Write(servoInfo_msg, 0);
    pubArmFeedback_publisher->Write(armFeedback_msg, 0);
}

void subServoAngle_callback(const void* msg)
{
    const unitree_arm::msg::dds_::SetServoAngle_* pm = (const unitree_arm::msg::dds_::SetServoAngle_*)msg;
    std::cout << "seq:" << pm->seq_() << ", id:" << pm->id_() << ", angle:" << pm->angle_() << ", delay_ms:" << pm->delay_ms_() << std::endl;
    std::cout << "seq:" << pm->seq_() << ", id:" << pm->id_() << std::endl;
    int seq = pm->seq_();
    int id = pm->id_();
    float angle = pm->angle_();
    int delay_ms = pm->delay_ms_();

    servo[id].setRawAngle(angle, delay_ms);

    //execStateFeedback(seq,1);
}

void subServoDumping_callback(const void* msg)
{
    const unitree_arm::msg::dds_::SetServoDumping_* pm = (const unitree_arm::msg::dds_::SetServoDumping_*)msg;
    std::cout << "seq:" << pm->seq_() << ", id:" << pm->id_() << ", power:" << pm->power_() << std::endl;

    int seq = pm->seq_();
    int id = pm->id_();
    int power = pm->power_();

    servo[id].setDamping(power);

    //execStateFeedback(seq,1);
}

int main()
{
    ChannelFactory::Instance()->Init(0);

    pubArmFeedback_publisher.reset(new ChannelPublisher<unitree_arm::msg::dds_::ArmString_>(PubArmFeedback_Topic));
    pubArmFeedback_publisher->InitChannel();
    pubServoAngle_publisher.reset(new ChannelPublisher<unitree_arm::msg::dds_::PubServoInfo_>(PubServoAngle_Topic));
    pubServoAngle_publisher->InitChannel();
    
    ChannelSubscriber<unitree_arm::msg::dds_::SetServoAngle_> subServoAngle_subscriber(SubServoAngle_Topic);
    subServoAngle_subscriber.InitChannel(subServoAngle_callback, 10);
    ChannelSubscriber<unitree_arm::msg::dds_::SetServoDumping_> subServoDumping_subscriber(SubServoDumping_Topic);
    subServoDumping_subscriber.InitChannel(subServoDumping_callback, 10);

    Timer_ timer(pubServoAngle_callback, 100);
    timer.start();

    while (true)
    {
        sleep(10);
    }

    timer.stop();

    return 0;
}
