#pragma once

#include <opencv2\opencv.hpp>

unsigned char *BGR2YUV444_p(cv::Mat &Src, int flag = 0);
unsigned char *BGR2YUV444_semip(cv::Mat &Src, int flag = 0);
unsigned char *BGR2YUV422_p_R(cv::Mat &Src, int flag = 0);
unsigned char *BGR2YUV422_p_YV16(cv::Mat &Src, int flag = 0);
unsigned char *BGR2YUV420_I420(cv::Mat &Src, int flag = 0);
unsigned char *BGR2YUV420_NV12(cv::Mat &Src, int flag = 0);
unsigned char *BGR2YUV422_semip(cv::Mat &Src, int flag = 0);

void YUV444_p2BGR(unsigned char *yuvData,cv::Mat &Dst, int height, int width, int flag=0);
void YUV444_semip2BGR(unsigned char *yuvData,cv::Mat &Dst, int height, int width, int flag=0);
void YUV422_p_R2BGR(unsigned char *yuvData,cv::Mat &Dst, int height, int width, int flag=0);
void YUV422_p_YV162BGR(unsigned char *yuvData,cv::Mat &Dst, int height, int width, int flag=0);
void YUV422_semip2BGR(unsigned char *yuvData,cv::Mat &Dst, int height, int width, int flag=0);
void YUV420_I4202BGR(unsigned char *yuvData,cv::Mat &Dst, int height, int width, int flag=0);
void YUV420_NV122BGR(unsigned char *yuvData,cv::Mat &Dst, int height, int width, int flag=0);


