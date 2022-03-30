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

## 2.1 resolve rate control package:

rrmc.hpp:

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

  

in the rrmc.cpp file:

```cpp
#include <rrmc/rrmc.hpp>

RRMC::RRMC( ros::NodeHandle& nh): nh(nh){

    std::string robot_description_string; 
    nh.param( "robot_description", robot_description_string, std::string() );

    if( kdl_parser::treeFromString( robot_description_string, tree )){// we want to build a tree from the string 
    // ang through this command we can do it and assign the value to the tree object

        if ( tree.getChain( "base", "tool0", ))
        {
            ik_vel = new KDL::ChainIkSolverVel_pinv( chain ); // if you dont use the new pointer to create a object here, you will need to instantiate a ik_vel object at the beginning right next to nh(nh)
        }
        else{
            ROS_ERROR("cannot get a chain");
        }
    }
    else{
        ROS_ERROR("cannot get a tree");
    }

}

RRMC::~RRMC(){};
```

If you don't new operate here, you will have to define the class as:

```cpp
RRMC::RRMC( ros::NodeHandle& nh): nh(nh), ik_vel(NULL){
    blah blah blah
}
RRMC::~RRMC(){ if( ik_vel) delete ik_vel;}
```

cmd_vel by default means the twist variables



# III. ROS control

ros can publish several kinds of control messages, like control through trajectory, through velocity, through joint torque...

```bash
$ rosrun rqt_controller_manager rqt_controller_manager
```



If you wanna everything to run smoothly, never print anything to screen and ban all the visualization process

