//
// Created by sziraqui on 22/6/19.
//

#include "MatFloat.h"

// static methods
template <>
Napi::Object classifier::FloatImage::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);
    Napi::Function func = DefineClass(env, "FloatImage", {
            InstanceMethod("type", &classifier::FloatImage::Type),
            InstanceMethod("height", &classifier::FloatImage::Rows),
            InstanceMethod("width", &classifier::FloatImage::Columns),
            InstanceMethod("channels", &classifier::FloatImage::Channels),
            InstanceMethod("toFloat32Array", &classifier::FloatImage::ToTypedArray),
            InstanceMethod("extract", &classifier::FloatImage::Extract)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("FloatImage", func);
    return exports;
}