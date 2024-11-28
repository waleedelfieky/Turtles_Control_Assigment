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

//stop turtle function
void stopTurtle(const std::string& turtle_name, ros::Publisher& publisher, turtlesim::Pose turtle_pose);

//relative distance checker
void turtle_distance_checker(turtlesim::Pose turtle1_pose,turtlesim::Pose turtle2_pose, float threshold, ros::Publisher& publisher_one,ros::Publisher& publisher_two);
/*==============================================================*/



int main(int argc, char** argv) {
    // Initialize our ROS node
    ros::init(argc, argv, "Node_two");

    // Initialize our handler
    ros::NodeHandle Node_Two_handler;

    // Subscribers for turtles' poses
    ros::Subscriber turtle1_sub = Node_Two_handler.subscribe("/turtleOne/pose", 10, turtle1PoseCallback);
    ros::Subscriber turtle2_sub = Node_Two_handler.subscribe("/turtleTwo/pose", 10, turtle2PoseCallback);
    ros::Publisher pub_turtle_one = Node_Two_handler.advertise<geometry_msgs::Twist>("/turtleOne/cmd_vel", 10);
    ros::Publisher pub_turtle_two = Node_Two_handler.advertise<geometry_msgs::Twist>("/turtleTwo/cmd_vel", 10);

    // Define a loop rate (e.g., 10 Hz)
    ros::Rate loop_rate(1000000);

    while (ros::ok()) {
        turtle_distance_checker(turtles[0].pose,turtles[1].pose, 1.5, pub_turtle_one, pub_turtle_two);

        // Process subscriber callbacks
        ros::spinOnce();

        // Sleep to maintain the loop rate
        loop_rate.sleep();
    }

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

}

/*==============================================================*/
/*==============================================================*/


void turtle_distance_checker(turtlesim::Pose turtle1_pose,turtlesim::Pose turtle2_pose, float threshold, ros::Publisher& publisher_one,ros::Publisher& publisher_two)
{

	//std::cout<<"t1 x: "<< turtle1_pose.x<<" t1 y: "<<turtle1_pose.y<<" t1 theta: "<<turtle1_pose.theta<<std::endl;
	//std::cout<<"t1 x: "<< turtle2_pose.x<<" t1 y: "<<turtle1_pose.y<<" t2 theta: "<<turtle2_pose.theta<<std::endl;
	// Calculate the distance between the two turtles
        float dx = turtle2_pose.x - turtle1_pose.x;
        float dy = turtle2_pose.y - turtle1_pose.y;
        float distance = std::sqrt(dx * dx + dy * dy);
        if (distance <= threshold) {
        	if(turtles[0].isMoving==true){
            		stopTurtle("turtleOne", publisher_one, turtle1_pose);
        	}
        	else if(turtles[1].isMoving==true){

            		stopTurtle("turtleTwo", publisher_two, turtle2_pose);

        	}
   	 }
   	 if(turtle1_pose.x>10 || turtle1_pose.y>10 || turtle1_pose.x<1 || turtle1_pose.y<1){
   	 	stopTurtle("turtleOne", publisher_one, turtle1_pose);
   	 }
   	 if(turtle2_pose.x>10 || turtle2_pose.y>10 || turtle2_pose.x<1 || turtle2_pose.y<1){
   	 	stopTurtle("turtleTwo", publisher_two, turtle2_pose);
   	 }
}


/*==============================================================*/
/*==============================================================*/

void stopTurtle(const std::string& turtle_name, ros::Publisher& publisher, turtlesim::Pose turtle_pose)
{
    geometry_msgs::Twist velocity;
    //as velcity data structure contain two structs one for linear velocity and other for angular velocity
    velocity.linear.x = -turtle_pose.linear_velocity*0.4;
    velocity.angular.z = -turtle_pose.angular_velocity*0.4;

    //now publish this velcoities to our the topic cmd which will publish this data to the turtle afterwards
    publisher.publish(velocity);

}

/*function one that takes the turtle names and fill in the array with the data of each turtle*/

