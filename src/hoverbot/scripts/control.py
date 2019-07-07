#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Vector3, Twist
from sensor_msgs.msg import Joy

import serial
import io

# listen to keyboard arrow keys produced by
# rosrun turtlesim turtle_teleop_key
# up arrow = linear.x > 0 down = linear.x < 0
# left arrow = angular.z > 0 right = angular.z < 0
def kb_callback(data):
    rospy.loginfo(rospy.get_caller_id() + " I heard %f, %f", data.linear.x,data.angular.z)
    if(data.linear.x > 0):
        ser.write(b's')
    elif(data.linear.x < 0):
        ser.write(b'w')
    elif(data.angular.z > 0):
        ser.write(b'a')
    elif(data.angular.z < 0):
        ser.write(b'd')

# listen to joystick messges produced by
#  rosrun joy joy_node
# up left knob = data.axes[1] > 0 down = data.axes[1] < 0
# left = data.axes[0] > 0 right = data.axes[0] < 0
def joy_callback(data):
    rospy.loginfo(rospy.get_caller_id() + " I heard %f, %f", data.axes[0], data.axes[1])
    if(data.axes[1] > 0):
        ser.write(b's')
    elif(data.axes[1] < 0):
        ser.write(b'w')
    if(data.axes[0] > 0.5):
        ser.write(b'a')
    elif(data.axes[0] < -0.5):
        ser.write(b'd')

if __name__ == '__main__':

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('control', anonymous=False)
    rospy.loginfo("Hoverboard control node")

    #open serial port
    port = rospy.get_param('~port', '/dev/ttyUSB0')
    baud = int(rospy.get_param('~baud', '115200'))
    ser = serial.Serial(port, baud)
    rospy.loginfo("Opened port " + ser.name)

    rospy.Subscriber("/turtle1/cmd_vel", Twist, kb_callback)

    rospy.Subscriber("/joy", Joy, joy_callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

