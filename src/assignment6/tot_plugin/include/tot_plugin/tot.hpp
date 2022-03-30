#include <gazebo/common/common.hh>  //gazebo stuff for ModelPtr and ConnectionPtr
#include <gazebo_plugins/gazebo_ros_utils.h>  //ros creation gives GazeboRosPtr

namespace gazebo{

    class tot_plugin : public ModelPlugin{
    private:

        GazeboRosPtr gazebo_ros;
        physics::ModelPtr parent;
        event::ConnectionPtr event1;

        physics::WorldPtr world;
        ros::Publisher publisher;

    public:

        tot_plugin();
        ~tot_plugin();

        void Init();
        void Reset();
        void Load(physics::ModelPtr, sdf::ElementPtr);

        void callback1();
    };

}