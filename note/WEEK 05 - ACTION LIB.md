<h1 align="center"> SYSTEM PROGRAMMING WEEK 05 </h1>
<h2 align = "center"> ACTION Service </h2>
<h5 align="right"> -- by Jeremy Zhang @ Johns Hopkins University</h5>


---

[TOC]

# I. Client Server Structure

Action Client and Action Server: references: __[action lib ros](http://wiki.ros.org/actionlib)__

better to separate every function in a single package, like message package, action package, service package and such.

## 1.1 .action file

example /action/DoDishes.action:

```yaml
# Define the goal
uint32 dishwasher_id  # Specify which dishwasher we want to use
---
# Define the result
uint32 total_dishes_cleaned
---
# Define a feedback message
float32 percent_complete
```

---

## 1.2 Create a message for a action file

We can define our own messages and implement them as the component of our action file.

In my_action package, mkdir a DIR called my_action/msg, there are three msg files, they are:

| message file name | function                                                     |
| ----------------- | ------------------------------------------------------------ |
| MyGoal.msg        | define the variable type and some default value for the goal of the action. |
| MyFeedback.msg    | define the 'return value' for the whole action               |
| MyResult.msg      | define the result of the action, whether it arrives, it fails, it runs out of power |

MyGoal.msg:

```yaml
geometry_msgs/Point coordinates
```

MyFeedback.msg:

```yaml
std_msgs/String message
```

MyResult.msg

```yaml
uint8 SUCCESS=0
uint8 FAILURE=0
uint8 POWER_OFF=2 

uint8 result_code
```

---

## 1.3 Create action file

in the my_action/action folder create a file called StickAndMove.action:

StickAndMove.action:

```yaml
my_action/MyGoal goal
---
my_action/MyResult result
---
my_action/MyFeedback feedback 
```

This action file instantiates the messages that we just created

> **Structure of the action file:** There are two triple dashes, they divide the code into 3 parts, the first one is the goal part the second one is the result part and the third one is the feedback part. For each part, you can put as many message as you want 

Here is an example with many messages 

```
#cartesian-move action interface
#minimally, it may contain just a command code
#more generally, it may contain desired tool-frame pose, as well
# as gripper pose (gripper opening, or vacuum gripper on/off)
# and an arrival time for the move
# It is assumed that a move starts from the previous commanded pose, or from the current joint state

#return codes provide status info, e.g. if a proposed move is reachable

#define message constants:  
uint8 ARM_TEST_MODE = 0

#queries
uint8 ARM_IS_SERVER_BUSY_QUERY = 1
uint8 ARM_QUERY_IS_PATH_VALID = 2
uint8 GET_TOOL_POSE = 5
uint8 GET_Q_DATA = 7

#requests for motion plans; 
uint8 PLAN_JSPACE_TRAJ_CURRENT_TO_WAITING_POSE=20
uint8 PLAN_JSPACE_TRAJ_CURRENT_TO_QGOAL = 21
uint8 PLAN_JSPACE_TRAJ_CURRENT_TO_CART_TOOL_POSE = 22 #plan a joint-space path from current arm pose to some IK soln of Cartesian goal

uint8 PLAN_CARTESIAN_TRAJ_CURRENT_TO_DES_TOOL_POSE = 23
uint8 PLAN_CARTESIAN_TRAJ_QSTART_TO_DES_TOOL_POSE = 24
uint8 PLAN_CARTESIAN_TRAJ_QPREV_TO_DES_TOOL_POSE = 25

uint8 CLEAR_MULTI_TRAJ_PLAN = 26
uint8 APPEND_MULTI_TRAJ_CART_SEGMENT = 27
uint8 APPEND_MULTI_TRAJ_JSPACE_SEGMENT = 28


uint8 REFINE_PLANNED_TRAJECTORY = 41 #if used approx IK soln, use this option to refine solns

# request to preview plan:
#uint8 DISPLAY_TRAJECTORY = 50

#MOVE command!
uint8 EXECUTE_PLANNED_TRAJ = 100
#specify a segment number to be executed from a multi-segment trajectory
uint8 EXECUTE_TRAJ_NSEG = 101

#uint8 ARM_DESCEND_20CM=101
#uint8 ARM_DEPART_20CM=102


#goal:
int32 command_code
geometry_msgs/PoseStamped des_pose_gripper
float64[] arm_dp #to command a 3-D vector displacement relative to current pose, fixed orientation
float64[] q_goal
float64[] q_start
int32 nsteps
float64 arrival_time
int32 nseg #to choose one of NSEG segments in a multi-traj vector
#float64 time_scale_stretch_factor
---
#result definition
#  response codes...
uint8 SUCCESS=0

uint8 ARM_RECEIVED_AND_INITIATED_RQST=1
uint8 ARM_REQUEST_REJECTED_ALREADY_BUSY=2
uint8 ARM_SERVER_NOT_BUSY=3
uint8 ARM_SERVER_IS_BUSY=4
uint8 ARM_RECEIVED_AND_COMPLETED_RQST=5
uint8 PATH_IS_VALID=6
uint8 PATH_NOT_VALID=7

uint8 COMMAND_CODE_NOT_RECOGNIZED=8
uint8 ARM_STATUS_UNDEFINED=9
uint8 NOT_FINISHED_BEFORE_TIMEOUT=10


int32 return_code
int32 err_code
string err_msg
float64 computed_arrival_time
float64[] q_arm
geometry_msgs/PoseStamped current_pose_gripper
---
#feedback: optional; 
int32 fdbk
```





---

## 1.4 Modify CMakeLists and package file

According the instruction in CMakelists, to generate message, you have to:

```cmake
################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a exec_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a exec_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)
```

**Remember this is applicable for all the msgs, actions, srvs**

And then, since you have a action file, you have to add the links in CMakeLists either like this:

```cmake
add_action_files(
  FILES
  StickAndMove.action
)
```



Afterwards, you can go back to terminal and catkin build this package. 



And in terminal

```bash
$ source devel/setup.bash
$ rosmsg list | grep my # will display all the messages that we just created
my_action/StickAndMoveAction
my_action/StickAndMoveActionFeedback
my_action/StickAndMoveActionGoal
my_action/StickAndMoveActionResult
my_action/StickAndMoveFeedback
my_action/StickAndMoveGoal
my_action/StickAndMoveResult
my_action/MyFeedback
my_action/MyGoal
my_action/MyResult
```

And now, the messages have been used to create header files in devel/include/my_action/ to make it available for the following cpp code.



# II. C++ Part for Server

## 2.1 Server header files actionlib_srv.hpp:

Refer to the documentation here __[actionlib::ActionServer](https://docs.ros.org/en/diamondback/api/actionlib/html/classactionlib_1_1ActionServer.html)__

But this one is pretty complicate. We choose the __[actionlib::SimpleActionServer](https://docs.ros.org/en/diamondback/api/actionlib/html/classactionlib_1_1SimpleActionServer.html)__ which is more handy to use.

In my_action/include/my_action create a header file called actionlib_srv.hpp

actionlib_srv.hpp:

```cpp
#include <ros/ros.h>
#include <my_action/StickAndMoveAction.h>
#include <actionlib/server/simple_action_server.h>
#include <algorithm>

namespace rsp{

    class actionlib_srv{

    private:
        
        ros::NodeHandle nh;
        std::unique_ptr<actionlib::SimpleActionServer<my_action::StickAndMoveAction>>  as; 
        // unique_ptr is a smart pointer
        // actionlib::SimpleActionServer<my_action::StickAndMoveAction is a templated class
    
    public:

        actionlib_srv( ros::NodeHandle& nh);
        ~actionlib_srv();

        void goalCB();
    };
}
```

Now we have the header file, we need to specify all the functions in its corresponding cpp files.



## 2.2 actionlib_srv.cpp:

```cpp
#include <my_action/actionlib_srv.hpp>


rsp::actionlib_srv::actionlib_srv( ros::NodeHandle& nh) :

    nh(nh){

        as.reset( new actionlib::SimpleActionServer<my_action::StickAndMoveAction>(nh, "stick_and_move", false));
        // it will create several topics called /stick_and_move/result, goal, feedback...  after the node starts     
        as->registerGoalCallback( boost::bind( &rsp::actionlib_srv::goalCB, this) );
        // registerGoalCallback is a callback function that tells you whether the goal is accepted or not, the callback function is defined in the following void rsp::actionlib_srv::goalCB()
        as->start(); 
// after start call, as will create some topics that publishes the msgs that we defined before like MyGoal MyFeedback ...
    }

void rsp::actionlib_srv::goalCB(){

    my_action::StickAndMoveGoal goal; 
    goal = *(as->acceptNewGoal()); // use the * to remove the reference because acceptNewGoal is a shared pointer

    std::cout << goal << std::endl; //print the goal

    my_action::StickAndMoveFeedback feedback;
    feedback.feedback.message.data = std::string("goal is accepted");
    as->publishFeedback(feedback);

    ros::Rate r(1);

    for(int i=0; i<5; i++){

        feedback.feedback.message.data = std::string("robot moving to ") + std::to_string(i);
        r.sleep();
        as->publisheFeedback( feedback );
    } // here we manually set some feedback for the server, pretending we are truly receiving something to test our code

    as->setSucceeded(); // send success flag to ros

}

rsp::actionlib_srv::~actionlib_srv(){}
```

 add the header file as a library in CMakeLists:

```cmake
include_directories(
  include # uncomment this to include the header file directory
  ${catkin_INCLUDE_DIRS}
)

add_library(myaction_srvcli # this one here is the name of the library
  src/actionlib_srv.cpp # this is the source code that the library refers to
)

add_executable(robot_action_node src/robot_action_node.cpp) 

target_link_libraries(myaction_srvcli # link the lib we just added to the catkin_LIBRARIES so that all the other code could find it easily
  ${catkin_LIBRARIES}
)
target_link_libraries(actionlib_srv_node
  ${catkin_LIBRARIES}
  myaction_srvcli
) # This link is to connect YOUR executable and YOUR library
```



## 2.3 actionlib_srv_node.cpp

This is a server node cpp file:

```cpp
#include <ros/ros.h>
#include <my_action/actionlib_srv.hpp>

int main(int argc, char** argv){

    ros::init(argc,argv, "actionlin_srv");
    ros::NodeHandle nh;

    rsp::actionlib_srv stickandmove(nh);
    ros::spin();

    return 0;

}
```

and in the CMakeLists:

```cmake
add_executable( actionlib_srv_node src/actionlib_srv_node.cpp) # make that main function into a .exe

target_link_libraries(actionlib_srv_node
  ${catkin_LIBRARIES}
  myaction_srvcli
) # add the target link to catkin_LIBRARIES, remember to include the previous myaction_srvcli
```



## 2.4 test

in the terminal

```bash
$ roscore # in terminal tab 1
$ rosrun xxx xxx_srv_node # in terminal tab 2

$ rostopic echo /stick_and_move/feedback # in terminal tab 3 remember to source the bash file before trying to operate self-created topics
$ rostopic pub -1 /stick_and_move/goal my_action/StickAndMoveActionGoal (+ TAB to display all the arguments)# in terminal tab 4 ti type in the values of the action messages
```

will allow you to set what to be published by the /stick_and_move/goal topic, when you do that, the cout << goal part in the callback function will catch that signal and print the message to screen. And echo will also print the message as we set in the cpp definition code at a rate of 1 Hz

```bash
$ rosrun my_action actionlib_srv_node 
goal: 
  coordinates: 
    x: 0
    y: 0
    z: 0
```



---

explain the rostopic pub -1 command:

```bash
rostopic pub -r 2 example_class_input_topic std_msgs / Float32 2.0
```

The rostopic command accepts arguments in YAML syntax. (See http://wiki.ros.org/ROS/YAMLCommandLine for more detail.) The option -r 2 means that the publication will be repeated with a rate of 2 Hz. After running this command, the output in terminal of
node example_ros_class appears as:

```bash
[ INFO] [1452374662.370650465]: myCallback activated: received value 2.000000
[ INFO] [1452374662.870560823]: myCallback activated: received value 2.000000
[ INFO] [1452374663.370577645]: myCallback activated: received value 2.000000
```



# III. C++ Part for Client

once you have the server part, you can always publish or test your code with terminal command like rostopic XXX, but you can also achieve this goal by having a client and using the template class called __[actionlib::SimpleActionClient](https://docs.ros.org/en/diamondback/api/actionlib/html/classactionlib_1_1SimpleActionClient.html)__ to do those "command line" things automactically.

## 3.1 actionlib_cli_node.cpp

```cpp
#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <my_action/StickAndMoveAction.h>

void done(  const actionlib::SimpleClientGoalState& state,
            const my_action::StickAndMoveResultConstPtr& result ){
                std::cout << result << std::endl;
    //  first parameter of this function is always the same since ros makes it so
    //  second parameter is our action stickandmove but it has to be a ConstPtr object
            }

void active(){
    std::cout << "Active" << std::endl;
}

void feedback(  const my_action::StickAndMoveFeedbackConstPtr& feedback){
    std::cout << *feedback << std::endl;
}

int main(int argc, char** argv)
{

    ros::init(argc, argv, "actionlib_cli_node");
    ros::NodeHandle nh;

    actionlib::SimpleActionClient<my_action::StickAndMoveAction> ac("stick_and_move");
    ac.waitForServer();

    my_action::StickAndMoveGoal goal;
    std::cout << goal << std::endl;

    goal.goal.coordinates.x = 1; //first "goal" is the object name, second "goal" is the message name defined in the /action/StickAndMove.action, coordinates is the message type (geometry_msgs/Point)
    goal.goal.coordinates.y = 1;
    goal.goal.coordinates.z = 1;

    ac.sendGoal( goal, &done, &active, &feedback); // goal is just the Goal object we defined, 

    // the following 3 lines are to wait for the result for 10 secs, and will print a meaage to tell you whether the result is received or not
    // bool ros = ac.waitForResult(  ros::Duration(10));
    // if( res) {std::cout << "Success" << std::endl;}
    // else { std::cout << "Failure" << std::endl;}

    ros::spin();

    return 0;
}
```

after creating this main function, modify the CMakeLists always.

```cmake
# add another add_executable for client node
add_executable( actionlib_srv_node src/actionlib_srv_node.cpp)
add_executable( actionlib_cli_node src/actionlib_cli_node.cpp) # create a new node

# add another target link
target_link_libraries(myaction_srvcli
  ${catkin_LIBRARIES}
)
target_link_libraries(actionlib_srv_node
  ${catkin_LIBRARIES}
  myaction_srvcli
)
target_link_libraries(actionlib_cli_node
  ${catkin_LIBRARIES}
  myaction_srvcli
) # this one here is the newly added link lib
```

Now simply run catkin build and rosrun these two nodes will give the same result as the 2.4 part shows.

## 3.2 Hierarchy of the clients

```cpp
//you can create several clients for different purpose
	nav_client.sendGoal(???);
    waitforResult();
    feedback.feedback.message.data = std::string("at the door");
    as->publishFeedback( feedback );

    opendoor_client.sendGoal(???);
    waitforResult();
    feedback.feedback.message.data = std::string("door opened");
    as->publishFeedback( feedback );
```

