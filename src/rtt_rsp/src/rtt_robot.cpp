#include <rtt_rsp/rtt_robot.hpp>
#include <rtt/Component.hpp>

rtt_robot::rtt_robot(const std::string &name) : TaskContext(name),
                                                port_msr_wrench("Measured Wrench"),
                                                port_cmd_cart_pos("Command position"),
                                                port_msr_jntstate("Measure joint state")
{

    std::cout << "rtt_robot" << std::endl;

    addPort("MsrWrench", port_msr_wrench);    // Give the port a name for calling it in the deployer
    addPort("CmdCartPos", port_cmd_cart_pos); // so that we can use it in the deployer like CmdCartPos ...
    addPort("MsrJntPos", port_msr_jntstate);

    addProperty("robot_description", robot_description_string);
    addProperty("base", base_string);// the value will be passed through "base" and stored in base_string
    addProperty("tool", tool_string); // passing the "base" and "tool" value can be done in the script ops file
}

bool rtt_robot::configureHook()
{

    rml = new ReflexxesAPI(1, getPeriod()); // practically we cannot set the joint numbers as 1, we need to parse the robot description
                                            // and find exactly how many joints should we have here
    ip = new RMLPositionInputParameters(1);
    op = new RMLPositionOutputParameters(1); // "1" means we only define one joint here

    ros::NodeHandle nh;
    std::string robot_description_value;

    nh.param(robot_description_string, robot_description_value, std::string());

    KDL::Tree tree;
    KDL::Chain chain;

    if (kdl_parser::treeFromString(robot_description_value, tree))
    {
        if (tree.getChain(base_string, tool_string, chain))
        {
            std::cout << chain.getNrOfJoints() << std::endl;
        }
    }
    return true;
}

bool rtt_robot::startHook()
{
    std::cout << "startup hook" << std::endl;

    // you have to preset the input state here
    // these values are either cartesian coordiantes or joint angles, they can be anything you like
    // the number is just to perform some interpolation

    ip->CurrentPositionVector->VecData[0] = 1.0; // set the first joint position at 1.0, if this is a real robot,
                                                 // you have to get the current robot state instead of giving it a value here
    ip->CurrentVelocityVector->VecData[0] = 0.0; // set the first joint velocity at 0
    ip->CurrentAccelerationVector->VecData[0] = 0.0;

    // This is motion limits, they should be set and leave untouched
    ip->MaxVelocityVector->VecData[0] = 1.0;
    ip->MaxAccelerationVector->VecData[0] = 1.0;
    ip->MaxJerkVector->VecData[0] = 10.0; // Jerk is the changing-rate of acceleration

    ip->SelectionVector->VecData[0] = true; // tell the program to calculate this all the time

    ip->TargetPositionVector->VecData[0] = 1.0; // To test on a real robot, you'd better set the start and target at the same position
    ip->TargetVelocityVector->VecData[0] = 0.0; // at begining so that the robot won't move and crash !!!!

    return true;
}
void rtt_robot::updateHook()
{

    geometry_msgs::PoseStamped pose;
    if (port_cmd_cart_pos.read(pose) == RTT::NewData) // RTT::NewData is set to 2 by rtt, when the read(pose) == 2 , means we are accepting
                                                      // new data here, if there is no new data, this line will give false
    {
        ip->TargetPositionVector->VecData[0] = pose.pose.position.x;
        ip->TargetVelocityVector->VecData[0] = 0.0;
    }

    geometry_msgs::WrenchStamped ft;
    if (port_msr_wrench.read(ft) == RTT::NewData)
    {
        std::cout << ft << std::endl;
    }

    int result = rml->RMLPosition(*ip, op, flags); // next step movement will be stored inside op

    *ip->CurrentPositionVector = *op->NewPositionVector;         // this part is for recursive process
    *ip->CurrentVelocityVector = *op->NewVelocityVector;         // put the output of last iteration as the input of next iteration
    *ip->CurrentAccelerationVector = *op->NewAccelerationVector; // This part connect different calculation epochs

    sensor_msgs::JointState js;
    js.header.stamp = ros::Time::now();
    js.name.push_back("q1");
    js.position.resize(1);
    js.velocity.resize(1);

    op->GetNewPositionVector(&js.position[0], sizeof(double));
    op->GetNewVelocityVector(&js.velocity[0], sizeof(double));

    port_msr_jntstate.write(js); // this is a rtt port, so we have to stream the result from deployer to a rostopic

    std::cout
        << op->NewPositionVector->VecData[0] << std::endl;
    // std::cout << ip->CurrentPositionVector->VecData[]
}
void rtt_robot::stopHook() {}
void rtt_robot::cleanupHook() {}

ORO_CREATE_COMPONENT(rtt_robot)