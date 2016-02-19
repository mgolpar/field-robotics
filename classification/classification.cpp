/*
 * classification.cpp
 * program doing data classification for the data coming from 
 * tango_driver: 3D pose and point cloud
 * this is for AE598MRM class
 * Xinke Deng(UIUC)
 * 2016.2.10
 */
// I used twist for testing, please change it if you want to use another type of data

// ros related
#include <ros/ros.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Twist.h> 
#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <boost/foreach.hpp>
// please include all the message packages you want to use


// some useful libraries in C++
#include <iostream>
#include <sstream>
#include <iterator>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <time.h>

geometry_msgs::Twist msg_pub;
ros::Publisher pub_classification;

typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

void callback(const PointCloud::ConstPtr& msg)
{
     printf ("Cloud: width = %d, height = %d\n", msg->width, msg->height);
     BOOST_FOREACH (const pcl::PointXYZ& pt, msg->points)
     printf ("\t(%f, %f, %f)\n", pt.x, pt.y, pt.z);
}

void callback_Classification (const geometry_msgs::Twist& msg)
{

  //ROS_INFO_STREAM("Sending random velocity command:"
			//<< " linear=" << msg.linear.x
		  //<< " angular=" << msg.angular.z);
  msg_pub = msg;
  
  pub_classification.publish(msg_pub);
}


int main(int argc, char* argv[])
{
  // Initialization of the publisher
  ros::init(argc, argv, "classification");

  ros::NodeHandle nh_class;

  pub_classification = nh_class.advertise<geometry_msgs::Twist>("classification/data", 1000);
  ros::Subscriber sub_classification = nh_class.subscribe ("tango/data",1000,&callback_Classification);
  ros::Subscriber sub_pcl = nh_class.subscribe<PointCloud>("tango/pcl",1000,&callback);

  ros::spin();

}

