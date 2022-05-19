//
// Created by sziraqui on 16/6/19.
//

#ifndef CLASSIFIER_OUTPUTUTILS_H
#define CLASSIFIER_OUTPUTUTILS_H

#include <opencv2/opencv.hpp>

#include <napi.h>

#include "../cvtypes/Mat.h"

namespace classifier {
    namespace OutputUtils {
        Napi::Object Init(Napi::Env env, Napi::Object exports);

        Napi::Value showImage(const Napi::CallbackInfo &info);

        Napi::Value readImage(const Napi::CallbackInfo &info);

        Napi::Value destroyWindow(const Napi::CallbackInfo &info);

        Napi::Value destroyAllWindows(const Napi::CallbackInfo &info);

        Napi::Value waitKey(const Napi::CallbackInfo &info);

        Napi::Value saveImage(const Napi::CallbackInfo &info);
    }
    

}

#endif //CLASSIFIER_OUTPUTUTILS_H
