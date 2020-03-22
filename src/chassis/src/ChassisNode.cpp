//
// Created by matous on 3/22/20.
//

#include <Topics.h>
#include "ChassisNode.h"

ChassisNode::ChassisNode(ros::NodeHandle &handle) {
    this->bus = std::make_unique<RoboUtils::I2C>(7);

    controlDataSubscriber = handle.subscribe(topics::chassis::CONTROL, 0, &ChassisNode::onControlDataReceived, this);
}

void ChassisNode::onControlDataReceived(const chassis::ChassisControl::ConstPtr &message) {
    float leftSpeed = message.get()->linearSpeed + 0.5 * message.get()->angularSpeed * 0.11;
    float rightSpeed = message.get()->linearSpeed - 0.5 * message.get()->angularSpeed * 0.11; // mps
    int16_t speeds[2] = {static_cast<int16_t>(leftSpeed / (3.14f * 0.65f) * 6400), static_cast<int16_t>(rightSpeed / (3.14f * 0.65f) * 6400) };
    bus->write16bitArray(0x71, 0x00, speeds, 2);
}
