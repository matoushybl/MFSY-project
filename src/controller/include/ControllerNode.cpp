//
// Created by matous on 4/23/20.
//

#include <Topics.h>
#include "ControllerNode.h"
#include <controller/ChassisControl.h>

ControllerNode::ControllerNode(ros::NodeHandle &handle) {
    lineDataSubscriber = handle.subscribe(topics::camera::LINE_POSITIONS, 0, &ControllerNode::onLineDataReceived, this);
    chassisControlPublisher = handle.advertise<controller::ChassisControl>(topics::chassis::CONTROL, 0);

    horizontalEngine = fl::FisImporter().fromFile("/home/pi/mfsy_workspace/src/controller/horizontal.fis");
    linePosition = horizontalEngine->getInputVariable("linePosition");
    horizontalSpeed = horizontalEngine->getOutputVariable("horizontalSpeed");

    linearEngine = fl::FisImporter().fromFile("/home/pi/mfsy_workspace/src/controller/linearSpeed.fis");
    linearLinePosition1 = linearEngine->getInputVariable("linePosition1");
    linearLinePosition2 = linearEngine->getInputVariable("linePosition2");
    linearLinePosition3 = linearEngine->getInputVariable("linePosition3");
    linearSpeed = linearEngine->getOutputVariable("linearSpeed");
}

void ControllerNode::onLineDataReceived(const controller::LinePosition::ConstPtr &message) {
    bool lineLost = message->widths[0] < 0.1;

    linePosition->setValue(message->centers[0] - 0.5f);
    horizontalEngine->process();
    const auto calculatedHorizontalSpeed = horizontalSpeed->getValue();
//    std::cout << "in: " << message->centers[0] << "out: " << horizontalSpeed->getValue() << std::endl;

    linearLinePosition1->setValue(std::abs(message->centers[0] - 0.5f));
    linearLinePosition2->setValue(std::abs(message->centers[3] - 0.5f));
    linearLinePosition3->setValue(std::abs(message->centers[5] - 0.5f));

    linearEngine->process();

    const auto calculatedLinearSpeed = linearSpeed->getValue();

//    std::cout << "L " <<  std::abs(message->centers[0] - 0.5f) << " "
//              <<  std::abs(message->centers[3] - 0.5f) << " "
//              <<  std::abs(message->centers[5] - 0.5f) << " S " << linearSpeed->getValue() << std::endl;

    controller::ChassisControl control;
    if (lineLost) {
        control.linearSpeed = pastActions.front().linearSpeed;
        control.angularSpeed = pastActions.front().angularSpeed;
        const auto front = pastActions.front();
        pastActions.pop();
        pastActions.push(front);
    } else {
        control.angularSpeed = calculatedHorizontalSpeed * 0.5;
        control.linearSpeed = 0.02 + 0.06 * calculatedLinearSpeed;

        pastActions.push(control);
        if (pastActions.size() >= pastActionsCount) {
            pastActions.pop();
        }
    }

    chassisControlPublisher.publish(control);
}

ControllerNode::~ControllerNode() {
    delete horizontalEngine;
}
