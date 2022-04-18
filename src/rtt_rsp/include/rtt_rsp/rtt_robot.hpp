#include <ros/ros.h>
#include <geometry_msgs/WrenchStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/JointState.h>
#include <realtime_tools/realtime_box.h>
#include <rtt/TaskContext.hpp>
#include <rtt/InputPort.hpp>
#include <rtt/OutputPort.hpp>
#include <ReflexxesAPI.h>
#include <RMLPositionInputParameters.h>
#include <RMLPositionOutputParameters.h>
#include <kdl/tree.hpp>
#include <kdl/chain.hpp>
#include <kdl_parser/kdl_parser.hpp>

class rtt_robot : public RTT::TaskContext {

private:
    RTT::InputPort<geometry_msgs::WrenchStamped> port_msr_wrench;
    RTT::InputPort<geometry_msgs::PoseStamped> port_cmd_cart_pos;
    RTT::OutputPort<sensor_msgs::JointState> port_msr_jntstate;

    std::string robot_description_string;
    std::string base_string;
    std::string tool_string;

    ReflexxesAPI *rml; // rml is short for reflexxes motion library
    RMLPositionInputParameters *ip;
    RMLPositionOutputParameters *op;
    RMLPositionFlags flags;

public:
    rtt_robot(const std::string &name);
    ~rtt_robot() {} // I can wirte an empty method here, the only difference is that this part will not be part of the library

    virtual bool configureHook();
    virtual bool startHook();
    virtual void updateHook();
    virtual void stopHook();
    virtual void cleanupHook();
};