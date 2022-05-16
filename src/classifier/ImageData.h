#ifndef IMAGE_NET_DATA_SET_H
#define IMAGE_NET_DATA_SET_H

#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <torch/torch.h>

// Function to return image read at location given as type torch::Tensor
// Resizes image to (224, 224, 3)
torch::Tensor read_data(std::string location);


#endif //IMAGE_DATA_SET_H