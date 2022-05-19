//
// Created by sziraqui on 17/6/19.
//

#include "Mat.h"

// static methods
template <>
Napi::Object classifier::Image::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);
    Napi::Function func = DefineClass(env, "Image", {
        InstanceMethod("type", &classifier::Image::Type),
        InstanceMethod("height", &classifier::Image::Rows),
        InstanceMethod("width", &classifier::Image::Columns),
        InstanceMethod("channels", &classifier::Image::Channels),
        InstanceMethod("toUint8Array", &classifier::Image::ToTypedArray),
        InstanceMethod("extract", &classifier::Image::Extract),
        InstanceMethod("resize", &classifier::Image::Resize),
        StaticMethod("fromBase64", &classifier::Image::FromBase64)
    });

    Image::constructor = Napi::Persistent(func);
    Image::constructor.SuppressDestruct();
    exports.Set("Image", func);
    return exports;
}

