<h1 align="center"> SYSTEM PROGRAMMING WEEK 06 </h1>
<h2 align="center"> GAZEBO </h2>
<h5 align="right"> -- by Jeremy Zhang @ Johns Hopkins University</h5>


---

[TOC]

# I. Gazebo

## 1.1  Introduction

Dynamic Simulation that gazebo relies on

-  __[Open Dynamic Engine](https://www.ode.org/)__  

- __[Bullet Real-time Physics](https://pybullet.org/wordpress/)__ 

gazebo is independent, it just behaves well on ROS

gazebo message in ROS is based on client-service system in ROS.

Rviz is a visualization process, it doesn't provide any dynamics simulation, which normally done by gazebo.

```bash
1. gzserver #  start the gazebo server to process the dynamics of the system
2. gzclient # start the visualize client gui after we have a gzserver
3. gazebo # this command is just a visualization part
```

For any robotics projects, it's better to perform some simulation first.

Now you'll have a blank gazebo GUI and you cannot find anything about gazebo in rostopic or rosnode. 'Cause they run  separately. You have to construct some "bridge" to connect ROS and GAZEBO: In command, put in:

```bash
# First you have to shut down gzserver that you started directly from the terminal.
# And you may start gazebo through rosrun
roscore
rosrun gazebo_ros gzserver
gzclient
# this is a normal start up to get gazebo connected to ros
```

simply just run gazebo means you start a gazebo that runs in its own workspace, it has nothing to do with ROS. If you want to catch the messages that come from gazebo, You gotta rosrun the gazebo node to do that.

Check gazebo-ros bridges after you start gazebo_ros node

```bash
$ rosservice list | grep gazebo
$ rostopic list
/clock
/gazebo/link_states
/gazebo/model_states
/gazebo/parameter_descriptions
/gazebo/parameter_updates
/gazebo/set_link_state
/gazebo/set_model_state
/rosout
/rosout_agg
$ rosnode list
/gazebo
/rosout
```

**To be more specific, there's one topic that publishes model status in rostopic**
**You can check that topic by rostopic echo /gazebo/model_states**

```bash
name: [ground_plane, unit_box, cart_front_steer, create] # all the geometry things are shown here.
pose: 
  - 
    position: 
      x: 0.0
      y: 0.0
      z: 0.0
    orientation: 
      x: 0.0
      y: 0.0
      z: 0.0
      w: 1.0
```

By default, gazebo uses "world" frame as the reference frame.



## 1.2 Use urdf to generate gazebo simulation

---

### Link

in urdf file, you'll need to specify the <inertial> and <collison> label for a link instead of specifying the <visual> part alone.

```bash
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

After modifying urdf file, you can either use roslaunch or command line to set the parameter in the parameter server like this:

```bash
$ rosparam set robot_description -t urdf_file_name
```

Now the next step we need to do is start a spawn model in gazebo to visualize the model we defined in urdf file.

```bash
$ rosrun gazebo_ros spawn_model -urdf -param /robot/robot_description -model robot
# /robot/robot_description is the robot description topic in the /robot namespace, you can specify any namespace for your project
```



### Joint

You have the gazebo showing the links' geometry now. If you wanna have some joints that connect those links can be loaded in gazebo either, you have to do a lot more things.



Btw, test your gazebo spawn by Jackal.

```bash
roslaunch jackal_description description.launch # publish the robot_description
rosrun gazebo_ros spawn_model -urdf -param robot_description -model robot # ask gazebo to generate a model based on the robot_description parameters
```

And you can set the state of the jackal's link in terminal

```bash
rostopic pub -1 /gazebo/set_link_state gazebo_msgs/LinkState "link_name: 'robot::base_link'
pose:
  position:
    x: 0.0
    y: 0.0
    z: 10.0
  orientation:
    x: 0.0
    y: 0.0
    z: 0.0
    w: 0.0
twist:
  linear:
    x: 0.0
    y: 0.0
    z: 0.0
  angular:
    x: 0.0
    y: 0.0
    z: 0.0
reference_frame: 'world'" 
```



## 1.3 SDF

sdf file can contain shade light and sort of thing like that. It's better than the urdf when it comes to simulation. In gazebo, its default robot description format is sdf.__[SDF introduction](http://sdformat.org/spec)__

ROS knows nothing about the geometries you have in gazebo. You have to create a urdf for that model on your own. urdf can be regarded as a simple version of sdf file. SDF contains more tags that allows users to do various things.

You can add sensors in sdf file. Or you can do the same thing in urdf with **[gazebo extension](http://gazebosim.org/tutorials?tut=ros_gzplugins)**.

```xml
  <!-- camera -->
  <gazebo reference="camera_link">
    <sensor type="camera" name="camera1">
      <update_rate>30.0</update_rate>
      <camera name="head">
        <horizontal_fov>1.3962634</horizontal_fov>
        <image>
          <width>800</width>
          <height>800</height>
          <format>R8G8B8</format>
        </image>
        <clip>
          <near>0.02</near>
          <far>300</far> <!-- range of the camera-->
        </clip>
        <noise>
          <type>gaussian</type>
          <!-- Noise is sampled independently per pixel on each frame.
               That pixel's noise value is added to each of its color
               channels, which at that point lie in the range [0,1]. -->
          <mean>0.0</mean>
          <stddev>0.007</stddev>
        </noise>
      </camera>
        
        
        
        
 <!-- plugin is the part that generate the gazebo model from xacro camera tags -->       
      <plugin name="camera_controller" filename="libgazebo_ros_camera.so">
        <alwaysOn>true</alwaysOn>
        <updateRate>0.0</updateRate>
        <cameraName>rrbot/camera1</cameraName>
        <imageTopicName>image_raw</imageTopicName>
        <cameraInfoTopicName>camera_info</cameraInfoTopicName>
        <frameName>camera_link</frameName>
        <hackBaseline>0.07</hackBaseline>
        <distortionK1>0.0</distortionK1>
        <distortionK2>0.0</distortionK2>
        <distortionK3>0.0</distortionK3>
        <distortionT1>0.0</distortionT1>
        <distortionT2>0.0</distortionT2>
      </plugin>
    </sensor>
  </gazebo>
```

 Once you have the camera model, you can use joint label to attach it to other rigid bodies or frames. And the rest part is as normal, you can just launch the urdf to write something into parameter server and then use gazebo to "track " those parameters with the "spawn" node. Afterwards, start a gui to check the view from the camera. 

```bash
$ rosrun rqt_image_view rqt_image_view
```

You can have different cameras by using the arg naming techniques. 

You are allowed to create plugins to start along with gazebo so that you can put some sensor data in it 

---

```bash
cd ~/.gazebo/models # will lead you to the model libraries where gazebo loads
```

you can view how those sdf file are constructed and use them as your tutorial.

In the SDF file we can notice that SDF is pretty much like the urdf. Here is a piece of code that clipped from /apollo15_landing_site_1000x1000.

```xml
      <visual name="visual">
        <geometry>
          <heightmap>
            <texture>
              <diffuse>model://apollo15_landing_site_1000x1000/materials/textures/AS16-110-18026HR-512x512.jpg</diffuse>
              <normal>file://media/materials/textures/flat_normal.png</normal>
              <size>2</size>
            </texture><uri>model://apollo15_landing_site_1000x1000/materials/textures/NAC_DTM_APOLLO15_E261N0036_257x257+7472+2152.png</uri>
            <size>514 514 49</size>
            <pos>0 0 -45</pos>
          </heightmap>
        </geometry>
      </visual>
```

You can load the COLLADA (COLLAborative Design Activity) model from a url link. That could save a lot of time.



joint_state_publisher_gui won't work for gazebo, which means you cannot adjust the model through slide bars in joint_state_publisher_gui. But you can make that happen through some kinda APIs.

---

And again, gazebo is not ros if you simply start gazebo like this:

```bash
$ gzserver # this will not create a node in ros 
$ rosrun gazebo_ros gzserver # You can only have gazebo known by ROS through this command
```

### Where are the worlds located?

You may have noticed the mysterious `worlds/pioneer2dx.world` argument in the above command. This instructs gazebo to find the `pioneer2dx.world` file, and load it on start.

World files are located in a versioned system directory, for example `/usr/share/gazebo-7` on Ubuntu.  If you have Gazebo 7.0 installed on Ubuntu, in a terminal type the following to see a complete list of worlds.

```
ls /usr/share/gazebo-7/worlds
```

For a Gazebo 7.0 installation on OS X using Homebrew, type the following to see a complete list of worlds.

```
ls /usr/local/share/gazebo-7/worlds
```

# II. C++ for gazebo plugin

GAZEBO has a bunch of plugins for cpp and ros you can find them __[here](http://gazebosim.org/tutorials?tut=ros_gzplugins)__

- [ModelPlugins](http://osrf-distributions.s3.amazonaws.com/gazebo/api/dev/classgazebo_1_1ModelPlugin.html), to provide access to the [physics::Model](http://osrf-distributions.s3.amazonaws.com/gazebo/api/dev/classgazebo_1_1physics_1_1Model.html) API
- [SensorPlugins](http://osrf-distributions.s3.amazonaws.com/gazebo/api/dev/classgazebo_1_1SensorPlugin.html), to provide access to the [sensors::Sensor](http://osrf-distributions.s3.amazonaws.com/gazebo/api/dev/classgazebo_1_1sensors_1_1Sensor.html) API
- [VisualPlugins](http://osrf-distributions.s3.amazonaws.com/gazebo/api/dev/classgazebo_1_1VisualPlugin.html), to provide access to the [rendering::Visual](http://osrf-distributions.s3.amazonaws.com/gazebo/api/dev/classgazebo_1_1rendering_1_1Visual.html) API

start the header file with **namespace gazebo**

## 2.1 wrench_apply_plugin.hpp 

```c++
#include <gazebo_plugins/gazebo_ros_utils.h>
#include <gazebo/common/common.hh> // gazebo has a habit of naming their hearders as .hh

namespace gazebo{

    class wrench_apply_plugin : public ModelPlugin{

        private:

            GazeboRosPtr gazebo_ros;
            physics::ModelPtr parent;
            event::ConnectionPtr event1;
            physics::WorldPtr world;
            ros::ros::Publisher publisher; // You can only get access to these things like Model World Connect etc through Pointer class since they are given and used by gazebo, you don't have the permission to pull them out from the RAM and modify them directly. Pointer is the only effective way to do that.

        public:

            wrench_apply_plugin();
            ~wrench_apply_plugin();

            void Init();
            void Reset();
            void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
            // Init Reset and Load are pure virtual member function that we always need to 
            // specify in a ModelPlugin class

            void callback1(); // You can have many callbacks in the same file
    };
}
```



## 2.2 wrench_apply_plugin.cpp

```cpp
#include <wrench_apply_plugin/wrench_apply_plugin.hpp>
#include <std_msgs/String.h>

namespace gazebo{

    wrench_apply_plugin::wrench_apply_plugin(){}
    wrench_apply_plugin::~wrench_apply_plugin(){}

    void wrench_apply_plugin::Init(){
        std::cout << "Init" << std::endl;
    }
    
    void wrench_apply_plugin::Reset(){
        std::cout << "Reset" << std::endl;
    }
    
    void wrench_apply_plugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf){
        
        std::cout << "Load" << std::endl;

        parent = _model;
        world = _model->GetWorld(); // That's how you manage to fetch the model and world geometry information from those pointers.
        gazebo_ros = GazeboRosPtr( new GazeboRos( _model, _sdf, "wrench_apply" ));
        gazebo_ros->isInitialized();

        std::cout << _model->GetName() << std::endl;
        std::cout << _sdf->GetName() << std::endl;
        std::cout << _sdf->HasElement("topic") <<std::endl; // _sdf gives you access to view and modify the things that are created in the <gazebo> label inside the urdf
        
        std::string topicname;
        gazebo_ros->getParameter<std::string>(topicname, "topic", "defaultname"); // pass parameter from this function such that you don't have to hardcode the topicname

        publisher = gazebo_ros->node()->advertise<std_msgs::String>(topicname,10);

        event1 = event::Events::ConnectWorldUpdateBegin( boost::bind( &wrench_apply_plugin::callback1, this));

    }

    void wrench_apply_plugin::callback1(){
    
        physics::Model_V vm = world->Models();// You can get access the Models listed in gazebo through this command in Gazebo 7, the member function is called as GetModels
    
        for( int i=0; i<vm.size(); i++){
            std_msgs::String name;
            name.data = vm[i]->GetName();
            publisher.publish(name);// publish what you just get from the model pointer

        }
    }

    GZ_REGISTER_MODEL_PLUGIN( wrench_apply_plugin )
} // this is a macro register to make the gazebo to tell this is a plugin
}
```

Let's break down the code a little bit:

```cpp
event1 = event::Events::ConnectWorldUpdateBegin( boost::bind( &wrench_apply_plugin::callback1, this));
```

This is a event class, you can find the API [description here](http://osrf-distributions.s3.amazonaws.com/gazebo/api/dev/group__gazebo__event.html)

Events allows you to create various kinds of callbacks when "something " happens.

For example, if you want to have a sychronic callback that runs as long as you have a world frame in the gazebo simulation.

```cpp
static ConnectionPtr ConnectWorldUpdateBegin( T_subscriber )
```

---

And for the last line for the cpp code, we are introducting plugins

Remember, Plugins are different creatures, just build a header file and build it as a library doesn't make it a "plugin", you gotta add something to have the complier know this is a plugin.

Then how does GAZEBO knows that this piece of code is a plugin? Through the last line of the code.

```cpp
GZ_REGISTER_MODEL_PLUGIN( wrench_apply_plugin )
```

This a macor register to build the above codes into a gazebo plugin. Always remember to put this at the end of you library code.

---

## 2.3 CMakeLists

Gazebo is not ros. SO you need to do a lot of things in CMakeLists to help catkin find gazebo packages.

First, you need to add another find_package tag for gazebo :

```cmake
find_package(catkin REQUIRED COMPONENTS
  gazebo_plugins # you have to imclude these plugin package here first 
  gazebo_ros
  roscpp
  std_msgs
)

