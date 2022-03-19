/* ----------------------------------------------------------------------
   2017-12-10 LLW Created gazebo_ros_edumip_balance_ros.h from 
                  gazebo_ros_diff_drive.h
   2018-03-05 LLW Moved to standalone ROS project edumip_ros_balance_gazebo_plugin
  ---------------------------------------------------------------------- */
/*
 * Copyright (c) 2010, Daniel Hewlett, Antons Rebguns
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *      * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *      * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *      * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY Antons Rebguns <email> ''AS IS'' AND ANY
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL Antons Rebguns <email> BE LIABLE FOR ANY
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/

/*
 * \file  gazebo_ros_diff_drive.h
 *
 * \brief A differential drive plugin for gazebo. Based on the diffdrive plugin
 * developed for the erratic robot (see copyright notice above). The original
 * plugin can be found in the ROS package gazebo_erratic_plugins.
 *
 * \author  Piyush Khandelwal (piyushk@gmail.com)
 *
 * $ Id: 06/21/2013 11:23:40 AM piyushk $
 */

#ifndef EDUMIP_BALANCE_ROS_PLUGIN_HH
#define EDUMIP_BALANCE_ROS_PLUGIN_HH

#include <map>

// Gazebo
#include <gazebo/common/common.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo_plugins/gazebo_ros_utils.h>


#include <ignition/math/Quaternion.hh>

// ROS
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/JointState.h>

// 2018/03/04 15:48:16 LLW: 
#include <edumip_msgs/EduMipState.h>

// Custom Callback Queue
#include <ros/callback_queue.h>
#include <ros/advertise_options.h>

// Boost
#include <boost/thread.hpp>
#include <boost/bind.hpp>

namespace gazebo {

  class Joint;
  class Entity;

  class EdumipBalanceRosPlugin : public ModelPlugin {

    enum OdomSource
    {
        ENCODER = 0,
        WORLD = 1,
    };
    public:
      EdumipBalanceRosPlugin();
      ~EdumipBalanceRosPlugin();
      void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf);
      void Reset();

    protected:
      virtual void UpdateChild();
      virtual void FiniChild();

    private:

      GazeboRosPtr gazebo_ros_;
      physics::ModelPtr parent;
      event::ConnectionPtr update_connection_;

      double wheel_separation_;
      double wheel_diameter_;
      double wheel_torque;

      std::vector<physics::JointPtr> joints_;

      // ROS STUFF
      ros::Subscriber cmd_vel_subscriber_;

      boost::mutex lock;

      std::string command_topic_;

      std::string robot_base_frame_;

      ros::CallbackQueue queue_;
      boost::thread callback_queue_thread_;
      void QueueThread();

      void cmdVelCallback(const geometry_msgs::Twist::ConstPtr& cmd_msg);

      // 2018/03/04 15:49:40 LLW: EdumipBalanceRos stuff
      std::string edumip_state_topic_;      
      ros::Publisher edumip_state_publisher_;

      // Last iteration's rotate and position information in order to calculate velocities
      ignition::math::Quaterniond last_Rot;
      ignition::math::Vector3d    last_Pos;

      double wheel_angle_L;
      double wheel_angle_R;      

      // double last_wheel_angle_L;
      // double last_wheel_angle_R;      

      double wheel_vel_L;
      double wheel_vel_R;
      
      // state vector theta
      // theta[0] is average wheel rotation in radians
      // theta[1] is body tilt forward from vertical
      // theta[2] is d/dt theta[0]
      // theta[2] is d/dt theta[1]      
      double theta[4];
      // desired state vector
      double theta_ref[4] = {0.0, 0.0, 0.0, 0.0};
      // error state vector;
      double theta_error[4];

      // yaw ref in units of radians and rad/sec
      double yaw_ref;
      double yaw_ref_vel;
            
      // torque command to balance robot units=N-m
      double balance_fb_torque;
      double yaw_fb_torque;
      double advance_fb_torque;
      
      geometry_msgs::Twist  cmd_vel;

      // max velocities for full joystick 1.0
      double max_advance_vel;      // m/s
      double max_yaw_vel;          // rad/s

      bool alive_;
      
      // Update Rate
      double update_rate_;
      double update_period_;
      common::Time last_update_time_;


  };

}

#endif

