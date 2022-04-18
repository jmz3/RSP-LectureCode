#include <rrmc/rrmc.hpp>
#include <std_msgs/Float64MultiArray.h>


RRMC::RRMC( ros::NodeHandle& nh): nh(nh){

    sub_vw = nh.subscribe("cmd_vel", 10, &RRMC::callback_vw, this); 
    /*
    when dealing with twist it's common to name twist as cmd_vel, 10 is the buffer
    the point of subscribe is that it binds the callback function and this class together and 
    it allows you to listen constantly to the topic you specified without getting stuck at this line of code
    in other words, the other part of this class can run independently even though this nh.subscribe is running
    Through this callback, we put what we get from cmd_vel to the parameter we defined for the callback_vw
    Here we define the parameter as a "const geometry_msgs::Twist& vw", that means we put whatever we got into that parameter
    So you gotta make sure the parameter you defined matches the type that you want to subtract 
    */
    sub_js = nh.subscribe("joint_states", 10, &RRMC::callback_js, this);
    pub_qd = nh.advertise<std_msgs::Float64MultiArray >("command",10);

    std::string robot_description_string; 
    nh.param( "robot_description", robot_description_string, std::string() );

    if( kdl_parser::treeFromString( robot_description_string, tree )){// we want to build a tree from the string 
    // ang through this command we can do it and assign the value to the tree object

        if ( tree.getChain( "base", "tool0", chain)) // base and tool0 are hardcoded here and it must matches the topic name provided
                                                     // in the robot_description
        {
            
            ik_vel = new KDL::ChainIkSolverVel_pinv( chain ); // if you dont use the new pointer to 
            // create a object here, you will need to instantiate a ik_vel object at the beginning right next to nh(nh)
            // and again ik_vel will get the joint velocities through inverse jacobian 
            /* you won't get a successful result if you do like this 
            KDL::ChainIkSolverVel_pinv ik_vel(chain); although it means the same as above, but it doesnn't new a memory
            so it will not be there if you are out of this if condition loop, but if you new it, it will exist until you delete it

            */

        }
        else{
            ROS_ERROR("cannot get a chain");
        }
    }
    else{
        ROS_ERROR("cannot get a tree");
    }

}

RRMC::~RRMC(){ if( ik_vel) delete ik_vel; }

void RRMC::callback_vw( const geometry_msgs::Twist& vw){

    std::cout << vw << std::endl;
    this->vw = vw;
}

void RRMC::callback_js( const sensor_msgs::JointState& js){//

    KDL::JntArray q_in( js.position.size()); // give the joint_array a right size by fetching the size from js
    for( int i = 0; i < q_in.rows(); i++){

        q_in(i) = js.position[i]; // get joint positions and put it into a Jntarray, here we just ignore the wrong order of 
    } // KDL::JntArray is a class that can contain all the joint positions into one vector


    double tmp = q_in(0);
    q_in(0) = q_in(2);
    q_in(2) = tmp;// these lines are for adjusting the messy UR joint order



    KDL::Twist kdl_vw;
    kdl_vw.vel.x( vw.linear.x );
    kdl_vw.vel.y( vw.linear.y );
    kdl_vw.vel.z( vw.linear.z );

    kdl_vw.rot.x( vw.angular.x );
    kdl_vw.rot.y( vw.angular.y );
    kdl_vw.rot.z( vw.angular.z );// donot expect kdl to automatically resize 

    KDL::JntArray qd_out( js.position.size() ); // qd_out here means the derivative of joint states, in other words, it's joint velovity
    // always remember to give the JntArray object a size otherwise the program will crash cause KDL would automatically do that for you
    
    ik_vel->CartToJnt(  q_in, kdl_vw, qd_out);// here we call the member fuction and have the joint velocities stored in the qd_out
    //the next step should be publish these "number" to ros

    std_msgs::Float64MultiArray msg_qd; // multiarray is the type that topic /joint_group_position accepts
    msg_qd.layout.dim.push_back( std_msgs::MultiArrayDimension() );
    msg_qd.layout.dim[0].stride = 1; // specify how many dimensions you need for this multidimensional array
    msg_qd.layout.data_offset = 0;

    for( int i = 0; i<qd_out.rows()  ; i++){

        msg_qd.data.push_back( qd_out(i) );
        
    }

    pub_qd.publish( msg_qd );
}