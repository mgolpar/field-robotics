/*
 * Template.cpp
 * file for jackal control
 * this is for AE598MRM class
 * Xinke Deng(UIUC)
 * 2016.2.10
 */
// I used twist for testing, please change it if you want to use another type of data

// ros related
#include <ros/ros.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/Twist.h> 
#include <geometry_msgs/Pose2D.h>
#include <tf/transform_datatypes.h>
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

// state variables
geometry_msgs::Pose2D pose_curr;
// desired pose
geometry_msgs::Pose2D pose_des;
bool pose_des_updated = 0;
// control message
geometry_msgs::Twist vel_cmd;
double k_rho = 0.5;
double k_alpha = 1.2;
double k_beta = -0.4;
ros::Publisher pub_control;

// HELPER FUNCTIONS
#define PI_f 3.14159265f
#define PI   3.14159265

// Function to convert quaternion to euler angles 

void Planner_Quat2Euler_f(geometry_msgs::Quaternion quat_param, double& Roll, double& Pitch, double& Yaw)
{
	//variables
  float sqw = quat_param.w*quat_param.w;
  float sqx = quat_param.x*quat_param.x;
  float sqy = quat_param.y*quat_param.y;
  float sqz = quat_param.z*quat_param.z;
  float unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor

  //singularity tests
  float test = quat_param.x*quat_param.y + quat_param.z*quat_param.w;
  if (test > 0.499*unit) { // singularity at north pole
    Pitch = 2 * atan2f(quat_param.x,quat_param.w);
    Yaw = PI_f/2;
    Roll = 0;
    return;
  }
  if (test < -0.499*unit) { // singularity at south pole
    Pitch = -2 * atan2f(quat_param.x,quat_param.w);
    Yaw = -PI_f/2;
    Roll = 0;
    return;
  }

  //no singularity
  Pitch = atan2f(2*quat_param.y*quat_param.w-2*quat_param.x*quat_param.z , sqx - sqy - sqz + sqw);
  Yaw = asin(2*test/unit);
  Roll = atan2f(2*quat_param.x*quat_param.w-2*quat_param.y*quat_param.z , -sqx + sqy - sqz + sqw);
}

void ugv_kinematic_controller()
{
  double x_ = cos(pose_des.theta)*(pose_curr.x - pose_des.x) + sin(pose_des.theta)*(pose_curr.y - pose_des.y);
  double y_ = -sin(pose_des.theta)*(pose_curr.x - pose_des.x) + cos(pose_des.theta)*(pose_curr.y - pose_des.y);
  double theta_ = pose_curr.theta - pose_des.theta;
  double rho = sqrt(x_*x_ + y_*y_);
  double alpha = -theta_ + atan2f(-y_,-x_);
  double beta = -theta_ - alpha;

  if (alpha > PI)
	alpha = alpha - 2*PI;
  else if (alpha < -PI)
	alpha = alpha + 2*PI;

  if (beta > PI)
	beta = beta - 2*PI;
  else if (beta < -PI)
	beta = beta + 2*PI;

  vel_cmd.linear.x = k_rho * rho;
  vel_cmd.angular.z = -(k_alpha * alpha + k_beta * beta);

  if (vel_cmd.linear.x > 1)
	vel_cmd.linear.x = 1; 
  else if (vel_cmd.linear.y < -1)
	vel_cmd.linear.y = -1;

  if (vel_cmd.angular.z > 1)
	vel_cmd.angular.z = 1;
  else if (vel_cmd.angular.z < -1)
	vel_cmd.angular.z = -1;

  //std::cout << vel_cmd.linear.x << ", " << vel_cmd.angular.z << std::endl;

  //std::cout << std::abs(theta_) << std::endl;
  if((rho <= 0.1))
  {
	vel_cmd.linear.x = 0;
	vel_cmd.angular.z = 0;
  }

}

// Receive the current pose data from motion capture system
void poseMessageRecieved_mocap(const geometry_msgs::PoseWithCovarianceStamped & msg)
{
  pose_curr.x = msg.pose.pose.position.x;
  pose_curr.y = msg.pose.pose.position.y;
  
  geometry_msgs::Quaternion quad_qtn;
  double Roll_mocap, Pitch_mocap;
  quad_qtn.x = msg.pose.pose.orientation.z;
  quad_qtn.y = -msg.pose.pose.orientation.x;
  quad_qtn.z = -msg.pose.pose.orientation.y;
  quad_qtn.w = msg.pose.pose.orientation.w;

  tf::Quaternion q(quad_qtn.x, quad_qtn.y, quad_qtn.z, quad_qtn.w);
  tf::Matrix3x3 m(q);
  m.getRPY(Roll_mocap, Pitch_mocap, pose_curr.theta);
  //ROS_INFO_STREAM("quaternion:" << quad_qtn);
  //pose_curr.theta = atan2f(quad_qtn.z, quad_qtn.w)*2;   
  
  //std::cout << std::setprecision(2) << std::fixed << "CurrPose: x = (" << pose_curr.x << "), y = (" << pose_curr.y << "), theta = (" << pose_curr.theta << ")."  << std::endl;

  if(pose_des_updated)
    {
 	ugv_kinematic_controller();
        ROS_INFO_STREAM("send velocity command:"
    		  << " linear=" << vel_cmd.linear.x
    		  << " angular=" << -vel_cmd.angular.z); 
	pub_control.publish(vel_cmd);	
    }
  else
    {
	vel_cmd.linear.x = 0;
	vel_cmd.angular.z = 0;	
    }

 
  pose_des_updated = 0;
}


// Receive the waypoint command and update pose_des
void waypointReceived(const geometry_msgs::Pose2D & msg)
{

  pose_des.x = msg.x;
  pose_des.y = msg.y;
  pose_des.theta = msg.theta;
  pose_des_updated = 1;
  //std::cout << std::setprecision(2) << std::fixed << "DesnPose: x = (" << pose_des.x << "), y = (" << pose_des.y << "), theta = (" << pose_des.theta << ")."  << "\n";
}


int main(int argc, char* argv[])
{

  ros::init(argc, argv, "control_jackal");

  ros::NodeHandle nh_control_jackal;
  ros::NodeHandle nh;

  ros::Subscriber sub = nh.subscribe("/pub_mocap_data/data", 1000, & poseMessageRecieved_mocap);
  pub_control = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
  
  ros::Subscriber sub_waypoint = nh_control_jackal.subscribe("planning/data",1000,&waypointReceived);

  ros::spin();

}

