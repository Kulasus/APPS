// ***********************************************************************
//
// Demo program for education in subject
// Computer Architectures and Parallel Systems.
// Petr Olivka, dep. of Computer Science, FEI, VSB-TU Ostrava
// email:petr.olivka@vsb.cz
//
// Example of CUDA Technology Usage.
//
// Image interface for CUDA
//
// ***********************************************************************

#pragma once
#include <opencv2/opencv.hpp>

// Structure definition for exchanging data between Host and Device
struct CudaPic
{
  uint3 m_size;				// size of picture
  union {
	  void   *m_p_void;		// data of picture
	  uchar1 *m_p_uchar1;	// data of picture
	  uchar3 *m_p_uchar3;	// data of picture
	  uchar4 *m_p_uchar4;	// data of picture
  };
  


  CudaPic(cv::Mat &m)
  {
	  this->m_size.x = m.cols;
	  this->m_size.y = m.rows;
	  this->m_p_void = m.data;
  }

  __host__ __device__ uchar1 &at1(size_t y, size_t x){
	  return m_p_uchar1[m_size.x * y + x];
  }
  __host__ __device__ uchar3 &at3(size_t y, size_t x){
  	  //return this->m_size.x * y + x;
  	  return m_p_uchar3[m_size.x * y + x];
    }
  __host__ __device__ uchar4 &at4(size_t y, size_t x){
  	  return m_p_uchar4[m_size.x * y + x];
    }



};
