# include <ros/ros.h>

#include <dynamic_reconfigure/server.h>
#include <rsp_sensor/SensorConfig.h>


#include <geometry_msgs/WrenchStamped.h>

class sensor{

    private:

    ros::NodeHandle nh;
    ros::Publisher publisher;

    dynamic_reconfigure::Server<rsp_sensor::SensorConfig> server;
    dynamic_reconfigure::Server<rsp_sensor::SensorConfig>::CallbackType callback;

    ros::Rate rate;
    bool enable;
    std::string frame_id;//cause we want to publish a wrench here, 
                        //so we need a frame id to generate the wrench message

    public:

    sensor( ros::NodeHandle& nh);

    void publish();
    void sensor_config( rsp_sensor::SensorConfig& config, uint32_t level);
    // here is the callback of our reconfiguration
};