#include <ros/ros.h>
#include <rtt/TaskContext.hpp>
#include <rtt/InputPort.hpp>
#include <rtt/OutputPort.hpp>

#include <std_msgs/Float64MultiArray.h>
#include <geometry_msgs/PoseStamped.h>
#include <kdl_parser/kdl_parser.hpp>
#include <kdl/jntarray.hpp>

#include <ReflexxesAPI.h>
#include <RMLPositionInputParameters.h>
#include <RMLPositionOutputParameters.h>
#include <RMLPositionFlags.h>

class rtt_reflexxes : public RTT::TaskContext
{

private:
    RTT::OutputPort<std_msgs::Float64MultiArray> port_output_joint_pos;
    std::string robot_string;

    ReflexxesAPI *rml;
    RMLPositionInputParameters *ip;
    RMLPositionOutputParameters *op;
    RMLPositionFlags flag;

    std::string robot_description_name;
    std::string base_name, tool_name;

public:
    rtt_reflexxes(const std::string &name);
    ~rtt_reflexxes(){}

    virtual bool configureHook();
    virtual bool startHook();
    virtual void updateHook();
    virtual void stopHook();
    virtual void cleanupHook();

    KDL::JntArray GetJointPos();
    void SetJointPos(const KDL::JntArray &q);
};
