//
// Created by sziraqui on 17/6/19.
//

#ifndef CLASSIFIER_MAT_H
#define CLASSIFIER_MAT_H

#include "MatImage.h"

namespace classifier {
    // Wraps OpenCV Mat partially for sending and receiving Mat objects to/from node
    typedef MatImage<uchar> Image;
}

#endif //CLASSIFIER_MAT_H
