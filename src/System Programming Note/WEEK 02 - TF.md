<h1 align="center"> SYSTEM PROGRAMMING WEEK 02 </h1>
<h2 align = "center"> TF </h2>
<h5 align="right"> -- by Jeremy Zhang @ Johns Hopkins University</h5>

---

[TOC]



# TF

tf is a package that lets the user keep track of multiple coordinate frames over time. tf maintains the relationship between coordinate frames in a tree structure buffered in time, and lets the user transform points, vectors, etc between any two coordinate frames at any desired point in time. 

**tf is a topic**

```bash
rosrun tf tf_echo # can display all the relationship between frames
rosrun tf static_transform_publisher 1 1 1  0 0 0 1 gripper_pick myframe 10 # can publish a frame defined
# by tx ty tx qx qy qz qw [1 1 1 0 0 0 1] in this example
```



references: http://docs.ros.org/en/indigo/api/tf/html/c++/classtf_1_1StampedTransform.html

ros::Time::now()

