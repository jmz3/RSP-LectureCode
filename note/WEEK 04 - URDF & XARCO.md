<h1 align="center"> SYSTEM PROGRAMMING WEEK 04 </h1>
<h2 align = "center"> URDF & XACRO file </h2>

<h5 align="right"> -- by Jeremy Zhang
@ Johns Hopkins University</h5>


[TOC]



## I. URDF

URDF - Universal Robot Description Format, written in XML

It's pretty much like the launch file, An example of .urdf file looks like: 

simple_robot.urdf

``` xml
<robot name = "simple">
    <link name="world"/>
        <visual>
            <origin xyz="0 0 0.5" rpy="0 0 0"/>
            <geometry>
                <box size="0.1 0.1 1"/>
            </geometry>
        </visual>
    </link>
	<joint name="q1" type="revolute">
        <origin xyz="0 0 0.5" rpy=" 0 0 0"/>
        <parent link="world"/>
        <child link="tool"/>
        <axis xyz="0 0 1"/> <!-- here you can specify the axis that the joints rotates along -->
        <limit lower="-1" upper="1" effort="1" velocity="1"/>
    </joint>
</robot>
```

labels explanation can be found on __[urdf/xml](http://wiki.ros.org/urdf/XML)__

Note: rpy in urdf represents yaw -> pitch -> roll order

```xml
 <link name="my_link">
   <inertial>
     <origin xyz="0 0 0.5" rpy="0 0 0"/>
     <mass value="1"/>
     <inertia ixx="100"  ixy="0"  ixz="0" iyy="100" iyz="0" izz="100" />
   </inertial>

   <visual>
     <origin xyz="0 0 0" rpy="0 0 0" />
     <geometry>
       <box size="1 1 1" />
     </geometry>
     <material name="Cyan">
       <color rgba="0 1.0 1.0 1.0"/>
     </material>
   </visual>

   <collision>
     <origin xyz="0 0 0" rpy="0 0 0"/>
     <geometry>
       <cylinder radius="1" length="0.5"/>
     </geometry>
   </collision>
 </link>
```



<inertial>: rotational inertia

<visual>: can load model form outer files, .stl or .dae files are supported

<joint>: define parent and child frame, types of joints are listed below:

- revolute - a hinge joint that rotates along the axis and has a limited range specified by the upper and lower limits. 
- continuous - a continuous hinge joint that rotates around the axis and has no upper and lower limits. 
- prismatic - a sliding joint that slides along the axis, and has a limited range specified by the upper and lower limits. 
- fixed - This is not really a joint because it cannot move. All degrees of  freedom are locked. This type of joint does not require the axis,  calibration, dynamics, limits or safety_controller. 
- floating - This joint allows motion for all 6 degrees of freedom. 
- planar - This joint allows motion in a plane perpendicular to the axis. 

(SDF - sdformat: create worlds, set gravity, mainly used for dynamics simulation in gazebo)

**remember to include urdf when creating a package** 

"roscd ur description" will show a file called urdf with description files in it, every time you want to find some description file, use "roscd PACKAGE description/" will help you find a description file

**Launch a test in terminal:** 

``` bash
roscore
rosparam set robot_description -t urdf_file_name # write the robot_description 
rosrun robot_state_publisher robot_state_publisher # to launch the state calculator and publish tf and stuff like that
rosrun joint_state_publisher_gui joint_state_publisher_gui # run this command in antoher terminal to start the control gui
rosrun rviz rviz # start rviz in another terminal to test whether the joint was successfully created in the rviz
```

**Inside RVIZ,** 

1. First, change the FIXED FRAME to "world", which is specified in our urdf file
2. Second, add a "TF"  to see the published frames
3. Third, add a "RobotModel" to display the geometry objects that are defined in the urdf





## II. XARCO (marco file)

<xacro> is recommended, the advantage of doing that is others could write their own urdf file to include you original xacro to make some changes on your code without changing your original description configuration

### 2.1 Single XACRO usage

macro file aims to reuse the urdf files, it is not a urdf, but it can produce urdf when complied

``` xml
<robot name="mylink" xmlns:xacro="http://www.ros.org/wiki/xacro"> 
<!-- always start the file like this, specify the ns by adding ros website to it -->
    <xacro:macro name="worldlink" > <!-- create a xacro block -->
        <link name="world"> <!-- create a label for looking for this link and reuse this link-->
            <visual>
                <origin xyz="0 0 0.5" rpy="0 0 0"/>
                <geometry>
                    <box size="0.1 0.1 1"/>
                </geometry>
            </visual>
        </link>
    </xacro:macro>

<!-- create another link called tool link and named as tool -->
    <xacro:macro name="toollink">
        <link name="tool">
            <visual>
                <origin xyz="0 0 -0.5" rpy="0 0 0"/>
                <geometry>
                    <<box size="0.1 0.1 1"/>
                </geometry>
            </visual>
        </link>
    </xacro:macro>
</robot>
```

That's how to write a xacro file, after that we need to put some code in the urdf file to "include it"

simple_robot-v1.urdf.xacro

``` xml
<robot name="simple" xmlns:xacro="http://www.ros.org/wiki/xacro">
	<xacro:include filename="$(find week04_description)/urdf/mylink.xacro"/> 
    <!-- include urdf/mylink.xacro file -->
    <xarco:worldlink/> 
    <xarco:toollink/> <!-- use the worldlink & toollink defined in the mylink.xacro -->
    ...
    other urdf content
    ...
</robot>
```

naming the file including xarcos as NAME.urdf.xarco, it's a convention that tells others this file is a macro file that creates urdf, just for other's convenience.

<xacro> is a terminal command
"xacro -i name.urdf.xacro > new.urdf" can auto-generate a urdf file with the specified link from the original xacro file

``` shell
xacro -i simple_robot-v1.urdf.xacro > test.urdf
rosparam set robot_description -t test.urdf 
# robot_description is a param that contains all the info from urdf files
```

-i option means in-order, it is default option in melodic so it can be ignored.

**Before running xacro file, you need to build the package first and source the bash file so that "find" command can locate the xacro file correctly.**

After running xacro -i command, xacro will be "complied" into the test.urdf, this is a authentic urdf file and it can be used generate robots. Besides, using rosparam to send the description data to parameter server. So you can launch the rviz to test the robot afterwards.

> **Keep in mind**: you need two macro files to complete the previous steps, first one is XXX.xacro and the second one is XXX.urdf.xacro, first file contains descriptions and the second one is more like a "CMAKELIST", compile the second one by typing "xacro -i" will generate a XXX.urdf file and this is the one that is used by ROS.



### 2.2 Combine and reuse through .urdf.xacro and xacro

To make it more reusable, you are allowed to add some embedded parameters in the mylink-v2.xacro like this:

``` xml
<xacro:macro name="mylink" params="link_name"> <!-- "link_name" is not a name, it's just a param, a sign-->
    <link name="${link_name}"> <!-- link_name can create a reusable parameter to enable to specify different names and launch several topics with the same xacro-->
    ...
</xacro:macro>
```

Then you can have many different links with the same properties by assigning different names to the " my link"

Afterwards, you need to make some changes to the simple_robot-v2.urdf.xacro:

``` xml
<xacro:include filename="$(find week04_description/urdf/mylink-v2.xacro)"/>

<!-- <xacro:worldlink/>
<xacro:toollink/> -->
<!-- instead of using above codes, we try to name them with parameters -->

<xacro:mylink link_name="world"/> 
<xacro:mylink link_name="tool"/>
<!-- link_name has been declared in the mylink-v2.xacro as params -->
```

And then set parameters in the terminal ( remember to launch roscore first ):

```shell
xacro -i simple_robot-v2.urdf.xacro > test-2.urdf
rosparam set robot_description -t test-2.urdf
```

- [ ] **Remember: never change anything in the auto-generated urdf **

now you can test it in the way that metioned above:

``` shell
rosrun robot_state_publisher robot_state_publisher # to launch the state calculator and publish tf and stuff like that
rosrun joint_state_publisher_gui joint_state_publisher_gui # run this command in antoher terminal to start the control gui
rosrun rviz rviz # start rviz in another terminal
```



### 2.3 Add a middle layer to pack those codes

Allow others to use your xacro to create their own urdf by adding an extra layer

We have three layers of xacro so far, they are:

``` bash
mylink-v2.xacro # bottom layer, most basic xacro that defines the geometry, origin, ... for the basic links
simple_robot.xacro # middle layer, include the mylink-v2.xacro, instantiated the links by specifying name parameters and define the joint type that connects those links, PACK those things into a block (kinda like a namespace) so it enables other users to duplicate some robots defined here, or create their own robots
simple_robot-v3.urdf.xacro # top layer, naming as XXX.urdf.xacro is just to make it looks differet from other two types of marco
```

**Note:** simple_robot-v2 is an example that middle layer can be ignored when you only need a single robot, which means bottom and top layers are sufficient for compiling a usable urdf file. **XXX.urdf.xacro is necessary because it is used to generate a legit urdf.**

middle layer, which is called as simple_robot.xacro typically looks like:

``` xml
<robot name="simple"  xmlns:xacro="http://www.ros.org/wiki/xacro">

    <xacro:include filename="$(find week04_description)/urdf/mylink-v2.xacro"/> <!--include the bottom layer -->
    <xacro:macro name="myrobot" params="robot_name"> <!-- define params named "robot_name" and use that parameter to define link names and joint names in the following instead of hardcoding those names in the XXX.urdf.xacro, pretty much the same as creating a namespace for the links and joints -->
        <xacro:mylink link_name="${robot_name}_world"/>
        <xacro:mylink link_name="${robot_name}_tool"/>

        <joint name="${robot_name}_q1" type="revolute">
            <origin xyz="0 0 1" rpy=" 0 0 0"/>
            <parent link="${robot_name}_world"/>
            <child link="${robot_name}_tool"/>

            <axis xyz="0 0 1"/>
            <limit lower="-1" upper="1" effort="1" velocity="1"/>
        </joint>
    </xacro:macro>
</robot>
```

With that spoken, you'll need another .urdf.xacro:

simple_robot-v3.urdf.xacro:

``` xml
<robot name="simple" xmlns:xacro="http://www.ros.org/wiki/xacro">
    <xacro:include filename="$(find week04_description/urdf/simple_robot.xacro)"/>

    <xacro:myrobot robot_name="r1"/>

</robot>
```

Then run:

``` bash
xacro -i simple_robot-v3.urdf.xacro > test-3.urdf
rosparam set robot_description -t test-3.urdf
```

Here you can see the test-3 which is auto-generated from simple_robot-v3.urdf.xacro

test-3.urdf

``` xml
<?xml version="1.0" encoding="utf-8"?>
<!-- =================================================================================== -->
<!-- |    This document was autogenerated by xacro from simple_robot-v3.urdf.xacro     | -->
<!-- |    EDITING THIS FILE BY HAND IS NOT RECOMMENDED                                 | -->
<!-- =================================================================================== -->
<robot name="simple">
  <!-- instead of repeating same code for different links, we can reuse the code like above -->
  <link name="r1_world"> <!-- name "r1" is gained from simple_robot-v3.urdf.xacro -->
    <visual>
      <origin rpy="0 0 0" xyz="0 0 0.5"/>
      <geometry>
        <box size="0.1 0.1 1"/>
      </geometry>
    </visual>
  </link>
  <link name="r1_tool"> <!-- name "r1" is gained from simple_robot-v3.urdf.xacro -->
    <visual>
      <origin rpy="0 0 0" xyz="0 0 0.5"/>
      <geometry>
        <box size="0.1 0.1 1"/>
      </geometry>
    </visual>
  </link>
  <joint name="r1_q1" type="revolute"> <!-- name "r1" is gained from simple_robot-v3.urdf.xacro -->
    <origin rpy=" 0 0 0" xyz="0 0 1"/>
    <parent link="r1_world"/>
    <child link="r1_tool"/>
    <axis xyz="0 0 1"/>
    <limit effort="1" lower="-1" upper="1" velocity="1"/>
  </joint>
</robot>

```

Now, if you want to create three similar robots, you can simply modify the .urdf.xacro file like this:

simple_robot-v3.urdf.xacro:

``` xml
<robot name="simple" xmlns:xacro="http://www.ros.org/wiki/xacro">
    <xacro:include filename="$(find week04_description)/urdf/simple_robot.xacro"/>

    <xacro:myrobot robot_name="r1"/>
    <xacro:myrobot robot_name="r2"/>
    <xacro:myrobot robot_name="r3"/>

</robot>
```

Simply just instantiate 3 <xacro:myrobot> block with different robot_name as parameters will achieve that goal.

Now, test-3.urdf will become:

``` xml
<?xml version="1.0" encoding="utf-8"?>
<!-- =================================================================================== -->
<!-- |    This document was autogenerated by xacro from simple_robot-v3.urdf.xacro     | -->
<!-- |    EDITING THIS FILE BY HAND IS NOT RECOMMENDED                                 | -->
<!-- =================================================================================== -->
<robot name="simple">
  <link name="r1_world">
    ...
  </link>
  <link name="r1_tool">
    ...
  </link>
  <joint name="r1_q1" type="revolute">
    ...
  </joint>
  <link name="r2_world">
    ...
  </link>
  <link name="r2_tool">
    ...
  </link>
  <joint name="r2_q1" type="revolute">
    ...
  </joint>
  <link name="r3_world">
    ...
  </link>
  <link name="r3_tool">
    ...
  </link>
  <joint name="r3_q1" type="revolute">
    ...
  </joint>
</robot>

```



If you want to put several robots/links in different coordinates, you can modify the simple_robot-v3.urdf.xacro:

simple_robot-v3.urdf.xacro:

``` xml
<robot name="simple"
    xmlns:xacro="http://www.ros.org/wiki/xacro">
    <xacro:include filename="$(find week04_description)/urdf/simple_robot.xacro"/>

    <xacro:myrobot robot_name="r1"/>
    <xacro:myrobot robot_name="r2"/>
    <xacro:myrobot robot_name="r3"/>

    <link name="world"/>
    
    
    <joint name="r1" type="fixed">
        <origin xyz="1 0 0" rpy="0 0 0"/>
        <parent link="world"/>
        <child link="r1_world"/>
    </joint>
    <!-- joint block creates a fixed connection between "r1_world" and "world", so that "r1_world" will be attached to "world" in the specified method, by doing this, you can put the robot wherever you want-->
    
    <joint name="r2" type="fixed">
        <origin xyz="0 0 0" rpy="0 0 0"/>
        <parent link="world"/>
        <child link="r2_world"/>
    </joint>
    <joint name="r3" type="fixed">
        <origin xyz="1 1 0" rpy="0 0 0"/>
        <parent link="world"/>
        <child link="r3_world"/>
    </joint>

</robot>
```

**Note**: you can simply define a world frame as a link without any visual properties like this:

```xml
<link name="world"/>
```



Add three joint block to fix their origin in a certain pose. These codes make it super easy to recursively add robots or parts of robot. You can even attach one part to another part through replacing the joint block as following:

``` xml
replace this one:    
<joint name="r2" type="fixed">
	<origin xyz="0 0 0" rpy="0 0 0"/>
    <parent link="world"/>
    <child link="r2_world"/>
</joint>
by this lines:
<joint name="r2" type="fixed">
	<origin xyz="0 0 0" rpy="0 0 0"/>
    <parent link="r1_tool"/> <!-- change parent frame to "r1_tool", so that we glued the r2_world frame on the r1_tool frame, it actually added a fixed joint between tool frame of r1 and world frame of r2-->
    <child link="r2_world"/>
</joint>
```

And you can write it into rosparam afterwards.

``` bash
xacro -i simple_robot-v3.urdf.xacro > test-3.urdf
rosparam set robot_description -t test-3.urdf
```

> With this hierarchy structure, you can define your robot in the first two layers and decide to launch which one, and launch how many of it in the top layer. It also makes it possible for others to add a robot, which is same as your previous one just through adding a simple line in the top layer. Or you can define some robot and just leave it there without instantiated in the top layer, so that if someone else want to use that part, he can simply add a line in the top layer to instantiate one.





## III. Apply launch file to this process

### 3.1 simple launch

Another thing you can do is that you can write launch file to pack these commands and launch it automatically.

You can find an example to that by typing these in terminal:

```bash
roscd ur_description/
cd launch/
more xxx.launch
```

A sample launch file looks like:

week04_description.launch

``` xml
<launch>
    <param name="robot_description" 
        command="$(find xacro)/xacro --inorder '$(find week04_description)/urdf/simple_robot-v3.urdf.xacro'"/>
</launch>
```

The param block does the same thing as "rosparam set XXX -t XXX" does.

"robot_name:=xxx" is an argument that specifies the robot name.

Running this launch file will automatically set the robot_description parameter as you want. It will open a master node for a instance and the shut it down. Don't be panic.

Typically, people like to set all the parameters this way just to put those things in the same place and make it easier for others to read your codes. Thus, you don't have to worry about the terminal commands.

---

Namespace can be introduced here to create completely separated robots .

First, Modify .urdf.xacro

simple_robot-v4.urdf.xacro:

```xml
<robot name="simple"
    xmlns:xacro="http://www.ros.org/wiki/xacro">
    <xacro:include filename="$(find week04_description)/urdf/simple_robot.xacro"/>

    <xacro:arg name="robot_name" default="robot"/>

    <xacro:myrobot robot_name="$(arg robot_name)"/>

</robot>
```

Now you will have only one arg instead of repeating the r1 r2 r3 stuff

Then modify the launch file:

```xml
<launch>
    <arg name="robot_name" default="robot"/>
    <group ns="left"> <!-- create a namespace-->
        <param name="robot_description" 
        command="$(find xacro)/xacro --inorder '$(find week04_description)/urdf/simple_robot-v4.urdf.xacro' robot_name:=$(arg robot_name)"/>
    </group>
</launch>
```

remember to specify the robot_name in the param block

After all these steps, you will get a rosparam that is called /left/robot_description, "/left" is the namespace you set in the launch file.

to test it, first start the launch file:

``` bash
roslaunch week04_description week04_upload.launch 
```

run the joint gui and declare the namespace the robot locates

``` bash
# 1st terminal:
rosrun joint_state_publisher_gui joint_state_publisher_gui __ns:=left
# 2nd terminal:
rosrun robot_state_publisher robot_state_publisher __ns:=left _tf_prefix:=left
```

"__ns:=left" is to declare the namespace,

"_tf_prefix:=left" is to add a prefix to the tf topic

- [x] Now you can launch rviz like before, but this time you have to manually set " Robot Description" label as "/left/robot_description", and set "TF Prefix" as "left" (because you assigned the namespace as left and you assigned the tf prefix as left), until then you can see the models in rviz.

**Since an arg is defined in the launch file, we can change it when calling it by roslaunch**

in launch file:

```bash
replace     <group ns="left">
with		<group ns="$(arg robot_name)">
```

It will allow you to name the namespace whatever you want when calling roslaunch

in terminal:

```bash
roslaunch week04_description week04_upload.launch robot_name:=foobar
```

and then run the two gui and modify RobotModel in rviz like before:

```bash
# 1st terminal:
rosrun joint_state_publisher_gui joint_state_publisher_gui __ns:=foobar
# 2nd terminal:
rosrun robot_state_publisher robot_state_publisher __ns:=foobar _tf_prefix:=foobar
```

by doing so, you can run the week04_description.launch at the same time by adding different namespace to it. There will be several rosparam that separately sending informations about robots that are launched by the same launch file.



### 3.2  Specifying the Robot name by xacro:arg method

If you want to leave the robot name as a TBD argument, you can set a arg to leave it to the launch file. In the xacro you may have codes like this:

```xml
<robot xmlns:xacro="http://ros.org/wiki/xacro"
       name="mobile_claw">

  <xacro:arg name="with_gripper" default="true" /> <!-- Here we define a argument that determines whether the mobile base has or doesn't have a gripper. The info is unknown now, but we can leave it there and let launch file to decide it.-->

  <!-- model descriptions -->
  <!-- mobile base description -->
  <xacro:include filename="$(find mobile_manipulator)/models/mobile_base/mobile_base.urdf.xacro"/>

  <!-- gripper -->
  <xacro:if value="$(arg with_gripper)">
    <xacro:include filename="$(find mobile_manipulator)/models/gripper_fake/gripper_fake.urdf.xacro"/>

    <joint name="gripper_to_mount" type="fixed">
      <origin xyz=".15 0 .205" rpy="0 0 0" />
      <parent link="chassis" />
      <child link="gripper_center" />
    </joint>
  </xacro:if>
</robot>
```

If you have things like **<xacro:arg name="with_gripper" default="true" />** , then you'll specify the value of that arg when runtime, in other words, you have to give a value to "with_gripper" in the **launch file**. Way to do that is as following:

```xml
<launch>
  <arg name="is_using_gripper" default="true" />

  <!-- Load the mobile-arm description on the parameter server -->
  <param name="robot_description" command="$(find xacro)/xacro --inorder
                       '$(find mobile_manipulator)/models/mobile_manipulator/mobile_manipulator.urdf.xacro'
                       with_gripper:=$(arg is_using_gripper)"/>
</launch>
```

In the launch file, at the <param/> label we can see that "with_gripper" argument gain its value at the end of in order xacro command.

One thing that needs to be awared is that, we actually add another arg to control whether the "with_gripper" arg should be used. By doing that, you will be able to control the parameter in the terminal:

```bash
roslaunch -v mobile_manipulator mobile_manipulator.launch is_using_gripper:=false
# is_using_gripper is the arg that you set in launch file, it controls the arg with_gripper in the xacro
```



## VI. Extra Knowledge

```xml
<link>
<visual>
<geometry>
    <mesh>
        <url></url>
    </mesh>
</geometry>
</visual>
</link>
```

Or

```xml
    <visual>
      <origin rpy="0 0 0" xyz="0 0 0"/>
      <geometry>
        <mesh filename="package://robot_description/meshes/base_link.DAE"/>
      </geometry>
    </visual>
```

This code allows you to using the .dae file from a website link. Dae file works better that step file since it contains color texture and shadow info to make the model looks more real.
