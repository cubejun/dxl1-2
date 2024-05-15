#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "dxl1-2/dxl.hpp"
#include <memory>
#include <functional>
#define pi 3.14159
#define wheel_separation 0.160
#define wheel_radius 0.033
using namespace std::placeholders;
void mysub_callback(rclcpp::Node::SharedPtr node, Dxl& dxl, const geometry_msgs::msg::Twist::SharedPtr msg)
{
    int rrpm, lrpm;
    RCLCPP_INFO(node->get_logger(), "Received message: %lf,%lf", msg->linear.x, msg->angular.y);
    rrpm = (msg->linear.x * 60)/(2*pi*wheel_radius)+(msg->angular.z * 60 * wheel_separation)/(2*pi*wheel_radius);
    lrpm = (msg->linear.x * 60)/(2*pi*wheel_radius);
    // rrpm = (2*pi*wheel_radius)/(msg->linear.x * 60)+((2*pi*wheel_radius)/(msg->angular.z * 60))*wheel_separation;
    // lrpm = (2*pi*wheel_radius)/(msg->linear.x * 60);
    dxl.setVelocity(rrpm, lrpm);

}
int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    Dxl dxl;
    auto node = std::make_shared<rclcpp::Node>("node_camsub");
    if(!dxl.open())
    {
        RCLCPP_ERROR(node->get_logger(), "dynamixel open error");
        rclcpp::shutdown();
        return -1;
    } 
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
    std::function<void(const geometry_msgs::msg::Twist::SharedPtr msg)> fn;
    fn = std::bind(mysub_callback, node, dxl, _1);
    auto mysub = node->create_subscription<geometry_msgs::msg::Twist>("topic_dxlpub",qos_profile,fn);
    
    rclcpp::spin(node);
    
    dxl.close();
    rclcpp::shutdown();
    return 0;
}
