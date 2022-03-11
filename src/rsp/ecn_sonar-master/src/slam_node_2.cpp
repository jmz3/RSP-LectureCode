/*
 * name : slam_node_2
 * author : Marie SCHNEIDER (ECN)
 * date : 2016-03-16
 *
 * description : a ROS node for ecn_sonar
 *               displays a map with the SLAM (Simultaneous Localization And Mapping) technique
 *
 * remarks : NOT WORKING (trial)
*/


#include <sstream>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "ros/ros.h"
#include <sensor_msgs/LaserScan.h>
#include <nav_msgs/Odometry.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace cv;
using namespace std;

class Listener
    {
    public : sensor_msgs::LaserScan last_msg_LaserScan;
    public : nav_msgs::Odometry last_msg_Pose;
    bool test_LaserScan;
    bool test_Pose;
    void Callback_LaserScan(const sensor_msgs::LaserScan& msg) {last_msg_LaserScan = msg; test_LaserScan = true;}
    void Callback_Pose(const nav_msgs::Odometry& msg) {last_msg_Pose = msg; test_Pose = true;}
    };


int main(int argc, char **argv){

    Listener listener_LaserScan;
    Listener listener_Pose;

    ros::init(argc, argv, "display_node");
    ros::NodeHandle nh;
    ros::Subscriber sub_LaserScan = nh.subscribe("/auv/sonar", 1, & Listener::Callback_LaserScan, & listener_LaserScan);
    ros::Subscriber sub_Pose = nh.subscribe("/auv/state", 1, & Listener::Callback_Pose, & listener_Pose);
    ros::Rate loop_rate(100);

    namedWindow("Sea Bed");
    startWindowThread();

    // Create black empty images
    float scale = 10;
    int sizey = 90*scale;
    int sizex = 90*scale;
    Mat K = Mat::zeros(sizey, sizex, CV_8UC3);
    Mat display_image = K;
    int loop = 0;
    float x, x_prec, xP, xP_prec, xx;
    float y, y_prec, yP, yP_prec, yy;
    int middle_x = sizex/2;
    int middle_y = sizey/2;
    float u;

    while(ros::ok())
        {
        if (listener_LaserScan.test_LaserScan && listener_Pose.test_Pose) {

            int size = listener_LaserScan.last_msg_LaserScan.ranges.size();

            int dist ;

            float angle_min = listener_LaserScan.last_msg_LaserScan.angle_min;
            float angle_max = listener_LaserScan.last_msg_LaserScan.angle_max;
            float angle_increment = listener_LaserScan.last_msg_LaserScan.angle_increment;

            xP = listener_Pose.last_msg_Pose.pose.pose.position.x;
            yP = listener_Pose.last_msg_Pose.pose.pose.position.y;

            // To calculate the angle
            float q0 = listener_Pose.last_msg_Pose.pose.pose.orientation.w;
            float q1 = listener_Pose.last_msg_Pose.pose.pose.orientation.x;
            float q2 = listener_Pose.last_msg_Pose.pose.pose.orientation.y;
            float q3 = listener_Pose.last_msg_Pose.pose.pose.orientation.z;
            float yaw = atan2(2*(q0*q1+q2*q3),1-2*(q1*q1+q2*q2));
            float pitch = asin(2*(q0*q2-q3*q1));
            float roll = atan2(2*(q0*q3+q1*q2),1-2*(q2*q2+q3*q3));


            for (int i = 0 ; i<size ; i++) { // i is the
                float alpha = angle_min+i*angle_increment;
                float range = listener_LaserScan.last_msg_LaserScan.ranges[i];
                float intensities = listener_LaserScan.last_msg_LaserScan.intensities[i];
                u = scale*range*sin(alpha+roll);

                // Greyscale profile
                float final_intensities=intensities/(range*range);
                float light = 6;
                Scalar greyscale = Scalar::all(light*final_intensities);
                // Drawing out the line, more and more with the range
                xx = u*sin(yaw) // location of the detected point
                        +xP*scale // First location detected = center of the image
                            -range*scale*sin(pitch)*cos(yaw)+middle_x; // Gap due to pitch angle
                yy = -u*cos(yaw)+yP*scale-range*scale*sin(pitch)*sin(yaw)-middle_y;
                line(display_image, Point(xx, (-yy)), Point(xx, (-yy)), greyscale,1,8,0);
                //cout<<"xx:"<<xx<<endl<<"yy:"<<yy<<endl;
            }
            //middle_x = sizex/2 + yP*scale;
            //middle_y = sizey/2 - xP*scale;

            // Display
            imshow("Sea Bed", display_image );

            float resolution_y = 100;
            waitKey(resolution_y);

            xP_prec = xP;
            yP_prec = yP;
            y_prec = y ;
            loop = loop+1;
        }

        ros::spinOnce();
        loop_rate.sleep();
        }

    cout << "clear" << endl;
    return 0;
}