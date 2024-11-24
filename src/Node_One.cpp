#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Kill.h"
#include <turtlesim/Spawn.h>
#include "string.h"
#include <chrono>


/*==================================================================*/
/*==================================================================*/
/*define error states*/
enum killing_state{
    killed,
    notkilled
};

enum spawn_state{
    spawn_Done,
    spawn_failed
};
/*==================================================================*/
/*==================================================================*/




/*==================================================================*/
/*==================================================================*/
//function prototypes
/*
    function to kill the original turtle to place the both turtles later wherever we want
*/
enum killing_state turtle_killer(std::string turtle_name, ros::NodeHandle& NodeHandler_t);
/*
    this function to spawn the new turtle in place x and y with orientation theta
*/
enum spawn_state turtle_spawn(float x, float y, float theta, std::string turtle_name, ros::NodeHandle& NodeHandler_t);
/*
    this function will take both turtle names and the duration of which we want this function to be repeated and the handler to our node
    handler is needed so we can do publich the velocity in velocity cmd topic
*/
void controlTurtle(const std::string& turtle1_name, const std::string& turtle2_name, int duration, ros::NodeHandle& NodeHandler_t);
/*==================================================================*/
/*==================================================================*/



int main (int argc, char **argv){

    //initliaze our node
    ros::init(argc, argv, "Node_one");
    //initlize node handler
    ros::NodeHandle NodeHandler_t;
    turtle_killer("turtle1",NodeHandler_t);
    turtle_spawn(2,2,0,"turtleOne",NodeHandler_t);
    turtle_spawn(3,6,0,"turtleTwo",NodeHandler_t);
    //start node work
    controlTurtle("turtleOne", "turtleTwo", 1, NodeHandler_t){

    return 0;
}
/*==================================================================*/
/*==================================================================*/

enum killing_state turtle_killer(std::string turtle_name, ros::NodeHandle& NodeHandler_t)
{
    //define client with the data type of turtlesim::kill and passing to it the service name /kill
    ros::ServiceClient turtle_killer_client = NodeHandler_t.serviceClient<turtlesim::Kill>("/kill");
    //define the killing message request
    turtlesim::Kill node_killer_request;
    //assign it to the turtle name we want to kill
    node_killer_request.request.name=turtle_name;
    //call killer service and check weather if it has exectuted successfuly or not
    if (turtle_killer_client.call(node_killer_request)){
    	return killed;
    	}
    else{
        ROS_ERROR("Failed to call /kill service.");
        return notkilled;
    }

}

/*==================================================================*/
/*==================================================================*/

enum spawn_state turtle_spawn(float x, float y, float theta, std::string turtle_name, ros::NodeHandle& NodeHandler_t){
    // define service client with data type of spawn and assoicated with spawn service    
    ros::ServiceClient turtle_spawn_client = NodeHandler_t.serviceClient<turtlesim::Spawn>("/spawn");
    //varible of this type to set paramters before turtle become alive
	turtlesim::Spawn spawn_turtle_t;
    spawn_turtle_t.request.x=x;
    spawn_turtle_t.request.y=y;
    spawn_turtle_t.request.theta=theta;
    spawn_turtle_t.request.name=turtle_name;
    // now give the turtle live ! and check if the mission has been done successfuly
    if (turtle_spawn_client.call(spawn_turtle_t)){
    	return spawn_Done;
    	}
    else {
        ROS_ERROR("Failed to call /spawn service.");
        return spawn_failed;
    }
}

/*==================================================================*/
/*==================================================================*/

void controlTurtle(const std::string& turtle1_name, const std::string& turtle2_name, int duration, ros::NodeHandle& NodeHandler_t){
// first step is to define our publichers so we can publish velocites to any turtle later through veclocity cmd topic
    ///" + turtle1_name + "/cmd_vel is the topic name 
    //10 is the queue size 
    /*
        The queue size determines how many messages can be stored in a buffer (queue) if the subscribers are not able to process the messages as quickly as they are published.
        so in our case 10 means 10 messages to be send in the buffer
    */
    ros::Publisher pub_turtle1 = NodeHandler_t.advertise<geometry_msgs::Twist>("/" + turtle1_name + "/cmd_vel", 10);
    ros::Publisher pub_turtle2 = NodeHandler_t.advertise<geometry_msgs::Twist>("/" + turtle2_name + "/cmd_vel", 10);

    //define the data structure that we will use to sned velocities 
    geometry_msgs::Twist velocity;

    // run this process in a loop 
    while (ros::ok()) {
        //define a varibale to store the data interaction from user in it
        std::string selected_turtle;
        //define a varibale for our veclocities linear and angular to be assigned later to the instance velocity which is of a twist data type 
        double linear_velocity, angular_velocity;

        // Ask the user which turtle to control
        std::cout << "Which turtle do you want to control? (" << turtle1_name << "/" << turtle2_name << "): ";
        // get this data to our varibale
        std::cin >> selected_turtle;

        // Validate user input to check if he entered a non valid name
        if (selected_turtle != turtle1_name && selected_turtle != turtle2_name) {
            std::cout << "Invalid turtle name. Please try again." << std::endl;
            continue;
        }

        // Ask the user for linear and angular velocities to control the wanted turtle
        std::cout << "Enter linear velocity: ";
        std::cin >> linear_velocity;
        std::cout << "Enter angular velocity: ";
        std::cin >> angular_velocity;

        // Set the velocities to the turtle
        velocity.linear.x = linear_velocity;
        velocity.angular.z = angular_velocity;


        // Publish to the appropriate turtle's topic
        ros::Publisher selected_pub = (selected_turtle == turtle1_name) ? pub_turtle1 : pub_turtle2;

        std::cout << "Sending command to " << selected_turtle << " for " << duration << " second(s)..." << std::endl;

        // Apply velocities for the specified duration
        auto start_time = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_time).count() < duration) {
            selected_pub.publish(velocity);
            ros::Duration(0.1).sleep(); // Sleep for 100ms
        }

        // Stop the turtle after the duration
        velocity.linear.x = 0;
        velocity.angular.z = 0;
        selected_pub.publish(velocity);

        std::cout << selected_turtle << " has stopped. You can insert a new command." << std::endl;
    }




}
/*==================================================================*/
/*==================================================================*/