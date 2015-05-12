//#include "hello_world.h"
#include "ros_interface.hpp"
#include <signal.h>
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <sstream>

#define PI 3.14159265359

using namespace std;

void teste(int signum) {
    cout << "saindo bla bla bla\n";
    exit(1);
}

//float t = 0;

//void sigalrm_handler(int sig) {
    //// This gets called when the timer runs out.  Try not to do too much here;
    //// the recommended practice is to set a flag (of type sig_atomic_t), and have
    //// code elsewhere check that flag (e.g. in the main loop of your program)
    //t += 0.1;
    //cout << "time" << t << endl;
//}

int main(int argc, char** argv) {
    ros_interface *ros_com =  new ros_interface(argc,argv,"ros_interface");

    //signal(SIGALRM, &sigalrm_handler);  // set a signal handler
    // set an alarm for 10 seconds from now
    //alarm(1);
    //
    //auto sys_time = std::chrono::system_clock::now();

    //while(1) {
        //cout << "working...\n";
        //std::this_thread::sleep_until(sys_time+=std::chrono::milliseconds(100));
        //std::time_t t = std::chrono::system_clock::to_time_t(sys_time);
        //cout << "time:" << t << endl;
    //}

    // Seed the random number generator
    //srand(time(0));
    //signal(SIGINT, teste);

    ////while(true);

    //position init_pose, init_pose2;
    //init_pose.x = 0;
    //init_pose.y = 0;
    //init_pose.yaw = 0;

    //init_pose2.x = 4;
    //init_pose2.y = 2;
    //init_pose2.yaw = 0;

    stringstream topic;
    topic.str("");
    topic << "rostopic info " << "/robot_0/odom";
    //topic << "rosnode info /stage_ros";

    const char *temp = topic.str().c_str();
    cout << temp << endl;

    int c  = system(temp);

    cout << c << endl;

    //ros_com->add_node(1,T_STAGE,"robot_0",init_pose);
    //ros_com.add_node(2,T_TURTLE,"tortoise",init_pose2);
    //ros_com.add_node(3,T_TURTLE,"K2",position());

    //while(ros_com->ros_ok()) {
    //for(int i = 0; i < 10; i++) {
        //ros_com->clock(0.1);

        //velocity vel, vel2;
        //vel.x = double(rand())/double(RAND_MAX);
        //vel.yaw = 2 * double(rand())/double(RAND_MAX) - 1;

        ////vel2.x = double(rand())/double(RAND_MAX);
        ////vel2.yaw = 2 * double(rand())/double(RAND_MAX) - 1;

        //ros_com->node_send(1,vel);
        ////ros_com.node_send(2,vel2);

        //position pose1 = ros_com->node_receive(1);
        ////position pose2 = ros_com.node_receive(2);

        //cout << "Robot 1 -> x:" << pose1.x << " y:" << pose1.y << " yaw:" << (pose1.yaw * 180 / PI) << endl;
        ////cout << "Robot 2 -> x:" << pose2.x << " y:" << pose2.y << " yaw:" << pose2.yaw << endl;
    //}

    return 0;
}

