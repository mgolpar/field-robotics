/*
 * tango_driver.cpp
 * to publish the real time 3D pose and point cloud come from Google Tango
 * this is for AE598MRM class
 * Xinke Deng(UIUC)
 * 2016.2.11
 */

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

int main(int argc, char* argv[])
{
  // Initialization of the publisher
  ros::init(argc, argv, "tango_driver");
  ros::NodeHandle tango_driver;
  ros::Publisher pub_tango = tango_driver.advertise<geometry_msgs::Twist>("tango/data", 1000);
  
  // For template testing
  srand(time(0));
  ros::Rate rate(2);

  while(ros::ok())
    {
      geometry_msgs::Twist msg;
      msg.linear.x = double(rand())/double(RAND_MAX);
      msg.angular.z = 2*double(rand())/double(RAND_MAX)-1;
      pub_tango.publish(msg);

      ROS_INFO_STREAM("Sending random velocity command:"
		      << " linear=" << msg.linear.x
		      << " angular=" << msg.angular.z);
      rate.sleep();
    }
    
  // End testing

}
