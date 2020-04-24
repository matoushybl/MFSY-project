//
// Created by matous on 4/11/20.
//

#include <ros/ros.h>

#include <fl/Headers.h>
#include <ControllerNode.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "controller_node");

//    auto *horizontalEngine = new fl::Engine();
//    horizontalEngine->setName("horizontalMovementEngine");
//
//
//    auto *horizontalPosition = new fl::InputVariable();
//    horizontalPosition->setName("horizontalPosition");
//
//    horizontalEngine->addInputVariable(horizontalPosition);
//
//    auto *angularVelocity = new fl::OutputVariable();
//    angularVelocity->setName("angularVelocity");
//
//    horizontalEngine->addOutputVariable(angularVelocity);
//
//    auto *mamdani = new fl::RuleBlock();
//
//    horizontalEngine->addRuleBlock(mamdani);
//
//    horizontalEngine->setInputValue("horizontalPosition", 0.5);
    ros::NodeHandle handle;

    const ControllerNode controllerNode(handle);

    ros::spin();


    return 0;
}