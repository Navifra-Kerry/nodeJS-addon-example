//
// Created by sziraqui on 4/6/19.
//

#ifndef CLASSIFIER_JSERRORS_H
#define CLASSIFIER_JSERRORS_H

#include <string>
#include <napi.h>

#ifdef _WIN32
    typedef unsigned int uint;
#endif

namespace JSErrors {

    const std::string NUMBER = "Number";
    const std::string STRING = "String";
    const std::string ARRAY = "Array";
    const std::string BOOLEAN = "Boolean";
    const std::string UINT8ARRAY = "Uint8Array";
    const std::string TYPEDARRAY = "TypedArray";
    const std::string NONE = "NULL";
    const std::string OBJECT = "Object";

    void SetterError(Napi::Env env, std::string jsTypeName);

    void InsufficientArguments(Napi::Env, uint expectedArgLen, uint receivedArgLen);

    void TooManyArguments(Napi::Env env, uint expectedArgLen, uint receivedArgLen);

    void IncorrectDatatype(Napi::Env env, std::string expectedType, int argNum = 0);

    void IncorrectImageType(Napi::Env env, std::string expectedImageType);
}
#endif //CLASSIFIER_JSERRORS_H
