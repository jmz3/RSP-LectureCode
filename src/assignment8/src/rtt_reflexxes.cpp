#include <assignment8/rtt_reflexxes.hpp>
#include <rtt/Component.hpp>
#include <ros/ros.h>

rtt_reflexxes::rtt_reflexxes(const std::string &name) : RTT::TaskContext(name),
                                                        port_output_joint_pos("Streaming Joint Pos")
{
    addPort("Output_Port", port_output_joint_pos);
    addOperation("GetJointPos", &rtt_reflexxes::GetJointPos, this, RTT::OwnThread);
    addOperation("SetJointPos", &rtt_reflexxes::SetJointPos, this, RTT::OwnThread);

    addProperty("robot_description", robot_description_name);
    addProperty("base", base_name);
    addProperty("tool", tool_name);
}

bool rtt_reflexxes::configureHook()
{

    rml = new ReflexxesAPI(6, getPeriod());

    ip = new RMLPositionInputParameters(6);
    op = new RMLPositionOutputParameters(6);

    ros::NodeHandle nh;
    std::string robot_description_value;

    nh.param(robot_description_name, robot_description_value, std::string());

    KDL::Tree tree;
    KDL::Chain chain;

    if (kdl_parser::treeFromString(robot_description_value, tree))
    {

        if (tree.getChain(base_name, tool_name, chain))
        {
            std::cout << chain.getNrOfJoints() << std::endl;
        }
    }

    return true;
}

bool rtt_reflexxes::startHook()
{

    for (int i = 0; i < 6; i++)
    {
        ip->CurrentPositionVector->VecData[i] = 1.0;
        ip->CurrentVelocityVector->VecData[i] = 0.0;
        ip->CurrentAccelerationVector->VecData[i] = 0.0;

        ip->MaxVelocityVector->VecData[i] = 1.0;
        ip->MaxAccelerationVector->VecData[i] = 1.0;
        ip->MaxJerkVector->VecData[i] = 10.0;

        ip->SelectionVector->VecData[i] = true;
        ip->TargetPositionVector->VecData[i] = 1.0;
        ip->TargetVelocityVector->VecData[i] = 0.0;
    }

    return true;
}

void rtt_reflexxes::updateHook()
{

    std_msgs::Float64MultiArray m_array;
    m_array.data.resize(6);

    int result = rml->RMLPosition(*ip, op, flag);

    *ip->CurrentPositionVector = *op->NewPositionVector;
    *ip->CurrentVelocityVector = *op->NewVelocityVector;
    *ip->CurrentAccelerationVector = *op->NewAccelerationVector;

    for(int i=0; i<6; i++){
        m_array.data[i] = op->GetNewPositionVectorElement(i);
    }

    port_output_joint_pos.write(m_array);
}

void rtt_reflexxes::stopHook() {}

void rtt_reflexxes::cleanupHook() {}

KDL::JntArray rtt_reflexxes::GetJointPos()
{

    KDL::JntArray q(6);
    for (int i = 0; i < 6; i++)
    {
        q(i) = ip->CurrentPositionVector->VecData[i];
    }

    return q;
}

void rtt_reflexxes::SetJointPos(const KDL::JntArray &q)
{
    for( int i=0; i<6; i++){
        ip->TargetPositionVector->VecData[i] = q(i);
    }
}

ORO_CREATE_COMPONENT(rtt_reflexxes)