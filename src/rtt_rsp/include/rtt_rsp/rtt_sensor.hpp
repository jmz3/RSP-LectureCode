#include <ros/ros.h>
#include <geometry_msgs/WrenchStamped.h>
#include <rtt/TaskContext.hpp>
#include <rtt/OutputPort.hpp>
#include <kdl_parser/kdl_parser.hpp>

class rtt_sensor : public RTT::TaskContext {

private:
    
    RTT::OutputPort<geometry_msgs::WrenchStamped> port_msr_wrench; // msr is short for measurement

public:

    rtt_sensor( const std::string& name);
    ~rtt_sensor();

    virtual bool configureHook();
    virtual bool startHook();

    virtual void updateHook();

    virtual void stopHook();
    virtual void cleanupHook(); // these method are provided by the rtt class

    // below are our own method
    void zero();
    KDL::Wrench getWrench();
    void setOffset( const KDL::Wrench& ft );

};