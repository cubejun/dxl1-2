#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "dxl1-2/dxl.hpp"
#include <memory>
#include <chrono>

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("node_dxlpub");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
    auto mypub = node->create_publisher<geometry_msgs::msg::Twist>("topic_dxlpub", qos_profile );
    geometry_msgs::msg::Twist vel;
    vel.linear.x = 0;
    vel.angular.z = 0;
    rclcpp::WallRate loop_rate(20.0); //20hz->50msec
    std::cout << "linear : ";
    std::cin >> vel.linear.x;
    std::cout << "angular : ";
    std::cin >> vel.angular.z;
    while(rclcpp::ok())
    {
        RCLCPP_INFO(node->get_logger(), "Publish: %lf,%lf", vel.linear.x, vel.angular.z);
        mypub->publish(vel);
        loop_rate.sleep();
    }
    rclcpp::shutdown();
    return 0;
}