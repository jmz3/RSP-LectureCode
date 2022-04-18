#include <rtt_rsp/rtt_sensor.hpp>
#include <rtt/Component.hpp>

rtt_sensor::rtt_sensor( const std::string& name):
    RTT::TaskContext( name ),
    port_msr_wrench("Measured wrench"){ // you can initialize this member variable when the class is called
                                        // the port_msr_wrench is named as "Measured wrench"

    addPort("MsrWrench", port_msr_wrench);
    addOperation("Zero", &rtt_sensor::zero, this, RTT::OwnThread); //OwnThread means that we can have this zero thing running in the
    // same thread inside our control loop
    addOperation("GetWrench", &rtt_sensor::getWrench, this, RTT::OwnThread);
    addOperation("SetOffset", &rtt_sensor::setOffset, this, RTT::OwnThread);


    std::cout<<"rtt_sensor::rtt_sensor"<< std::endl;
    std::cout<<"rtt sensor created successfully"<<std::endl;

}

rtt_sensor::~rtt_sensor(){}

bool rtt_sensor::configureHook(){
    std::cout<<"configure hook created successfully"<<std::endl;
}
bool rtt_sensor::startHook(){
    std::cout<<"start hook created successfully"<<std::endl;
}
void rtt_sensor::updateHook(){
    // std::cout<<"update hook created successfully"<<std::endl;
    // it's not a good idea to have things printed to the screen in update hook

    /////////////////////////////////////////////////////////////////////////////
    //pretend to read data from sensor
    geometry_msgs::WrenchStamped ft;
    ft.header.stamp = ros::Time::now();
    ft.wrench.force.x = 10;
    port_msr_wrench.write( ft );
}
void rtt_sensor::stopHook(){
    std::cout<<"stop hook created successfully"<<std::endl;
}
void rtt_sensor::cleanupHook(){
    std::cout<<"clean hook created successfully"<<std::endl;
}

void rtt_sensor::zero(){
    std::cout<< "zero" <<std::endl; // this is not a part of rtt, so it wouldn't be called natually like other hooks
    // so we need some extra operation by add operation code in the constructor above
}

void rtt_sensor::setOffset( const KDL::Wrench& ft){
    std::cout<< ft.force.x() <<std::endl;
}

KDL::Wrench rtt_sensor::getWrench(){
    KDL::Wrench ft;
    ft.force.x( 5 );
    return ft;
}

ORO_CREATE_COMPONENT(rtt_sensor) // we won't have a node ( main function in other words) so we need to make it like a plugin