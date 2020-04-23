//
// Created by matous on 4/9/20.
//

#ifndef ROS_WORKSPACE_CAMERANODE_H
#define ROS_WORKSPACE_CAMERANODE_H

#include <ros/ros.h>
#include <opencv2/opencv.hpp>

struct RowData {
    uint8_t lineStart = 0;
    uint8_t lineEnd = 0;

    float lineWidth = 0;
    float lineCenter = 0;
};

struct CentersAndWidths {
    float centers[6] = { 0 };
    float widths[6] = { 0 };
};

class CameraNode {
public:
    explicit CameraNode(ros::NodeHandle &handle);
private:
    ros::Publisher rawImagePublisher;
    ros::Publisher imagePublisher;
    ros::Publisher locationPublisher;

    ros::Timer loopTimer;

    cv::VideoCapture cam;

    void onLoopTick(const ros::TimerEvent&);

    cv::Mat preprocessImage(const cv::Mat &image);
    CentersAndWidths processImage(const cv::Mat &image);

    uint64_t sqnCounter = 0;
};


#endif //ROS_WORKSPACE_CAMERANODE_H
