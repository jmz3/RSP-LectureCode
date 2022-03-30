<h1 align="center"> SYSTEM PROGRAMMING WEEK 01 </h1>
<h2 align = "center"> BASIC OPERATIONS ABOUT ROS </h2>
<h5 align="right"> -- by Jeremy Zhang @ Johns Hopkins University</h5>

***



### rosnode rostopic rosservice rosmsg rosbag...

have several options like: list info echo...

```bash
rosnode/rostopic/rosservice/rosmsg list | grep somename
```

" | grep " will help you to filter the things that contains the name you want to look up for.



### cmakelist.txt function named as include_directories( include ):

	here include means the file that names as include in the package file

### rosnode list:

	diplay all the active list 

### rosrun week01 node __name:node2 headline:headline1

	reuse the same code but create different topic and thread

### rosmsg list:

	display all the messages that are alive, can be used to check whether your message has been created

###  hpp file in the include file and cpp in the src file

### source devel/setup.bash

	source the code after catkin build
	**have to be done everytime you open a new terminal**
	source this file will automatically source the root bash file

```cpp
pub = nh.advertise<std_msgs::String>("headline",10); //here 10 is a buffer, means we can save the past 10 msgs that has been advertised
```



catkin clean :
	clean all the stuff that has been created by catkin build

### catkin build:

	always operate on the workspace level
	when run this code, catkin will try to find all the dependencies to make sure they are all up to date
	and are all built 

#catkin build PACKAGE_NAME:
	just build the specified package alone

### week01 is a publisher template

### ros::NodeHandle is a thing that create some node and all the stuff, don't have to know what's inside

#uncomment add_executable line in CMakelist.txt to create a executable program:
	add_executable(${PROJECT_NAME}_node src/week01_node.cpp)
	and uncomment target_link_libraries(${PROJECT_NAME}_node in the lines below
	target_link_libraries should contain all the libraries, ros libraries can be included by ${catkin_LIBRARIES}
	and the libraries that you build in this project should also be included, for example:
	we declare a new library by add_library_( xxx ) then we have to add that lib in target_link_libraries in the exact same 
	CMakeLists.txt file

### "ps aux" will list all the processes running / "ps aux | grep ros" 

### if "undefined reference" appear during catkin build, it means there's a linker error

### after catkin build

	there are build, devel, logs, src 4 files
	build: compilers, g++, links
	devel: symbolic link, the most important stuff, the final project
	logs: as its name
	src: the only one that should be committed to professor

### after catkin build and before running the code

	you have to go in to the /devel file and source the bash file 
	the command is : source devel/setup.bash
	and then you can use rosrun to run the package
	everytime in a new ternimal have to source the bash file and then rosrun

### 3 useful command to debug rosnode:

```bash
$ rosnode list: find the node name
$ rosnode ping: test the node
$ rosnode info:
```

### rostopic is another useful tool:

​	rostopic list: find the topic name( i.e. "headline")
​	rostopic echo: to check what's been published
​

### ros can reuse the same node and topic with different name that are set at runtime, this is called remapping

re-use code is so important that you don't have to change the node name or topic name that is hardcoded in the source code
you just assign a new name by remapping at runtime

```bash
rosrun week01 node __name:=node2
above is creating another node with the same code
rosrun week01 node __name:=node2 headline:=headline2
above is creating another topic with the same code
```




### rosparam set + parameter_Name_ + param_Value:

can generate a golbal variable that can be used through nodehandle.getParam

### rosparam load and rosparam set can be used to put some parameter for robot 

```bash
rosparam set + ROBOT_description -t simple_robot.urdf
```

### Private node handle:

normal node handle: ros::NodeHandle nh;
private node handle: ros::NodeHandle nhp("~")
way to declare private variable: rosrun PACKAGE_NAME_ NODE_NAME_ _rate_priv=10
in launch file, if <param/> is at the same level with node level, it's a golbal variable
otherwise, it would be in child level of node level, at that time, it becomes a private variable. And it can only be 
visited in the node field

### inside launch file, arguements can be defined:

```launch
<arg name="newparam"/>
<param name="rate", value="$(arg newparam)",type="double"/>
```


in command line, you can give that new argument a value
**why use that? create a way for users to define some value when runtime, if that value is not given, ros will throw a warn to remind you to do so**
roslaunch week01 test.launch newparam:=10
you can set a default value like <arg name="looprate" default="1"/> to ensure it runs when no input to this argument

### command locate + FILENAME can help to find the directory of the file

### topic only exist within node


### for assignment1 make sure all the dependency are good

if sub packages are dependent to each other, directly catkin build will fail since only one cmake file is executed bu other packages are not built, so you have to change the build command at the bottom of package.xml file to tell catkin prebuild the dependencies



# ROS service

.srv file is alike to .msg file,srv files are just like msg files, except they  contain two parts: a request and a response.  The two parts are  separated by a '---' line. Here is an example of a srv file: 

```yaml
int64 A
int64 B
---
int64 Sum
```




# UR package on Melodic

https://github.com/UniversalRobots/Universal_Robots_ROS_Driver

```bash
[elbow_joint, shoulder_lift_joint, shoulder_pan_joint, wrist_1_joint, wrist_2_joint,
  wrist_3_joint]
```

The ur packages use a strange order of joints however like [ 3 2 1 4 5 6 ] keep that in mind to avoid some mistakes
