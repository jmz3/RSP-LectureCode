// #include <ros/ros.h>
// #include <tf/transform_broadcaster.h>

// int main(int argc, cahr** argv){

//     ros::init(argc, argv, "broadcaster");
//     ros::NodeHandle nh;
//     ros::TransformBroadcaster bc;

//     double hz=10;
//     ros::Rate rate(hz);
    
//     double z = 0;

//     while(nh.ok()){

//         tf::Vector3 t(0.0, 0.0, sin(z));
//         tf::Quaternion q(0.0, 0.0, 0.0, 1.0);
//         tf::Transform Rt(q,t);
//         z += 0.01;

//         std::string ref("gripper_pick");
//         std::string tgt("moving_frame");

//         tf::StampedTransform Rt_stamped( Rt, ros::Time::now(), ref, tgt);
//         bc.sendTransform( Rt_stamped );

//         rate.sleep();
        
//     }

//     return 0;

// }