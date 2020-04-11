//
// Created by matous on 3/22/20.
//

#ifndef ROS_WORKSPACE_TOPICS_H
#define ROS_WORKSPACE_TOPICS_H

#include <string>

namespace topics {
    namespace chassis {
        const std::string CONTROL = "chassis_control";
    }

    namespace camera {
        const std::string RAW_IMAGE = "raw_image";
        const std::string PROCESSED_IMAGE = "processed_image";
        const std::string LINE_POSITIONS = "line_positions";
    }
}

#endif //ROS_WORKSPACE_TOPICS_H
