//
// Created by matous on 3/22/20.
//

#include <ros/ros.h>
#include <ChassisNode.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "chassis_node");
    ros::NodeHandle n;

    const auto chassisNode = ChassisNode(n);

    ros::spin();
}