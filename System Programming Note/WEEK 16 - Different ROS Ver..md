<h1 align="center"> SYSTEM PROGRAMMING WEEK 16 </h1>
<h2 align="center"> Different ROS Ver. </h2>
<h5 align="right"> -- by Jeremy Zhang @ Johns Hopkins University</h5>

<br></br>

# Introduction

ROS has a series of distros. There are cases when we installed multiple distros in the same develop environment. It will be a big problem if we don't manage them properly. Basically, all the distros are incompatible with each other, implying that we can only have one sole activated distro at the same time. 

The good thing is that the engineers who developed ROS actually have worried about that problem for us in advance. They provide a bash script to set up all the environment variables for every distro they publish and they call it as setup.bash.


# Solution
With that being said, to jump between different distro version, essentially you just need to type the source command to cover the previous one, i.e.
```bash
$ source /opt/ros/YOUR_ROS_DISTRO_VERSION/setup.bash 
```
If you are working with ROS 1, that command should be enough. But sometimes when you want to jump between ROS 1 and ROS 2, you might see a warning prompt out, saying:
```bash
ROS_DISTRO was set to 'galactic' before. Please make sure that the environment does not mix paths from different distributions.
```
To fix that, you just simply use the export command like this:
```bash
$ export ROS_DISTRO=noetic
```
