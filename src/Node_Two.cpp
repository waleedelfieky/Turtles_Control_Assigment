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
/*==============================================================*/
//stop turtle function
void stopTurtle(const std::string& turtle_name, ros::NodeHandle& NodeHandler_t);
// relative distance checker
void turtle_relative_distance_checker(const turtlesim::Pose::ConstPtr& turtle1_pose,const turtlesim::Pose::ConstPtr& turtle2_pose, float threshold);

ros::NodeHandle Node_Two_handler;

int main (int argc, char **argv){
    // Initialize our ROS node
    ros::init(argc, argv, "Node_two");
    //intlilize our handler
    // Subscribers for turtles' poses
    ros::Subscriber turtle1_sub = Node_Two_handler.subscribe("/turtleOne/pose", 10, turtle1PoseCallback);
    ros::Subscriber turtle2_sub = Node_Two_handler.subscribe("/turtleTwo/pose", 10, turtle2PoseCallback);
    //since we have the pose now we want to check the relative distance first
    turtle_relative_distance_checker(turtles[0].pose ,turtles[1].pose, 1)


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
//function to stop the turtle that is moving by passing the turtle name along with the node handeler
void stopTurtle(const std::string& turtle_name, ros::NodeHandle& NodeHandler_t)
{
    ros::Publisher pub_turtle = NodeHandler_t.advertise<geometry_msgs::Twist>("/" + turtle_name + "/cmd_vel", 10);
    geometry_msgs::Twist velocity;
    //as velcity data structure contain two structs one for linear velocity and other for angular velocity
    velocity.linear.x = 0;
    velocity.angular.z = 0;
    //now publish this velcoities to our the topic cmd which will publish this data to the turtle afterwards
    pub_turtle.publish(velocity);

}
/*==============================================================*/
/*==============================================================*/

void turtle_relative_distance_checker(const turtlesim::Pose::ConstPtr& turtle1_pose,const turtlesim::Pose::ConstPtr& turtle2_pose, float threshold)
{
    // Calculate the distance between the two turtles
    float dx = turtle2_pose->x - turtle1_pose->x;
    float dy = turtle2_pose->y - turtle1_pose->y;
    float distance = std::sqrt(dx * dx + dy * dy);

    // Check if the distance exceeds the threshold
    if (distance > threshold) {

        if(turtles[0].isMoving=true){
            stopTurtle("turtleOne", Node_Two_handler)

        }
        else if(turtles[1].isMoving=true){
            stopTurtle("turtleTwo", Node_Two_handler)

        }
    } 
}

/*==============================================================*/
/*==============================================================*/

/*function one that takes the turtle names and fill in the array with the data of each turtle*/

