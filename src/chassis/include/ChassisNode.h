//
// Created by matous on 3/22/20.
//

#ifndef ROS_WORKSPACE_CHASSISNODE_H
#define ROS_WORKSPACE_CHASSISNODE_H

#include <ros/ros.h>
#include <chassis/ChassisControl.h>
#include <I2C.h>

class ChassisNode {
public:
    explicit ChassisNode(ros::NodeHandle &handle);

private:
    void onControlDataReceived(const chassis::ChassisControl::ConstPtr &message);

private:
    std::unique_ptr<RoboUtils::I2C> bus;
    ros::Subscriber controlDataSubscriber;
};


#endif //ROS_WORKSPACE_CHASSISNODE_H
