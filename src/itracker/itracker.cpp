#include "itracker/itracker.h"

#include <iostream>
#include <memory>
#include <string>

#include "caffe/caffe.hpp"
#include "opencv2/opencv.hpp"

namespace itracker {

/**
 * Private Implementation pattern to hide caffe dependency in Gaze.
 */
class ITrackerImpl {
  std::unique_ptr<caffe::Net<float>> net;
  std::vector<cv::Mat> means;
 public:
  ITrackerImpl(const std::string& model_file,
               const std::string& weights_file,
               const std::string& mean_left_eye_file,
               const std::string& mean_right_eye_file,
               const std::string& mean_face_file);

  cv::Vec2f predict(const cv::Mat& left_eye, const cv::Mat& right_eye,
                    const cv::Mat& face, const cv::Mat& face_mask);
};

namespace util {

/**
 * Loads a mean binaryproto file and fills a Mat of size 224x224 with the values.
 *
 * See the caffe example for more details (there it's a class method of the Classifier).
 * https://github.com/BVLC/caffe/blob/1.0/examples/cpp_classification/classification.cpp
 *
 * @param mean_file The mean file to read
 * @return a mean mat
 */
cv::Mat load_mean(const std::string& mean_file) {
  caffe::BlobProto blob_proto;
  caffe::ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blob_proto);
  // Convert from BlobProto to Blob<float>
  caffe::Blob<float> mean_blob;
  mean_blob.FromProto(blob_proto);

  std::vector<cv::Mat> channels;
  float* data = mean_blob.mutable_cpu_data();
  for (int i = 0; i < 3; ++i) {
    cv::Mat channel(mean_blob.height(), mean_blob.width(), CV_32FC1, data);
    channels.push_back(channel);
    data += mean_blob.height() * mean_blob.width();
  }

  cv::Mat mean;
  cv::merge(channels, mean);

  cv::Scalar channel_mean = cv::mean(mean);
  return cv::Mat(224, 224, mean.type(), channel_mean);
}

}  // namespace util

ITrackerImpl::ITrackerImpl(const std::string& model_file,
                           const std::string& weights_file,
                           const std::string& mean_left_eye_file,
                           const std::string& mean_right_eye_file,
                           const std::string& mean_face_file) {
  // Init network
  this->net = std::unique_ptr<caffe::Net<float>>(
      new caffe::Net<float>(model_file, caffe::TEST));
  // Load stored weights
  this->net->CopyTrainedLayersFrom(weights_file);

  // Resize network (batchsize 1)
  for (auto i = 0; i < 4; ++i) {
    auto input = this->net->input_blobs()[i];
    std::vector<int> shape = input->shape();
    shape[0] = 1;
    input->Reshape(shape);
  }
  this->net->Reshape();

  // Load means
  this->means = {
    util::load_mean(mean_left_eye_file),
    util::load_mean(mean_right_eye_file),
    util::load_mean(mean_face_file)
  };
}

cv::Vec2f ITrackerImpl::predict(
    const cv::Mat& left_eye, const cv::Mat& right_eye,
    const cv::Mat& face, const cv::Mat& face_mask) {
  // Gather input
  std::vector<cv::Mat> input_images = {left_eye, right_eye, face, face_mask};

  // Convert and feed input
  for (int i = 0; i < 4; ++i) {
    auto input = this->net->input_blobs()[i];
    std::vector<int> shape = input->shape();

    // Create mat object per channel with pointer to layer data
    std::vector<cv::Mat> channels;
    float* input_data = input->mutable_cpu_data();
    for (int j = 0; j < shape[1]; ++j) {
      cv::Mat channel(shape[2], shape[3], CV_32FC1, input_data);
      channels.push_back(channel);
      input_data += shape[2] * shape[3];
    }

    // Resize image to channel shape (or reshape for the map)
    cv::Mat input_image;
    if (i != 3) {  // Default case is just filling the zero-mean images
      cv::Mat resized(shape[2], shape[3], CV_32FC3);
      input_images[i].convertTo(input_image, CV_32FC3);
      cv::resize(input_image, resized, resized.size());
      cv::subtract(resized, this->means[i], resized);
      cv::split(resized, channels);
    } else {  // Mask has special layout, thus copying value by value
      input_images[i].convertTo(input_image, CV_32FC1);
      for (int j = 0; j < shape[1]; ++j) {
        channels[j] = input_image.at<float>(j);
      }
    }
  }

  // Inference step
  this->net->Forward();

  // Read out results
  caffe::Blob<float>* output_layer = this->net->output_blobs()[0];
  const float* begin = output_layer->cpu_data();
  std::vector<float> out(begin, begin + output_layer->channels());

  // Store result
  return cv::Vec2f(out[0], out[1]);
}

ITracker::ITracker(const std::string& model_file,
                   const std::string& weights_file,
                   const std::string& mean_left_eye_file,
                   const std::string& mean_right_eye_file,
                   const std::string& mean_face_file)
  : impl(new ITrackerImpl(model_file, weights_file, mean_left_eye_file,
                          mean_left_eye_file, mean_face_file)) {
}

cv::Vec2f ITracker::predict(const cv::Mat& left_eye, const cv::Mat& right_eye,
                            const cv::Mat& face, const cv::Mat& face_mask) {
  return this->impl->predict(left_eye, right_eye, face, face_mask);
}

ITracker::~ITracker() {
  delete this->impl;
}

}  // namespace itracker
