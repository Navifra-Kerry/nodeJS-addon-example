//
// Created by sziraqui on 3/7/19.
//

#ifndef CLASSIFIER_MATIMAGE_H
#define CLASSIFIER_MATIMAGE_H
#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>

#include <napi.h>

#include "../extras/napiextra.h"
#include "../jserrors/JSErrors.h"

#include "../utils/base64.h"

namespace classifier {
    // Wraps OpenCV Mat partially for sending and receiving Mat objects to/from node
    template <class numericType = uchar>
    class MatImage : public Napi::ObjectWrap<MatImage<numericType>> {

    public:

        static Napi::Object Init(Napi::Env env, Napi::Object exports);

        static Napi::Object NewObject(Napi::Env env, cv::Mat& mat);

        static Napi::Object NewObject(Napi::Env env, cv::Mat_<numericType>& mat);

        static cv::Mat* NewMat(Napi::TypedArrayOf<numericType>& arr, Napi::Number& rows, Napi::Number& cols, Napi::Number& type);

        MatImage(const Napi::CallbackInfo& info);

        cv::Mat GetMat();

        ~MatImage();

        cv::Mat* mat;

        static Napi::FunctionReference constructor;

        Napi::Value Type(const Napi::CallbackInfo& info);

        Napi::Value Rows(const Napi::CallbackInfo& info);

        Napi::Value Columns(const Napi::CallbackInfo& info);

        Napi::Value Channels(const Napi::CallbackInfo& info);

        Napi::Value ToTypedArray(const Napi::CallbackInfo& info);

        Napi::Value Extract(const Napi::CallbackInfo& info);

        Napi::Value Resize(const Napi::CallbackInfo& info);

        static Napi::Value FromBase64(const Napi::CallbackInfo& info);
    };
}

template <class numericType>
Napi::FunctionReference classifier::MatImage<numericType>::constructor;

template <class numericType>
Napi::Object classifier::MatImage<numericType>::NewObject(Napi::Env env, cv::Mat& mat) {
    Napi::EscapableHandleScope scope(env);
    // Get continuous mat data

    size_t length = mat.total() * mat.channels();
    if(!mat.isContinuous()) {
        mat =  mat.clone(); // ensures mat is continuous and also prevents segfault
    }

    // Create ArrayBuffer from data
    Napi::TypedArrayOf<numericType> arr = NapiExtra::cvMat2TypedArray<numericType>(env, mat);
    // Mat dimensions
    Napi::Number rows = Napi::Number::New(env, mat.rows); // arg2
    Napi::Number cols = Napi::Number::New(env, mat.cols); // arg3
    // Create mat type Number
    Napi::Number type = Napi::Number::New(env, mat.type());  // arg4
    // Call constructor
    Napi::Object imageObj = MatImage<numericType>::constructor.New({ arr, rows, cols, type });

    return scope.Escape(imageObj).As<Napi::Object>();
}

template <class numericType>
Napi::Object classifier::MatImage<numericType>::NewObject(Napi::Env env, cv::Mat_<numericType>& mat) {
    Napi::EscapableHandleScope scope(env);
    // Get continuous mat data

    size_t length = mat.total() * mat.channels();
    if(!mat.isContinuous()) {
        mat =  mat.clone(); // ensures mat is continuous and also prevents segfault
    }

    // Create ArrayBuffer from data
    Napi::TypedArrayOf<numericType> arr = NapiExtra::cvMat2TypedArray<numericType>(env, mat);
    // Mat dimensions
    Napi::Number rows = Napi::Number::New(env, mat.rows); // arg2
    Napi::Number cols = Napi::Number::New(env, mat.cols); // arg3
    // Create mat type Number
    Napi::Number type = Napi::Number::New(env, mat.type());  // arg4
    // Call constructor
    Napi::Object imageObj = MatImage<numericType>::constructor.New({ arr, rows, cols, type });

    return scope.Escape(imageObj).As<Napi::Object>();
}

// cpp only
template <class numericType>
cv::Mat* classifier::MatImage<numericType>::NewMat(Napi::TypedArrayOf<numericType> &arr, Napi::Number& rows, Napi::Number& cols, Napi::Number& type) {
    // Get mat data
    auto buf = arr.ArrayBuffer();
    numericType* data = reinterpret_cast<numericType *>(buf.Data()); // reinterpret_cast for void* to uchar*
    // Get mat size
    size_t matRows = rows.Int32Value();
    size_t matCols = cols.Int32Value();
    // Get Mat type
    int matType = type.Int32Value();
    // Create mat
    cv::Mat* mat = new cv::Mat(matRows, matCols, matType, data);

    return mat;
}

