#include "classifier.h"
#include <napi.h>
#include <string>
#include "resnet.h"

template <typename... Args>
std::string string_format(const std::string& format, Args... args) {
  size_t size =
      snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
  if (size <= 0) {
    throw std::runtime_error("Error during formatting.");
  }
  std::unique_ptr<char[]> buf(new char[size]);
  snprintf(buf.get(), size, format.c_str(), args...);
  return std::string(
      buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

Napi::Object classifier::Utils::Init(Napi::Env env, Napi::Object exports) {
  exports.Set(
      "classify", Napi::Function::New(env, classifier::Utils::classify));
  return exports;
}

// js-args: classifier.Image instance
Napi::String classifier::Utils::classify(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  try {
    //// Unwrap Image and get mat
    Napi::Object parent = info[0].As<Napi::Object>();

    classifier::Image* image = classifier::Image::Unwrap(parent);
    cv::Mat mat = image->GetMat().clone();
    // cv::imwrite("test.jpg", mat);
    ////uint i = 1;
    ////bool asGray = info.Length() > i ? info[i].As<Napi::Boolean>().Value():
    /// false; /if (asGray) { /    cv::cvtColor(mat, mat, cv::COLOR_GRAY2BGR);
    ////} else {
    ////}

    torch::DeviceType device;
    device = torch::kCPU;

    cv::resize(mat, mat, cv::Size(224, 224), cv::INTER_CUBIC);
    cv::Mat rgb[3];
    cv::split(mat, rgb);
    // Concatenate channels
    cv::Mat rgbConcat;
    cv::vconcat(rgb[0], rgb[1], rgbConcat);
    cv::vconcat(rgbConcat, rgb[2], rgbConcat);

    // Convert Mat image to tensor C x H x W
    at::Tensor tensor_image = torch::from_blob(
        rgbConcat.data, {mat.channels(), mat.rows, mat.cols}, at::kByte);

    // Normalize tensor values from [0, 255] to [0, 1]
    tensor_image = tensor_image.toType(at::kFloat);
    tensor_image = tensor_image.div_(255);
    auto normalizeChannels = torch::data::transforms::Normalize<>(
        {0.485, 0.456, 0.406}, {0.229, 0.224, 0.225});
    tensor_image = normalizeChannels(tensor_image);
    tensor_image = torch::stack(tensor_image);
    tensor_image = tensor_image.to(device);
    ResNet18 model;
#ifdef _WIN32
    torch::load(model, "resnet18_Python.pt");
#else
    torch::load(model, "/home/node/models/resnet18_Python.pt");
#endif
    model->to(device);
    torch::NoGradGuard no_grad;
    model->eval();
    torch::Tensor out = torch::softmax(model->forward(tensor_image), 1);

    std::tuple<torch::Tensor, torch::Tensor> result = torch::max(out, 1);

    torch::Tensor prob = std::get<0>(result);
    torch::Tensor index = std::get<1>(result);

    auto probability = prob.accessor<float, 1>();
    auto idx = index.accessor<int64_t, 1>();

    return Napi::String::New(
        env,
        string_format(
            "{ \"Class\": \"%d\", \"Probability\": \"%.3f\" }",
            idx[0],
            probability[0]));

  } catch (std::exception& ex) {
    return Napi::String::New(env, ex.what());
  }
}
