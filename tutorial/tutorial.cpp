/*
 * Template.cpp
 * template file for communication between subsystems
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
ros::Publisher pub_template;
//ros::Subscriber sub_template;

/*void callbackTemplate (const geometry_msgs::Twist& msg)
{

  ROS_INFO_STREAM("Sending random velocity command:"
		  << " linear=" << msg.linear.x
		  << " angular=" << msg.angular.z);
  msg_pub = msg;
  
  // Publish the data immediately when the node receives the data
  //pub_template.publish(msg_pub);
}
*/


int main(int argc, char* argv[])
{
  // Initialization of the publisher
  ros::init(argc, argv, "comm_template");

  ros::NodeHandle nh_template;

  //pub_template = nh_template.advertise<geometry_msgs::Twist>("pub_template/data", 1000);
  pub_template = nh_template.advertise<geometry_msgs::Twist>("jackal_velocity_controller/cmd_vel", 1000);
  //sub_template = nh_template.subscribe ("pub_template/data",1000,&callbackTemplate);

  // For the tutorial purpose
  srand(time(0));
  ros::Rate rate(10);

  while(ros::ok())
    {
      msg_pub.linear.x = double(rand())/double(RAND_MAX);
      msg_pub.angular.z = 2*double(rand())/double(RAND_MAX)-1;
      pub_template.publish(msg_pub);

      ROS_INFO_STREAM("Sending random velocity command:"
		      << " linear=" << msg_pub.linear.x
		      << " angular=" << msg_pub.angular.z);
      rate.sleep();
    }
  // End of publishing a random velocity

  //ros::spin();

}

