#include "ros/ros.h"
#include "std_msgs/String.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/PoseStamped.h"
#include "nav_msgs/Odometry.h"
#include "stdio.h"
#include <fstream>
// #include "include/common.h"


using namespace  std;

ros::Subscriber sub;
bool enable_output_file;
std::ofstream fd;
std::string output_file_path;
std::string sub_topic;
std::string sub_type;
// int frame_id_path = 0;
// int last_pose_n = -2;
// int cur_pose_n = -1;


static void process(ros::Time stamp, double x, double y, double z,
                    double qw, double qx, double qy, double qz)
{
    static ros::Time last_time=ros::Time::now();
    if((ros::Time::now().toSec()-last_time.toSec())>0.1)
    {
        if(enable_output_file)
        {
            fd << setprecision(6)
               << stamp << " "
               << setprecision(6)
               << x << " "
               << y << " "
               << z << " "
               << qw << " "
               << qx << " "
               << qy << " "
               << qz << std::endl;
        }
    }
}

void TransformStamped_callback(const geometry_msgs::TransformStampedConstPtr msg)
{
    process(msg->header.stamp,
            msg->transform.translation.x, msg->transform.translation.y, msg->transform.translation.z,
            msg->transform.rotation.w, msg->transform.rotation.x, msg->transform.rotation.y,  msg->transform.rotation.z);
}

void PointStamped_callback(const geometry_msgs::PointStampedConstPtr msg)
{
    process(msg->header.stamp,
            msg->point.x, msg->point.y, msg->point.z,
            1, 0, 0, 0);
}

void PoseStamped_callback(const geometry_msgs::PoseStampedConstPtr msg)
{
    process(msg->header.stamp,
            msg->pose.position.x, msg->pose.position.y, msg->pose.position.z,
            msg->pose.orientation.w, msg->pose.orientation.x, msg->pose.orientation.y, msg->pose.orientation.z);
}

void Odometry_callback(const nav_msgs::OdometryConstPtr msg)
{

    process(msg->header.stamp,
            msg->pose.pose.position.x, msg->pose.pose.position.y, msg->pose.pose.position.z,
            msg->pose.pose.orientation.w, msg->pose.pose.orientation.x,
            msg->pose.pose.orientation.y, msg->pose.pose.orientation.z);
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "vo_repub_rec");
    ros::NodeHandle nh("~");

    nh.getParam("sub_topic",        sub_topic);
    nh.getParam("sub_type",         sub_type);
    nh.getParam("output_file_path", output_file_path);
    cout << output_file_path << endl;
    if(output_file_path=="0") enable_output_file = false;
    else
    {
        enable_output_file = true;
        fd.open(output_file_path.c_str());
    }

    if(sub_type=="TransformStamped")
    {
        sub = nh.subscribe(sub_topic, 2, TransformStamped_callback);
    }
    if(sub_type=="PointStamped")
    {
        sub = nh.subscribe(sub_topic, 2, PointStamped_callback);
    }
    if(sub_type=="PoseStamped")
    {
        sub = nh.subscribe(sub_topic, 2, PoseStamped_callback);
    }
    if(sub_type=="Odometry")
    {
        sub = nh.subscribe(sub_topic, 2, Odometry_callback);
    }

    ros::spin();

    return 0;
}
