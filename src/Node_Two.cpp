#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"



/*
    we need to implement follwoing functions
    Read Pose of Both Turtles
    Publish poses to a topic
    Check relative distances between turtles
    Threehold_checker_STOP
    Check_Boundary_condition_stop

    first we need to define array of struct of 2 and this struct contiant the elements of pose
        get it from pose topic 
*/

/*step 1 define a struct to hold the turtles data*/

// Define a struct for turtle data
struct TurtleData {
    turtlesim::Pose pose;           // Pose of the turtle
    geometry_msgs::Twist velocity;  // Current velocity of the turtle
    bool isMoving;                  // Status of the turtle (moving or stopped)
};


// Global array to store data for both turtles
TurtleData turtles[2]{};


// Callback function for Turtle1's pose
void turtle1PoseCallback(const turtlesim::Pose::ConstPtr& msg) {
    turtles[0].pose = *msg;
    ROS_INFO("Turtle1 Pose: x = %.2f, y = %.2f, theta = %.2f", 
             msg->x, msg->y, msg->theta);
}

// Callback function for Turtle2's pose
void turtle2PoseCallback(const turtlesim::Pose::ConstPtr& msg) {
    turtles[1].pose = *msg;
    ROS_INFO("Turtle2 Pose: x = %.2f, y = %.2f, theta = %.2f", 
             msg->x, msg->y, msg->theta);
}



int main (int argc, char **argv){
    // Initialize our ROS node
    ros::init(argc, argv, "Node_two");
    //intlilize our handler
    ros::NodeHandle Node_Two_handler;

    // Subscribers for turtles' poses
    ros::Subscriber turtle1_sub = Node_Two_handler.subscribe("/turtleOne/pose", 10, turtle1PoseCallback);
    ros::Subscriber turtle2_sub = Node_Two_handler.subscribe("/turtleTwo/pose", 10, turtle2PoseCallback);
    
    // std::cout<<"turtle_one x: "<<turtles[0].pose.x<<" and y: "<<turtles[0].pose.y<<" and theta: "<< turtles[0].pose.theta<<std::endl;
    // std::cout<<"turtle_two x: "<<turtles[1].pose.x<<" and y: "<<turtles[1].pose.y<<" and theta: "<< turtles[1].pose.theta<<std::endl;
    // Spin to process callbacks
    ros::spin();

    return 0;
}



/*function one that takes the turtle names and fill in the array with the data of each turtle*/

