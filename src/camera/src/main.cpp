//
// Created by matous on 3/24/20.
//

#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
#include <chrono>
#include <camera/LinePosition.h>

inline long long millis() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

struct RowData {
    uint8_t lineStart = 0;
    uint8_t lineEnd = 0;

    float lineWidth = 0;
    float lineCenter = 0;
};

int main(int argc, char **argv) {
    ros::init(argc, argv, "camera_node");
    ros::NodeHandle node;

    ros::Publisher imagePub = node.advertise<sensor_msgs::Image>("image", 0);
    ros::Publisher locationPub = node.advertise<camera::LinePosition>("line_position", 0);

    ros::Rate loopRate(5);

    system("v4l2-ctl -d 0 -c auto_exposure=1 -c exposure_time_absolute=50");

    cv::VideoCapture cam(0);

    cam.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cam.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    if (!cam.isOpened()) {
        std::cout << "pici" << std::endl;
        return -1;
    }

    cv::Mat image;
    unsigned int counter = 0;
    while (ros::ok()) {
        auto start = millis();
        cam >> image;

        cv::Mat bw;
        cv::cvtColor(image, bw, cv::COLOR_RGB2GRAY);
        cv::threshold(bw, bw, 127, 255, cv::ThresholdTypes::THRESH_BINARY);

        cv::Size targetSize(64, 48);
        cv::Mat resized;
        cv::resize(bw, resized, targetSize);

        cv::transpose(resized, resized);
        cv::flip(resized, resized, 1);

        std::cout << millis() - start << std::endl;

        std::vector<RowData> data;

        for (int i = 0; i < resized.rows; ++i) {
            RowData currentRowData;
            bool startFound = false;
            bool stopFound = false;
            for (int j = 0; j < resized.cols; ++j) {
                if (resized.at<uint8_t>(i, j) == 0) {
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
        float centers[6] = { 0 };
        for (int l = 0; l < 6; l++) {
            for (int k = l * 10; k < ((l + 1) * 10); ++k) {
                centers[l] += data[k].lineCenter / static_cast<float>(resized.cols);
            }
            centers[l] /= 10;
        }


        std::cout << centers[0] << std::endl;

        std_msgs::Header header;
        header.seq = counter;
        header.stamp = ros::Time::now();

        cv_bridge::CvImage outputImage;
        outputImage.header = header;
        outputImage.encoding = sensor_msgs::image_encodings::MONO8;
        outputImage.image = resized;

        imagePub.publish(outputImage.toImageMsg());

        camera::LinePosition positionMsg;
        positionMsg.horizontalCenter = centers[0];
        for (int m = 0; m < 5; ++m) {
            positionMsg.verticalCenters[m] = centers[m + 1];
        }

        locationPub.publish(positionMsg);

        ros::spinOnce();
        loopRate.sleep();
        counter++;
    }
}