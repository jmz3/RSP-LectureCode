/*
 * name : mapping_node_scaled
 * author : Marie SCHNEIDER (ECN)
 * date : 2016-03-16
 *
 * description : a ROS node for ecn_sonar
 *               displays a map image using the LaserScan data and the position of the robot
 *
 * remarks : - no autoscale : if too close or too far from the bottom, display could be less accurate
 *           - it is possible to change the display color by changing the variable "mode"
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
    float scale = 20;
    int sizey = 45*scale;
    int sizex = 40*scale;
    Mat K = Mat::zeros(sizey, sizex, CV_8UC3);
    Mat display_image = K;
    int loop = 0;
    float x;
    float y;
    float y_prec = 0;

    while(ros::ok())
        {
        if (listener_LaserScan.test_LaserScan && listener_Pose.test_Pose) {

            int size = listener_LaserScan.last_msg_LaserScan.ranges.size();
            int middle_up = sizex/2;
            int dist ;

            float angle_min = listener_LaserScan.last_msg_LaserScan.angle_min;
            float angle_increment = listener_LaserScan.last_msg_LaserScan.angle_increment;

            float xP = listener_Pose.last_msg_Pose.pose.pose.position.x;

            for (int i = 0 ; i<size ; i++) {
                float alpha = angle_min+i*angle_increment;
                float range = listener_LaserScan.last_msg_LaserScan.ranges[i];
                float intensities = listener_LaserScan.last_msg_LaserScan.intensities[i];
                x = -scale*range*sin(alpha)+(rand() % 2 - 1)+middle_up;
                y = xP;

                // Display mode :
                /// 0 = grey image without noise
                /// 1 = grey image with noise
                /// 2 = blue image with noise
                /// 3 = cyan image with noise
                /// 4 = pink image with noise
                int mode = 2 ;

                // float tone = light*intensities/(range*range)+rand()%20;
                float tone = 0.25*intensities/range+rand()%20;
                Scalar blues = Scalar(2.5*tone,tone,0.4*tone);
                Scalar greyscale = Scalar::all(tone);
                Scalar noises = Scalar::all(tone);
                Scalar cyans = Scalar(3*tone,2*tone,0.5*tone);
                Scalar pinks = Scalar(2*tone,tone,2*tone);

                switch(mode){
                case 0 :
                    circle(display_image, Point(x, 10), scale*0.14,greyscale,-1,8,0);
                    break;
                case 1 :
                    circle(display_image, Point(x, 10), scale*0.14 , noises,-1,8,0);
                    break;
                case 2 :
                    circle(display_image, Point(x, 10), scale*0.14 , blues,-1,8,0);
                    break;
                case 3 :
                    circle(display_image, Point(x, 10), scale*0.14 , cyans,-1,8,0);
                    break;
                case 4 :
                    circle(display_image, Point(x, 10), scale*0.14 , pinks,-1,8,0);
                    break;
                }
            }

            if (loop>1){
                dist = abs(scale*(y-y_prec))+1 ;
                // Scrolling image dist-further
                for (int j = 1 ; j<sizey-dist ; j++){
                    display_image.row(sizey-dist-1-j).copyTo(display_image.row(sizey-j));
                }

            }

            // Display
            imshow("Sea Bed", display_image );

            float resolution_y = 100;
            waitKey(resolution_y);

            y_prec = y ;
            loop = loop+1;
        }

        ros::spinOnce();
        loop_rate.sleep();
        }

    cout << "clear" << endl;
    return 0;
}