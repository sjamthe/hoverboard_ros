#include "ros/ros.h"
#include "sensor_msgs/JointState.h"
#include <string.h>

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "wheels_cmd");

  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */
  ros::Publisher wheels_cmd_pub = n.advertise<sensor_msgs::JointState>("wheels_cmd", 10);

  ros::Rate loop_rate(10);

  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  int count = 0;
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */

     sensor_msgs::JointState wheelPositions; //This has to be local variable. for some reason.
     wheelPositions.header.stamp = ros::Time::now();
     wheelPositions.name.resize(2);
     wheelPositions.name[0] = "LEFT";
     wheelPositions.name[1] = "RIGHT";
     wheelPositions.position.resize(2);
     wheelPositions.position[0] = 90;
     wheelPositions.position[1] = 90;
     wheelPositions.velocity.resize(2);
     wheelPositions.velocity[0] = 0;
     wheelPositions.velocity[1] = 0;
     wheelPositions.effort.resize(2);
     wheelPositions.effort[0] = 0;
     wheelPositions.effort[1] = 0;

     //ROS_INFO("%s", msg.data.c_str());

    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    wheels_cmd_pub.publish(wheelPositions);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}
