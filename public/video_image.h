/*
 *  This code is based on the WebRTC project. See the LICENSE file at
 *  the top of the tree.
 *
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *  Copyright (c) 2014, Mozilla
 *
 */

#ifndef VIDEO_CODEC_VIDEO_IMAGE_H_
#define VIDEO_CODEC_VIDEO_IMAGE_H_

#include <stdlib.h>

enum VideoFrameType
{
    kKeyFrame = 0,
    kDeltaFrame = 1,
    kGoldenFrame = 2,
    kAltRefFrame = 3,
    kSkipFrame = 4
};

class EncodedImage
{
public:
    EncodedImage()
        : _encodedWidth(0),
          _encodedHeight(0),
          _timeStamp(0),
          capture_time_ms_(0),
          _frameType(kDeltaFrame),
          _buffer(NULL),
          _length(0),
          _size(0),
          _completeFrame(false) {}

    EncodedImage(uint8_t* buffer,
                 uint32_t length,
                 uint32_t size)
        : _encodedWidth(0),
          _encodedHeight(0),
          _timeStamp(0),
          capture_time_ms_(0),
          _frameType(kDeltaFrame),
          _buffer(buffer),
          _length(length),
          _size(size),
          _completeFrame(false) {}

    uint32_t                    _encodedWidth;
    uint32_t                    _encodedHeight;
    uint32_t                    _timeStamp;
    int64_t                      capture_time_ms_;
    VideoFrameType              _frameType;
    uint8_t*                    _buffer;
    uint32_t                    _length;
    uint32_t                    _size;
    bool                        _completeFrame;
};

#endif