## System dependencies are found with CMake's conventions
find_package(gazebo REQUIRED) # and then include gazebo packages here
```

Second, you need to add a include directory

```cmake
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${GAZEBO_INCLUDE_DIRS} # include gazebo library 
)
```

Third, build your own code into a library

```cmake
add_library(wrench_apply_plugin
  src/wrench_apply_plugin.cpp
)
target_link_libraries(wrench_apply_plugin
  ${catkin_LIBRARIES}
)
# for target link here you dont have to include the gazebo plugin dependencies because catkin_LIBRARIES have those dependencies ( you've specified those things in the find_package part)
```



## 2.4 package.xml

There is one thing that any package that has a plugin needs to do. That is to tell the program ( in our case GAZEBO) that we provide a legitimate plugin in this package.

```xml
  <export>

    <gazebo_ros plugin_path="${prefix}/lib" gazebo_media_path="${prefix}"/>
  </export>
    <!-- This thing should appear at the end of the package.xml to indicate where the plugin static library locates. Through gazebo_media_path you can tell gazebo more information about your model, you can add cad files, texture files, etc  -->
```



## 2.5 Load Plugins when running gazebo

```bash
$ rosrun gazebo_ros gzserver
```

will not automatically start the plugins

Before that one important thing to remember:

```bash
$ rosrun gazebo_ros gzserver --verbose 
# --verbose will print the warning and error log to help you debug you codes
```



After you have created a robot in urdf file.

```bash
$ rosrun gazebo_ros gazebo # start the server and GUI within one command
$ rosparam set robot_description -t wrench_apply.urdf # set description in the parameter server
$ rosrun gazebo_ros spawn_model -urdf -param robot_description -model jzhan -x 5# will spawn a robot based on the urdf and name it as jzhan and set the origin at x = 5 
# A spawn model will generate several ros topics according to the robot_description parameter so that these topics can be subscribed by rviz and other ros ndoes
# -param robot_description means you need to load the model data from a ros parameter called robot_description
# -urdf means the robot_description you upload is generated based on a urdf format file
```



```bash
$ rosservice call /gazebo/delete_model "model_name: 'jzhan'" # will delete a existing model in gazebo
```

If you wanna build a bridge between gazebo and ros, the only way is through gazebo plugins. There is no other way around



```bash
$ rosservice call /gazebo/reset_world # can restart the simulation
```



## 2.6 In the end...

Gazebo is not that complcated, it only provides some visualization process and it allows users to play with it through plugins. The math is actually done by the engine, not gazebo. Gazebo acts like a agent.





# Gazebo Pre-Defined Sensors

## 3.1 Bumper

#### Understanding Bumper Sensors in Gazebo

**Bumper Sensor Overview:**

- **Functionality:** A bumper sensor in Gazebo is used to detect collisions between different objects in a simulation environment. It's often employed in robotics to simulate the physical interaction of a robot with its surroundings.
- **How It Works:** The sensor monitors a specified collision element (part of the robot or object model) and reports whenever this element comes into contact with another object. This data is crucial for tasks like obstacle avoidance, interaction with environments, and tactile feedback.
- **Implementation:** In ROS-Gazebo integration, the bumper sensor is typically implemented through a plugin (e.g., `libgazebo_ros_bumper.so`) in the URDF/SDF model of the robot or object. It publishes data to a ROS topic, which can then be used by other ROS nodes for various computational tasks.

#### Issue: Collision Element Naming in Gazebo SDF Generation and Its Effects

**Symptoms and Behaviors:**

- **Non-Functional Bumper Sensor:** The primary symptom of the naming discrepancy issue is that the bumper sensor does not function as expected. In the specific case you encountered, the bumper sensor failed to report collisions.
- **Empty ROS Topic:** The ROS topic associated with the bumper sensor (`/ball_contact` in your case) showed empty messages or lacked the expected collision data. This was a direct result of the sensor not being able to correctly reference the collision element due to the name change after URDF to SDF conversion.

**Problem Description:** In Gazebo simulations integrated with ROS, when a URDF file is converted to SDF format, the names of collision elements are automatically altered. Gazebo appends an additional `_collision` to each collision element's name. For example, a collision element named `robot_collision` in the URDF would be renamed to `robot_collision_collision` in the SDF. This renaming can lead to functional discrepancies, especially in simulations that use specific element names for interaction or detection.

**Symptoms and Behaviors:**

- **Sensor Malfunction:** Sensors such as bumper or contact sensors may fail to detect collisions or report incorrect data. This is because the sensor's configuration might reference the original URDF name, which no longer matches the altered name in the SDF.
- **Plugin Configuration Issues:** Plugins relying on specific naming conventions (e.g., for linking with collision elements) might not function as intended. This could manifest as a complete lack of plugin activity or unexpected behaviors.
- **Inaccurate Simulation Feedback:** The simulation might not reflect physical interactions accurately, leading to confusion about whether the issue is due to physical parameters or a software bug.

**Example Case:** A bumper sensor configured to detect contacts with `robot_collision` failed to function because, after URDF to SDF conversion, the actual collision element name in the simulation became `robot_collision_collision`.

**Solution:**

- **Name Adjustment:** Align the names used in plugins, sensors, or other functionalities with the modified names in the SDF file.
- **SDF Modification:** Directly edit the SDF file to ensure the names of elements are consistent with the expected naming conventions.

**Best Practices:**

- **SDF Inspection:** Regularly check the auto-generated SDF files for discrepancies in element names.
- **Awareness in Development:** Be cognizant of Gazebo's automatic renaming when developing ROS packages and designing simulations involving Gazebo.
- **Simulation Testing:** Conduct thorough testing of simulations, particularly when introducing new elements or sensors, to ensure all components interact correctly.

To generate the sdf from given urdf, you can try with:

```bash
gz sdf -p /path/to/your/model.urdf > /path/to/your/model.sdf
```



**Conclusion:** The automatic renaming of elements during URDF to SDF conversion in Gazebo can lead to significant functional issues in ROS-Gazebo simulations. This behavior underscores the importance of thorough testing and validation of simulations, especially when integrating sensors and plugins. Recognizing the symptoms of this issue - such as malfunctioning sensors or plugins not behaving as expected - can be key in diagnosing and resolving problems stemming from naming discrepancies. Awareness of this conversion behavior is crucial for developers working with ROS and Gazebo to ensure accurate and reliable simulation outcomes.
