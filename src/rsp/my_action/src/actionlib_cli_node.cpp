#include <ros/ros.h>
#include <actionlib/client/simple_action_client.h>
#include <my_action/StickAndMoveAction.h>

void done(  const actionlib::SimpleClientGoalState& state,
            const my_action::StickAndMoveResultConstPtr& result ){
                std::cout << result << std::endl;
            //  first parameter of this function is always the same since ros makes it so
            //  second parameter is our action stickandmove but it has to be a ConstPtr object
            }

void active(){
    std::cout << "Active" << std::endl;
}

void feedback(  const my_action::StickAndMoveFeedbackConstPtr& feedback){

    std::cout << *feedback << std::endl;
}

int main(int argc, char** argv)
{

    ros::init(argc, argv, "actionlib_cli_node");
    ros::NodeHandle nh;

    actionlib::SimpleActionClient<my_action::StickAndMoveAction> ac("stick_and_move");
    ac.waitForServer();

    my_action::StickAndMoveGoal goal;
    std::cout << goal << std::endl;

    goal.goal.coordinates.x = 1;
    goal.goal.coordinates.y = 1;
    goal.goal.coordinates.z = 1;

    ac.sendGoal( goal, &done, &active, &feedback); // done active feedback are all callback functions

    // the following 3 lines are to wait for the result for 10 secs, 
    //and will print a meaage to tell you whether the result is received or not
    // bool ros = ac.waitForResult(  ros::Duration(10));
    // if( res) {std::cout << "Success" << std::endl;}
    // else { std::cout << "Failure" << std::endl;}

    ros::spin();

    return 0;
}