/* ----------------------------------------------------------------------
   2017-12-10 LLW Created gazebo_ros_edumip_balance_ros.cpp from 
                  gazebo_ros_diff_drive.cpp 
   2018-03-05 LLW Moved to standalone ROS project edumip_balance_ros_gazebo_plugin

  ---------------------------------------------------------------------- */
/*
    Copyright (c) 2010, Daniel Hewlett, Antons Rebguns
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Antons Rebguns <email> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Antons Rebguns <email> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/*
 * \file  gazebo_ros_diff_drive.cpp
 *
 * \brief A differential drive plugin for gazebo. Based on the diffdrive plugin
 * developed for the erratic robot (see copyright notice above). The original
 * plugin can be found in the ROS package gazebo_erratic_plugins.
 *
 * \author  Piyush Khandelwal (piyushk@gmail.com)
 *
 * $ Id: 06/21/2013 11:23:40 AM piyushk $
 */


/*
 *
 * The support of acceleration limit was added by
 * \author   George Todoran <todorangrg@gmail.com>
 * \author   Markus Bader <markus.bader@tuwien.ac.at>
 * \date 22th of May 2014
 */

#include <algorithm>
#include <assert.h>

#include <edumip_balance_ros_gazebo_plugin/edumip_balance_ros_gazebo_plugin.h>

#include <ignition/math/Angle.hh>
#include <ignition/math/Pose3.hh>
#include <ignition/math/Quaternion.hh>
#include <ignition/math/Vector3.hh>
#include <sdf/sdf.hh>

#include <ros/ros.h>

#define PI 3.14159265359
#define RTOD (360.0/(2*PI))
#define DTOR (1.0/RTOD)

#define EDUMIP_WHEEL_SEPARATION  0.070
#define EDUMIP_WHEEL_RADIUS      0.034

// Converts from -pi to pi to 0 to 2pi
#define ZTP(a) (a < 0 ? a + 2.0 * PI : a)
// Converts from 0 to 2pi to -pi to pi
#define NPP(a) (a > PI ? 2.0 * PI - a : a)

#define SIGN(a) (a >= 0 ? 1.0 : -1.0)
#define MAX_MAG(a, b) (fabs(a) > b ? b * SIGN(a) : a)
#define MAX_FB_TORQUE 0.20
#define YAW_MAX_EFFORT 0.05

