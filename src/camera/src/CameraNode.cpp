//
// Created by matous on 4/9/20.
//

#include "CameraNode.h"
#include <sensor_msgs/Image.h>
#include <Topics.h>
#include <cv_bridge/cv_bridge.h>
#include <utils.h>
#include <camera/LinePosition.h>

CameraNode::CameraNode(ros::NodeHandle &handle) {
    rawImagePublisher = handle.advertise<sensor_msgs::Image>(topics::camera::RAW_IMAGE, 0);
    imagePublisher = handle.advertise<sensor_msgs::Image>(topics::camera::PROCESSED_IMAGE, 0);

    locationPublisher = handle.advertise<camera::LinePosition>(topics::camera::LINE_POSITIONS, 0);

    loopTimer = handle.createTimer(ros::Duration(0.02), &CameraNode::onLoopTick, this);

    cam = cv::VideoCapture(0);

    cam.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cam.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    if (!cam.isOpened()) {
        throw std::runtime_error("Failed to open camera.");
    }

    system("v4l2-ctl -d 0 -c auto_exposure=1 -c exposure_time_absolute=50");
}

void CameraNode::onLoopTick(const ros::TimerEvent &) {
    cv::Mat rawImage;
    cam >> rawImage;
    rawImagePublisher.publish(createImageMsg(rawImage).toImageMsg());

    const auto preprocessedImage = preprocessImage(rawImage);
    imagePublisher.publish(createImageMsg(preprocessedImage).toImageMsg());

    const auto data = processImage(preprocessedImage);

    camera::LinePosition positionMsg;
    for (int i = 0; i < 6; ++i) {
        positionMsg.centers[i] = data.centers[i];
        positionMsg.widths[i] = data.widths[i];
    }

    locationPublisher.publish(positionMsg);
}

CentersAndWidths CameraNode::processImage(const cv::Mat &image) {
    CentersAndWidths result = { .centers = { 0 }, .widths = { 0 }};
    std::vector<RowData> data;

    for (int i = 0; i < image.rows; ++i) {
        RowData currentRowData;
        bool startFound = false;
        bool stopFound = false;
        for (int j = 0; j < image.cols; ++j) {
            if (image.at<uint8_t>(i, j) == 0) {
                if (!startFound) {
                    startFound = true;
                    currentRowData.lineStart = j;
                }
                if (!stopFound) {
                    currentRowData.lineEnd = j;
                }
            } else {
                if (startFound) {
                    stopFound = true;
                }
            }
        }
        currentRowData.lineWidth = currentRowData.lineEnd - currentRowData.lineStart;
        currentRowData.lineCenter = currentRowData.lineStart + currentRowData.lineWidth / 2;

        data.push_back(currentRowData);
    }

    // compute line horizontal position
    for (int l = 0; l < 6; l++) {
        for (int k = l * 10; k < ((l + 1) * 10); ++k) {
            result.centers[l] += data[k].lineCenter / static_cast<float>(image.cols);
            result.widths[l] += data[k].lineWidth / static_cast<float>(image.cols);
        }
        result.centers[l] /= 10;
        result.widths[l] /= 10;
    }

    return result;
}

cv::Mat CameraNode::preprocessImage(const cv::Mat &image) {
    cv::Mat bw;
    cv::cvtColor(image, bw, cv::COLOR_RGB2GRAY);
    cv::threshold(bw, bw, 127, 255, cv::ThresholdTypes::THRESH_BINARY);

    cv::Size targetSize(64, 48);
    cv::Mat resized;
    cv::resize(bw, resized, targetSize);

    cv::transpose(resized, resized);
    cv::flip(resized, resized, 1);

    return resized;
}
