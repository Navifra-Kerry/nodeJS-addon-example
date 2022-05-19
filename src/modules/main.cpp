#include <napi.h>
#include "io/OutputUtils.h"
#include "cvtypes/Mat.h"
#include "cvtypes/MatFloat.h"
#include "cvtypes/MatInt.h"
#include "classifier/classifier.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
    classifier::Image::Init(env, exports);
    classifier::FloatImage::Init(env, exports);
    classifier::IntImage::Init(env, exports);
    classifier::OutputUtils::Init(env, exports);
    classifier::Utils::Init(env, exports);
    //classifier::Init(env, exports);   
    return exports;
}

NODE_API_MODULE(classifier, InitAll)