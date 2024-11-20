#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Kill.h"
#include <turtlesim/Spawn.h>


enum killing_state{
    killed,
    notkilled
};

enum spawn_state{
    spawn_Done,
    spawn_failed
};



#include "string.h"


enum killing_state turtle_killer(std::string turtle_name, ros::NodeHandle& NodeHandler_t);
enum spawn_state turtle_spawn(float x, float y, float theta, std::string turtle_name, ros::NodeHandle& NodeHandler_t);

//define struct for the turtles data

// struct turtle_t {
//     string name;
//     float32 linear_velocity;
//     float32 angular_velocity;
// }


int main (int argc, char **argv){

    //initliaze our node
    ros::init(argc, argv, "Node_one");
    //initlize node handler
    ros::NodeHandle NodeHandler_t;
    turtle_killer("turtle1",NodeHandler_t);
    turtle_spawn(2,2,0,"turtle2",NodeHandler_t);
    return 0;
}

enum killing_state turtle_killer(std::string turtle_name, ros::NodeHandle& NodeHandler_t);
{
    //define client with the data type of turtlesim::kill and passing to it the service name /kill
    ros::ServiceClient turtle_killer_client = NodeHandler_t.serviceClient<turtlesim::Kill>("/kill");
    turtlesim::Kill node_killer_request;
    node_killer_request.name=turtle_name;
    //call killer service
    turtle_killer_client.call(node_killer_request);

    return killed;

}


enum spawn_state turtle_spawn(float x, float y, float theta, std::string turtle_name, ros::NodeHandle& NodeHandler_t){
    // define service client with data type of spawn and assoicated with spawn service    
    ros::ServiceClient turtle_spawn_client = NodeHandler_t.serviceClient<turtlesim::Spawn>("/spawn");
    //varible of this type to set paramters before turtle become alive
	turtlesim::Spawn spawn_turtle_t;
    spawn_turtle_t.x=x;
    spawn_turtle_t.y=y;
    spawn_turtle_t.theta=theta;
    spawn_turtle_t.name=turtle_name;
    // now give the turtle live !
    turtle_client.call(spawn_turtle_t);
    return spawn_Done;
}
