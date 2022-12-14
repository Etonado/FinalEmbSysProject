#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include <rclcpp/rclcpp.hpp>
#include "dynamixel_sdk_custom_interfaces/msg/set_position.hpp"

using namespace std::chrono_literals;

class MotorPublisher : public rclcpp::Node
{   
    public:
        MotorPublisher():Node("motor_publisher"), count_(0)
        {
            publisher_ = this->create_publisher<dynamixel_sdk_custom_interfaces::msg::SetPosition>("motor_ctrl",10);
            timer_ = this->create_wall_timer(
                500ms, std::bind(&MotorPublisher::timer_callback, this));
            
        }

    private:
    void timer_callback()
    {
      //auto message = std_msgs::msg::String();
      //message.data = "Hello, world! " + std::to_string(count_++);
      //RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      //publisher_->publish(message);
    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<dynamixel_sdk_custom_interfaces::msg::SetPosition>::SharedPtr publisher_;
    size_t count_;
};