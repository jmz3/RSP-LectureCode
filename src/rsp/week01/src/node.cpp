#include <week01/publisher.hpp>

int main( int argc, char** argv ){

    ros::init( argc, argv, "publisher_node");
    ros::NodeHandle nh;
    // ros::NodeHandle nhp("~");

    rsp::publisher pub(nh);

    double rate;
    // double rate_priv;

    if( !nh.getParam("rate", rate) ){
        std::cout<<"error, no rate provided"<<std::endl;
    };
    // nhp.getParam("rate",rate_priv);
    
    ros::Rate r(rate);
    while(nh.ok()){

        pub.publish();
        r.sleep();
    }
    return 0;
}
