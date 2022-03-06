#include <week01/publisher.hpp>
#include <my_msgs/Triangle.h>

rsp::publisher::publisher(ros::NodeHandle& nh):
    nh( nh ){

    pub = nh.advertise<my_msgs::Triangle>("headline",10);

}

rsp::publisher::~publisher(){}

void rsp::publisher::publish(){

    std::cout<<"foobar"<<std::endl;

    my_msgs::Triangle t;
    t.p1.x = 0;
    t.p1.y = 0;
    t.p1.z = 0;

    t.p2.x = 0;
    t.p2.y = 0;
    t.p2.z = 0;

    t.p3.x = 0;
    t.p3.y = 0;
    t.p3.z = 0;

    pub.publish( t );
}