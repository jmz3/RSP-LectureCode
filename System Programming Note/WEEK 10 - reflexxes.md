

# I. Reflexxes Intro

Reflexxes is a trajectory library that really do the math.

Say we can know the full state of the robot in the start state. We know everything about the robot at this time, joint positions, joint velocities, joint torques. And we know where the robot should go and in what velocity at the goal state. So reflexxes will interpolate the intermidiate state for you.

Installation about reflexxes:![Screenshot from 2022-04-12 18-47-32](/home/jeremy/rsp_workspace/src/System Programming Note/Screenshot from 2022-04-12 18-47-32.png)

**Note**: <u>Try to keep your /usr/local/include directory tidy. Every time you add something like Eigen OpenCV and things like that, try to add them into corresponding sub files. Don't just put them all in the include without any classification.</u>



# II. Implement Reflexxes in rtt_robot 



## 2.1 Implement robot state interpolation

Based on  what we developed in week 09, we are now trying to have a rtt_robot stuff to send some control command to the robot.

rtt_robot.hpp

```cpp
#include <ros/ros.h>
#include <geometry_msgs/WrenchStamped.h>
#include <geometry_msgs/PoseStamped.h>
#include <sensor_msgs/JointState.h>
#include <realtime_tools/realtime_box.h>
#include <rtt/TaskContext.h>
#include <rtt/InputPort.hpp>
#include <rtt/OutputPort.hpp>
#include <ReflexxesAPI.h>
#include <RMLPositionInputParameters.h>
#include <RMLPositionOutputParameters.h>
#include <kdl/tree.hpp>
#include <kdl/chain.hpp>
#include <kdl_parser/kdl_parser.hpp>


class rtt_robot : public RTT::TaskContest
{

private:

    RTT::InputPort<geometry_msgs::WrenchStamped> port_msr_wrench;
    RTT::InputPort<geometry_msgs::PoseStamped> port_cmd_cart_pos;
    RTT::OutputPort<sensor_msgs::JointState> port_msr_jntstate;

    std::string robot_description_string;
    std::string base_string;
    std::string tool_string;

    ReflexxesAPI* rml;
    RMLPositionInputParameters* ip;
    RMLPositionOutputParameters* op;
    RMLPositionFlags flags;

public:
    rtt_robot( const std::string& name);
    ~rtt_robot(){} // I can wirte an empty method here, the only difference is that this part will not be part of the library

    virtual bool configureHook();
    virtual bool startHook();
    virtual void updateHook();
    virtual void stopHook();
    virtual void cleanupHook();
}
```





rtt_robot.cpp

```cpp
#include <rtt_rsp/rtt_robot.hpp>
#include <rtt/Component.hpp>

rtt_robot::rtt_robot(const std::string &name) : TaskContext(name),
                                                port_msr_wrench("Measured Wrench"),
                                                port_cmd_cart_pos("Command position"),
                                                port_msr_jntstate("Measure joint state")
{

    std::cout << "rtt_robot" << std::endl;

    addPort("Msrwrench", port_msr_wrench);    // Give the port a name for calling it in the deployer
    addPort("CmdCartPos", port_cmd_cart_pos); // so that we can use it in the deployer like CmdCartPos ...
    addPort("MsrJntPos", port_msr_jntstate);

    addProperty("robot_description ", robot_description_string);
    addProperty("base", base_string);
    addProperty("tool", tool_string);
}

bool rtt_robot::configureHook()
{

    rml = new ReflexxesAPI(1, getPeriod()); // practically we cannot set the joint numbers as 1, we need to parse the robot description
                                            // and find exactly how many joints should we have here
    ip = new RMLPositionInputParameters(1);
    op = new RMLPositionOutputParameters(1); // "1" means we only define one joint here

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

    ip->TargetPositionVector->VecData[0] = 3.0; // To test on a real robot, you'd better set the start and target at the same position
    ip->TargetVelocityVector->VecData[0] = 0.0; // at begining so that the robot won't move and crash !!!!

    return true;
}
void rtt_robot::updateHook(){

    int result = rml->RMLPosition(*ip, op, flags); // next step movement will be stored inside op

    *ip->CurrentPositionVector = *op->NewPositionVector;         // this part is for recursive process
    *ip->CurrentVelocityVector = *op->NewVelocityVector;         // put the output of last iteration as the input of next iteration
    *ip->CurrentAccelerationVector = *op->NewAccelerationVector; // This part connect different calculation epochs


    std::cout
        << op->NewPositionVector->VecData[0] << std::endl;
    // std::cout << ip->CurrentPositionVector->VecData[]
}
void rtt_robot::stopHook() {}
void rtt_robot::cleanupHook() {}

ORO_CREATE_COMPONENT(rtt_robot)
```



CMakeLists.txt

```cmake
include_directories(
  include

  /usr/local/include/reflexxes # this line exists because we didn't put the reflexxes headers directly under /usr/local/include. The include parser can't find it through "include" so we have to specify the DIR
  ${catkin_INCLUDE_DIRS}
)

orocos_component( rtt_sensor src/rtt_sensor.cpp ) # remember to add this every time you need a orocos code snippet
orocos_component( rtt_robot src/rtt_robot.cpp ) # remember to add this every time you need a orocos code snippet

target_link_libraries( rtt_sensor 
  ${catkin_LIBRARIES}
) # we need to put our class a library
target_link_libraries( rtt_robot 
  ${catkin_LIBRARIES}
  ReflexxesTypeII
)
```



## 2.2 set the target outside the cpp ( through terminal or other rostopic ... )

Now if you want to change the target in the middle of the program. If you wanna read the input and the output state from the robot, you have to do the following things:

```cpp
#include <rtt_rsp/rtt_robot.hpp>
#include <rtt/Component.hpp>

rtt_robot::rtt_robot(const std::string &name) : TaskContext(name),
                                                port_msr_wrench("Measured Wrench"),
                                                port_cmd_cart_pos("Command position"),
                                                port_msr_jntstate("Measure joint state")
{

    std::cout << "rtt_robot" << std::endl;

    addPort("Msrwrench", port_msr_wrench);    // Give the port a name for calling it in the deployer
    addPort("CmdCartPos", port_cmd_cart_pos); // so that we can use it in the deployer like CmdCartPos ...
    addPort("MsrJntPos", port_msr_jntstate);

    addProperty("robot_description ", robot_description_string);
    addProperty("base", base_string);
    addProperty("tool", tool_string);
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
    if (port_cmd_cart_pos.read(pose) == RTT::NewData) // RTT::NewData is set to 2, when the read(pose) == 2 , means we are accepting
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
```

in sensor.ops, publish the rtt port message to rostopic

```script
stream( "robot.MsrJntPos", ros.topic("/robot/joint_state"))
stream( "robot.CmdCartPos", ros.topic("/robot/command"))
```



Now when you rosrun the deployer, you can go to another terminal and launch a rqt_plot

```bash
$ rqt_plot
```

and Now you can use **rostopic pub** through the /robot/command to input some "target" to the robot and see how the reflexxes reacts to this command.



## 2.3 Link the robot control command with the sensor joint measurement

```ops
var ConnPolicy cp
connect( "robot.MsrWrench", "sensor.MsrWrench", cp)
```

