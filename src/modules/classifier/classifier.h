#pragma once

#include <napi.h>
#include <opencv2/opencv.hpp>
#include "../cvtypes/Mat.h"

namespace classifier {
    namespace Utils {
        Napi::Object Init(Napi::Env env, Napi::Object exports);
        Napi::String classify(const Napi::CallbackInfo &info);
    }   
}
