#include "ImageData.h"

torch::Tensor read_data(std::string location) {
	/*
	 Function to return image read at location given as type torch::Tensor
	 Resizes image to (224, 224, 3)
	 Parameters
	 ===========
	 1. location (std::string type) - required to load image from the location

	 Returns
	 ===========
	 torch::Tensor type - image read as tensor
	*/
	cv::Mat img = cv::imread(location, 1);
	cv::resize(img, img, cv::Size(224, 224), cv::INTER_CUBIC);
	torch::Tensor img_tensor = torch::from_blob(img.data, { img.rows, img.cols, 3 }, torch::kByte);
	img_tensor = img_tensor.permute({ 2, 0, 1 });
	return img_tensor.clone();
}