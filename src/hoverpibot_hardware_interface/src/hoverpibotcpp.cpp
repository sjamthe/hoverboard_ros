#include <hoverpibotcpp/hoverpibot.h>

namespace hoverpibotcpp
{
  HoverPiBot::HoverPiBot(ros::NodeHandle& nh) : nh_(nh) {

  }

  HoverPiBot::~HoverPiBot() {
    
  }

  void HoverPiBot::init() {
     wheels_cmd_pub_ = nh_.advertise<sensor_msgs::JointState>("wheels_cmd", 10);
  }

  void HoverPiBot::actuate(sensor_msgs::JointState joint) {
    wheels_cmd_pub_.publish(joint);
  }

  sensor_msgs::JointState HoverPiBot::read() {

  }
}
