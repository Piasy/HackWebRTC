/*
 *  Copyright (c) 2017 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_API_AUDIO_CODECS_G711_AUDIO_ENCODER_G711_H_
#define WEBRTC_API_AUDIO_CODECS_G711_AUDIO_ENCODER_G711_H_

#include <memory>
#include <vector>

#include "webrtc/api/audio_codecs/audio_encoder.h"
#include "webrtc/api/audio_codecs/audio_format.h"
#include "webrtc/rtc_base/optional.h"

namespace webrtc {

// G711 encoder API for use as a template parameter to
// CreateAudioEncoderFactory<...>().
//
// NOTE: This struct is still under development and may change without notice.
struct AudioEncoderG711 {
  struct Config {
    enum class Type { kPcmU, kPcmA };
    bool IsOk() const {
      return (type == Type::kPcmU || type == Type::kPcmA) &&
             frame_size_ms > 0 && frame_size_ms % 10 == 0 && num_channels >= 1;
    }
    Type type = Type::kPcmU;
    int num_channels = 1;
    int frame_size_ms = 20;
  };
  static rtc::Optional<AudioEncoderG711::Config> SdpToConfig(
      const SdpAudioFormat& audio_format);
  static void AppendSupportedEncoders(std::vector<AudioCodecSpec>* specs);
  static AudioCodecInfo QueryAudioEncoder(const Config& config);
  static std::unique_ptr<AudioEncoder> MakeAudioEncoder(const Config& config,
                                                        int payload_type);
};

}  // namespace webrtc

#endif  // WEBRTC_API_AUDIO_CODECS_G711_AUDIO_ENCODER_G711_H_
