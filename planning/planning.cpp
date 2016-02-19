/*
 * Template.cpp
 * file for planning
 * this is for AE598MRM class
 * Xinke Deng(UIUC)
 * 2016.2.10
 */
// I used twist for testing, please change it if you want to use another type of data

// ros related
#include <ros/ros.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Twist.h> 
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
ros::Publisher pub_planning;

void callback_Planning (const geometry_msgs::Twist& msg)
{

  ROS_INFO_STREAM("Sending random velocity command:"
		  << " linear=" << msg.linear.x
		  << " angular=" << msg.angular.z);
  msg_pub = msg;
  
  pub_planning.publish(msg_pub);
}


int main(int argc, char* argv[])
{
  // Initialization of the publisher
  ros::init(argc, argv, "planning");

  ros::NodeHandle nh_planning;

  pub_planning = nh_planning.advertise<geometry_msgs::Twist>("planning/data", 1000);
  ros::Subscriber sub_planning = nh_planning.subscribe ("classification/data",1000,&callback_Planning);

  
  ros::spin();

}

