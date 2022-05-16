//
// Created by sziraqui on 22/6/19.
//

#ifndef CLASSIFIER_MATFLOAT_H
#define CLASSIFIER_MATFLOAT_H

#include "MatImage.h"

namespace classifier {
    // Wraps OpenCV Mat_<float> partially for sending and receiving Mat_<float> objects to/from node
    typedef MatImage<float> FloatImage;
}

#endif //CLASSIFIER_MATFLOAT_H
