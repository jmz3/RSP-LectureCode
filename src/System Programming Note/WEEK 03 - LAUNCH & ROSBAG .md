<h1 align="center"> SYSTEM PROGRAMMING WEEK 03 </h1>
<h2 align = "center"> LAUNCH FILE & ROSBAG </h2>

<h5 align="right"> -- by Jeremy Zhang
@ Johns Hopkins University</h5>

---

[TOC]



# launch file

launch file is kind of like running script
it isn't automatically created in the runtime
you have to create a dir called "launch" and make a xml file called xxx.launch inside it
if there is no roscore running, roslaunch will automatically start a roscore when running roslaunch
if there exists a roscore, roslaunch will run on that core
"ns" means namespace, it clusters all the nodes into a group

you can assign package name, executable name, topic name, in the launch file

use command **roslaunch PACKAGE_NAME_ LAUNCHFILE_NAME_** to run the launchfile in terminal

***

## launch file components:

check http://wiki.ros.org/roslaunch/XML for references

```xml
<launch>
<node>: can add two nodes in different namespace with different name(here refers to topic name) but the node name can be the same: 
<node	pkg="week01"
        type="node"
        name="publisher_1"
        ns="left"
        output="screen"/>
<node	pkg="week01"
        type="node"
		name="publisher_2"
		ns="right"
		output="screen"/>

<machine>
<include>: to include other launch files
<remap>:	<remap from="headline" to="$(arg topicname)"/>
it enables users to change the topic name in rumtime
<env>
<param>
<rosparam>: run by parameter server, maintained by roscore

<group>: The <group> tag makes it easier to apply settings to a group of nodes. Or you can reuse the same node for different purposes.
```

It has an ns attribute that lets you push the group of nodes into a separate namespace. You can also use the <remap> tag to apply remap setting across the group. :http://wiki.ros.org/roslaunch/XML/group

```xml
<test>
<arg>
```

## Use launch file to run turtlesim and mimic it

	mimic provides a simple interface for making one turtlesim mimic another.
	
	-- Subscribed Topics
	-- input (turtlesim/Pose)
	
		The input topic for the mimic node. The topic must be remapped to the pose topic of the desired turtle to mimic. 
	
	-- Published Topics
	-- output (geometry_msgs/Twist)
	
		The output topic for the mimic node. The topic must be remapped to the cmd_vel topic of the mimicking turtle. 

**examples**

```xml
<node pkg="turtlesim" name="mimic" type="mimic" >
    <remap from="input" to="t1"/>
    <remap from="output" to="t2"/>
</node> 
 here t1 and t2 are the name of two turtles, 
	they are in the same namespace with mimic node so there's no need to specify the namespace field before t1, 
	if mimic node and turtle node are in different namespace, you must add namespace before turtle name like: 
<node pkg="turtlesim" name="mimic" type="mimic">
    <remap from="input" to="turtlesim1/turtle1"/>
    <remap from="output" to="turtlesim2/turtle1"/>
</node>
```





# rosparam :

create a global parameter, usually a string that can be used inside the code
the good thing of using it is you can hardcode the parameter in cpp and change the 
value outside, in the terminal, so you don't have to change the 
source code everytime

you can specify the parameter namespace so that limit its margin
the command is like: " rosparam set /left/rate 20" so that only in left namespace
a rate=20 parameter will be generated
"rosparam delete PARAMNAME" will delete the parameters

nh.getParam("rate", ratehere) in cpp file will find the parameter called "rate" and assign its value to ratehere
otherwise, you can use nh.param<double>("rate",ratehere,10) to set a default value for ratehere

parameter can be loaded from a parameter file ( a yaml file )


in cpp file you can set a param using :"nh.setParam()"
in launch file you can set a param using: " <param name="rate" value="1" type="double"/>


if you have different namespaces in launch file, you'd better declare the param in the <node></node>, otherwise the global param won't be accepted in the namespace
or you can do in the terminal:" rosparam set /namespace/rate 10" to put the param in the namespace



# ROSBAG

rosbag record: to write the data and log into a .bag file "rosbag record -0 NAME.bag + TOPICNAME"
		it records every thing that has been published by the specific topic

rosbag play: will replay the whole bag data according to the time stamp

rosbag info + BAGENAME: will tell you all the topics that are recorded in the .bag file


rosrun rqt_image_view rqt_image_view

rqt_plot can visualize different topic data

rqt_bag



``` bash
.bashrc

# everytime you open a new terminal .bashrc will automatically sourced and ros bash file is included in it.

source devel/setup.bash
# to run your own ros code, you'd better source this file 
```

