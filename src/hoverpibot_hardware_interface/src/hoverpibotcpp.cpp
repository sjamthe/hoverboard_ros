#include <hoverpibotcpp/hoverpibot.h>

namespace hoverpibotcpp
{
  HoverPiBot::HoverPiBot(ros::NodeHandle& nh) : nh_(nh) {

  }

  HoverPiBot::~HoverPiBot() {

  }

  void HoverPiBot::callback(const sensor_msgs::JointState& joint)
  {
    ROS_INFO("I heard: [%s]", joint.name[0].c_str());
    //Convert velocity in m/s and position in meters.
    initComplete = 1;
    botJointState = joint;
    botJointState.position[0] *= WHEEL_POS_PER_TICK;
    botJointState.position[1] *= WHEEL_POS_PER_TICK;
    botJointState.velocity[0] *= WHEEL_POS_PER_TICK;
    botJointState.velocity[1] *= WHEEL_POS_PER_TICK;
  }

  void HoverPiBot::init() {
    initComplete = 0;
    wheels_cmd_pub_ = nh_.advertise<sensor_msgs::JointState>("wheels_cmd", 10);
    sub = nh_.subscribe("/hoverbot_state", 1000, &HoverPiBot::callback, this);
    //wait till callback is working
    /*while(initComplete == 0) {
      printf("got %d. sleeping\n",initComplete);
      ros::Duration(0.5).sleep(); // sleep for half a second
    }*/
    printf("Init done subscribed to %s %d\n",sub.getTopic().c_str(),sub.getNumPublishers());
  }

  void HoverPiBot::actuate(sensor_msgs::JointState joint) {
    wheels_cmd_pub_.publish(joint);
  }

  sensor_msgs::JointState HoverPiBot::read() {
    return botJointState;
  }
}
