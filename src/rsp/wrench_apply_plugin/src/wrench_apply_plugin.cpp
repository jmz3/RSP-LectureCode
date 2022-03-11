#include <wrench_apply_plugin/wrench_apply_plugin.hpp>
#include <std_msgs/String.h>

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

        gazebo_ros = GazeboRosPtr( new GazeboRos( _model, _sdf, "wrench_apply" ));
        gazebo_ros->isInitialized();

        std::cout << _model->GetName() << std::endl;
        std::cout << _sdf->GetName() << std::endl;
        std::cout << _sdf->HasElement("topic") <<std::endl;

        std::string topicname;
        gazebo_ros->getParameter<std::string>(topicname, "topic", "defaultname");

        publisher = gazebo_ros->node()->advertise<std_msgs::String>(topicname,10);

        event1 = event::Events::ConnectWorldUpdateBegin( boost::bind( &wrench_apply_plugin::callback1, this));

    }

    void wrench_apply_plugin::callback1(){
    
        physics::Model_V vm = world->Models();
    
        for( int i=0; i<vm.size(); i++){
            std_msgs::String name;
            name.data = vm[i]->GetName();
            publisher.publish(name);

        }
    }

    GZ_REGISTER_MODEL_PLUGIN( wrench_apply_plugin )
}