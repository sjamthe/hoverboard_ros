#include <sstream>
#include <hoverpibot_hardware_interface/hoverpibot_hardware_interface.h>
#include <joint_limits_interface/joint_limits_interface.h>
#include <joint_limits_interface/joint_limits.h>
#include <joint_limits_interface/joint_limits_urdf.h>
#include <joint_limits_interface/joint_limits_rosparam.h>
#include <hoverpibotcpp/hoverpibot.h>

using namespace hardware_interface;
using joint_limits_interface::JointLimits;
using joint_limits_interface::SoftJointLimits;
using joint_limits_interface::PositionJointSoftLimitsHandle;
using joint_limits_interface::PositionJointSoftLimitsInterface;

namespace hoverpibot_hardware_interface
{
    hoverpibotHardwareInterface::hoverpibotHardwareInterface(ros::NodeHandle& nh) : nh_(nh),hoverpibot(nh) {
        init();
        hoverpibot.init();
        controller_manager_.reset(new controller_manager::ControllerManager(this, nh_));
        nh_.param("/hoverpibot/hardware_interface/loop_hz", loop_hz_, 0.1);
        ros::Duration update_freq = ros::Duration(1.0/loop_hz_);
        non_realtime_loop_ = nh_.createTimer(update_freq, &hoverpibotHardwareInterface::update, this);
    }

    hoverpibotHardwareInterface::~hoverpibotHardwareInterface() {

    }

    void hoverpibotHardwareInterface::init() {
        // Get joint names
        nh_.getParam("/hoverpibot/hardware_interface/joints", joint_names_);
        num_joints_ = joint_names_.size();

        // Resize vectors
        joint_position_.resize(num_joints_);
        joint_velocity_.resize(num_joints_);
        joint_effort_.resize(num_joints_);
        joint_position_command_.resize(num_joints_);
        joint_velocity_command_.resize(num_joints_);
        joint_effort_command_.resize(num_joints_);

        // Initialize Controller
        for (int i = 0; i < num_joints_; ++i) {
            //hoverpibotcpp::Joint joint = hoverpibot.getJoint(joint_names_[i]);

             // Create joint state interface
            JointStateHandle jointStateHandle(joint_names_[i], &joint_position_[i], &joint_velocity_[i], &joint_effort_[i]);
             joint_state_interface_.registerHandle(jointStateHandle);

            // Create position joint interface
            JointHandle jointPositionHandle(jointStateHandle, &joint_position_command_[i]);
            JointLimits limits;
                SoftJointLimits softLimits;
            getJointLimits(joint_names_[i], nh_, limits);
            PositionJointSoftLimitsHandle jointLimitsHandle(jointPositionHandle, limits, softLimits);
            positionJointSoftLimitsInterface.registerHandle(jointLimitsHandle);
            position_joint_interface_.registerHandle(jointPositionHandle);

            // Create effort joint interface
            JointHandle jointEffortHandle(jointStateHandle, &joint_effort_command_[i]);
            effort_joint_interface_.registerHandle(jointEffortHandle);
        }

        registerInterface(&joint_state_interface_);
        registerInterface(&position_joint_interface_);
        registerInterface(&effort_joint_interface_);
        registerInterface(&positionJointSoftLimitsInterface);
    }

    void hoverpibotHardwareInterface::update(const ros::TimerEvent& e) {
        elapsed_time_ = ros::Duration(e.current_real - e.last_real);
        read();
        controller_manager_->update(ros::Time::now(), elapsed_time_);
        write(elapsed_time_);
    }

    void hoverpibotHardwareInterface::read() {

    }

    void hoverpibotHardwareInterface::write(ros::Duration elapsed_time) {
        positionJointSoftLimitsInterface.enforceLimits(elapsed_time);

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
        //hoverpibot.actuate(wheelPositions);
    }
}
