//
// Created by sziraqui on 12/6/19.
//

#ifndef CLASSIFIER_NDARRAY_H
#define CLASSIFIER_NDARRAY_H
// system includes
#include <vector>
#include <string>
#include <initializer_list>

// opencv includes
#include <opencv2/core/mat.hpp>
#include <opencv2/core/utility.hpp>

// napi
#include <napi.h>


namespace NapiExtra {

    // NdArray handle conversions to/from Napi::ArrayBuffer and cv::Mat
    // By default this stores cv::Mat<uchar> or Array of Numbers
    template<class numericType = uchar>
    class NdArray {
    public:
        // Creating an instance
        static NdArray<numericType> From(Napi::TypedArrayOf<numericType> arr, Napi::Int32Array dims, bool copy = false);

        static NdArray<numericType> From(cv::Mat &mat); // must be uchar

        NdArray(std::vector<int> &shape, numericType *ptr);

        NdArray(std::vector<int> &shape, std::vector<numericType> &dataCopy);

        // Get underlying data
        // instance.Get({0,1,2}) to get element at (0,1,2) index of input cv::Mat or Array
        numericType Get(std::initializer_list<int> index);

        // instance.Set(8, {0,1,2}) to set value of element at (0,1,2) index of cv::Mat or Array
        bool Set(numericType value, std::initializer_list<int> indices);

        // matType must be as returned by original mat.type()
        cv::Mat ToMat(int matType);

        // Convert C++ array to Napi::ArrayBuffer
        Napi::ArrayBuffer ToArrayBuffer(Napi::Env &env); // calculate byteLength and call next overload
        // Convert C++ array to Napi::TypedArrayOf
        Napi::TypedArrayOf<numericType> ToTypedArray(Napi::Env &env);

        // Get array whose elements do are clamped to 255 when value exceeds its range
        Napi::Uint8Array ToUint8ClampedArray(Napi::Env &env);

        size_t ElementCount();

    private:
        // pointer to actual array backing cv::Mat or ArrayBuffer
        numericType *ptr;
        // Number of elements pointed by ptr
        // = product of values in this->shape
        size_t n_elements;
        // Values of buffer will be copied to this vector if needed
        std::vector<numericType> data;
        // actual dimensions of cv::Mat or multi-dimension array passed as array buffer
        // Eg, For cv::Mat image, shape = {rows, cols, channels}
        std::vector<int> shape;

        // Private constructor
        NdArray() {}

        //
        Napi::ArrayBuffer ToArrayBuffer(Napi::Env &env, size_t byteLength); // actual buffer creation
        static std::vector<int> GetShape(cv::Mat &mat);

        void CalculatePixels(const std::vector<int> &shape);
    };

    template<class numericType>
    NdArray<numericType>
    NdArray<numericType>::From(Napi::TypedArrayOf<numericType> arr, Napi::Int32Array dims, bool copy) {
        // Copy shape information
        std::vector<int> shape(dims.ElementLength());
        for (uint32_t i = 0u; i < shape.size(); ++i) {
            shape[i] = dims[i];
        }

        // Get data buffer of typed array
        Napi::ArrayBuffer buf = arr.ArrayBuffer();
        numericType *raw = reinterpret_cast<numericType *>(buf.Data());
        if (copy) {
            // Copy buffer data into a vector
            uint32_t bufLen = arr.ElementLength();
            std::vector<numericType> dataCopy(raw, raw + bufLen);
            return NdArray<numericType>(shape, dataCopy);
        } else {
            return NdArray<numericType>(shape, raw);
        }
    }

    template<class numericType>
    NdArray<numericType> NdArray<numericType>::From(cv::Mat &mat) {
        std::vector<int> shape = GetShape(mat);
        // Get reference to underlying array ptr
        size_t npixels = mat.total() * mat.channels(); // total() gives rows*cols for any flat image
        cv::Mat flatView = mat.reshape(1, npixels); // O(1)
        std::vector<numericType> data = mat.isContinuous() ? flatView
                                                           : flatView.clone(); // always a copy, O(npixels) time
        return NdArray<numericType>(shape, data);
    }

