/*
 * name : display_block_node_2
 * author : Marie SCHNEIDER (ECN)
 * date : 2016-03-16
 *
 * description : a ROS node for ecn_sonar_block
 *               displays a sonar-like projected image
 *
 * remarks : only works with a block-sonar oriented at 45° and with angle limits from -0.5 to 0.5
*/


#include <sstream>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include "ros/ros.h"
#include <sensor_msgs/PointCloud.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace cv;
using namespace std;

class Listener
    {
    public : sensor_msgs::PointCloud last_msg;
        bool test;
        void Callback(const sensor_msgs::PointCloud& msg) {last_msg = msg; test = true;}
        };

int main(int argc, char **argv){

    Listener listener;

    ros::init(argc, argv, "display_sonar_image_2");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("/ecn_auv/sonar", 1, & Listener::Callback, & listener);
    ros::Rate loop_rate(100);

    namedWindow("Sonar Image");
    startWindowThread();

    // Create black empty images
    int sizey = 380;
    int sizex = 600;
    Mat display_image = Mat::zeros(sizey, sizex, CV_8UC3);

    bool first = true ;
    float y_i;
    float z_i;

    float coef = 250 ;

    while(ros::ok())
        {
        if (listener.test) {

            int nbpoints = listener.last_msg.points.size();
            // Creating a randomly blue background
            for (int i=0 ; i<nbpoints ; i++){
                float x = listener.last_msg.points[i].x;
                float y = listener.last_msg.points[i].y;
                float z = listener.last_msg.points[i].z;
                float intensity = listener.last_msg.channels[0].values[i];
                float dist = sqrt(x*x+y*y+z*z);

                float tone = 0.25*intensity/dist+rand()%5;
                Scalar blues = Scalar(2.5*tone,tone,0.4*tone);

                y_i = listener.last_msg.points[nbpoints-1].y;
                z_i = listener.last_msg.points[nbpoints-1].z;

                // With blobs
                /// circle(display_image, Point(sizex/2+coef*y/x,sizey/2-coef*z/x), 5, blues ,-1,8,0);
                // With pixels
                rectangle (display_image, Point(sizex/2-coef*y/(x-z)-0.5*dist, 0.8*sizey-coef*z/(x-z)-0.5*dist),
                           Point(sizex/2 // centered image
                                 -coef*y/(x-z) // projection on the real horizontal plane and enlargement
                                 +0.5*dist // background pixel size
                                 ,0.8*sizey-coef*z/(x-z)+0.5*dist), blues,-1,8,0);
            }

            // Displaying the measures with an error
            for (int j=0 ; j<nbpoints ; j++){
                float x = listener.last_msg.points[j].x;
                float y = listener.last_msg.points[j].y;
                float z = listener.last_msg.points[j].z;
                float intensity = listener.last_msg.channels[0].values[j];
                float dist = sqrt(x*x+y*y+z*z);

                float tone = 0.25*intensity/dist+rand()%20;
                Scalar blues = Scalar(2.5*tone,tone,0.4*tone);

                if (first == true){
                    y_i = listener.last_msg.points[nbpoints-1].y;
                    z_i = listener.last_msg.points[nbpoints-1].z;
                    first = false;
                }

                int err = 1+0.8*dist ;
                int rescy = 1 ;
                int rescz = 1 ;
                circle(display_image, Point(sizex/2-coef*y/(x-z)+rand()%4-2, 0.8*sizey-coef*z/(x-z)+rand()%4-2), 2, blues ,-1,8,0);

            }

            // Mask

            Point rook_points[2][3];
            rook_points[0][0]  = Point(0,sizey);
            rook_points[0][1]  = Point(0.335*sizex,sizey);
            rook_points[0][2]  = Point(0,0);
            rook_points[1][0]  = Point(0.655*sizex,sizey);
            rook_points[1][1]  = Point(sizex-1,sizey);
            rook_points[1][2]  = Point(sizex-1,0);
            const Point* mask1[1] = { rook_points[0] };
            const Point* mask2[1] = { rook_points[1] };
            int npt[] = { 3 };

            fillPoly(display_image,mask1,npt,1,Scalar::all(0),8);
            fillPoly(display_image,mask2,npt,1,Scalar::all(0),8);

            // Final display
            imshow("Sonar Image", display_image );
            waitKey(100);
            display_image = Mat::zeros(sizey, sizex, CV_8UC3);
        }

        ros::spinOnce();
        loop_rate.sleep();
        }

    cout << "clear" << endl;
    return 0;
}
