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
        printf("Inside hoverpibotHardwareInterface init\n");
        hoverpibot.init();
        init();
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

            JointLimits limits;
            getJointLimits(joint_names_[i], nh_, limits);

            SoftJointLimits softLimits;

            /* Create position joint interface
            JointHandle jointPositionHandle(jointStateHandle, &joint_position_command_[i]);

            PositionJointSoftLimitsHandle jointLimitsHandle(jointPositionHandle, limits, softLimits);
            positionJointSoftLimitsInterface.registerHandle(jointLimitsHandle);
            position_joint_interface_.registerHandle(jointPositionHandle);
            */
            // Create effort joint interface
            JointHandle jointEffortHandle(jointStateHandle, &joint_effort_command_[i]);
            effort_joint_interface_.registerHandle(jointEffortHandle);

            JointHandle jointVelocityHandle(jointStateHandle, &joint_velocity_command_[i]);
            velocity_joint_interface_.registerHandle(jointVelocityHandle);

        }

        registerInterface(&joint_state_interface_);
        //registerInterface(&position_joint_interface_);
        //registerInterface(&effort_joint_interface_);
        registerInterface(&velocity_joint_interface_);
        //registerInterface(&positionJointSoftLimitsInterface);
    }

    void hoverpibotHardwareInterface::update(const ros::TimerEvent& e) {
        elapsed_time_ = ros::Duration(e.current_real - e.last_real);
        //printf("Inside update %lf\n",elapsed_time_.toSec());
        if(hoverpibot.initComplete) {
          read();
          controller_manager_->update(ros::Time::now(), elapsed_time_);
          write(elapsed_time_);
        }
    }

    void hoverpibotHardwareInterface::read() {
        sensor_msgs::JointState wheelPositions = hoverpibot.read();
        if(sizeof(wheelPositions.position) == sizeof(joint_position_)) {
          joint_position_[0] = wheelPositions.position[0];
          joint_position_[1] = wheelPositions.position[1];
          joint_velocity_[0] = wheelPositions.velocity[0];
          joint_velocity_[1] = wheelPositions.velocity[1];
          joint_effort_[0] = wheelPositions.effort[0];
          joint_effort_[1] = wheelPositions.effort[1];
      }
    }

    void hoverpibotHardwareInterface::write(ros::Duration elapsed_time) {
        positionJointSoftLimitsInterface.enforceLimits(elapsed_time);

        //double effort_left = joint_effort_command_[0];
        //double effort_right = joint_effort_command_[1];
       //double effort_left = joint_position_command_[0];
        //double effort_right = joint_position_command_[1];

        std::ostringstream jointEffortStr;
			  jointEffortStr << joint_effort_command_[0]
                       << " vel=" << joint_velocity_command_[0]
                       << " pos=" << joint_position_[0];
        std::string _logInfo = "left command: eff=" + jointEffortStr.str();
        ROS_INFO_STREAM(_logInfo);
        jointEffortStr.str("");

        jointEffortStr << joint_effort_command_[1]
                       << " vel=" << joint_velocity_command_[1]
                       << " pos=" << joint_position_[1];
        _logInfo = "right command: eff=" + jointEffortStr.str();
        ROS_INFO_STREAM(_logInfo);
        jointEffortStr.str("");

        sensor_msgs::JointState newWheelPositions; //This has to be local variable. for some reason.
        newWheelPositions.header.stamp = ros::Time::now();
        newWheelPositions.name.resize(2);
        newWheelPositions.name[0] = "LEFT";
        newWheelPositions.name[1] = "RIGHT";
        newWheelPositions.position.resize(2);
        newWheelPositions.position[0] = 0;
        newWheelPositions.position[1] = 0;
        newWheelPositions.velocity.resize(2);
        newWheelPositions.velocity[0] = joint_velocity_command_[0];
        newWheelPositions.velocity[1] = joint_velocity_command_[1];
        newWheelPositions.effort.resize(2);
        newWheelPositions.effort[0] = joint_effort_command_[0];
        newWheelPositions.effort[1] = joint_effort_command_[1]*-1;//Right wheel turns opposite dir
        hoverpibot.actuate(newWheelPositions);
    }
}
