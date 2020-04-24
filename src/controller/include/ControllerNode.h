//
// Created by matous on 4/23/20.
//

#ifndef ROS_WORKSPACE_CONTROLLERNODE_H
#define ROS_WORKSPACE_CONTROLLERNODE_H

#include <ros/ros.h>
#include <controller/LinePosition.h>
#include <fl/Headers.h>
#include <controller/ChassisControl.h>
#include <queue>

class ControllerNode {
public:
    explicit ControllerNode(ros::NodeHandle &handle);
private:
    void onLineDataReceived(const controller::LinePosition::ConstPtr &message);

public:
    virtual ~ControllerNode();

private:
    ros::Subscriber lineDataSubscriber;
    ros::Publisher chassisControlPublisher;
    fl::Engine *horizontalEngine;
    fl::InputVariable *linePosition;
    fl::OutputVariable *horizontalSpeed;

    fl::Engine *linearEngine;
    fl::InputVariable *linearLinePosition1;
    fl::InputVariable *linearLinePosition2;
    fl::InputVariable *linearLinePosition3;
    fl::OutputVariable *linearSpeed;

    const int pastActionsCount = 10;
    std::queue<controller::ChassisControl> pastActions;
};


#endif //ROS_WORKSPACE_CONTROLLERNODE_H
