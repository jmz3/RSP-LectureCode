/*
 * name : display_node
 * author : Marie SCHNEIDER (ECN)
 * date : 2016-03-16
 *
 * description : an intermediate ROS node for ecn_sonar
 *               displays the surface of the sea bed and the depth at the middle point
 *
 * remarks : not compulsory, but useful to check if something is working
*/


#include <sstream>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "ros/ros.h"
#include <sensor_msgs/LaserScan.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace cv;
using namespace std;

class Listener
    {
    public : sensor_msgs::LaserScan last_msg;
        bool test;
        void Callback(const sensor_msgs::LaserScan& msg) {last_msg = msg; test = true;}
        };

// Function headers
void MyLine( Mat img, Point start, Point end, Scalar color);

// Function declaration
void MyLine( Mat img, Point start, Point end, Scalar color)
{
  int thickness = 1;
  int lineType = 8;
  line( img,
    start,
    end,
    color,
    thickness,
    lineType );
}

string float2string(float f)
{
  ostringstream os;
  os <<  f;
  return os.str();
}

int main(int argc, char **argv){

    Listener listener;

    ros::init(argc, argv, "display_node");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("/auv/sonar", 1, & Listener::Callback, & listener);
    ros::Rate loop_rate(100);

    namedWindow("Sea Bed");
    namedWindow("Depth");
    startWindowThread();

    // Create white empty images
    float scale = 21;
    int sizex = 15*scale;
    int sizey = 40*scale;
    Mat display_image = Mat::zeros( sizex, sizey, CV_8UC3 );
    Mat depth_image = Mat::zeros(100, 300, CV_8UC3 );
    float depth;

    while(ros::ok())
        {
        if (listener.test) {

            int size = listener.last_msg.ranges.size();
            int middle_up = sizey/2;
            float x_prec = 0;
            float y_prec = 0;

            float angle_min = listener.last_msg.angle_min;
            float angle_max = listener.last_msg.angle_max;
            float angle_increment = listener.last_msg.angle_increment;

            for (int i = 0 ; i<size ; i++) {
                float alpha = angle_min+i*angle_increment;
                float range = listener.last_msg.ranges[i];
                float x = -scale*range*sin(alpha)+middle_up;
                float y = scale*range*cos(alpha);

                if (i==size/2){
                    Scalar yellow(0,255,255);
                    MyLine(display_image, Point(x, 0), Point(x, y), yellow);
                    depth = range ;
                }
                if (i>0){
                    Scalar white(255,255,255);
                    // To draw points
                    MyLine( display_image, Point(x, y), Point(x, y), white);
                    // To draw lines (uncomment)
                    // MyLine( display_image, Point(x_prec, y_prec), Point(x, y), white);
                }
                x_prec = x;
                y_prec = y;
            }
            depth = roundf(depth * 100) / 100;
            string text = float2string(depth);
            int fontFace = 0;
            double fontScale = 2;
            int thickness = 2;

            Size textSize = getTextSize(text, fontFace,
                                        fontScale, thickness, 0);

            // center the text
            Point textOrg((depth_image.cols - textSize.width)/2,
                          (depth_image.rows + textSize.height)/2);

            // then put the text itself
            putText(depth_image, text, textOrg, fontFace, fontScale,
                    Scalar::all(255), thickness, 8);

            // Display
            imshow("Sea Bed", display_image );
            imshow("Depth", depth_image );

            waitKey(100);
            display_image = Mat::zeros( sizex, sizey, CV_8UC3);
            depth_image = Mat::zeros(100, 300, CV_8UC3);
        }

        ros::spinOnce();
        loop_rate.sleep();
        }

    cout << "clear" << endl;
    return 0;
}