#ifndef ROS_CONTROL__hoverpibot_HARDWARE_INTERFACE_H
#define ROS_CONTROL__hoverpibot_HARDWARE_INTERFACE_H

#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/joint_command_interface.h>
#include <hardware_interface/robot_hw.h>
#include <joint_limits_interface/joint_limits_interface.h>
#include <joint_limits_interface/joint_limits.h>
#include <joint_limits_interface/joint_limits_urdf.h>
#include <joint_limits_interface/joint_limits_rosparam.h>
#include <controller_manager/controller_manager.h>
#include <boost/scoped_ptr.hpp>
#include <ros/ros.h>
#include <hoverpibotcpp/hoverpibot.h>
#include <hoverpibot_hardware_interface/hoverpibot_hardware.h>

using namespace hardware_interface;
using joint_limits_interface::JointLimits;
using joint_limits_interface::SoftJointLimits;
using joint_limits_interface::PositionJointSoftLimitsHandle;
using joint_limits_interface::PositionJointSoftLimitsInterface;


namespace hoverpibot_hardware_interface
{
    static const double POSITION_STEP_FACTOR = 1;
    static const double VELOCITY_STEP_FACTOR = 1;

    class hoverpibotHardwareInterface: public hoverpibot_hardware_interface::hoverpibotHardware
    {
        public:
            hoverpibotHardwareInterface(ros::NodeHandle& nh);
            ~hoverpibotHardwareInterface();
            void init();
            void update(const ros::TimerEvent& e);
            void read();
            void write(ros::Duration elapsed_time);

        protected:
            hoverpibotcpp::HoverPiBot hoverpibot;
            ros::NodeHandle nh_;
            ros::Timer non_realtime_loop_;
            ros::Duration control_period_;
            ros::Duration elapsed_time_;
            //PositionJointInterface positionJointInterface;
            //VelocityJointInterface velocityJointInterface;
            PositionJointSoftLimitsInterface positionJointSoftLimitsInterface;
            double loop_hz_;
            boost::shared_ptr<controller_manager::ControllerManager> controller_manager_;
            double p_error_, v_error_, e_error_;
    };

}

#endif