// ----------------------------------------------------------------------  
namespace gazebo
// ----------------------------------------------------------------------  
{

  enum {
    RIGHT,
    LEFT,
  };

  // ----------------------------------------------------------------------
  // constructor
  EdumipBalanceRosPlugin::EdumipBalanceRosPlugin()
  // ----------------------------------------------------------------------  
    
  {
    // 2018/03/04 15:49:40 LLW: EdumipBalanceRos stuff
    int i;

    last_Rot = ignition::math::Quaterniond::Identity;
    last_Pos.Set(0.0, 0.0, 0.0);

    balance_fb_torque = 0.0;

    yaw_ref = 0.0;   
    yaw_ref_vel = 0.0;   
    yaw_fb_torque = 0.0;

    // advance_ref = 0.0;   
    // advance_ref_vel = 0.0;   
    // advance_fb_torque = 0.0;

    for(i = 0; i<4; i++)
      theta_ref[i] = 0.0;

    cmd_vel.linear.x = 0.0;
    cmd_vel.linear.y = 0.0;
    cmd_vel.linear.z = 0.0;
      
    cmd_vel.angular.x = 0.0;
    cmd_vel.angular.y = 0.0;
    cmd_vel.angular.z = 0.0;

    max_advance_vel = 0.20;          // m/s
    max_yaw_vel     = 20.0 * DTOR;   // rad/s    

  }

  // ----------------------------------------------------------------------  
  // Destructor
  EdumipBalanceRosPlugin::~EdumipBalanceRosPlugin() {}
  // ----------------------------------------------------------------------  

  // ----------------------------------------------------------------------  
  // Load the controller
  void EdumipBalanceRosPlugin::Load ( physics::ModelPtr _parent, sdf::ElementPtr _sdf )
  // ----------------------------------------------------------------------  
  {

    this->parent = _parent;
    gazebo_ros_ = GazeboRosPtr ( new GazeboRos ( _parent, _sdf, "EdumipBalanceRos" ) );

    // Make sure the ROS node for Gazebo has already been initialized
    gazebo_ros_->isInitialized();

    // 2018/03/04 15:49:40 LLW: EdumipBalanceRos stuff
    
    ROS_INFO("EdumipBalanceRos is Loading...");
    gazebo_ros_->getParameter<std::string> ( edumip_state_topic_, "stateTopic", "state" );
    ROS_INFO("EdumipBalanceRos: state_topic_   is %s", edumip_state_topic_.c_str());

    gazebo_ros_->getParameter<std::string> ( command_topic_, "commandTopic", "cmd_vel" );
    ROS_INFO("EdumipBalanceRos: command_topic_ is %s", command_topic_.c_str());
    
    gazebo_ros_->getParameter<std::string> ( robot_base_frame_, "robotBaseFrame", "edumip_base" );
    ROS_INFO("EdumipBalanceRos: robot_base_frame_ is %s", robot_base_frame_.c_str());    
    
    gazebo_ros_->getParameter<double> ( wheel_separation_, "wheelSeparation", 0.34 );
    ROS_INFO("EdumipBalanceRos: wheel_separation_ is %f", wheel_separation_);

    gazebo_ros_->getParameter<double> ( wheel_diameter_, "wheelDiameter", 0.15 );
    ROS_INFO("EdumipBalanceRos: wheel_diameter_ is %f", wheel_diameter_);

    // 2017-12-10 LLW Set wheel torque.  Note that wheelTorque
    // parameter appears to be a coulomb friction threshold, set
    // it to zero in the URDF i.e.  <wheelTorque>0.0</wheelTorque>
    // gazebo_ros_->getParameter<double> ( wheel_torque, "wheelTorque", 0.0 );
    // ROS_INFO("EdumipBalanceRos wheel_torque is %f", wheel_torque);    
    wheel_torque = 0.0;

    gazebo_ros_->getParameter<double> ( update_rate_, "updateRate", 100.0 );
    ROS_INFO("EdumipBalanceRos: update_rate_ is %f", update_rate_);

    // setup joint object
    joints_.resize ( 2 );
    joints_[LEFT] = gazebo_ros_->getJoint ( parent, "leftJoint", "left_joint" );
    joints_[RIGHT] = gazebo_ros_->getJoint ( parent, "rightJoint", "right_joint" );
    joints_[LEFT]->SetParam ( "fmax", 0, wheel_torque );
    joints_[RIGHT]->SetParam ( "fmax", 0, wheel_torque );

    // Initialize update rate stuff
    if ( this->update_rate_ > 0.0 ) this->update_period_ = 1.0 / this->update_rate_;
    else this->update_period_ = 0.0;
    last_update_time_ = parent->GetWorld()->SimTime();

    // initialize command velocoties
    cmd_vel.linear.x  = 0.0;
    cmd_vel.angular.x  = 0.0;    
    alive_ = true;    

    // 2018/03/04 15:56:31 LLW: 
    edumip_state_publisher_ = gazebo_ros_->node()->advertise<edumip_msgs::EduMipState>(edumip_state_topic_, 10);
    ROS_INFO("EdumipBalanceRos: Advertising EduMIP state on topic %s", edumip_state_topic_.c_str());

    // ROS: Subscribe to the velocity command topic (usually "cmd_vel")
    //    ROS_INFO_NAMED("EdumipBalanceRos ", "%s: Try to subscribe to %s", gazebo_ros_->info(), command_topic_.c_str());
    ROS_INFO("EdumipBalanceRos: Try to subscribe to %s", command_topic_.c_str());    

    ros::SubscribeOptions so =
      ros::SubscribeOptions::create<geometry_msgs::Twist>(command_topic_, 1,
							  boost::bind(&EdumipBalanceRosPlugin::cmdVelCallback, this, _1),
							  ros::VoidPtr(), &queue_);

    cmd_vel_subscriber_ = gazebo_ros_->node()->subscribe(so);
    //    ROS_INFO_NAMED("diff_drive", "%s: Subscribe to %s", gazebo_ros_->info(), command_topic_.c_str());
    ROS_INFO("EdumipBalanceRos: Subscribe to %s", command_topic_.c_str());    

    // start custom queue for diff drive
    this->callback_queue_thread_ =
      boost::thread ( boost::bind ( &EdumipBalanceRosPlugin::QueueThread, this ) );

    // listen to the update event (broadcast every simulation iteration)
    this->update_connection_ =
      event::Events::ConnectWorldUpdateBegin ( boost::bind ( &EdumipBalanceRosPlugin::UpdateChild, this ) );

  }

  // ----------------------------------------------------------------------
  void EdumipBalanceRosPlugin::Reset()
  // ----------------------------------------------------------------------    
  {
    last_update_time_ = parent->GetWorld()->SimTime();

    cmd_vel.linear.x  = 0.0;
    cmd_vel.angular.x  = 0.0;    

    joints_[LEFT]->SetParam ( "fmax", 0, wheel_torque );
    joints_[RIGHT]->SetParam ( "fmax", 0, wheel_torque );
  }

  // ----------------------------------------------------------------------
  // Finalize the controller
  void EdumipBalanceRosPlugin::FiniChild()
  // ----------------------------------------------------------------------    
  {
    alive_ = false;
    queue_.clear();
    queue_.disable();
    gazebo_ros_->node()->shutdown();
    callback_queue_thread_.join();
  }

  // ----------------------------------------------------------------------
  void EdumipBalanceRosPlugin::cmdVelCallback ( const geometry_msgs::Twist::ConstPtr& cmd_msg )
  // ----------------------------------------------------------------------    
  {
    boost::mutex::scoped_lock scoped_lock ( lock );
    // x_ = cmd_msg->linear.x;
    // rot_ = cmd_msg->angular.z;
    cmd_vel = *cmd_msg;
  }

  void EdumipBalanceRosPlugin::QueueThread()
  {
    static const double timeout = 0.01;

    while ( alive_ && gazebo_ros_->node()->ok() ) {
      queue_.callAvailable ( ros::WallDuration ( timeout ) );
    }
  }


  // ----------------------------------------------------------------------
  // Update the controller - this is called on every Gazebo sinulation step
  // ----------------------------------------------------------------------  
  void EdumipBalanceRosPlugin::UpdateChild()
  {

    /* force reset SetParam("fmax") since Joint::Reset reset MaxForce to zero at
       https://bitbucket.org/osrf/gazebo/src/8091da8b3c529a362f39b042095e12c94656a5d1/gazebo/physics/Joint.cc?at=gazebo2_2.2.5#cl-331
       (this has been solved in https://bitbucket.org/osrf/gazebo/diff/gazebo/physics/Joint.cc?diff2=b64ff1b7b6ff&at=issue_964 )
       and Joint::Reset is called after ModelPlugin::Reset, so we need to set maxForce to wheel_torque other than EdumipBalanceRosPlugin::Reset
       (this seems to be solved in https://bitbucket.org/osrf/gazebo/commits/ec8801d8683160eccae22c74bf865d59fac81f1e)
    */
    for ( int i = 0; i < 2; i++ ) {
      if ( fabs(wheel_torque -joints_[i]->GetParam ( "fmax", 0 )) > 1e-6 ) {
        joints_[i]->SetParam ( "fmax", 0, wheel_torque );
      }
    }


    // if ( odom_source_ == ENCODER ) UpdateOdometryEncoder();
    common::Time current_time = parent->GetWorld()->SimTime();
    double seconds_since_last_update = ( current_time - last_update_time_ ).Double();


    if ( seconds_since_last_update > update_period_ )
      {

	// get pose
	ignition::math::Quaterniond rot = parent->WorldPose().Rot();
	ignition::math::Vector3d    pos = parent->WorldPose().Pos();

	ignition::math::Vector3d    omega = parent->RelativeAngularVel();
	//	math::Vector3    vel = parent->RelativeLinearVel();
	
	// Current attitude
	double roll   = rot.Roll();
	double pitch  = rot.Pitch();
	double yaw    = rot.Yaw();

	// // // Current angular velocities
	double rollV =0.0;
	double pitchV =0.0;
	double yawV =0.0;

	// don't compute velocities if dt less than 100 uS
	if(seconds_since_last_update > 0.0001)	  
	  {
	    rollV  = (roll  - last_Rot.Roll())  / seconds_since_last_update;
	    pitchV = (pitch - last_Rot.Pitch()) / seconds_since_last_update;
	    yawV   = (yaw   - last_Rot.Yaw())   / seconds_since_last_update;
	  }

	//	rollV = omega.x;
	//	pitchV = omega.y;
	yawV = omega.Z();	

	// store last pose
	last_Rot = rot;
	last_Pos = pos;

	// get joint positions
	wheel_angle_L = joints_[RIGHT]->Position( 0 );
	wheel_angle_R  = joints_[LEFT]->Position( 0 );

	// get joint velocities
	wheel_vel_L = joints_[RIGHT]->GetVelocity ( 0 );
	wheel_vel_R  = joints_[LEFT]->GetVelocity ( 0 );

	// assign state vector
	// theta[0] is average anglular position of both wheels, with respect to rest pos
	// note that joint position is angle between edumip_body and wheel, so add body pitch
	theta[0] = pitch  + (0.5 * (wheel_angle_L + wheel_angle_R));
	// theta[2] is average angular vel of both wheels
	theta[2] = pitchV + (0.5 * (wheel_vel_L + wheel_vel_R));

	// theta[1] is pitch of edumip_body
	theta[1] = pitch;
	// theta[3] is pitch rate of edumip_body
	theta[3] = pitchV;

	// compute control
	// poles at -1
	// double balance_fb_gains[4] ={ 0.000000172477064, -0.036038321654179, 0.000000689908257, 0.000080874932648};
	// this give a stable balance, no advance feedback
	//	double balance_fb_gains[4] ={ 0.0,  0.05,  -0.00,  0.05};

	// this give a stable balance with advance feedback
	//  double fb_gains[4] = { +0.00001,  0.05,  0.0005,  0.05};

	// this give a stable balance with advance feedback	
	double fb_gains[4] = { +0.00001,  0.05,  0.002,  0.05};	

        // state vector theta
        // theta_ref[0] is ref average wheel rotation in radians
        // theta_ref[1] is ref body tilt forward from vertical
        // theta_ref[2] is ref d/dt theta[0]
        // theta_ref[2] is ref d/dt theta[1]      
	// double theta_ref[4] = {0.0, 0.0, 0.0, 0.0};
	// double theta_error[4];

	// ------------------------------------------------------------
	// balance+ advance, and yaw controllers
	// ------------------------------------------------------------	

	// ------------------------------------------------------------
	// balance and advance controller
	// ------------------------------------------------------------
	
	// compute referendce average wheel rotation (advance)
	// and rotation velocity
	theta_ref[2] = (max_advance_vel / EDUMIP_WHEEL_RADIUS) * cmd_vel.linear.x;
	theta_ref[0] += (theta_ref[2] * seconds_since_last_update);

      	// compute controlr if pitch magnitude is less than 30 degrees from vertical
	balance_fb_torque = 0.0;
	if( fabs(pitch) < (DTOR*30.0))
	  {
	    int i;
	    for(i=0; i<=3;i++)
	      {
		theta_error[i] = theta[i]-theta_ref[i];
	        balance_fb_torque += fb_gains[i] * theta_error[i];
	      }
	  }
	
	// saturate feedback command
	balance_fb_torque = MAX_MAG( balance_fb_torque, MAX_FB_TORQUE);	

	// ------------------------------------------------------------
	// yaw controller
	// ------------------------------------------------------------	
	yaw_ref_vel = max_yaw_vel * cmd_vel.angular.z;
	yaw_ref += (yaw_ref_vel * seconds_since_last_update);

	// wrap yaw error to -PI ot PI
	double yaw_error = NPP( fmod((yaw - yaw_ref), 2.0*PI));
        
	yaw_fb_torque = 0.0;
	if( fabs(pitch) < (DTOR*30.0))	
	  if( fabs(yaw_error) < (DTOR*15.0))
	    {
	      //	    damped yaw control with no wheel separation term in SetForce command
	      yaw_fb_torque = (-0.01 * (yaw_error)) + (-0.01 * (yawV - yaw_ref_vel));	    

	    }

	//	saturate feedback command
	yaw_fb_torque = MAX_MAG( yaw_fb_torque, MAX_FB_TORQUE);

	// publish controller state at full controller bandwidth
	
	// 2018/03/04 19:21:18 LLW: publish edumip state at 5 Hz
	static double last_edumip_state_time = 0;
	static int kount = 0;

        if(current_time.Double() >= (last_edumip_state_time + 0.1))
	  {
	    last_edumip_state_time = current_time.Double();
	    edumip_msgs::EduMipState edumip_state;

	    
	    edumip_state.setpoint_phi_dot     = theta_ref[2];   // commanded average  wheel vel ~ trans vel
	    edumip_state.setpoint_gamma_dot   = yaw_ref_vel;    // commanded steering angle vel ~ angualr vel
	    edumip_state.setpoint_phi         = theta_ref[0];   // commanded average wheel pos
	    edumip_state.phi                  = theta[0];       // average wheel pos
            edumip_state.setpoint_gamma       = yaw_ref;        // commanded steering angle
	    edumip_state.gamma                = yaw;            // steering angle
	    edumip_state.setpoint_theta       = theta_ref[1];   // commanded body tilt
	    edumip_state.theta                = theta[1];       // body tilt
	    edumip_state.d1_u = balance_fb_torque;              // control command for balnce loop
	    edumip_state.d3_u = yaw_fb_torque;                  // control command for steering loop
            edumip_state.dutyL = 0.0;                           // left  motor duty cycle
            edumip_state.dutyR = 0.0;                           // right motor duty cycle

	    // 2017-02-22 LLW Added odometry data 
	    edumip_state.wheel_angle_L        = wheel_angle_L;  // total rotation of left  wheel (radians) (+ is forward)
	    edumip_state.wheel_angle_R        = wheel_angle_R;  // total rotation of right wheel (radians) (+ is forward)
	    edumip_state.body_frame_northing  = pos.X();        // displacemnt of body frame (m) (+X is North)
	    edumip_state.body_frame_easting   = -pos.Y();       // displacemnt of body frame (m) (-Y is East )
	    edumip_state.body_frame_heading   = -yaw;           // compass heading (radians)  X=North +=clockwise

	    edumip_state.vBatt                = PI;             // battery voltage in volts
	    edumip_state.armed                = TRUE;           // controllers are active
	    edumip_state.running              = TRUE;           // balance program is running

	    edumip_state.header.stamp = ros::Time::now();
	    edumip_state.header.frame_id = robot_base_frame_;
	    edumip_state_publisher_.publish(edumip_state);

     	    // ROS_INFO("t=%.3f tb=%10.5f, theta=[%+10.3f, %+10.3f,   %+10.3f, %+10.3f], ty=%10.5f, yaw[%+10.3f, %+10.3f]",
	    // 	     current_time.Double(),
	    // 	     balance_fb_torque, theta[0]*RTOD, theta[1]*RTOD,  theta[2]*RTOD, theta[3]*RTOD,
	    // 	     yaw_fb_torque, yaw*RTOD, yawV*RTOD
	    // 	     );

	    // 	    ROS_INFO("EdumipBalanceRos seconds_since_last_update=%f, update_period_=%f  kount=%d",seconds_since_last_update, update_period_, kount);
	    // 	    ROS_INFO("EdumipBalanceRos balance_fb_torque=%+f",balance_fb_torque);

	    // 	    ROS_INFO("EdumipBalanceRos theta=[%+f, %+f,    %+f, %+f]",theta[0], theta[1],  theta[2], theta[3]);
	    
     	    // ROS_INFO("EdumipBalanceRos LEFT=%.6f, RIGHT=%.6f, LEFT_VEL=%.6f, RIGHT_VEL=%.6f",		     
	    // 	     wheel_angle_L,
	    // 	     wheel_angle_R,
	    // 	     wheel_vel_L,
	    // 	     wheel_vel_R);


     	    // ROS_INFO("EdumipBalanceRos roll=%+.6f, pitch=%+.6f, yaw=%+.6f, rollV=%+.6f, pitchV=%+.6f, yawV=%+.6f",
	    // 	     roll*RTOD,
	    // 	     pitch*RTOD,
	    // 	     yaw*RTOD,
	    // 	     rollV*RTOD,
	    // 	     pitchV*RTOD,
	    // 	     yawV*RTOD);

	  }
	
        last_update_time_+= common::Time ( update_period_ );
      }

    // torque for yaw need to be converted to wheel torque
    // EduMIP wheel separation is 0.07 m,  wheel radius is 0.035
    //    joints_[LEFT]->SetForce(0,  0.5 * ((-yaw_fb_torque/0.035) + balance_fb_torque));
    //    joints_[RIGHT]->SetForce(0, 0.5 * ((+yaw_fb_torque/0.035)  + balance_fb_torque));

    joints_[LEFT]->SetForce(0,  0.5 * (-yaw_fb_torque + balance_fb_torque));
    joints_[RIGHT]->SetForce(0, 0.5 * (+yaw_fb_torque + balance_fb_torque));    
    
  }

  GZ_REGISTER_MODEL_PLUGIN ( EdumipBalanceRosPlugin )
}
