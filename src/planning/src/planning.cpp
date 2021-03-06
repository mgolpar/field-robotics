/*
 * Template 0.2 for basic communication between nodes.
 *
 * This template creates a class that contains a publisher and a subscriber.
 * Refer to temp_class.cpp and temp_class.h for detailed info.
 * 
 * Chad Li
 * 2016.03.03
*/

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <iostream>

using namespace std;

class planning_class {
private:
    ros::NodeHandle n;
    ros::Publisher pub;
    ros::Subscriber sub;

    // Just for testing
    geometry_msgs::Twist vel;

public:
    planning_class() {
        pub = n.advertise<geometry_msgs::Twist>("planning/data",1000);
	sub = n.subscribe("classification/data",1000,&planning_class::PoseCallback,this);
        // For testing
	x=0;
	y=0;
        // -------------
	ROS_INFO_STREAM("initialized");
    }
    ~planning_class() {
    }
    double x;
    double y;

    void setVel(const double x, const double y);
    void PoseCallback(const geometry_msgs::Twist& msg) {
	ROS_INFO_STREAM("I received " << "position=(" << msg.linear.x << ", " << msg.linear.y << ")");
    }

};


void planning_class::setVel(const double x, const double y) {
    vel.linear.x = x;
    vel.linear.y = y;
    pub.publish(vel);
    ROS_INFO_STREAM("publishing velocity command: " << "linear (x)=" << vel.linear.x << ", angular (z)=" << vel.linear.y);  
}

int main(int argc, char **argv) {
    // (1) Assign a name to your node
    // Place where you initialize your node, remember all nodes should have
    // different names, here the node name is "test_node_master"
    ros::init(argc, argv, "planning");

    // Create a "class" with name of your choice, here the name is "temp2"
    // Define a class which contains a publisher and a subscriber, for more
    // details please refer to temp_class.h
    planning_class planning;
    // Setting the rate of this node, paired with r.sleep(); below to ensure
    // rate.
    ros::Rate r(10);

    while(ros::ok()){
        // -----------------Put your code below-------------------------------
        cout <<"enter x" << endl;
        cin >> planning.x;
	cout <<"enter y" << endl;
	cin >> planning.y;
        planning.setVel(planning.x,planning.y);
        // -----------------Put your code above--------------------------------
        // Don't care, don't change unless you know what you are doing
	ros::spinOnce();
	r.sleep();
        // --------------------------------------------------------------------
    }
    return 0;	
}
