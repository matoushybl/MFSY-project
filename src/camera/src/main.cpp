//
// Created by matous on 3/24/20.
//

#include <ros/ros.h>
#include <CameraNode.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "camera_node");
    ros::NodeHandle handle;

    CameraNode node(handle);

    ros::spin();
}