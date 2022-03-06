#include <ros/ros.h>
#include <my_action/actionlib_srv.hpp>

int main(int argc, char** argv){

    ros::init(argc,argv, "actionlib_srv");
    ros::NodeHandle nh;

    rsp::actionlib_srv stickandmove(nh);
    ros::spin();

    return 0;

}