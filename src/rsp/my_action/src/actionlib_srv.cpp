#include <my_action/actionlib_srv.hpp>

rsp::actionlib_srv::actionlib_srv( ros::NodeHandle& nh) :

    nh(nh){

        as.reset( new actionlib::SimpleActionServer<my_action::StickAndMoveAction>(nh, "stick_and_move", false));
        
        as->registerGoalCallback( boost::bind( &rsp::actionlib_srv::goalCB, this) );
        as->start();

        //a simple action client
        // ac1 = ???
        // ac2 = ???
        // ac3 = ???
        // these client can be navigation client, manipulation client ...
    }

void rsp::actionlib_srv::goalCB(){

    my_action::StickAndMoveGoal goal;
    goal = *(as->acceptNewGoal());  

    std::cout << goal << std::endl;

    my_action::StickAndMoveFeedback feedback;
    feedback.feedback.message.data = std::string("goal is accepted");
    as->publishFeedback(feedback);

    ros::Rate r(1);

    for(int i=0; i<5; i++){

        feedback.feedback.message.data = std::string("robot moving to ") + std::to_string(i);
        r.sleep();
        as->publishFeedback( feedback );
    }

    // you can create several clients for different purpose
    // nav_client.sendGoal(???);
    // waitforResult();
    // feedback.feedback.message.data = std::string("at the door");
    // as->publishFeedback( feedback );

    // opendoor_client.sendGoal(???);
    // waitforResult();
    // feedback.feedback.message.data = std::string("door opened");
    // as->publishFeedback( feedback );



    as->setSucceeded();



}

rsp::actionlib_srv::~actionlib_srv(){}