<h1 align="center"> SYSTEM PROGRAMMING WEEK 14 </h1>
<h2 align="center"> ROS DYNAMIC RECONFIGURE </h2>
<h5 align="right"> -- by Jeremy Zhang @ Johns Hopkins University</h5>

---



# I. Introduction

The dynamic_reconfigure package provides a means to update parameters   at runtime without having to restart the node.

- Maintainer status: maintained
- Maintainer: Michael Carroll <michael AT openrobotics DOT org>
- Author: Blaise Gassend
- License: BSD
- Bug / feature tracker:  https://github.com/ros/dynamic_reconfigure/issues
- Source: git [https://github.com/ros/dynamic_reconfigure.git](https://github.com/ros/dynamic_reconfigure) (branch: noetic-devel)

At present, the focus of `dynamic_reconfigure` is on providing a standard way to expose a subset of a node's  parameters to external reconfiguration.  Client programs, e.g., GUIs,  can query the node for the set of reconfigurable parameters, including  their names, types, and ranges, and present a customized interface to  the user. This is especially useful for hardware drivers, but has  broader applicability. 



It depend on python, so make sure to make rospy into your package dependency



# II. Details...

## 2.1 package called rsp_sensor

```bash
$ catkin_create_pkg rsp_sensor dynamic_reconfigure roscpp rospy geometry_msgs 
```

first we need a node to publish some wrench msgs:

sensor.hpp

```cpp
# include <ros/ros.h>

#include <dynamic_reconfigure/server.h>
#include <rsp_sensor/SensorConfig.h>
#include <geometry_msgs/WrenchStamped.h>

class sensor{

    private:

    ros::NodeHandle nh;
    ros::Publisher publisher;


    ros::Rate rate;
    bool enable;
    std::string frame_id;//cause we want to publish a wrench here, 
                        //so we need a frame id to generate the wrench message

    public:

    sensor( ros::NodeHandle& nh);

    void publish();
}
```

sensor.cpp

```cpp
#include <rsp_sensor/sensor.hpp>

sensor::sensor(ros::NodeHandle &nh) : nh(nh),
                                      rate(1),
                                      enable(false)
{
    publisher = nh.advertise<geometry_msgs::WrenchStsmped>("wrench", 1);
    // publish a wrench message with the rate of 1

}

void sensor::publish()
{
    if (enable)
    {
        geometry_msgs::WrenchStamped ft;
        ft.header.frame_id = frame_id;
        publisher.publish(ft);
        rate.sleep();
    }
}
```

sensor_node.cpp

```cpp
#include <rsp_sensor/sensor.hpp>

int main( int argc, char** argv){

    ros::init(argc, argv, "sensor");
    ros::NodeHandle nh;
    sensor my_sensor(nh);

    while(nh.ok()){
        my_sensor.publish();
        ros::spinOnce();
    }
}
```

Modify some cmakelist things and then we will have a fully functioned publisher to publish some wrench messages.



## 2.2 How to get the parameter?

Well we published a wrench that never have any values. Typically, we can use NodeHandle.getParam to obtain a value from the existing parameter. But this is not what we want to do. We want to have a GUI and directly send the value to the publisher and then be published. So now we have to turn to dynamic reconfigure stuff.



Most of the time, the locates in a package/cfg directory

Sensor.cfg: It's a python script

```python
#!/usr/bin/env python

PACKAGE = "rsp_sensor"

from dynamic_reconfigure.parameter_generator_catkin import *

gen = ParameterGenerator()

# we can change whatever parameter we want by gen.add
gen.add("sensor_frequency", double_t, 1, "Sensor frequency loop", 1.0, 0.0,
        10.0)
# for the last three numbers 
# 1.0 is the default value
# 0.0 is the min value
# 1.0 is the max
gen.add("frame_id", str_t, 2, "Frame ID", "Sensor")
# "Sensor" is the default value for frame_it
gen.add("enable", bool_t, 4, "Enable sensor", False)
# False is default value

units_enum = gen.enum([
    gen.const("US", int_t, 0, "US lb/lb-in"),
    gen.const("SI", int_t, 1, "SI N/Nm")
], "Sensor units")
# enumeration method can help you to create some drop down menu that you can select

gen.add("units", int_t, 8, "Units (US SI)", 1, 0, 1, edit_method=units_enum)
# add the enum to the parametor generator
# "Units (US SI)" is the name 
# edit_method=units_enum is where you specify the enum thing

exit(gen.generate(PACKAGE, "rsp_sensor", "Sensor"))
# Sensor is the name of this file

```

And ros will make this file into a xxx.h , further make it an excutable file and use it to create a GUI with the options you've set in this py script.



So you must make it available for ros

```bash
~/rsp_workspace/src/rsp_sensor/cfg$ chmod 755 Sensor.cfg
```

and add an option in the cmakelists

```cmake
generate_dynamic_reconfigure_options(
  cfg/Sensor.cfg
  # cfg/DynReconf2.cfg
)
```

Now catkin build and a Sensor.h file will be created.



## 2.3 So what happens behind:

reconfigure actually act as a service to somehow " watch " the parameter server

so it can change the pararmeters dynamically.

To use these service, we will need a callback.

make some modification in the hpp and cpp like this:

sensor.hpp:

```cpp
# include <ros/ros.h>

#include <dynamic_reconfigure/server.h>
#include <rsp_sensor/SensorConfig.h>


#include <geometry_msgs/WrenchStamped.h>

class sensor{

    private:

    ros::NodeHandle nh;
    ros::Publisher publisher;

    dynamic_reconfigure::Server<rsp_sensor::SensorConfig> server;
    dynamic_reconfigure::Server<rsp_sensor::SensorConfig>::CallbackType callback;

    ros::Rate rate;
    bool enable;
    std::string frame_id;//cause we want to publish a wrench here, 
                        //so we need a frame id to generate the wrench message

    public:

    sensor( ros::NodeHandle& nh);

    void publish();
    void sensor_config( rsp_sensor::SensorConfig& config, uint32_t level);
    // here is the callback of our reconfiguration
};
```



sensor.cpp

```cpp
#include <rsp_sensor/sensor.hpp>

sensor::sensor(ros::NodeHandle &nh) : nh(nh),
                                      rate(1),
                                      enable(false)
{
    publisher = nh.advertise<geometry_msgs::WrenchStamped>("wrench", 1);
    // publish a wrench message with the rate of 1

    callback = boost::bind(&sensor::sensor_config, this, _1, _2);
    // &sensor::sensor_config is the name of the callback
    // since there are two parameters for the callback function, so here we use _1 and _2 to clarify
    // to tell the bind function put the first thing it found into the rsp_sensor::SensorConfig &config
    // and put the second thing in uint32_t level
    server.setCallback(callback);
    // you can have a server with multiple callbacks
    
}

void sensor::sensor_config(rsp_sensor::SensorConfig &config, uint32_t level)
{

    std::cout << config.sensor_frequency << std::endl
              << config.frame_id << std::endl
              << config.enable << std::endl
              << config.units << std::endl
              << level << std::endl;

    if (level == 4) { enable = config.enable; }
    if (level == 2) { frame_id = config.frame_id; }
    if (level == 1) { rate = ros::Rate(config.sensor_frequency); }
}

void sensor::publish()
{
    if (enable)
    {
        geometry_msgs::WrenchStamped ft;
        ft.header.frame_id = frame_id;
        publisher.publish(ft);
        rate.sleep();
    }
}
```



Now catkin build.

And then rosrun this node.

If you want to have a look at the gui, you can run rqt

```bash
~/rsp_workspace$ rosrun rqt_reconfigure rqt_reconfigure 
```

and you will see a rqt GUI:

![Screenshot from 2022-04-29 00-33-32](/home/jeremy/rsp_workspace/src/System Programming Note/Screenshot from 2022-04-29 00-33-32.png)

Here the four parameters we added in the python scripts are shown.



You can toggle the bar , select a units in the drop down menu, click the enable to set the boolean value ...

**Every change you've made will be sent to the server and be fetched by the reconfiguration callback at realtime.**

There will be /sensor/enable /sensor/units rostopics in the rostopic list





# III. Play with RQT GUI

```bash
rosrun rqt_gui rqt_gui
```

You can add the reconfigure plugin in the plugin section

