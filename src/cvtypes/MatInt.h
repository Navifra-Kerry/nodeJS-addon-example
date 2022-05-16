//
// Created by sziraqui on 22/6/19.
//

#ifndef CLASSIFIER_MATINT_H
#define CLASSIFIER_MATINT_H

#include "MatImage.h"

namespace classifier {
    // Wraps OpenCV Mat_<int> partially for sending and receiving Mat_<int> objects to/from node
    typedef MatImage<int> IntImage;
}



#endif //CLASSIFIER_MATINT_H
