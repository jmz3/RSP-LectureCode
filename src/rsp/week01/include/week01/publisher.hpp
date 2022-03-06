#include <ros/ros.h>
#include <my_msgs/Triangle.h>

namespace rsp{
    // declaration of publisher class
    class publisher{
    
    private:
        ros::NodeHandle nh;
        ros::Publisher pub;

    public:
        publisher( ros::NodeHandle& nh );
        ~publisher();
        void publish();
    };
};