    template<class numericType>
    std::vector<int> NdArray<numericType>::GetShape(cv::Mat &mat) {// Copy shape information
        std::vector<int> shape(mat.size.dims() + 1); // +1 for channel
        for (int i = 0; i < mat.size.dims(); ++i) {
            shape[i] = mat.size[i];
        }
        shape[mat.size.dims()] = mat.channels();

        return shape;
    }

    template<class numericType>
    NdArray<numericType>::NdArray(std::vector<int> &shape, numericType *ptr) {
        this->ptr = ptr;
        this->shape = shape;
        this->CalculatePixels(shape);
    }

    template<class numericType>
    NdArray<numericType>::NdArray(std::vector<int> &shape, std::vector<numericType> &data) {
        this->ptr = data.data();
        this->shape = shape;
        this->data = data;
        this->CalculatePixels(shape);
    }

    template<class numericType>
    void NdArray<numericType>::CalculatePixels(const std::vector<int> &shape) {
        int total = 1;
        for (uint i = 0; i < shape.size(); ++i) {
            total = total * shape[i];
        }
        this->n_elements = total;
    }

    template<class numericType>
    numericType NdArray<numericType>::Get(std::initializer_list<int> indices) {
        uint32_t offset = 0;
        for (auto index: indices) {
            offset += index;
        }
        if (this->data.size() > 0) { // prefer getting value from vector
            return this->data[offset];
        } else {
            return this->ptr[offset];
        }
    }

    template<class numericType>
    bool NdArray<numericType>::Set(numericType value, std::initializer_list<int> indices) {
        uint32_t offset = 0;
        for (auto index: indices) {
            offset += index;
        }
        if (this->data.size() > 0) { // prefer manipulating vector
            this->data[offset] = value;
            return true;
        } else if (this->ptr != nullptr) {
            this->ptr[offset] = value;
            return true;
        }
        return false;
    }

    template<class numericType>
    cv::Mat NdArray<numericType>::ToMat(int matType) {
        const int *sizes = const_cast<int *>(this->shape.data()); // reference to internal array
        cv::Mat mat(this->shape.size(), sizes, matType, this->ptr);
        return mat;
    }

    template<class numericType>
    Napi::ArrayBuffer NdArray<numericType>::ToArrayBuffer(Napi::Env &env) {
        size_t byteLength = sizeof(numericType) * this->n_elements;
        return this->ToArrayBuffer(env, byteLength);
    }

    template<class numericType>
    Napi::ArrayBuffer NdArray<numericType>::ToArrayBuffer(Napi::Env &env, size_t byteLength) {
        if (this->data.size() > 0) { // data was copied, use it
            // create from this->data
            return Napi::ArrayBuffer::New(env, this->data.data(), byteLength);
        }
        // create from this->ptr
        return Napi::ArrayBuffer::New(env, this->ptr, byteLength);
    }

    template<class numericType>
    Napi::TypedArrayOf<numericType> NdArray<numericType>::ToTypedArray(Napi::Env &env) {
        Napi::ArrayBuffer ab = this->ToArrayBuffer(env);
        return Napi::TypedArrayOf<numericType>::New(env, this->ElementCount(), ab, 0 /**bufferOffset*/);
    }

    template<class numericType>
    Napi::Uint8Array NdArray<numericType>::ToUint8ClampedArray(Napi::Env &env) {
        Napi::ArrayBuffer ab = this->ToArrayBuffer(env);
        return Napi::Uint8Array::New(env, this->ElementCount(), ab, 0 /**bufferOffset*/, napi_uint8_clamped_array);
    }

    template<class numericType>
    size_t NdArray<numericType>::ElementCount() {
        return this->n_elements;
    }
}

#endif //CLASSIFIER_NDARRAY_H
