/*
 *
 *  Copyright (c) 2015, Facebook, Inc. All rights reserved.
 *
 *  Licensed under the Creative Commons Attribution-NonCommercial 3.0
 *  License (the "License"). You may obtain a copy of the License at
 *  https://creativecommons.org/licenses/by-nc/3.0/.
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 *  License for the specific language governing permissions and limitations
 *  under the License.
 *
 *
 */


#ifndef VIDEO_DATA_LAYER_HPP_
#define VIDEO_DATA_LAYER_HPP_

#include <string>
#include <utility>
#include <vector>

#include "pthread.h"
#include "boost/scoped_ptr.hpp"

#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

using std::string;

namespace caffe {

template <typename Dtype>
void* VideoDataLayerPrefetch(void* layer_pointer);

template <typename Dtype>
class VideoDataLayer : public Layer<Dtype> {
  // The function used to perform prefetching.
  friend void* VideoDataLayerPrefetch<Dtype>(void* layer_pointer);

 public:
  explicit VideoDataLayer(const LayerParameter& param)
      : Layer<Dtype>(param) {}
  virtual ~VideoDataLayer();
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {}

  virtual inline const char* type() const { return "VideoData"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int ExactNumTopBlobs() const { return 2; }

 protected:
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*> &top);
  virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) { return; }
  virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) { return; }

  virtual void CreatePrefetchThread();
  virtual void JoinPrefetchThread();
  virtual unsigned int PrefetchRand();

  shared_ptr<Caffe::RNG> prefetch_rng_;
  vector<string> file_list_;
  vector<int> start_frm_list_;
  vector<int> label_list_;
  vector<int> shuffle_index_;
  int lines_id_;

  int datum_channels_;
  int datum_length_;
  int datum_height_;
  int datum_width_;
  int datum_size_;
  pthread_t thread_;
  shared_ptr<Blob<Dtype> > prefetch_data_;
  shared_ptr<Blob<Dtype> > prefetch_label_;
  Blob<Dtype> data_mean_;
  bool output_labels_;
  //Phase phase_;     // we do not need phase_, Layer class has it.
};

}



#endif /* VIDEO_DATA_LAYER_HPP_ */
