/*
 *  This code is based on the WebRTC project. See the LICENSE file at
 *  the top of the tree.
 *
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *  Copyright (c) 2014, Mozilla
 *
 */

#ifndef VIDEO_CODEC_DECODER_H_
#define VIDEO_CODEC_DECODER_H_


#include "video_codec_common.h"
#include "video_codec_errors.h"
#include "i420_video_frame.h"
#include "video_image.h"

class DecodedImageCallback
{
public:
    virtual ~DecodedImageCallback() {};

    // Callback function which is called when an image has been decoded.
    //
    // Input:
    //          - decodedImage         : The decoded image.
    //
    // Return value                    : 0 if OK, < 0 otherwise.
    virtual int32_t Decoded(I420VideoFrame& decodedImage) = 0;

    virtual int32_t ReceivedDecodedReferenceFrame(const uint64_t pictureId) {return -1;}

    virtual int32_t ReceivedDecodedFrame(const uint64_t pictureId) {return -1;}
};

class VideoDecoder
{
public:
    virtual ~VideoDecoder() {};

    // Initialize the decoder with the information from the VideoCodec.
    //
    // Input:
    //          - inst              : Codec settings
    //          - numberOfCores     : Number of cores available for the decoder
    //
    // Return value                 : VIDEO_CODEC_OK if OK, < 0 otherwise.
    virtual int32_t InitDecode(const VideoCodec* codecSettings, int32_t numberOfCores) = 0;

    // Decode encoded image (as a part of a video stream). The decoded image
    // will be returned to the user through the decode complete callback.
    //
    // Input:
    //          - inputImage        : Encoded image to be decoded
    //          - missingFrames     : True if one or more frames have been lost
    //                                since the previous decode call.
    //          - fragmentation     : Specifies where the encoded frame can be
    //                                split into separate fragments. The meaning
    //                                of fragment is codec specific, but often
    //                                means that each fragment is decodable by
    //                                itself.
    //          - codecSpecificInfo : Pointer to codec specific data
    //          - renderTimeMs      : System time to render in milliseconds. Only
    //                                used by decoders with internal rendering.
    //
    // Return value                 : VIDEO_CODEC_OK if OK, < 0 otherwise.
    virtual int32_t
    Decode(const EncodedImage& inputImage,
           bool missingFrames,
           const RTPFragmentationHeader* fragmentation,
           const CodecSpecificInfo* codecSpecificInfo = NULL,
           int64_t renderTimeMs = -1) = 0;

    // Register an decode complete callback object.
    //
    // Input:
    //          - callback         : Callback object which handles decoded images.
    //
    // Return value                : VIDEO_CODEC_OK if OK, < 0 otherwise.
    virtual int32_t RegisterDecodeCompleteCallback(DecodedImageCallback* callback) = 0;

    // Free decoder memory.
    //
    // Return value                : VIDEO_CODEC_OK if OK, < 0 otherwise.
    virtual int32_t Release() = 0;

    // Reset decoder state and prepare for a new call.
    //
    // Return value                : VIDEO_CODEC_OK if OK, < 0 otherwise.
    virtual int32_t Reset() = 0;

    // Codec configuration data sent out-of-band, i.e. in SIP call setup
    //
    // Input/Output:
    //          - buffer           : Buffer pointer to the configuration data
    //          - size             : The size of the configuration data in
    //                               bytes
    //
    // Return value                : VIDEO_CODEC_OK if OK, < 0 otherwise.
    virtual int32_t SetCodecConfigParameters(const uint8_t* /*buffer*/, int32_t /*size*/) { return VIDEO_CODEC_ERROR; }

    // Create a copy of the codec and its internal state.
    //
    // Return value                : A copy of the instance if OK, NULL otherwise.
    virtual VideoDecoder* Copy() { return NULL; }
};

#endif
