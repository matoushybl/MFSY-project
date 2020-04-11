// Created by matous on 4/11/20.
//

#ifndef ROS_WORKSPACE_UTILS_H
#define ROS_WORKSPACE_UTILS_H

#include <chrono>
#include <cv_bridge/cv_bridge.h>

inline long long millis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

inline cv_bridge::CvImage createImageMsg(const cv::Mat &mat) {
    std_msgs::Header header;
    header.stamp = ros::Time::now();

    cv_bridge::CvImage outputImage;
    outputImage.header = header;
    outputImage.encoding = sensor_msgs::image_encodings::MONO8;
    outputImage.image = mat;

    return outputImage;
}

#endif //ROS_WORKSPACE_UTILS_H
