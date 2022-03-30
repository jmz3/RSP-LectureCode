#include <ros/ros.h>
#include <geometry_msgs/TwistStamped.h>
#include <std_msgs/Float64MultiArray.h>
#include <sensor_msgs/JointState.h> // KDL requires the joint state to do the math
#include <kdl/chainiksolvervel_pinv.hpp>
#include <kdl_parser/kdl_parser.hpp>

class RRMC
{

private:
    ros::NodeHandle nh;
    ros::Subscriber sub_vw;
    ros::Subscriber sub_js; // js for joint states
    ros::Publisher pub_qd;  // qd for q dot

    KDL::ChainIkSolverVel_pinv* ik_vel;// if you dont use the pointer* here, you will need to provide a chain object to it
    // But typically we dont have a chain object, we need to gain it from urdf 
    KDL::Tree tree;
    KDL::Chain chain;

    geometry_msgs::Twist vw;

public:
    RRMC(ros::NodeHandle &nh);
    ~RRMC();

    void callback_vw( const geometry_msgs::Twist& vw);
    void callback_js( const sensor_msgs::JointState& js);
};