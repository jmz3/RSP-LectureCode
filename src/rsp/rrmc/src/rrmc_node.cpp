#include <rrmc/rrmc.hpp>

int main ( int argc, char** argv){

    ros::init( argc, argv, "rrmc");

    ros::NodeHandle nh;

    RRMC rrmc(nh);

    ros::spin();

    return 0;
}