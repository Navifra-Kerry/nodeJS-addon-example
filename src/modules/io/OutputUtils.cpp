//
// Created by sziraqui on 16/6/19.
//
#include "OutputUtils.h"

Napi::Object classifier::OutputUtils::Init(Napi::Env env, Napi::Object exports) {
    exports.Set("showImage", Napi::Function::New(env, classifier::OutputUtils::showImage));
    exports.Set("readImage", Napi::Function::New(env, classifier::OutputUtils::readImage));
    exports.Set("destroyWindow", Napi::Function::New(env, classifier::OutputUtils::destroyWindow));
    exports.Set("destroyAllWindows", Napi::Function::New(env, classifier::OutputUtils::destroyAllWindows));
    exports.Set("waitKey", Napi::Function::New(env, classifier::OutputUtils::waitKey));
    exports.Set("saveImage", Napi::Function::New(env, classifier::OutputUtils::saveImage));
    return exports;
}

// js-args: classifier.Image instance
Napi::Value classifier::OutputUtils::showImage(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    // TODO: input validation
    // Unwrap Image and get mat
    Napi::Object parent = info[0].As<Napi::Object>();

    classifier::Image *image = classifier::Image::Unwrap(parent);
    cv::Mat mat = image->GetMat().clone();
    // Get optional arguments
    uint i = 1;
    std::string title = info.Length() > i ? info[i].As<Napi::String>().Utf8Value() : "output"; i++;
    cv::namedWindow(title, cv::WINDOW_AUTOSIZE);
    bool asGray = info.Length() > i? info[i].As<Napi::Boolean>().Value(): false;
    if (asGray) {
        cv::cvtColor(mat, mat, cv::COLOR_GRAY2BGR);
    } else {
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
    }
    cv::imshow(title, mat);
    return env.Undefined();
}

Napi::Value classifier::OutputUtils::readImage(const Napi::CallbackInfo &info) {
    Napi::EscapableHandleScope scope(info.Env());
    // TODO: input validation
    std::string path = info[0].As<Napi::String>().Utf8Value();
    bool asGray = info.Length() > 1? info[1].As<Napi::Boolean>().Value(): false;
    cv::Mat *mat;
    if(asGray) {
        mat = new cv::Mat(cv::imread(path, cv::IMREAD_GRAYSCALE));
    } else {
        mat = new cv::Mat(cv::imread(path, cv::IMREAD_COLOR));
        cv::cvtColor(*mat, *mat, cv::COLOR_BGR2RGB);
    }

    auto imgObj = classifier::Image::NewObject(info.Env(), *mat).As<Napi::Object>();
    return scope.Escape(imgObj);
}

Napi::Value classifier::OutputUtils::destroyWindow(const Napi::CallbackInfo &info) {
    // TODO: input validation
    auto windowName = info[0].As<Napi::String>().Utf8Value();
    cv::destroyWindow(windowName);
    return info.Env().Undefined();
}

Napi::Value classifier::OutputUtils::destroyAllWindows(const Napi::CallbackInfo &info) {
    cv::destroyAllWindows();
    return info.Env().Undefined();
}

Napi::Value classifier::OutputUtils::waitKey(const Napi::CallbackInfo &info) {
    // TODO: input validation
    auto delay = info[0].As<Napi::Number>().Int32Value();
    int key = cv::waitKey(delay);
    return Napi::Number::New(info.Env(), key);
}

Napi::Value classifier::OutputUtils::saveImage(const Napi::CallbackInfo &info) {
    // TODO: input validation
    auto absPath = info[0].As<Napi::String>().Utf8Value();
    auto imageObj = info[1].As<Napi::Object>();
    classifier::Image *img = classifier::Image::Unwrap(imageObj);
    
    cv::Mat mat = img->GetMat().clone();
    if(img->GetMat().channels() == 1) {
        cv::cvtColor(mat, mat, cv::COLOR_GRAY2BGR);
    } else {
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
    }
    cv::imwrite(absPath, mat);
    return info.Env().Undefined();
}
