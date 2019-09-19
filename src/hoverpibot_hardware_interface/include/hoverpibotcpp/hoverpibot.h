#ifndef HOVERPIBOTCPP__HOVERPIBOT_H
#define HOVERPIBOTCPP__HOVERPIBOT_H

#include <sstream>
#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <string.h>

namespace hoverpibotcpp
{
	class HoverPiBot
	{
		public:
			HoverPiBot(ros::NodeHandle& nh);
			~HoverPiBot();
			sensor_msgs::JointState read();
			void init(void);
			void actuate(sensor_msgs::JointState joint);

		protected:
			ros::NodeHandle nh_;
			ros::Publisher wheels_cmd_pub_;

	};
}

#endif
