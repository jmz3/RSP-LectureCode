# Robot System Programming

## Description

Contents [[hide](https://dscl.lcsr.jhu.edu/me530707/#)]

- - [0.1 Description](https://dscl.lcsr.jhu.edu/me530707/#Description)
  - [0.2 Instructors](https://dscl.lcsr.jhu.edu/me530707/#Instructors)
  - [0.3 Class Meeting Schedule](https://dscl.lcsr.jhu.edu/me530707/#Class_Meeting_Schedule)
  - [0.4 Office Hours](https://dscl.lcsr.jhu.edu/me530707/#Office_Hours)
  - 0.5 Prerequisites
    - [0.5.1 Courses](https://dscl.lcsr.jhu.edu/me530707/#Courses)
    - [0.5.2 Required Computer](https://dscl.lcsr.jhu.edu/me530707/#Required_Computer)
    - [0.5.3 Knowledge:](https://dscl.lcsr.jhu.edu/me530707/#Knowledge)
  - [0.6 Textbooks](https://dscl.lcsr.jhu.edu/me530707/#Textbooks)
  - [0.7 Regarding plagiarism](https://dscl.lcsr.jhu.edu/me530707/#Regarding_plagiarism)
  - 0.8 Robotics Teaching Lab – Wyman 170
    - [0.8.1 Wyman 170 Lab Etiquette:](https://dscl.lcsr.jhu.edu/me530707/#Wyman_170_Lab_Etiquette)
  - 0.9 Course Grade Policy
    - [0.9.1 Weekly Assignments](https://dscl.lcsr.jhu.edu/me530707/#Weekly_Assignments)

- 1 Schedule

  - 1.1 Week 1: Course Overview and ROS Basics
    - [1.1.1 Topics](https://dscl.lcsr.jhu.edu/me530707/#Topics)
    - [1.1.2 Reading](https://dscl.lcsr.jhu.edu/me530707/#Reading)
    - [1.1.3 Tutorials](https://dscl.lcsr.jhu.edu/me530707/#Tutorials)
  - 1.2 Week 2: Roslaunch, Nodes, tf, Parameters, and Rosbag
    - [1.2.1 Topics](https://dscl.lcsr.jhu.edu/me530707/#Topics-2)
    - [1.2.2 Reading](https://dscl.lcsr.jhu.edu/me530707/#Reading-2)
    - [1.2.3 Tutorials](https://dscl.lcsr.jhu.edu/me530707/#Tutorials-2)
  - 1.3 Week 3: URDF and Robot State Publisher
    - [1.3.1 Topics](https://dscl.lcsr.jhu.edu/me530707/#Topics-3)
    - [1.3.2 Reading](https://dscl.lcsr.jhu.edu/me530707/#Reading-3)
    - [1.3.3 Tutorials](https://dscl.lcsr.jhu.edu/me530707/#Tutorials-3)
  - 1.4 Week 4: Gazebo Intro, SDF, and worlds
    - [1.4.1 Topics](https://dscl.lcsr.jhu.edu/me530707/#Topics-4)
    - [1.4.2 Reading](https://dscl.lcsr.jhu.edu/me530707/#Reading-4)
  - 1.5 Week 5: Gazebo physical simulation, ROS Integration
    - [1.5.1 Preliminary Project Ideas Due 10:30AM Thursday March 2, 2021](https://dscl.lcsr.jhu.edu/me530707/#Preliminary_Project_Ideas_Due_1030AM_Thursday_March_2_2021)
    - [1.5.2 Topics](https://dscl.lcsr.jhu.edu/me530707/#Topics-5)
    - [1.5.3 Reading](https://dscl.lcsr.jhu.edu/me530707/#Reading-5)
    - [1.5.4 Tutorials](https://dscl.lcsr.jhu.edu/me530707/#Tutorials-4)
  - 1.6 Week 6: Jackal, Joy, and ROS Node development in C++
    - [1.6.1 Topics](https://dscl.lcsr.jhu.edu/me530707/#Topics-6)
    - [1.6.2 Reading](https://dscl.lcsr.jhu.edu/me530707/#Reading-6)
  - 1.7 Week 7:  Turtlebot-2 Simulation in Gazebo, SLAM Navigation, Adaptive Monte-Carlo Localization
    - [1.7.1 Topics](https://dscl.lcsr.jhu.edu/me530707/#Topics-7)
    - [1.7.2 Reading](https://dscl.lcsr.jhu.edu/me530707/#Reading-7)
  - 1.8 Week 8: OROCOS KDL
    - [1.8.1 Topics](https://dscl.lcsr.jhu.edu/me530707/#Topics-8)
    - [1.8.2 Reading](https://dscl.lcsr.jhu.edu/me530707/#Reading-8)
  - 1.9 Week 9: ROS Control and Plugins
    - [1.9.1 Topics](https://dscl.lcsr.jhu.edu/me530707/#Topics-9)
    - [1.9.2 Reading](https://dscl.lcsr.jhu.edu/me530707/#Reading-9)
  - 1.10 Week 10: MoveIt! Plugins
    - [1.10.1 Topics](https://dscl.lcsr.jhu.edu/me530707/#Topics-10)
  - [1.11 Reading](https://dscl.lcsr.jhu.edu/me530707/#Reading-10)
  - [1.12 Week 11: Rviz Plugins](https://dscl.lcsr.jhu.edu/me530707/#Week_11_Rviz_Plugins)
  - [1.13 Week 12: Orocos Realtime-Toolkit (RTT)](https://dscl.lcsr.jhu.edu/me530707/#Week_12_Orocos_Realtime-Toolkit_RTT)
  - [1.14 Reading](https://dscl.lcsr.jhu.edu/me530707/#Reading-11)
  - [1.15 Week 13: Orocos Bayesian Filter Library (BFL)](https://dscl.lcsr.jhu.edu/me530707/#Week_13_Orocos_Bayesian_Filter_Library_BFL)
  - [1.16 Reading](https://dscl.lcsr.jhu.edu/me530707/#Reading-12)

This course seeks to introduce students to open-source software tools that are available today for building complex experimental and  deployable robotic systems. The course is grouped into four sections,  each of which building on the previous in increasing complexity and  specificity: 

Students will need to provide a computer running **[Ubuntu 18.04 LTS](http://releases.ubuntu.com/18.04/) or one of its variants such as [Xubuntu](https://xubuntu.org/release/16-04/), [Kubuntu](https://kubuntu.org/tag/kubuntu-16-04/) and ROS[ Melodic](http://wiki.ros.org/melodic/Installation)** – note that these specific versions of Linux and ROS are required!

Students should have an understanding of intermediate programming in  C/C++. Familiarity with Linux the environment and its administration.  Familiarity with software version control systems (e.g. subversion,  mercurial, git), linear algebra. **Required Course Background:** Familiarity with robot kinematics and algorithms such as those covered  in EN.530.646 Robot Devices, Kinematics, Dynamics, and Control and  EN.600.636 Algorithms for Sensor Based Robotics, and  EN.601.220. Intermediate Programming in C++ on Linux.

## Instructors

Simon Leonard

## Class Meeting Schedule

**Times:** Tuesday and Thursdays 10:30-11:45AM
**Location:** Hodson 216

Parts of assignments will be done during lectures. If you cannot attend a lecture (for whatever reason), I need to know ***before*** lectures.

## Office Hours

- Wednesday 3-4PM
- Friday 2-3PM

## Prerequisites

### Courses

**530.646 Robot Devices, Kinematics, Dynamics, and Control** and **601.463/601.663 Algorithms for Sensor-Based Robotics**.  It is OK, if you have previously taken one of these two courses and  take the other course concurrently with 530.707. It is NOT OK to be  taking both of these courses while you are taking 530.707.

You must have already taken **601.220 Intermediate Programming in C++** or an equivalent course. This is a prerequisite, not a co-requisite.  You must have completed 601.220 or the equivalent **before** you take this 530.707.

### Required Computer

Students will need to provide their own laptop running Ubuntu 18.04  LTS or one of its variants such as Xubuntu or Kubuntu. You will also  need to install ROS Melodic. Your computer can be dual boot. Virtual  machines are NOT recommended.

### Knowledge:

This course will benefit you the most if you are already familiar with the following subjects:

- Kinematics & Dynamics
- Basic Machine Vision
- Basic Probability Theory and Random Processes
- Data Structures
- Linear Algebra
- Linux Operating System
- Use the following programming languages: 
  - Intermediate C++ programming including data structures (absolutely required)
  - bash
  - Python (optional) 
- Use the following markup languages: 
  - XML 
- Use the following software tools: 
  - Git
  - CMake 

## Textbooks

Although there is no required text for the course, if you are new to  ROS we recommend that you get and read one or more of the following two  introductory ROS texts. But really, the best reading for this course are API documentation!

## Regarding plagiarism

The strength of the university depends on academic and personal   integrity. In this course, you must be honest and truthful. Ethical   violations include cheating, plagiarism, reuse of assignments,  improper use of the Internet and electronic devices, unauthorized   collaboration, alteration of graded assignments, forgery and   falsification, lying, facilitating academic dishonesty, and unfair   competition.

In addition, the specific ethics guidelines for this course are:

1.  No teamwork or collaboration on assignments
2. Public or private sharing of code, assignments

Report any violations you witness to the instructor. You may consult  the associate dean of student conduct (or designee) by calling the  Office of the Dean of Students at 410-516-8208 or via email at  integrity@jhu.edu. For more information, see the Homewood Student  Affairs site on academic ethics: [https://studentaffairs.jhu.edu/policies-guidelines/undergrad-ethics](https://studentaffairs.jhu.edu/policies-guidelines/undergrad-ethics/)

Read the [Graduate Academic Misconduct Policy](https://krieger.jhu.edu/hwgradaffairs/wp-content/uploads/sites/35/2018/08/Homewood-WSE_KSAS_-WSE-EP_KSAS-AAP-Graduate-Academic-Misconduct-Policy-2018SU.pdf):

Examples of academic misconduct:

- Use of material produced by another person without acknowledging its source
- Submission of the same or substantially similar work of another person (e.g., an author, a classmate, etc.)
- Intentionally or knowingly aiding another student to commit an academic ethics violation
- Allowing another student to copy from one’s own assignment, test, or examination
- Making available copies of course materials whose circulation is prohibited (e.g., old assignments, texts or examinations, etc.)

Teamwork for assignments is **not** allowed. You cannot  use work from someone else (plagiarism). You cannot share your  assignments to someone else (facilitating academic dishonesty).

- You cannot use **any** material from a classmate or student who took the course in previous semesters.
- You cannot share **any** material from this course in future semester.

Any academic misconducts (big or small) will be reported to Graduate  Affairs (or Office of the Dean) and will be at least sanctioned by an   “F” course grade. No negotiation. I will retroactively sanction   plagiarism (i.e. a student plagiarize your assignment in future   semesters). This means that you will get a retroactive “F” if your code  shows up in a submission in the coming years (and yes, you may have to  return your diploma because of that).

## Robotics Teaching Lab – Wyman 170

We have 12 workstations with Ubuntu Linux 18.04 LTS and ROS Melodic installed. You can log in with your JHED ID and password.

### Wyman 170 Lab Etiquette:

- Lab is shared with 601.463/663 (~60 students) and 530.646 (~30 students)
- ***Never\*** turn off the computers
- Your account ***does*** ***not\*** have sudo privileges. If you need packages, you must clone/build/run them from your account.
- The only version of the operating system we will support is Ubuntu 18.04 64-bit.
- The only version of ROS we will support is ROS Melodic
- Leave the lab spotless.
- Never turn off the computers
- 2GB account (until further notice)
- Never “lock” a workstation with a screen-saver. Locked computers will be powered off.
- No Backup!: The files on these computers are NOT BACKED UP. Any files that you leave in your account on these computers can disappear at any time. Use https://git-teach.lcsr.jhu.edu to save your projects during and  after every work session.
- When you are finished a work session, log off the computer.
- **If you encounter a problem:** Notify us on Piazza if you have any problems with the lab or its equipment.
- In case you miss: Never turn off the computers.

## Course Grade Policy

Course grade will be based upon ten weekly assignments (50%) , and a final independent course project (50%)

### Weekly Assignments

- Each assignment will be worth 5%
- Parts of assignments will be done during lectures
- Expect to have “in class” parts in each assignment
- Weekly assignments are due at **11:59PM Thursday each wee**k.
- All assignments are submitted on Github
- Assignments are pulled at midnight sharp. Whatever gets pulled at that time is what gets graded.
- Assignments must build and run cleanly. Failure to do so will get a “0”. No appeal.
- Package structure must be as instructed. Failure to do so will get a “0”. No appeal.

# Schedule

## Week 1: Course Overview and ROS Basics

### Topics

- Course Overview 
  - Course Description
  - Prerequisites
  - Assignments
  - Class Project
- Background of Robot Software Frameworks and The Open-Source Robotics Community
- Development Tools, Managing Code, Environments, Installing Software, Building Code
- The ROS core systems: Packaging, build system, messaging, command line & GUI tools, nodes, topics, services and parameters.
- Writing a Simple Publisher and Subscriber (C++)
- Examining the Simple Publisher and Subscriber
- Writing a Simple Service and Client (C++)
- Examining the Simple Service and Client

### Reading

- ROS 

  - [ROS Introduction](http://wiki.ros.org/ROS/Introduction)
  - [ROS Started Guide](http://wiki.ros.org/ROS/StartGuide) Introduction, Concepts, Higher-Level Concepts, Client Libraries, Technical Overview
  - [ROS developer’s guide](http://wiki.ros.org/DevelopersGuide)
  - [ROS C++ Style Guide](http://wiki.ros.org/CppStyleGuide)
  - [ROS Python Style Guide](http://wiki.ros.org/PyStyleGuide) 

- Git: You should already know git (it is a prerequisite for the course), but if you need a refresher, do these tutorials: 

  - Read chapters 1, 2, and 4 of the 

    Git Book

    : 

    - [Git Book Chapter 1: Getting Started](http://git-scm.com/book/en/Getting-Started)
    - [Git Book Chapter 2: Git Basics](http://git-scm.com/book/en/Git-Basics)
    - [Git Book Chapter 4: Git on the Server](https://git-scm.com/book/en/v2/Git-on-the-Server-The-Protocols)  

- Useful cheat-sheets: 

  - [ROS cheat-sheet for kinetic and Catkin](https://github.com/ros/cheatsheet/releases/download/0.0.1/ROScheatsheet_catkin.pdf) 

- Documentation on command line tools 

  - [rosnode](http://wiki.ros.org/rosnode)
  - [rostopic](http://wiki.ros.org/rostopic)
  - [rosmsg](http://wiki.ros.org/rosmsg) 
  - [rosparam](http://wiki.ros.org/rosparam)
  - [rosservic](http://wiki.ros.org/rosservice)e

### Tutorials

- Install Ubuntu 18.04 LTS (or an equivalent release).  

- Install [ROS Melodic](http://wiki.ros.org/kinetic/Installation/Ubuntu)

- Complete these 

  Tutorials

  - Installing and Configuring Your ROS Environment.
  - Navigating the ROS Filesystem
  - Creating a ROS Package
  - Building a ROS Package
  - Understanding ROS Nodes
  - Understanding ROS Topics
  - Understanding ROS Services and Parameters
  - Using rqt_console and roslaunch
  - Using rosed to edit files in ROS
  - Creating a ROS msg and a ROS srv
  - Writing a publisher and subscriber in C++
  - Writing a Simple Publisher and Subscriber (C++)
  - Examining the Simple Publisher and Subscriber
  - Writing a service and client in C++
  - Examining the Simple Service and Client 

## Week 2: Roslaunch, Nodes, tf, Parameters, and Rosbag

### Topics

- rosbag 
- roswtf 
- ROS.org 
- tf 
- Rviz 
- Getting and setting parameters in roslaunch and C++ nodes

### Reading

- [Documentation on tf command-line tools and C++ client libraries](http://wiki.ros.org/tf)
- [Documentation on paramater command-line tool (rosparam) and C++ client libraries](http://wiki.ros.org/Parameter Server)
- [Documentation on rosbag](http://wiki.ros.org/rosbag/Commandline)
- [Documentation on ROS timers](http://wiki.ros.org/roscpp/Overview/Timers)

### Tutorials

- [Recording and playing back data](http://wiki.ros.org/ROS/Tutorials/Recording and playing back data)

- [Getting started with roswtf](http://wiki.ros.org/ROS/Tutorials/Getting started with roswtf)

- [Navigating the ROS wiki](http://wiki.ros.org/ROS/Tutorials/NavigatingTheWiki)

- tf tutorials (C++)

   NOTE: Be sure to do the version of these tutorials for ROS kinetic with the CATKIN build system.

  - [Introduction to tf](http://wiki.ros.org/tf/Tutorials/Introduction to tf)
  - [Writing a tf broadcaster (C++)](http://wiki.ros.org/tf/Tutorials/Writing a tf broadcaster (C%2B%2B))
  - [Writing a tf listener (C++)](http://wiki.ros.org/tf/Tutorials/Writing a tf listener (C%2B%2B))
  - [Adding a frame (C++)](http://wiki.ros.org/tf/Tutorials/Adding a frame (C%2B%2B))
  - [Learning about tf and time (C++)](http://wiki.ros.org/tf/Tutorials/tf and Time (C%2B%2B))
  - [Time travel with tf (C++)](http://wiki.ros.org/tf/Tutorials/Time travel with tf (C%2B%2B)) 

- [ROS Timers tutorial](http://wiki.ros.org/roscpp_tutorials/Tutorials/Timers)

- [ROS Parameter Tutorial](http://wiki.ros.org/roscpp_tutorials/Tutorials/Parameters)

## Week 3: URDF and Robot State Publisher

### Topics

- Unified Robot Description Format (URDF)
- Robot State Publisher

### Reading

- URDF Documentation
  - [URDF XML documentation](http://wiki.ros.org/urdf/XML)  
- [Joint State Publisher Documentation](http://wiki.ros.org/joint_state_publisher?distro=kinetic)
- [ROS Workspaces Documentaiton](http://wiki.ros.org/ROS/Patterns/Workspaces)

### Tutorials

- **Learning URDF Step by Step**
  - \1. Building a Visual Robot Model with URDF from Scratch
  - \2. Building a Movable Robot Model with URDF
  - \3. Adding Physical and Collision Properties to a URDF Model
  - \4. Using Xacro to Clean Up a URDF File 
- **Learning URDF (including C++ API)**
  - \1. Create your own urdf file
  - \2. Parse a urdf file
  - \3. Using the robot state publisher on your own robot
  - \4. Skip this one for now:  Start using the KDL parser *(You can skip this tutorial for now if you like, it is not required for this module’s assignment.)*
  - \5. Using urdf with robot_state_publisher 

## Week 4: Gazebo Intro, SDF, and worlds

### Topics

- Simulating robots, their environments, and robot-environment interaction with [Gazebo](http://gazebosim.org/)

- Gazebo ROS integration.

- NOTE: You do not need to install Gazebo.

   Your full ROS kinetic desktop installation will have installed Gazebo V7.0.0 So DO NOT follow the installation instructions on gazebosim.org. If for some reason the ros gazebo package is not installed, install it with 

  sudo apt-get install ros-kinetic-gazebo-ros ros-kinetic-gazebo-ros-pkgs ros-kinetic-gazebo-ros-control

  - You can verify that Gazebo is installed by issuing the command  “gazebo” on the command line – a gazebo window should open after a few  seconds delay.

- **NOTE:** Gazebo is CPU-intensive, and will not run very well in virtual boxes.

### Reading

- [Gazebo Overview](http://gazebosim.org/tutorials?tut=components&cat=get_started)
- [Gazebo API](http://osrf-distributions.s3.amazonaws.com/gazebo/api/2.2.1/index.html)
- [SDFormat Specification](http://gazebosim.org/sdf.html): The SDF XML file format is a superset of URDF. SDF files are how  Gazebo defines robots and the environment. You can generate SDF from  URDF or XACRO on-the-fly, so in practice it is easier to maintain a  single XACRO file, and use it to generate URDF and SDF from it on-the  fly.

## Week 5: Gazebo physical simulation, ROS Integration

### Preliminary Project Ideas Due 10:30AM Thursday March 2, 2021

### Topics

- Simulating robots, their environments, and robot-environment interaction with [Gazebo](http://gazebosim.org/)
- Gazebo ROS integration.
- Gazebo Intermediate Concepts

### Reading

- [Gazebo Overview](http://gazebosim.org/wiki/Overview)
- [Gazebo API](http://gazebosim.org/api.html)
- [SDFormat Specification](http://gazebosim.org/sdf.html) SDF is how Gazebo defines robots and the environment. You can generate SDF from URDF.
- Gazebo ROS Packages Documentation
  - [Gazebo msgs](http://wiki.ros.org/gazebo_msgs?distro=kinetic)
  - [Gazebo Plugins](http://wiki.ros.org/gazebo_plugins?distro=kinetic)
  - [Gazebo ROS](http://wiki.ros.org/gazebo_ros?distro=kinetic)

### Tutorials

- Gazebo Tutorials

  

  

  - [Write a Plugin](http://gazebosim.org/tutorials?cat=write_plugin)
  - [Plugins](http://gazebosim.org/tutorials?cat=plugins)   

## Week 6: Jackal, Joy, and ROS Node development in C++

### Topics

- Installing and testing joysticks
- Publishing /joy topic with the ROS joy package
- Joystick tutorials – including teleoperating a robot from a joystick
- ROS timers
- Writing your own package to subscribe to the joystick /joy and publish a geometry_msgs/Twist topic to command the EduMIP.
- Writing launch files for same.
- Running ROS systems spanning more than one computers.

### Reading

- [ROS joy package documentation and tutorials](http://wiki.ros.org/joy)

- [ROS timer documentation](http://wiki.ros.org/roscpp/Overview/Timers)

- [Running ROS Across Multiple Machines](http://wiki.ros.org/ROS/Tutorials/MultipleMachines)

- Documentation on some useful standard 

  common message types

  : 

  - [sensor_msgs/Joy message type](http://docs.ros.org/api/sensor_msgs/html/msg/Joy.html) this is just one of the standard [sensor message types](http://wiki.ros.org/sensor_msgs?distro=kinetic)
  - [geometry_msgs/Twist message type](http://docs.ros.org/api/geometry_msgs/html/msg/Twist.html) this is just one of the standard [geometry message types](http://wiki.ros.org/geometry_msgs)
  - Peruse the documentation on other [common message types](http://wiki.ros.org/common_msgs?distro=kinetic) such as actionlib_msgs, diagnostic_msgs, geometry_msgs, nav_msgs,  sensor_msgs, shape_msgs, stereo_msgs, trajectory_msgs, and  visualization_msgs. 

## Week 7:  Turtlebot-2 Simulation in Gazebo, SLAM Navigation, Adaptive Monte-Carlo Localization

This assignment demonstrates a larger robot system using  sophisticated ROS packages to use a simulate mobile robot and  Simultaneous Localization and Mapping (SLAM) algorithms to construct a  2D map, and then to use the map to do 2D adaptive monte-carlo navigation of this robot, all using existing available ROS packages.

### Topics

- What is a [turtlebot](http://www.turtlebot.com/)?
- Turtlebot ROS [documentation](http://wiki.ros.org/Robots/TurtleBot).
- Turtlebot [Simulation with Gazebo](http://wiki.ros.org/turtlebot_simulator).
- SLAM: [Wikipedia](http://en.wikipedia.org/wiki/Simultaneous_localization_and_mapping), 
- [OpenSLAM](https://openslam-org.github.io/)
- ROS slam [gmapping package](http://wiki.ros.org/slam_gmapping?distro=kinetic)
- ROS adaptive monte-carlo localization package [amcl](http://wiki.ros.org/amcl)

### Reading

- What is a [turtlebot](http://www.turtlebot.com/);
- Turtlebot ROS [documentation](http://wiki.ros.org/Robots/TurtleBot).
- Turtlebot [Simulation with Gazebo](http://wiki.ros.org/turtlebot_simulator).
- ROS slam [gmapping package documentation](http://wiki.ros.org/slam_gmapping?distro=kinetic)
- ROS adaptive monte-carlo localization package [amcl package documentation](http://wiki.ros.org/amcl)

## Week 8: OROCOS KDL

### Topics

- Formulate your class project and
- Form your team
- Write your project proposal.
- Get your project proposal approved by Prof. Leonard.
- OROCOS Kinematics Dynamics Library

### Reading

- [Orocos Kinematics and Dynamics Library](https://www.orocos.org/kdl): [API](http://docs.ros.org/indigo/api/orocos_kdl/html/) and [Examples](https://www.orocos.org/kdl/examples)

## Week 9: ROS Control and Plugins

### Topics

- [ROS Control and ROS controllers](https://www.orocos.org/bfl)

### Reading

- ROS Control Gazebo tutorial http://gazebosim.org/tutorials/?tut=ros_control

## Week 10: MoveIt! Plugins

### Topics

- MoveIt!, path planning and plugins

## Reading

- MoveIt! tutorial https://ros-planning.github.io/moveit_tutorials/

## Week 11: Rviz Plugins

## Week 12: Orocos Realtime-Toolkit (RTT)

## Reading

- https://www.orocos.org/rtt/presentations

## Week 13: Orocos Bayesian Filter Library (BFL)

## Reading

- https://www.orocos.org/bfl