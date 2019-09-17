#include <hoverpibot_hardware_interface/hoverpibot_hardware_interface.h>

int main(int argc, char** argv)
{
  ros::init(argc, argv, "hoverpibot_hardware_interface");
  ros::NodeHandle nh;

  // NOTE: We run the ROS loop in a separate thread as external calls such
  // as service callbacks to load controllers can block the (main) control loop
  ros::AsyncSpinner spinner(1);
  spinner.start();

  hoverpibot_hardware_interface::hoverpibotHardwareInterface rhi(nh);

  ros::spin();

  return 0;
}
