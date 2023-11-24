<h1 align="center"> SYSTEM PROGRAMMING WEEK 15 </h1>
<h2 align="center"> ROSPY </h2>
<h5 align="right"> -- by Jeremy Zhang @ Johns Hopkins University</h5>

<br></br>

# Create Packages
To create ROS python package, we just need to catkin create a package that depend on rospy and then create a sub folder within the package folder.

The name of the subfolder should be scripts, indicating that is for python scripts, which is different with the c++ source code.



```bash
$ sudo chmod +x YOUR_NODE.py
```



# Run Python Code from Terminal

If you want to have fully supported python libraries, you need to source the rospy env in every terminal:

```bash
$ source /opt/ros/noetic/setup.bash
$ rosrun YOUR_PACKAGE YOUR_CODE.py
```

Otherwise, there could be error messages saying some packages are not installed like rospy/rosbag/std_msgs/...
