#ifndef HOVERPIBOTCPP__HOVERPIBOT_H
#define HOVERPIBOTCPP__HOVERPIBOT_H

#include <sstream>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <string.h>

#define DEFAULT_WHEEL_SIZE_INCHES 6.5
#define PI 3.14159265358979f
#define WHEEL_POS_PER_TICK (PI*DEFAULT_WHEEL_SIZE_INCHES*25.4/90/1000)

namespace hoverpibotcpp
{
	class HoverPiBot
	{
		public:
			HoverPiBot(ros::NodeHandle& nh);
			~HoverPiBot();
			sensor_msgs::JointState read(void);
			void init(void);
			void actuate(sensor_msgs::JointState joint);

		protected:
			ros::NodeHandle nh_;
			ros::Publisher wheels_cmd_pub_;
			ros::Subscriber sub;
			void callback(const sensor_msgs::JointState& joint);
			sensor_msgs::JointState botJointState;
	};
}

#endif