// constructor
template <class numericType>
classifier::MatImage<numericType>::MatImage(const Napi::CallbackInfo& info) : Napi::ObjectWrap<classifier::MatImage<numericType>>(info) {
    int len = info.Length();
    auto env = info.Env();
    if(len == 0 || len == 2) {
        JSErrors::InsufficientArguments(env, 4, 0);
    } else if (len == 1 && info[0].IsObject()) {
        // copy constructor
        MatImage<numericType> * img = Napi::ObjectWrap<classifier::MatImage<numericType>>::Unwrap(info[1].As<Napi::Object>());
        this->mat = img->mat;
    } else if (len == 4) {
        // constructor with data as arrays
        if(!info[0].IsTypedArray()) {
            JSErrors::IncorrectDatatype(env, JSErrors::TYPEDARRAY, 1);
        }
        for (int i = 1; i <= 3 && !info[i].IsNumber(); i++) {
            JSErrors::IncorrectDatatype(env, JSErrors::NUMBER, i);
        }
        auto arr = info[0].As<Napi::TypedArrayOf<numericType>>();
        auto rows = info[1].As<Napi::Number>();
        auto cols = info[2].As<Napi::Number>();
        auto type = info[3].As<Napi::Number>();
        this->mat = MatImage<numericType>::NewMat(arr, rows, cols, type);

    } else  {
        JSErrors::TooManyArguments(env, 4, len);
    }
}
// instance methods
template <class numericType>
Napi::Value classifier::MatImage<numericType>::Type(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), this->mat->type());
}

template <class numericType>
Napi::Value classifier::MatImage<numericType>::Rows(const Napi::CallbackInfo &info) {
    return Napi::Number::New(info.Env(), this->mat->rows);
}

template <class numericType>
Napi::Value classifier::MatImage<numericType>::Columns(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), this->mat->cols);
}

template <class numericType>
Napi::Value classifier::MatImage<numericType>::Channels(const Napi::CallbackInfo &info) {
    return Napi::Number::New(info.Env(), this->mat->channels());
}
// cpp only
template <class numericType>
cv::Mat classifier::MatImage<numericType>::GetMat() {
    return *this->mat;
}

template <class numericType>
Napi::Value classifier::MatImage<numericType>::ToTypedArray(const Napi::CallbackInfo &info) {
    cv::Mat mat = this->GetMat();
    Napi::TypedArrayOf<numericType> arr = NapiExtra::cvMat2TypedArray<numericType>(info.Env(), mat);
    return arr;
}

template <class numericType>
Napi::Value classifier::MatImage<numericType>::Extract(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    uint argLen = info.Length();
    if(argLen != 1 && !info[0].IsObject()) {
        JSErrors::IncorrectDatatype(env, "Rect", 0);
    }
    cv::Rect rect = NapiExtra::Napi2Rect<int>(info[0].As<Napi::Object>());
    cv::Mat img = this->GetMat();
    if(rect.x < 0) rect.x = 0;
    if(rect.y < 0) rect.y = 0;
    if(rect.x + rect.width > img.cols) rect.width = img.cols - rect.x;
    if(rect.y + rect.height > img.rows) rect.height = img.rows - rect.y;
    cv::Mat* region = new cv::Mat(img(rect));
    return NewObject(env, *region);
}

template <class numericType>
Napi::Value classifier::MatImage<numericType>::Resize(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    uint argLen = info.Length();
    if(argLen < 2) {
        JSErrors::InsufficientArguments(env, 2, argLen);
    } else if(argLen == 2 && !info[0].IsNumber()) {
        JSErrors::IncorrectDatatype(env, JSErrors::NUMBER, 0);
    } else if(argLen == 2 && !info[1].IsNumber()) {
        JSErrors::IncorrectDatatype(env, JSErrors::NUMBER, 1);
    }
    uint width = info[0].As<Napi::Number>();
    uint height = info[1].As<Napi::Number>();

    cv::Mat dest;
    cv::resize(this->GetMat(), dest, cv::Size(width, height));
    this->mat = NULL;
    this->mat = new cv::Mat(dest.clone());
    return env.Undefined();
}

template <class numericType>
Napi::Value classifier::MatImage<numericType>::FromBase64(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    uint argLen = info.Length();
    if(argLen == 0) {
        JSErrors::InsufficientArguments(env, 1, argLen);
    } else if(argLen == 1 && !info[0].IsString()) {
        JSErrors::IncorrectDatatype(env, JSErrors::STRING, 0);
    } else if(argLen > 1) {
        JSErrors::IncorrectDatatype(env, JSErrors::NUMBER, 1);
    }
    const std::string encoded = info[0].As<Napi::String>().Utf8Value();
    std::vector<uchar> decoded = base64_decode(encoded);
    cv::Mat mat = cv::imdecode(decoded, cv::IMREAD_UNCHANGED);

    cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
    cv::Mat* img = new cv::Mat(mat);
    
    return NewObject(env, *img);
}

template <class numericType>
classifier::MatImage<numericType>::~MatImage() {
    // std::cout<<"MatImage: Destructor called"<<std::endl;
    this->mat->release();
    delete this->mat;
}
#endif //CLASSIFIER_MATIMAGE_H
