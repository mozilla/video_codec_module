/*
 *  This code is based on the WebRTC project. See the LICENSE file at
 *  the top of the tree.
 *
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *  Copyright (c) 2014, Mozilla
 *
 */

#ifndef VIDEO_CODEC_PLANE_H_
#define VIDEO_CODEC_PLANE_H_

// Helper class for I420VideoFrame: Store plane data and perform basic plane
// operations.
class Plane {
 public:
  Plane();
  ~Plane();
  // CreateEmptyPlane - set allocated size, actual plane size and stride:
  // If current size is smaller than current size, then a buffer of sufficient
  // size will be allocated.
  // Return value: 0 on success ,-1 on error.
  int CreateEmptyPlane(int allocated_size, int stride, int plane_size);

  // Copy the entire plane data.
  // Return value: 0 on success ,-1 on error.
  int Copy(const Plane& plane);

  // Copy buffer: If current size is smaller
  // than current size, then a buffer of sufficient size will be allocated.
  // Return value: 0 on success ,-1 on error.
  int Copy(int size, int stride, const uint8_t* buffer);

  // Swap plane data.
  void Swap(Plane& plane);

  // Get allocated size.
  int allocated_size() const {return allocated_size_;}

  // Set actual size.
  void ResetSize() {plane_size_ = 0;}

  // Return true is plane size is zero, false if not.
  bool IsZeroSize() const {return plane_size_ == 0;}

  // Get stride value.
  int stride() const {return stride_;}

  // Return data pointer.
  const uint8_t* buffer() const {return buffer_;}
  // Overloading with non-const.
  uint8_t* buffer() {return buffer_;}

 private:
  // Resize when needed: If current allocated size is less than new_size, buffer
  // will be updated. Old data will be copied to new buffer.
  // Return value: 0 on success ,-1 on error.
  int MaybeResize(int new_size);

  uint8_t* buffer_;
  int allocated_size_;
  int plane_size_;
  int stride_;
};  // Plane

#endif
