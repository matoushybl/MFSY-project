//
// Created by matous on 4/11/20.
//

#include <ros/ros.h>

#include <fl/Headers.h>

int main(int argc, char **argv) {
    ros::init(argc, argv, "controller_node");

    auto *engine = new fl::Engine();
    engine->setName("horizontalMovementEngine");


    auto *horizontalPosition = new fl::InputVariable();
    horizontalPosition->setName("horizontalPosition");

    engine->addInputVariable(horizontalPosition);

    auto *angularVelocity = new fl::OutputVariable();
    angularVelocity->setName("angularVelocity");

    engine->addOutputVariable(angularVelocity);

    auto *mamdani = new fl::RuleBlock();

    engine->addRuleBlock(mamdani);

    engine->setInputValue("horizontalPosition", 0.5);

    return 0;
}