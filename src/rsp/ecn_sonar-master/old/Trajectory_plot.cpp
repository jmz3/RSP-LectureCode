#include <sstream>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "ros/ros.h"
#include <sensor_msgs/LaserScan.h>
#include <freefloating_gazebo/BodySetpoint.h>
#include <nav_msgs/Odometry.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace cv;
using namespace std;

class Listener
    {
    public : nav_msgs::Odometry last_msg_Pose;
    bool test_Pose;
    void Callback_Pose(const nav_msgs::Odometry& msg) {last_msg_Pose = msg; test_Pose = true;}
    };


int main(int argc, char **argv){

    Listener listener_Pose;

    ros::init(argc, argv, "display_node");
    ros::NodeHandle nh;
    ros::Subscriber sub_Pose = nh.subscribe("/auv/state", 1, & Listener::Callback_Pose, & listener_Pose);
    ros::Rate loop_rate(100);

    namedWindow("Trajectory");
    startWindowThread();

    // Create black empty images
    float scale = 10;
    int sizey = 90*scale;
    int sizex = 90*scale;
    Mat Traj = Mat::zeros(sizey, sizex, CV_8UC3);
    Mat display_image = Traj;
    int loop = 0;
    float x, x_prec, xP, xP_prec, xx;
    float y, y_prec, yP, yP_prec, yy;
    int middle_x = sizex/2;
    int middle_y = sizey/2;
    float u;

    while(ros::ok())
        {
        if (listener_Pose.test_Pose) {



            xP = 10*listener_Pose.last_msg_Pose.pose.pose.position.x+600;
            yP = 10*listener_Pose.last_msg_Pose.pose.pose.position.y+600;


            Scalar white(255,255,255);
            //cout<<xP<<endl<<yP<<endl;
                line(display_image, Point(xP, yP), Point(xP, yP), white,1,8,0);

            }

            imshow("Trajectory", display_image );




        ros::spinOnce();
        loop_rate.sleep();
        }

    cout << "clear" << endl;
    return 0;
}