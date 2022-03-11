#include <wrench_apply_plugin/wrench_apply_plugin.hpp>

namespace gazebo{

    wrench_apply_plugin::wrench_apply_plugin(){}
    wrench_apply_plugin::~wrench_apply_plugin(){}

    void wrench_apply_plugin::Init(){
        std::cout << "Init" << std::endl;
    }
    
    void wrench_apply_plugin::Reset(){
        std::cout << "Reset" << std::endl;
    }
    
    void wrench_apply_plugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf){
        
        std::cout << "Load" << std::endl;

        parent = _model;
        world = _model->GetWorld();

        publisher = gazebo_ros->node()->advertise<std_msgs::String>("wrench_apply",10);

        event1 = event::Events::ConnectWorldUpdateBegin( boost::bind( &wrench_apply_plugin::callback1, this));

    }

    void wrench_apply_plugin::callback1(){
        std::cout << "callback1" << std::endl;
    }

    GZ_REGISTER_MODEL_PLUGIN( wrench_apply_plugin )
}