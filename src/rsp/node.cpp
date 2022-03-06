#include </week01/pulisher.hpp>

int main( int argc, char** argv ){

    ros::init( argc, argv, "publisher_node")
    ros::NodeHandle nh;

    rsp::publisher pub(nh);

    ros::Rate r(10);
    while(nh.ok()      ){

        pub.publish();
        r.sleep()
    }
    return 0;
}