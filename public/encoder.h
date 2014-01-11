/*
 *  This code is based on the WebRTC project. See the LICENSE file at
 *  the top of the tree.
 *
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *  Copyright (c) 2014, Mozilla
 *
 */

#ifndef VIDEO_CODEC_ENCODER_H_
#define VIDEO_CODEC_ENCODER_H_


#include <vector>

#include "video_codec_common.h"
#include "video_codec_errors.h"
#include "i420_video_frame.h"
#include "video_image.h"

class EncodedImageCallback
{
public:
    virtual ~EncodedImageCallback() {};

    // Callback function which is called when an image has been encoded.
    //
    // Input:
    //          - encodedImage         : The encoded image
    //
    // Return value                    : > 0,   signals to the caller that one or more future frames
    //                                          should be dropped to keep bit rate or frame rate.
    //                                   = 0,   if OK.
    //                                   < 0,   on error.
    virtual int32_t
    Encoded(EncodedImage& encodedImage,
            const CodecSpecificInfo* codecSpecificInfo = NULL,
            const RTPFragmentationHeader* fragmentation = NULL) = 0;
};

class VideoEncoder
{
public:
    virtual ~VideoEncoder() {};

    // Initialize the encoder with the information from the VideoCodec.
    //
    // Input:
    //          - codecSettings     : Codec settings
    //          - numberOfCores     : Number of cores available for the encoder
    //          - maxPayloadSize    : The maximum size each payload is allowed
    //                                to have. Usually MTU - overhead.
    //
    // Return value                 : VIDEO_CODEC_OK if OK, < 0 otherwise.
    virtual int32_t InitEncode(const VideoCodec* codecSettings, int32_t numberOfCores, uint32_t maxPayloadSize) = 0;

    // Encode an I420 image (as a part of a video stream). The encoded image
    // will be returned to the user through the encode complete callback.
    //
    // Input:
    //          - inputImage        : Image to be encoded
    //          - codecSpecificInfo : Pointer to codec specific data
    //          - frame_types        : The frame type to encode
    //
    // Return value                 : VIDEO_CODEC_OK if OK, < 0
    //                                otherwise.
    virtual int32_t Encode(
        const I420VideoFrame& inputImage,
        const CodecSpecificInfo* codecSpecificInfo,
        const std::vector<VideoFrameType>* frame_types) = 0;

    // Register an encode complete callback object.
    //
    // Input:
    //          - callback         : Callback object which handles encoded images.
    //
    // Return value                : VIDEO_CODEC_OK if OK, < 0 otherwise.
    virtual int32_t RegisterEncodeCompleteCallback(EncodedImageCallback* callback) = 0;

    // Free encoder memory.
    //
    // Return value                : VIDEO_CODEC_OK if OK, < 0 otherwise.
    virtual int32_t Release() = 0;

    // Inform the encoder about the packet loss and round trip time on the
    // network used to decide the best pattern and signaling.
    //
    //          - packetLoss       : Fraction lost (loss rate in percent =
    //                               100 * packetLoss / 255)
    //          - rtt              : Round-trip time in milliseconds
    //
    // Return value                : VIDEO_CODEC_OK if OK, < 0 otherwise.
    virtual int32_t SetChannelParameters(uint32_t packetLoss, int rtt) = 0;

    // Inform the encoder about the new target bit rate.
    //
    //          - newBitRate       : New target bit rate
    //          - frameRate        : The target frame rate
    //
    // Return value                : VIDEO_CODEC_OK if OK, < 0 otherwise.
    virtual int32_t SetRates(uint32_t newBitRate, uint32_t frameRate) = 0;

    // Use this function to enable or disable periodic key frames. Can be useful for codecs
    // which have other ways of stopping error propagation.
    //
    //          - enable           : Enable or disable periodic key frames
    //
    // Return value                : VIDEO_CODEC_OK if OK, < 0 otherwise.
    virtual int32_t SetPeriodicKeyFrames(bool enable) { return VIDEO_CODEC_ERROR; }

    // Codec configuration data to send out-of-band, i.e. in SIP call setup
    //
    //          - buffer           : Buffer pointer to where the configuration data
    //                               should be stored
    //          - size             : The size of the buffer in bytes
    //
    // Return value                : VIDEO_CODEC_OK if OK, < 0 otherwise.
    virtual int32_t CodecConfigParameters(uint8_t* /*buffer*/, int32_t /*size*/) { return VIDEO_CODEC_ERROR; }
};

#endif
