#!/usr/bin/env python3

import rospy
from sensor_msgs.msg import Imu
from geometry_msgs.msg import Vector3

pub = rospy.Publisher('/t265/imu_new', Imu, queue_size=1)

def callback(data):
    imu = Imu()
    imu.header = data.header
    imu.orientation=data.orientation
    imu.orientation_covariance=data.orientation_covariance
    imu.linear_acceleration=data.linear_acceleration
    imu.linear_acceleration_covariance=data.linear_acceleration_covariance
    imu.angular_velocity_covariance=data.angular_velocity_covariance
    angular_vel=Vector3()
    angular_vel.x=0.0
    angular_vel.z=0.0
    angular_vel.y=data.angular_velocity.y
    imu.angular_velocity=angular_vel
    pub.publish(imu)
    
def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # name are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('imu_relay', anonymous=True)

    rospy.Subscriber("/t265/imu", Imu, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()