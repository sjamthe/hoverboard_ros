#!/usr/bin/env python3
import rospy
from sensor_msgs.msg import CompressedImage
from sensor_msgs.msg import Image
import numpy as np
from edgetpu.detection.engine import DetectionEngine
#from PIL import Image
import PIL

writeImage = True

def callback(cimage):
    global writeImage
    rospy.loginfo(rospy.get_caller_id() + "I read %s", cimage.header.seq)
    if(writeImage):
        arr = cimage.data
        size = (cimage.width,cimage.height)
        im = PIL.Image.frombytes('RGB',size,arr)
        b, g, r = im.split()
        im = PIL.Image.merge("RGB", (r, g, b))
        #rgb = bgr[...,::-1]
        im.save("/home/ubuntu/Downloads/out.jpg")
        writeImage = False
 
def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('watcher', anonymous=True)

    rospy.Subscriber("/raspicam_node/image", Image, callback)
    #rospy.Subscriber("/raspicam_node/image/compressed", CompressedImage, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
