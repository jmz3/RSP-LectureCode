/*
 * name : display_block_node
 * author : Marie SCHNEIDER (ECN)
 * date : 2016-03-16
 *
 * description : a ROS node for ecn_sonar_block
 *               displays a raw sonar-like image (with shadows)
 * 
 * remarks : autoscale based on 1st point detected
 *           if this point is uncommon, the image jumps temporarily
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

    ros::init(argc, argv, "display_sonar_image");
    ros::NodeHandle nh;
    ros::Subscriber sub = nh.subscribe("/ecn_auv/sonar", 1, & Listener::Callback, & listener);
    ros::Rate loop_rate(100);

    namedWindow("Sonar Image");
    startWindowThread();

    // Create black empty images
    int sizey = 550;
    int sizex = 500;
    Mat display_image = Mat::zeros(sizey, sizex, CV_8UC3);

    bool first = true ;
    float y_i;
    float z_i;

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

                int err = 1+0.8*dist ;
                // With blobs
                /// circle(display_image, Point(sizex/2-y*sizex/(7*y_i)+(rand() % err - err/2), 0.7*sizey+z*sizey/(4.5*z_i)+(rand() % err - err/2)), dist*0.5, blues ,-1,8,0);
                // With pixels
                rectangle(display_image,
                          Point(sizex/2 // centered image
                                -y*sizex/(7*y_i) // autoscale
                                +(rand() % err - err/2) // random geometric error
                                +dist*0.5, // background pixel size
                                0.7*sizey+z*sizey/(4.5*z_i)+(rand() % err - err/2)+dist*0.5),
                          Point(sizex/2-y*sizex/(7*y_i)+(rand() % err - err/2)-dist*0.5, 0.7*sizey+z*sizey/(4.5*z_i)+(rand() % err - err/2)-dist*0.5),
                          blues,-1,8,0);
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
                circle(display_image, Point(sizex/2-y*sizex/(7*y_i)+(rand() % err - err/2), 0.7*sizey+z*sizey/(4.5*z_i)+(rand() % err - err/2)), 2, blues ,-1,8,0);

            }
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
