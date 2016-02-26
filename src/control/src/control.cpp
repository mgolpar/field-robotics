/*
 * Template.cpp
 * file for jackal control
 * this is for AE598MRM class
 * Xinke Deng(UIUC)
 * 2016.2.10
 * Chad
 * 2016.02.25
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

ros::Publisher cmd_vel;
geometry_msgs::Twist cmd_velocity;

void callback_Control (const geometry_msgs::Twist& msg)
{

  ROS_INFO_STREAM("receive random velocity command:"
		  << " linear=" << msg.linear.x
		  << " angular=" << msg.angular.z);
	cmd_velocity = msg;
	//cmd_velocity.linear.x = 1;
	cmd_vel.publish(cmd_velocity);
}


int main(int argc, char* argv[])
{

  ros::init(argc, argv, "control");

  ros::NodeHandle nh_control_jackal;

  cmd_vel = nh_control_jackal.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
  ros::Subscriber sub_control = nh_control_jackal.subscribe ("planning/data",1000,&callback_Control);
  
  //cmd_velocity.linear.x = 5;
  cmd_vel.publish(cmd_velocity);
	
  while(ros::ok())
  {

	ros::spinOnce();
  }

  
}

