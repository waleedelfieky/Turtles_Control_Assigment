#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"

/*==============================================================*/
/*step 1 define a struct to hold the turtles data*/

// Define a struct for turtle data
struct TurtleData {
    turtlesim::Pose pose;           // Pose of the turtle
    geometry_msgs::Twist velocity;  // Current velocity of the turtle
    bool isMoving;                  // Status of the turtle (moving or stopped)
};
/*==============================================================*/


/*==============================================================*/
// Global array to store data for both turtles
TurtleData turtles[2]{};

// Callback function for Turtle1's pose
void turtle1PoseCallback(const turtlesim::Pose::ConstPtr& msg);
// Callback function for Turtle2's pose
void turtle2PoseCallback(const turtlesim::Pose::ConstPtr& msg);
// relative distance checker
//void turtle_relative_distance_checker(const turtlesim::Pose::ConstPtr& turtle1_pose,const turtlesim::Pose::ConstPtr& turtle2_pose, float threshold);
/*==============================================================*/



int main (int argc, char **argv){
    // Initialize our ROS node
    ros::init(argc, argv, "Node_two");
    //intlilize our handler
    ros::NodeHandle Node_Two_handler;

    // Subscribers for turtles' poses
    ros::Subscriber turtle1_sub = Node_Two_handler.subscribe("/turtleOne/pose", 10, turtle1PoseCallback);
    ros::Subscriber turtle2_sub = Node_Two_handler.subscribe("/turtleTwo/pose", 10, turtle2PoseCallback);
    //since we have the pose now we want to check the relative distance first

    ros::spin();

    return 0;
}

/*==============================================================*/
/*==============================================================*/
void turtle1PoseCallback(const turtlesim::Pose::ConstPtr& msg) {
    turtles[0].pose = *msg;
    if(msg->linear_velocity==0 && msg->angular_velocity==0){
        turtles[0].isMoving=false;
    }
    else {
        turtles[0].isMoving=true;
    }
    std::cout<<"turtle_one is: " <<turtles[0].isMoving<<std::endl;
}
/*==============================================================*/
/*==============================================================*/

// Callback function for Turtle2's pose
void turtle2PoseCallback(const turtlesim::Pose::ConstPtr& msg) {
    turtles[1].pose = *msg;
    if(msg->linear_velocity==0 && msg->angular_velocity==0){
        turtles[1].isMoving=false;
    }
    else {
        turtles[1].isMoving=true;
    }
    std::cout<<"turtle_two is: " <<turtles[1].isMoving<<std::endl;
}

/*==============================================================*/
/*==============================================================*/

// void turtle_relative_distance_checker(const turtlesim::Pose::ConstPtr& turtle1_pose,const turtlesim::Pose::ConstPtr& turtle2_pose, float threshold)
// {
//     /*calcualte distance*/

//     /*if postive check threehold T1 to T2 as T1 is the one that is behind*/

//     /*if negative convert first the number to be postive then
//     check threehold from T2 to T1
//     */
// }

/*==============================================================*/
/*==============================================================*/

/*function one that takes the turtle names and fill in the array with the data of each turtle*/

