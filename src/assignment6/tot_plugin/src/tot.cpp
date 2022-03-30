#include <std_msgs/String.h>
#include <tot_plugin/tot.hpp>
#include <ignition/math/Pose3.hh>
#include <ignition/math/Quaternion.hh>
#include <ignition/math/Vector3.hh>
#include <tot_msgs/TOT.h>

namespace gazebo {

    tot_plugin::tot_plugin(){}

    tot_plugin::~tot_plugin(){}

    void tot_plugin::Init(){
        std::cout << "Init" << std::endl;
    }

    void tot_plugin::Reset(){
        std::cout << "Reset" << std::endl;
    }

    void tot_plugin::Load(physics::ModelPtr _model, sdf::ElementPtr _sdf){
        std::cout << "Load" << std::endl;

        parent = _model;
        world = _model->GetWorld();

        gazebo_ros = GazeboRosPtr(new GazeboRos( _model, _sdf, "tot" ));
        gazebo_ros->isInitialized();

        std::string towerA;
        std::string towerB;
        gazebo_ros->getParameter<std::string>( towerA, "towerA", "towerA" );
        gazebo_ros->getParameter<std::string>( towerB, "towerB", "towerB" );

        publisher = gazebo_ros->node()->advertise<tot_msgs::TOT>("range", 10);

        event1 = event::Events::ConnectWorldUpdateBegin( boost::bind(&tot_plugin::callback1, this) );
    }

    void tot_plugin::callback1(){
        std::cout << "Callback1" << std::endl;
        tot_msgs::TOT my_tot;
        double A_X;
        double A_Y;
        double B_X;
        double B_Y;
        double edumip_X;
        double edumip_Y;

        physics::Model_V vm = world->Models();

        for (int i=0; i<vm.size(); i++) {
            ignition::math::Vector3d pos = vm[i]->WorldPose().Pos();
            if (vm[i]->GetName() == "towerA") {
                my_tot.name_of_towers.push_back(vm[i]->GetName());
                my_tot.co_of_towers.push_back(pos.X());
                my_tot.co_of_towers.push_back(pos.Y());
                A_X = pos.X();
                A_Y = pos.Y();
            } else if (vm[i]->GetName() == "towerB") {
                my_tot.name_of_towers.push_back(vm[i]->GetName());
                my_tot.co_of_towers.push_back(pos.X());
                my_tot.co_of_towers.push_back(pos.Y());
                B_X = pos.X();
                B_Y = pos.Y();
            } else if (vm[i]->GetName() == "edumip") {
                edumip_X = pos.X();
                edumip_Y = pos.Y();
            }
        }
        my_tot.ranges.push_back(sqrt((edumip_X-A_X)*(edumip_X-A_X)) + (edumip_Y-A_Y)*(edumip_Y-A_Y));
        my_tot.ranges.push_back(sqrt((edumip_X-B_X)*(edumip_X-B_X)) + (edumip_Y-B_Y)*(edumip_Y-B_Y));
        publisher.publish(my_tot);
    }

    //GZ
    GZ_REGISTER_MODEL_PLUGIN( tot_plugin )

}