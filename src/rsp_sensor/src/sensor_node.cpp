#include <rsp_sensor/sensor.hpp>

int main( int argc, char** argv){

    ros::init(argc, argv, "sensor");
    ros::NodeHandle nh;
    sensor my_sensor(nh);

    while(nh.ok()){
        my_sensor.publish();
        ros::spinOnce();
    }
}