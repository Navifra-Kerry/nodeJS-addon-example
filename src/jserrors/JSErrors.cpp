//
// Created by sziraqui on 4/6/19.
//
#include "JSErrors.h"

void JSErrors::SetterError(Napi::Env env, std::string jsTypeName) {
    std::string msg = "SetterError: Expected exactly 1 argument of type " + jsTypeName;
    Napi::TypeError::New(env, msg).ThrowAsJavaScriptException();
}

void JSErrors::TooManyArguments(Napi::Env env, uint expectedArgLen, uint receivedArgLen) {
    std::string msg = "TooManyArguments: Expected " + std::to_string(expectedArgLen) + " args(s) but got " +
                      std::to_string(receivedArgLen) + " args(s)";
    Napi::Error::New(env, msg).ThrowAsJavaScriptException();
}

void JSErrors::InsufficientArguments(Napi::Env env, uint expectedArgLen, uint receivedArgLen) {
    std::string msg =
            "InsufficientArguments: Expected atleast " + std::to_string(expectedArgLen) + " args(s) but got only" +
            std::to_string(receivedArgLen) + " args(s)";
    Napi::Error::New(env, msg).ThrowAsJavaScriptException();
}

void JSErrors::IncorrectDatatype(Napi::Env env, std::string expectedType, int argNum) {
    std::string msg = "IncorrectDatatype: Expected type " + expectedType + " at argument " + std::to_string(argNum);
    Napi::Error::New(env, msg).ThrowAsJavaScriptException();
}

void JSErrors::IncorrectImageType(Napi::Env env, std::string expectedImageType) {
    std::string msg = "IncorrectImageType: Expected " + expectedImageType + " image";
    Napi::TypeError::New(env, msg).ThrowAsJavaScriptException();
}
