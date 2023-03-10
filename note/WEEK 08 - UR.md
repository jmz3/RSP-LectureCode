<h1 align="center"> SYSTEM PROGRAMMING WEEK 08 </h1>
<h2 align="center"> Universal Robot </h2>
<h5 align="right"> -- by Jeremy Zhang @ Johns Hopkins University</h5>



[TOC]

# I. Installation for UR packages on ROS-melodic

following the github repos to install the ur packages from source code. UR no longer supports installing from sudo apt get command for melodic.

Instructions are here: [https://github.com/UniversalRobots/Universal_Robots_ROS_Driver](https://github.com/UniversalRobots/Universal_Robots_ROS_Driver)

One thing to metion, we clone the repo to the rsp_workspace instead of catkin_ws, which is shown on the README.md. Here is my installation scripts:

```bash
$ cd rsp_workspace

$ git clone https://github.com/UniversalRobots/Universal_Robots_ROS_Driver.git src/Universal_Robots_ROS_Driver

# clone fork of the description. This is currently necessary, until the changes are merged upstream.
$ git clone -b calibration_devel https://github.com/fmauch/universal_robot.git src/fmauch_universal_robot

# install dependencies
$ sudo apt update -qq
$ rosdep update
$ rosdep install --from-paths src --ignore-src -y

$ catkin build
```

After installation, you'll find two folders:

```bash
Universal_Robots_ROS_Driver
fmauch_universal_robot 
```

If you look into the fmauch_universal_robot folder, you'll find several different ur robot



# II. Enable UR velocity control

## 2.1 Start a new UR simulation

in the terminal:

```bash
$ roslaunch ur_gazebo ur5_bringup.launch
```

will start a simple ur5 simulation in gazebo



Look up to the description launch file :  ../fmauch_universal_robot/ur_description/launch/load_ur.launch

```xml
<?xml version="1.0"?>
<launch>
  <!--ur parameters files -->
  <arg name="joint_limit_params" doc="YAML file containing the joint limit values"/>
  <arg name="kinematics_params" doc="YAML file containing the robot's kinematic parameters. These will be different for each robot as they contain the robot's calibration."/>
  <arg name="physical_params" doc="YAML file containing the phycical parameters of the robots"/>
  <arg name="visual_params" doc="YAML file containing the visual model of the robots"/>
  <!--common parameters  -->
  <arg name="transmission_hw_interface" default="hardware_interface/PositionJointInterface" /><!-- This is a hardware interface-->
  <arg name="safety_limits" default="false" doc="If True, enable the safety limits controller"/>
  <arg name="safety_pos_margin" default="0.15" doc="The lower/upper limits in the safety controller" />
  <arg name="safety_k_position" default="20" doc="Used to set k position in the safety controller" />

  <!-- Load the top-level (ie: stand-alone and complete) xacro for the UR
       variant defined by the set of yaml parameter files (so to load a UR5
       onto the ROS parameter server, provide paths to the .yaml files which
       contain the limits, kinematics, physical and visual parameters which
       together describe a UR5 robot.

       NOTE: users will typically want to use use one of the other .launch files
       in this directory (ie: 'load_urXXX.launch'), as those already contain
       appropriate default values for the required arguments for the various
       supported robots.

       NOTE2: if you have a custom robot configuration, or your robot is
       integrated into a work cell, do NOT change this file or add all of the
       work cell objects to the ur.xacro file. Create a new top-level xacro and
       include the ur_macro.xacro file into it. Then write a new .launch file
       to load it onto the parameter server.
  -->
  <param name="robot_description" command="$(find xacro)/xacro '$(find ur_description)/urdf/ur.xacro'
    joint_limit_params:=$(arg joint_limit_params)
    kinematics_params:=$(arg kinematics_params)
    physical_params:=$(arg physical_params)
    visual_params:=$(arg visual_params)
    transmission_hw_interface:=$(arg transmission_hw_interface)
    safety_limits:=$(arg safety_limits)
    safety_pos_margin:=$(arg safety_pos_margin)
    safety_k_position:=$(arg safety_k_position)"
    />
</launch>

```



## 2.1 Enable a velocity controller for ur5

<u>step 1: go to ur5_bringup.launch</u>

in fmauch_universal_robot/ur_gazebo/launch/ur5_bringup.launch

```xml
  <include file="$(arg robot_description_file)">
    <arg name="joint_limit_params" value="$(arg joint_limit_params)"/>
    <arg name="kinematics_params" value="$(arg kinematics_params)"/>
    <arg name="physical_params" value="$(arg physical_params)"/>
    <arg name="visual_params" value="$(arg visual_params)"/>

    <!-- additional arg added by jeremy zhang at 04/01/2022-->
    <arg name="transmission_hw_interface" value="hardware_interface/VelocityJointInterface"/> 
      add this line to enable the hardware interface, you can find the original arg in  above file
  </include>
```



open the interface in fmauch_universal_robot/ur_gazebo/config/ur5_controllers.yaml

and add a block

```yaml
joint_state_controller:
  type: joint_state_controller/JointStateController
  publish_rate: &loop_hz 125

pos_joint_traj_controller:
  type: position_controllers/JointTrajectoryController
  joints: &robot_joints
    - shoulder_pan_joint
    - shoulder_lift_joint
    - elbow_joint
    - wrist_1_joint
    - wrist_2_joint
    - wrist_3_joint
  constraints:
    goal_time: 0.6
    stopped_velocity_tolerance: 0.05
    shoulder_pan_joint: {trajectory: 0.1, goal: 0.1}
    shoulder_lift_joint: {trajectory: 0.1, goal: 0.1}
    elbow_joint: {trajectory: 0.1, goal: 0.1}
    wrist_1_joint: {trajectory: 0.1, goal: 0.1}
    wrist_2_joint: {trajectory: 0.1, goal: 0.1}
    wrist_3_joint: {trajectory: 0.1, goal: 0.1}
  stop_trajectory_duration: 0.5
  state_publish_rate: *loop_hz
  action_monitor_rate: 10

joint_group_pos_controller:
  type: position_controllers/JointGroupPositionController
  joints: *robot_joints
 
 # this is velocity controller we added

joint_group_vel_controller:
  type: velocity_controllers/JointGroupVelocityController
  joints: *robot_joints
```



And now, after these configuration, you can launch a simulation to check the result:

```bash
$  roslaunch ur_gazebo ur5_bringup.launch
```

Use rostopic to check and you'll find a topic called /joint_group_vel_controller/command

Besides, we start a teleop control :

```bash
$ roslaunch spacenav_node classic.launch
```

After that, we want to control that robot through our own rrmc node, If you remember, we subscribe control through teleop provided by spacenav we publish our control topic to a topic called "command", so we have to remap that topic here:

```bash
$ rosrun rrmc rrmc_node cmd_vel:=/spacenav/twist command:=/joint_group_vel_controller/command
```

**One thing to metion: ** For me, I can't start the velocity controller successfully. 
