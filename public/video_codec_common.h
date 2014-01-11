/*
 *  This code is based on the WebRTC project. See the LICENSE file at
 *  the top of the tree.
 *
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *  Copyright (c) 2014, Mozilla
 *
 */

#ifndef VIDEO_CODEC_VIDEO_CODEC_COMMON_H_
#define VIDEO_CODEC_VIDEO_CODEC_COMMON_H_


#include <stdint.h>

// Originally from common_types.h
// Video codec
enum { kConfigParameterSize = 128};
enum { kPayloadNameSize = 32};
enum { kMaxSimulcastStreams = 4};
enum { kMaxTemporalStreams = 4};

enum VideoCodecComplexity
{
    kComplexityNormal = 0,
    kComplexityHigh    = 1,
    kComplexityHigher  = 2,
    kComplexityMax     = 3
};

enum VideoCodecProfile
{
    kProfileBase = 0x00,
    kProfileMain = 0x01
};

enum VP8ResilienceMode {
  kResilienceOff,    // The stream produced by the encoder requires a
                     // recovery frame (typically a key frame) to be
                     // decodable after a packet loss.
  kResilientStream,  // A stream produced by the encoder is resilient to
                     // packet losses, but packets within a frame subsequent
                     // to a loss can't be decoded.
  kResilientFrames   // Same as kResilientStream but with added resilience
                     // within a frame.
};


// VP8 specific
struct VideoCodecVP8
{
    bool                 pictureLossIndicationOn;
    bool                 feedbackModeOn;
    VideoCodecComplexity complexity;
    VP8ResilienceMode    resilience;
    unsigned char        numberOfTemporalLayers;
    bool                 denoisingOn;
    bool                 errorConcealmentOn;
    bool                 automaticResizeOn;
    bool                 frameDroppingOn;
    int                  keyFrameInterval;
};

// Unknown specific
struct VideoCodecGeneric
{
};

// Video codec types
enum VideoCodecType
{
    kVideoCodecVP8,
    kVideoCodecI420,
    kVideoCodecRED,
    kVideoCodecULPFEC,
    kVideoCodecGeneric,
    kVideoCodecUnknown
};

union VideoCodecUnion
{
    VideoCodecVP8       VP8;
    VideoCodecGeneric   Generic;
};

// Simulcast is when the same stream is encoded multiple times with different
// settings such as resolution.
struct SimulcastStream
{
    unsigned short      width;
    unsigned short      height;
    unsigned char       numberOfTemporalLayers;
    unsigned int        maxBitrate;  // kilobits/sec.
    unsigned int        targetBitrate;  // kilobits/sec.
    unsigned int        minBitrate;  // kilobits/sec.
    unsigned int        qpMax; // minimum quality
};

enum VideoCodecMode {
  kRealtimeVideo,
  kScreensharing
};

// Common video codec properties
struct VideoCodec
{
    VideoCodecType      codecType;
    char                plName[kPayloadNameSize];
    unsigned char       plType;

    unsigned short      width;
    unsigned short      height;

    unsigned int        startBitrate;  // kilobits/sec.
    unsigned int        maxBitrate;  // kilobits/sec.
    unsigned int        minBitrate;  // kilobits/sec.
    unsigned char       maxFramerate;

    VideoCodecUnion     codecSpecific;

    unsigned int        qpMax;
    unsigned char       numberOfSimulcastStreams;
    SimulcastStream     simulcastStream[kMaxSimulcastStreams];

    VideoCodecMode      mode;

#ifdef WEBRTC
    // When using an external encoder/decoder this allows to pass
    // extra options without requiring webrtc to be aware of them.
    Config*  extra_options;
#endif
};


// Originally from video_codec_interface.h
class RTPFragmentationHeader; // forward declaration

// Note: if any pointers are added to this struct, it must be fitted
// with a copy-constructor. See below.
struct CodecSpecificInfoVP8
{
    bool             hasReceivedSLI;
    uint8_t    pictureIdSLI;
    bool             hasReceivedRPSI;
    uint64_t   pictureIdRPSI;
    int16_t    pictureId;         // negative value to skip pictureId
    bool             nonReference;
    uint8_t    simulcastIdx;
    uint8_t    temporalIdx;
    bool             layerSync;
    int              tl0PicIdx;         // Negative value to skip tl0PicIdx
    int8_t     keyIdx;            // negative value to skip keyIdx
};

struct CodecSpecificInfoGeneric {
  uint8_t simulcast_idx;
};

union CodecSpecificInfoUnion
{
    CodecSpecificInfoGeneric   generic;
    CodecSpecificInfoVP8       VP8;
};

// Note: if any pointers are added to this struct or its sub-structs, it
// must be fitted with a copy-constructor. This is because it is copied
// in the copy-constructor of VCMEncodedFrame.
struct CodecSpecificInfo
{
    VideoCodecType   codecType;
    CodecSpecificInfoUnion codecSpecific;
};

#endif
