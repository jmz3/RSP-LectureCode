<h1 align="center"> SYSTEM PROGRAMMING WEEK 07 </h1>
<h2 align="center"> Orocos </h2>
<h5 align="right"> -- by Jeremy Zhang @ Johns Hopkins University</h5>

---

[TOC]



> KDL - Kinematics and Dynamics Library

# I. Orocos Project

This website is a home for **portable** C++ libraries for advanced machine and robot control.

![img](https://orocos.org/wp-content/uploads/2020/09/front-logos2.png)

The new documentation webpage is offered at [Orocos Documentation](https://docs.orocos.org/).

- [Kinematics and Dynamics Library (KDL)](https://docs.orocos.org/kdl/overview.html): an application independent framework for modeling and computation of kinematic chains.**All the tf things that robot state publisher gives is gained from the results of KDL.**
- [Bayesian Filtering Library (BFL)](https://docs.orocos.org/bfl/overview.html): an application independent framework for inference in Dynamic Bayesian Networks, i.e., recursive information processing and estimation algorithms based on Bayes’ rule.
- [Reduced Finite State Machine (rFSM)](https://docs.orocos.org/rfsm/overview.html): a small and powerful statechart implementation in Lua.
- [Instantaneous Task Specification using Constraints (iTaSC)](https://orocos.org/wiki/orocos/itasc-wiki): is a framework to generate robot motions by specifying constraints between (parts of) the robots and their environment.



## 1.1 KDL

- [Geometric Primitives](http://docs.ros.org/en/indigo/api/orocos_kdl/html/geomprim.html)
- [Kinematic Families](http://docs.ros.org/en/indigo/api/orocos_kdl/html/group__KinematicFamily.html) : functionality to build kinematic chains and access their kinematic  and dynamic properties, such as e.g. Forward and Inverse kinematics and  dynamics.
- [Motion](http://docs.ros.org/en/indigo/api/orocos_kdl/html/group__Motion.html) : functionality to specify motion trajectories of frames and kinematic chains, such as e.g. Trapezoidal Velocity profiles.
- KDLTK : the interface code to integrate [KDL](http://docs.ros.org/en/indigo/api/orocos_kdl/html/namespaceKDL.html) into the Orocos [Real-Time Toolkit](http://www.orocos.org/rtt/) (RTT). 



### 1.1.1 Geometric Primitives

- [KDL::Vector](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Vector.html) : 3D translational vector
- [KDL::Rotation](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Rotation.html) : different representations of SO(3)
- [KDL::Frame](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Frame.html) : combination of vectot and rotation
- [KDL::Twist](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Twist.html) : combination of vector and rotation 
- [KDL::Wrench](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Wrench.html)



### 1.1.2 Kinematic Families

KDL::chains is the class that contains sequence of joints in cpp. The joints can be defined in urdf or sdf.

KDL::ChainFkSloverPos class is the class that solve the foward kinematics, it is just a abstract class (  or a pure virtual class ) it contains some pure virtual functions. You cannot use the class by itself. What this class do it that it defines the rules that foward kinematics should follow.

KDL::ChainFkSloverPos_recursive : solve the serial forward kinematics recursively (from shoulder to elbow to hand )

And if you are not satisfied with the recursive class itself, you can develop another class based on the abstract class KDL::ChainFkSloverPos



[KDL::ChainIkSolverVel](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1ChainIkSolverVel.html) : the class that calculates the velocity of a frame

Inheritance diagram for KDL::ChainIkSolverPos:

![Inheritance graph](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1ChainIkSolverPos__inherit__graph.png)

LMA: Levenberg–Marquardt algorithm
NR: newton raphson method
NR_JL: newton raphson method with joint limits

[KDL::Jacobian](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Jacobian.html) : take care of this jacobian here, this is not the same as the **manipulator jacobian**, it's just the analytic Jacobian (just pure derivative of a homogeneous matrix) , it does not contain the inverse of a homogeneous matrix. Hence when you want to use these jacobian, you'd better take that into account and be careful.





# II. CPP code

## 2.1 resolve rate control (RRMC) package:

### rrmc.hpp:

```cpp
#include <ros/ros.h>
#include <geometry_msgs/TwistStamped.h>
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

public:
    RRMC(ros::NodeHandle &nh);
    ~RRMC();
};
```

---

Tree object is very handy for us to get a chain out of it instead of defining a chain by ourself.

[Tree](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html) : 

This class encapsulates a **tree** kinematic interconnection structure. It is built out of segments.   [More...](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#details)

```c++
# include <tree.hpp>
```

[List of all members.](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree-members.html)

| Public Member Functions                                      |                                                              |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| bool                                                         | [addChain](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#a64846c4151ff7ef97897ae4b462a23c4) (const [Chain](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Chain.html) &chain, const std::string &hook_name) |
| bool                                                         | [addSegment](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#a6a0b05c008c3d1d64696560bd30df666) (const [Segment](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Segment.html) &segment, const std::string &hook_name) |
| bool                                                         | [addTree](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#a8696ea29e3e98668f37b4ba937d0e9ba) (const [Tree](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html) &tree, const std::string &hook_name) |
| bool                                                         | [getChain](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#a45e93dab5e0ef6b390820904edf82bd5) (const std::string &chain_root, const std::string &chain_tip, [Chain](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Chain.html) &chain) const |
| unsigned int                                                 | [getNrOfJoints](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#a6e144e04e7e6de5117706c486f9a862e) () const |
| unsigned int                                                 | [getNrOfSegments](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#a1d38b2d77aa9d2c0c8981f183bb508f0) () const |
| SegmentMap::const_iterator                                   | [getRootSegment](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#a616a62b11fe403911a85f70448472889) () const |
| SegmentMap::const_iterator                                   | [getSegment](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#a1312d262efef26b7b8a5f8a75f28a3b9) (const std::string &segment_name) const |
| const [SegmentMap](http://docs.ros.org/en/indigo/api/orocos_kdl/html/namespaceKDL.html#a6bfcccbcc840654a88822ddd0ee6bdaa) & | [getSegments](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#a3ae13538500e949c7e84cb9d42f1ef6d) () const |
| [Tree](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html) & | [operator=](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#a0bae12b4952112c15c3647ae04dd35e9) (const [Tree](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html) &arg) |
|                                                              | [Tree](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#acdc877b83d656b85ac5e8d9a1f162f3c) (const std::string &[root_name](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#a44ddc18522a5cd2ea096dce215f309ec)="root") |
|                                                              | [Tree](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#adb37b7fbd9be35e875efc3c84af67090) (const [Tree](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html) &in) |
| virtual                                                      | [~Tree](http://docs.ros.org/en/indigo/api/orocos_kdl/html/classKDL_1_1Tree.html#a992c012dce434166eebaa759087e5fdb) () |

  

### rrmc.cpp file:

```cpp
#include <rrmc/rrmc.hpp>
#include <std_msgs/Float64MultiArray.h>


RRMC::RRMC( ros::NodeHandle& nh): nh(nh){

    sub_vw = nh.subscribe("cmd_vel", 10, &RRMC::callback_vw, this); 
    /*
    when dealing with twist it's common to name twist as cmd_vel, 10 is the buffer
    the point of subscribe is that it binds the callback function and this class together and 
    it allows you to listen constantly to the topic you specified without getting stuck at this line of code
    in other words, the other part of this class can run independently even though this nh.subscribe is running
    Through this callback, we put what we get from cmd_vel to the parameter we defined for the callback_vw
    Here we define the parameter as a "const geometry_msgs::Twist& vw", that means we put whatever we got into that parameter
    So you gotta make sure the parameter you defined matches the type that you want to subtract 
    */
    sub_js = nh.subscribe("joint_states", 10, &RRMC::callback_js, this);
    pub_qd = nh.advertise<std_msgs::Float64MultiArray >("command",10);

    std::string robot_description_string; 
    nh.param( "robot_description", robot_description_string, std::string() );

    if( kdl_parser::treeFromString( robot_description_string, tree )){// we want to build a tree from the string 
    // ang through this command we can do it and assign the value to the tree object

        if ( tree.getChain( "base", "tool0", chain))
        {
            ik_vel = new KDL::ChainIkSolverVel_pinv( chain ); // if you dont use the new pointer to 
            // create a object here, you will need to instantiate a ik_vel object at the beginning right next to nh(nh)
            // and again ik_vel will get the joint velocities through inverse jacobian 
        }
        else{
            ROS_ERROR("cannot get a chain");
        }
    }
    else{
        ROS_ERROR("cannot get a tree");
    }

}

RRMC::~RRMC(){ if( ik_vel) delete ik_vel; }

// Here are the callback functions :

void RRMC::callback_vw( const geometry_msgs::Twist& vw){

    std::cout << vw << std::endl;
    this->vw = vw;
}

void RRMC::callback_js( const sensor_msgs::JointState& js){

    KDL::JntArray q_in( js.position.size()); // give the joint_array a right size by fetching the size from js
    for( int i = 0; i < q_in.rows(); i++){

        q_in(i) = js.position[i]; // get joint positions and put it into a Jntarray, here we just ignore the wrong order of 
    } // KDL::JntArray is a class that can contain all the joint positions into one vector

    KDL::Twist kdl_vw;
    kdl_vw.vel.x( vw.linear.x );
    kdl_vw.vel.y( vw.linear.y );
    kdl_vw.vel.z( vw.linear.z );

    kdl_vw.rot.x( vw.angular.x );
    kdl_vw.rot.y( vw.angular.y );
    kdl_vw.rot.z( vw.angular.z );// donot expect kdl to automatically resize 

    KDL::JntArray qd_out( js.position.size() ); // qd_out here means the derivative of joint states, in other words, it's joint velovity
    // always remember to give the JntArray object a size otherwise the program will crash cause KDL would automatically do that for you
    
    ik_vel->CartToJnt(  q_in, kdl_vw, qd_out);// here we call the member fuction and have the joint velocities stored in the qd_out
    //the next step should be publish these "number" to ros

    std_msgs::Float64MultiArray msg_qd; // multiarray is the type that topic /joint_group_position accepts
    msg_qd.layout.dim.push_back( std_msgs::MultiArrayDimension() );
    msg_qd.layout.dim[0].stride = 1; // specify how many dimensions you need for this multidimensional array
    msg_qd.layout.data_offset = 0;

    for( int i = 0; i<qd_out.rows()  ; i++){

        msg_qd.data.push_back( qd_out(i) );
        
    }
}
```

If you don't new operate here, you will have to define the class as:

```cpp
RRMC::RRMC( ros::NodeHandle& nh): nh(nh), ik_vel(NULL){
    blah blah blah
}
// nh(nh) means in the runtime you give the variable a value of ros::NodeHandle nh
RRMC::~RRMC(){ if( ik_vel) delete ik_vel;}
```

 cmd_vel by default means the twist variables



> **One important thing to metion:**
>
> When you try to get the kinematics chain of Universal Robot through getChain, always remember that UR has a chaotic order of joints. Typically the order should be: First Joint -> Second Joint -> Third Joint -> Forth Joint -> Fifth Joint -> Sixth Joint, from base to hand. But UR's order is like:  3 -> 2-> 1 -> 4 -> 5 -> 6



### rrmc_node.cpp

this is the main function that instaniate a object of class rrmc

```cpp
#include <rrmc/rrmc.hpp>

int main ( int argc, char** argv){

    ros::init( argc, argv, "rrmc");

    ros::NodeHandle nh;

    RRMC rrmc(nh);

    ros::spin();

    return 0;
}
```



## 2.2 Insight about callback functions

Like what we did in the rrmc.cpp, callback functions has a very clear working field.

```cpp
{
    sub_vw = nh.subscribe("cmd_vel", 10, &RRMC::callback_vw, this); 
    /*
    when dealing with twist it's common to name twist as cmd_vel, 10 is the buffer
    the point of subscribe is that it binds the callback function and this class together and 
    it allows you to listen constantly to the topic you specified without getting stuck at this line of code
    in other words, the other part of this class can run independently even though this nh.subscribe is running
    Through this callback, we put what we get from cmd_vel to the parameter we defined for the callback_vw
    Here we define the parameter as a "const geometry_msgs::Twist& vw", that means we put whatever we got into that parameter
    So you gotta make sure the parameter you defined matches the type that you want to subtract 
    */
}
void RRMC::callback_vw( const geometry_msgs::Twist& vw){

    std::cout << vw << std::endl;
    this->vw = vw;
}

```



## 2.3 Run rrmc node

First launch a simple control environment

```bash
roslaunch spacenav_node classic.launch
```

Then run our rrmc node and remap our control command to what the spacenav node need

```bash
 rosrun rrmc rrmc_node cmd_vel:=/spacenav/twist
```

 And Remember, **NEVER PRINT** the result to the screen when you are dealing with the real robot !!!!!!!!!!!!!!!!!



## 2.4 Conclusion on KDL

Most of the time, what we are doing is just grab data and generate some KDL objects. And then call the KDL member functions to do the  math, and then we publish the result back to ros to tell the robot about its kinematics state.





# III. ROS control

ros can publish several kinds of control messages, like control through trajectory, through velocity, through joint torque...

```bash
$ rosrun rqt_controller_manager rqt_controller_manager
```

If you wanna everything to run smoothly, never print anything to screen and ban all the visualization process



## 3.1 How do we control a robot

Through Joint velocities ?

Through Joint torques ?

Through Joint position ?

All are possible through ros. But You can only use one at a time and other components will be get through different algorithms. For example, if you give the robot a target position and apply PID control. It will drive the robot to the position in an instance. 
