#include <hoverpibot_hardware_interface/hoverpibot_hardware_interface.h>
#include <ros/callback_queue.h>

int main2(int argc, char** argv)
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

int main(int argc, char** argv)
{
    ros::init(argc, argv, "hoverpibot_hardware_interface");
    ros::CallbackQueue ros_queue;

    ros::NodeHandle nh;
    nh.setCallbackQueue(&ros_queue);
    hoverpibot_hardware_interface::hoverpibotHardwareInterface rhi(nh);

    ros::MultiThreadedSpinner spinner(0);
    spinner.spin(&ros_queue);
    //spinner.spin();
    return 0;
}
