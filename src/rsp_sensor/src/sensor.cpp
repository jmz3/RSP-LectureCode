#include <rsp_sensor/sensor.hpp>

sensor::sensor(ros::NodeHandle &nh) : nh(nh),
                                      rate(1),
                                      enable(false)
{
    publisher = nh.advertise<geometry_msgs::WrenchStamped>("wrench", 1);
    // publish a wrench message with the rate of 1

    callback = boost::bind(&sensor::sensor_config, this, _1, _2);
    // &sensor::sensor_config is the name of the callback
    // since there are two parameters for the callback function, so here we use _1 and _2 to clarify
    // to tell the bind function put the first thing it found into the rsp_sensor::SensorConfig &config
    // and put the second thing in uint32_t level
    server.setCallback(callback);
    // you can have a server with multiple callbacks
    
}

void sensor::sensor_config(rsp_sensor::SensorConfig &config, uint32_t level)
{

    std::cout << config.sensor_frequency << std::endl
              << config.frame_id << std::endl
              << config.enable << std::endl
              << config.units << std::endl
              << level << std::endl;

    if (level == 4) { enable = config.enable; }
    if (level == 2) { frame_id = config.frame_id; }
    if (level == 1) { rate = ros::Rate(config.sensor_frequency); }
    // with different level flag, the program will update the one have changed and keep others 
    // remain the same 
    // so you don't have to change all the values everytime
}

void sensor::publish()
{
    if (enable)
    {
        geometry_msgs::WrenchStamped ft;
        ft.header.frame_id = frame_id;
        publisher.publish(ft);
        rate.sleep();
    }
}
