//
// Created by sziraqui on 22/6/19.
//

#include "MatInt.h"

// static methods
template <>
Napi::Object classifier::IntImage::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);
    Napi::Function func = DefineClass(env, "IntImage", {
            InstanceMethod("type", &classifier::IntImage::Type),
            InstanceMethod("height", &classifier::IntImage::Rows),
            InstanceMethod("width", &classifier::IntImage::Columns),
            InstanceMethod("channels", &classifier::IntImage::Channels),
            InstanceMethod("toInt32Array", &classifier::IntImage::ToTypedArray),
            InstanceMethod("extract", &classifier::IntImage::Extract)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();
    exports.Set("IntImage", func);
    return exports;
}
