#include <gazebo_plugins/gazebo_ros_utils.h>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>


namespace gazebo{

    class wrench_apply_plugin : public ModelPlugin{

        private:

            GazeboRosPtr gazebo_ros;
            physics::ModelPtr parent;
            event::ConnectionPtr event1;
            physics::WorldPtr world;
            ros::Publisher publisher;

        public:

            wrench_apply_plugin();
            ~wrench_apply_plugin();

            void Init();
            void Reset();
            void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);
            // Init Reset and Load are pure virtual member function that we always need to 
            // specify in a ModelPlugin class

            void callback1();
    };
}