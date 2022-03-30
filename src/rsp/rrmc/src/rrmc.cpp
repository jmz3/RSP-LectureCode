#include <rrmc/rrmc.hpp>

RRMC::RRMC( ros::NodeHandle& nh): nh(nh){

    sub_vw = nh.subscribe("cmd_vel", 10, &RRMC::callback_vw, this);
    sub_js = nh.subscribe("joint_states", 10, &RRMC::callback_js, this);
    pub_qd = nh.advertise<std_msgs::Float64MultiArray >("command",10);

    std::string robot_description_string; 
    nh.param( "robot_description", robot_description_string, std::string() );

    if( kdl_parser::treeFromString( robot_description_string, tree )){// we want to build a tree from the string 
    // ang through this command we can do it and assign the value to the tree object

        if ( tree.getChain( "base", "tool0", chain))
        {
            ik_vel = new KDL::ChainIkSolverVel_pinv( chain ); // if you dont use the new pointer to 
            // create a object here, you will need to instantiate a ik_vel object at the beginning right next to nh(nh)
        }
        else{
            ROS_ERROR("cannot get a chain");
        }
    }
    else{
        ROS_ERROR("cannot get a tree");
    }

}

RRMC::~RRMC(){};

void RRMC::callback_vw( const geometry_msgs::Twist& vw){

    std::cout << vw << std::endl;
    this->vw = vw;
}

void RRMC::callback_js( const sensor_msgs::JointState& js){

    KDL::JntArray q_in( js.position.size()); // give the joint_array a right size by fetching the size from js
    for( int i = 0; i < q_in.rows(); i++){

        q_in(i) = js.position[i]; // get joint positions and put it into a Jntarray
    }

    KDL::Twist kdl_vw;
    kdl_vw.vel.x( vw.linear.x );
    kdl_vw.vel.y( vw.linear.y );
    kdl_vw.vel.z( vw.linear.z );

    kdl_vw.rot.x( vw.angular.x );
    kdl_vw.rot.y( vw.angular.y );
    kdl_vw.rot.z( vw.angular.z );// donot expect kdl to automatically resize 

    KDL::JntArray qd_out( js.position.size() );
    ik_vel->CartToJnt(  q_in, kdl_vw, qd_out);

    std_msgs::Float64MultiArray msg_qd;
    msg_qd.layout.dim.push_back( std_msgs::MultiArrayDimension() );
    msg_qd.layout.dim[0].stride = 1;
    msg_qd.layout.data_offset = 0;

    for( int i = 0; i<qd_out.rows()  ; i++){

        msg_qd.data.push_back( qd_out(i) );
        
    }
}