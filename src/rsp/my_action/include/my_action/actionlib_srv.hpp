#include <ros/ros.h>
#include <my_action/StickAndMoveAction.h>
#include <actionlib/server/simple_action_server.h>
#include <algorithm>

namespace rsp{

    class actionlib_srv{

    private:
        
        ros::NodeHandle nh;
        std::unique_ptr<actionlib::SimpleActionServer<my_action::StickAndMoveAction>>  as;
    
    public:

        actionlib_srv( ros::NodeHandle& nh);
        ~actionlib_srv();

        void goalCB();
    };
}