#include <napi.h>
#include "classifier.h"
#include "resnet.h"
#include <string>

template<typename ... Args>
std::string string_format(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
	if (size <= 0) { throw std::runtime_error("Error during formatting."); }
	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), size, format.c_str(), args ...);
	return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

Napi::Object classifier::Utils::Init(Napi::Env env, Napi::Object exports) {
    exports.Set("classify", Napi::Function::New(env, classifier::Utils::classify));
    return exports;
}

// js-args: classifier.Image instance
Napi::String classifier::Utils::classify(const Napi::CallbackInfo &info) {
    Napi::Env env = info.Env();
    try {
        //// Unwrap Image and get mat
        Napi::Object parent = info[0].As<Napi::Object>();

        classifier::Image *image = classifier::Image::Unwrap(parent);
        cv::Mat mat = image->GetMat().clone();
        cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
        //cv::imwrite("test.jpg", mat);
        ////uint i = 1;
        ////bool asGray = info.Length() > i ? info[i].As<Napi::Boolean>().Value(): false;
        ////if (asGray) {
        ////    cv::cvtColor(mat, mat, cv::COLOR_GRAY2BGR);
        ////} else {        
        ////}

        torch::DeviceType device_type;
        device_type = torch::kCPU;

        cv::resize(mat, mat, cv::Size(224, 224), cv::INTER_CUBIC);
        torch::Tensor img_tensor = torch::from_blob(mat.data, { mat.rows, mat.cols, 3 }, torch::kByte);
        img_tensor = img_tensor.permute({ 2, 0, 1 });
        img_tensor = img_tensor.to(torch::kF32);

        ResNet18 model;
#ifdef _WIN32        
        torch::load(model, "C:/workspace/cassifierexample/build/resnet18_Python.pt");
#else
        torch::load(model, "/home/node/models/resnet18_Python.pt");    
#endif        
        model->to(device_type);
        img_tensor = img_tensor.to(device_type);

        img_tensor = torch::unsqueeze(img_tensor, 0);
        torch::NoGradGuard no_grad;
        model->eval();
        torch::Tensor out = torch::softmax(model->forward(img_tensor), 1);

        std::tuple<torch::Tensor, torch::Tensor> result = torch::max(out, 1);

        torch::Tensor prob = std::get<0>(result);
        torch::Tensor index = std::get<1>(result);

        auto probability = prob.accessor<float,1>();
        auto idx = index.accessor<int64_t, 1>();       
        
        return Napi::String::New(env, string_format("{ \"Class\": \"%d\", \"Probability\": \"%.3f\" }",
                idx[0], probability[0]));

    } catch( std::exception& ex) {
        return Napi::String::New(env, ex.what());
    }
}
