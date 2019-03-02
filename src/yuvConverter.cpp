#include <opencv2\opencv.hpp>
#include "yuvConverter.h"

//done
void YUV444_p2BGR(unsigned char *yuvData, cv::Mat &Dst, int height, int width, int flag)
{
	if (!yuvData) { return; }
	Dst = cv::Mat(height, width, CV_8UC3);

	int frameSize = height * width;
	int i, j;
	int pu, pv;
	if (flag)
	{
		pv = frameSize;
		pu = pv + frameSize;
	}
	else
	{
		pu = frameSize;
		pv = pu + frameSize;
	}

	for (i = 0; i < height; ++i)
	{
		uchar *DstPtr = Dst.ptr<uchar>(i);
		int baseI = i*width;
		for (j = 0; j < width; ++j)
		{
			int baseJ = j * 3;
			int Y = int(yuvData[baseI + j]);
			int U = int(yuvData[pu + baseI + j]);
			int V = int(yuvData[pv + baseI + j]);

			int B = Y + (1.7790*(U - 128));
			int G = Y - (0.3455*(U - 128) + (0.7169 * (V - 128)));
			int R = Y + (1.4075 * (V - 128));

			DstPtr[baseJ] = cv::saturate_cast<uchar>(B);
			DstPtr[baseJ + 1] = cv::saturate_cast<uchar>(G);
			DstPtr[baseJ + 2] = cv::saturate_cast<uchar>(R);
		}
	}
}

//done
unsigned char *BGR2YUV444_p(cv::Mat &Src, int flag)
{
	//flag: 0->YUV444_p(UV) , 1->YUV444_p(VU)
	if (Src.channels() != 3) { return NULL; }
	int height = Src.rows, width = Src.cols;
	int frameSize = height*width;
	int pu, pv;
	if (flag)
	{
		pv = frameSize;
		pu = pv + frameSize;
	}
	else
	{
		pu = frameSize;
		pv = pu + frameSize;
	}

	unsigned char *yuvoutput = new unsigned char[(height*width * 3)]();
	int i, j, k;

	for (i = 0,k=0; i<height; ++i)
	{
		uchar *ptr = Src.ptr<uchar>(i);
		for (j = 0; j<width; ++j)
		{
			int J = j * 3;
			int B = ptr[J], G = ptr[J + 1], R = ptr[J + 2];
			yuvoutput[k++] = cv::saturate_cast<uchar>(0.114*B + 0.587*G + 0.299*R);
			yuvoutput[pu++] = cv::saturate_cast<uchar>(0.5*B - 0.332*G - 0.169*R + 128);
			yuvoutput[pv++] = cv::saturate_cast<uchar>(-0.0813*B - 0.419*G + 0.5*R + 128);
		}
	}

	return yuvoutput;
}

void YUV444_semip2BGR(unsigned char *yuvData, cv::Mat &Dst, int height, int width, int flag)
{
	if (!yuvData) { return; }
	int frameSize = height *width;
	Dst = cv::Mat(height, width, CV_8UC3);
	int Uflag = 1, Vflag = 2;
	if (flag) { Uflag = 2; Vflag = 1; }

	int i, j, k;

	for (i = 0, k = 0; i < height; ++i)
	{
		uchar *DstPtr = Dst.ptr<uchar>(i);

		for (j = 0; j < width; ++j, k+= 3)
		{
			int baseJ = j * 3;
			int Y = int(yuvData[k]);
			int U = int(yuvData[k + Uflag]);
			int V = int(yuvData[k + Vflag]);

			int B = Y + (1.7790*(U - 128));
			int G = Y - (0.3455*(U - 128) + (0.7169 * (V - 128)));
			int R = Y + (1.4075 * (V - 128));

			DstPtr[baseJ] = cv::saturate_cast<uchar>(B);
			DstPtr[baseJ + 1] = cv::saturate_cast<uchar>(G);
			DstPtr[baseJ + 2] = cv::saturate_cast<uchar>(R);
		}
	}
}

//done
unsigned char *BGR2YUV444_semip(cv::Mat &Src, int flag)
{
	//flag: 0->YUV444_semip(UV), 1->YUV444_semip(VU)

	if (Src.channels() != 3) { return NULL; }
	int Uflag = 1, Vflag = 2;
	if (flag) { Uflag = 2; Vflag = 1; }

	int height = Src.rows, width = Src.cols;
	unsigned char *yuvoutput = new unsigned char[(height*width * 3)]();
	int i, j, k;

	for (i = 0, k = 0; i<height; ++i)
	{
		uchar *ptr = Src.ptr<uchar>(i);
		for (j = 0; j<width; j++)
		{
			int J = j * 3;
			int B = ptr[J], G = ptr[J + 1], R = ptr[J + 2];
			float Y = 0.114*B + 0.587*G + 0.299*R;
			float U = 0.5*B - 0.332*G - 0.169*R + 128;
			float V = -0.0813*B - 0.419*G + 0.5*R + 128;
			yuvoutput[k] = cv::saturate_cast<uchar>(Y);
			yuvoutput[k + Uflag] = cv::saturate_cast<uchar>(U);
			yuvoutput[k + Vflag] = cv::saturate_cast<uchar>(V);
			k += 3;
		}
	}

	return yuvoutput;
}

void YUV422_p_YV162BGR(unsigned char *yuvData, cv::Mat &Dst, int height, int width, int flag)
{
	if (!yuvData) { return; }
	int frameSize = height*width;
	Dst = cv::Mat(height, width, CV_8UC3);
	int pu, pv;
	if (flag)
	{
		pv = frameSize;
		pu = pv + (frameSize >> 1);
	}
	else
	{
		pu = frameSize;
		pv = pu + (frameSize >> 1);
	}

	int i, j,k;
	for (i = 0, k = 0; i < height; ++i)
	{
		uchar *DstPtr = Dst.ptr<uchar>(i);
		for (j = 0; j < width;)
		{
			int count = 0;
			while (count < 2 && j < width)
			{
				int baseJ = j * 3;
				int Y = int(yuvData[k++]);
				int U = int(yuvData[pu]);
				int V = int(yuvData[pv]);

				int B = Y + (1.7790*(U - 128));
				int G = Y - (0.3455*(U - 128) + (0.7169 * (V - 128)));
				int R = Y + (1.4075 * (V - 128));

				DstPtr[baseJ] = cv::saturate_cast<uchar>(B);
				DstPtr[baseJ + 1] = cv::saturate_cast<uchar>(G);
				DstPtr[baseJ + 2] = cv::saturate_cast<uchar>(R);
				count++;
				j++;
			}
			pu++;
			pv++;
			
		}
	}
}

//done
unsigned char *BGR2YUV422_p_YV16(cv::Mat &Src, int flag)
{
	if (Src.channels() != 3) { return NULL; }
	int height = Src.rows, width = Src.cols;
	int frameSize = height *width;
	//flag:0->YUV422p_UV, 1->YUV422p_VU

	int pu, pv;
	if (flag)
	{
		pv = frameSize;
		pu = pv + (frameSize >> 1);
	}
	else
	{
		pu = frameSize;
		pv = pu + (frameSize >> 1);
	}

	unsigned char *yuvoutput = new unsigned char[(height*width) << 1]();
	int i, j, k;
	for (i = 0, k = 0; i<height; i++)
	{
		uchar *ptr = Src.ptr<uchar>(i);
		for (j = 0; j<width;)
		{
			int U = 0, V = 0;
			int count = 0;
			while (count<2 && j<width)
			{
				int J = j * 3;
				int B = ptr[J], G = ptr[J + 1], R = ptr[J + 2];		
				yuvoutput[k++] = cv::saturate_cast<uchar>(0.114*B + 0.587*G + 0.299*R);
				U += 0.5*B - 0.332*G - 0.169*R + 128;
				V += -0.0813*B - 0.419*G + 0.5*R + 128;
				++count;
				++j;
			}

			U = U >> 1;
			V = V >> 1;
			yuvoutput[pu++] = cv::saturate_cast<uchar>(U);
			yuvoutput[pv++] = cv::saturate_cast<uchar>(V);
		}
	}
	return yuvoutput;
}

void YUV422_p_R2BGR(unsigned char *yuvData, cv::Mat &Dst, int height, int width, int flag)
{
	if (!yuvData) { return; }

	Dst = cv::Mat(height, width, CV_8UC3);

	int frameSize = height * width;
	int pu, pv;
	if (flag)
	{
		pv = frameSize;
		pu = pv + (frameSize >> 1);
	}
	else
	{
		pu = frameSize;
		pv = pu + (frameSize >> 1);
	}

	int i, j, k;

	for (i = 0,k=0; i < height; i+= 2,k+=width)
	{
		uchar *prev_DstPtr = Dst.ptr<uchar>(i);
		uchar *next_DstPtr = Dst.ptr<uchar>(i + 1);
		for (j = 0; j < width; ++j)
		{
			int baseJ = j * 3;

			int Y2 = int(yuvData[k + width]);
			int Y1 = int(yuvData[k++]);
			int U = int(yuvData[pu++]);
			int V = int(yuvData[pv++]);

			int B1 = Y1 + (1.7790*(U - 128));
			int G1 = Y1 - (0.3455*(U - 128) + (0.7169 * (V - 128)));
			int R1 = Y1 + (1.4075 * (V - 128));

			int B2 = Y2 + (1.7790*(U - 128));
			int G2 = Y2 - (0.3455*(U - 128) + (0.7169 * (V - 128)));
			int R2 = Y2 + (1.4075 * (V - 128));

			prev_DstPtr[baseJ] = cv::saturate_cast<uchar>(B1);
			prev_DstPtr[baseJ + 1] = cv::saturate_cast<uchar>(G1);
			prev_DstPtr[baseJ + 2] = cv::saturate_cast<uchar>(R1);

			next_DstPtr[baseJ] = cv::saturate_cast<uchar>(B2);
			next_DstPtr[baseJ + 1] = cv::saturate_cast<uchar>(G2);
			next_DstPtr[baseJ + 2] = cv::saturate_cast<uchar>(R2);
		}
	}
}

//done
unsigned char *BGR2YUV422_p_R(cv::Mat &Src, int flag)
{
	if (Src.channels() != 3) { return NULL; }
	int height = Src.rows, width = Src.cols;
	int frameSize = height *width;
	//flag:0->YUV422p_UV, 1->YUV422p_VU

	int pu, pv;
	if (flag)
	{
		pv = frameSize ;
		pu = pv + (frameSize >> 1);
	}
	else
	{
		pu = frameSize ;
		pv = pu + (frameSize >> 1);
	}

	unsigned char *yuvoutput = new unsigned char[(height*width) << 1]();
	int i, j, k;
	for (i = 0, k = 0; i<height; i+=2,k+=width)
	{
		uchar *ptr_prev = Src.ptr<uchar>(i);
		uchar *ptr_next = Src.ptr<uchar>(i + 1);
		for (j = 0; j<width;)
		{
			int U = 0, V = 0;
			int count = 0;
			while (count<1 && j<width)
			{
				int J = j * 3;
				int Bp = ptr_prev[J], Gp = ptr_prev[J + 1], Rp = ptr_prev[J + 2];
				int Bn = ptr_next[J], Gn = ptr_next[J + 1], Rn = ptr_next[J + 2];
				yuvoutput[k+width] = cv::saturate_cast<uchar>(0.114*Bn + 0.587*Gn + 0.299*Rn);
				yuvoutput[k++] = cv::saturate_cast<uchar>(0.114*Bp + 0.587*Gp + 0.299*Rp);

				U += 0.5*(Bn + Bp) - 0.332*(Gn + Gp) - 0.169*(Rn + Rp) + 256;
				V += -0.0813*(Bn + Bp) - 0.419*(Gn + Gp) + 0.5*(Rn + Rp) + 256;
				++count;
				++j;
			}

			U = U >> 1;
			V = V >> 1;


			yuvoutput[pu++] = cv::saturate_cast<uchar>(U);
			yuvoutput[pv++] = cv::saturate_cast<uchar>(V);
		}
	}
	return yuvoutput;
}

void YUV420_I4202BGR(unsigned char *yuvData, cv::Mat &Dst, int height, int width, int flag)
{
	if (!yuvData) { return; }
	int frameSize = height*width;
	Dst = cv::Mat(height, width, CV_8UC3);
	int pu, pv;
	int i, j, k;
	if (flag)
	{
		pv = frameSize;
		pu = pv + (frameSize >> 2);
	}
	else
	{
		pu = frameSize;
		pv = pu + (frameSize >> 2);
	}

	for (i = 0,k=0; i < height; i+=2,k+= width )
	{
		uchar *prev_DstPtr = Dst.ptr<uchar>(i);
		uchar *next_DstPtr = Dst.ptr<uchar>(i + 1);
	

		for (j = 0; j < width;)
		{
			int count = 0;
			
			while (count < 2 && j<width)
			{
				int baseJ = j * 3;
				int Y2 = int(yuvData[k + width]);
				int Y1 = int(yuvData[k++]);
				
				int U = int(yuvData[pu]);
				int V = int(yuvData[pv]);

				int B1 = Y1 + (1.7790*(U - 128));
				int G1 = Y1 - (0.3455*(U - 128) + (0.7169 * (V - 128)));
				int R1 = Y1 + (1.4075 * (V - 128));

				int B2 = Y2 + (1.7790*(U - 128));
				int G2 = Y2 - (0.3455*(U - 128) + (0.7169 * (V - 128)));
				int R2 = Y2 + (1.4075 * (V - 128));

				prev_DstPtr[baseJ] = cv::saturate_cast<uchar>(B1);
				prev_DstPtr[baseJ + 1] = cv::saturate_cast<uchar>(G1);
				prev_DstPtr[baseJ + 2] = cv::saturate_cast<uchar>(R1);

				next_DstPtr[baseJ] = cv::saturate_cast<uchar>(B2);
				next_DstPtr[baseJ + 1] = cv::saturate_cast<uchar>(G2);
				next_DstPtr[baseJ + 2] = cv::saturate_cast<uchar>(R2);

				count++;
				j++;

			}
			pu++;
			pv++;
		}
	}

}

//done
unsigned char *BGR2YUV420_I420(cv::Mat &Src, int flag)
{
	if (Src.channels() != 3) { return NULL; }
	int height = Src.rows, width = Src.cols;
	int frameSize = height *width;
	//flag:0->I420(YU12) , 1->YV12
	int pu, pv;
	int i, j, k;

	if (flag)
	{
		pv = frameSize;
		pu = pv + (frameSize >> 2);
	}
	else
	{
		pu = frameSize;
		pv = pu + (frameSize >> 2);
	}

	unsigned char *yuvoutput = new unsigned char[((height*width * 3) >> 1) + 0.5];

	for (i = 0, k = 0; i<height; i+=2,k+=width)
	{
		uchar *ptr_prev = Src.ptr<uchar>(i);
		uchar *ptr_next = Src.ptr<uchar>(i + 1);
		for (j = 0; j<width;)
		{
			int count = 0;
			int U = 0, V = 0;
			while (count<2 && j<width)
			{
				int J = j * 3;
				int Bp = (int)ptr_prev[J], Gp = (int)ptr_prev[J + 1], Rp = (int)ptr_prev[J + 2];
				int Bn = (int)ptr_next[J], Gn = (int)ptr_next[J + 1], Rn = (int)ptr_next[J + 2];
				yuvoutput[k+width] = cv::saturate_cast<uchar>(0.114*Bn + 0.587*Gn + 0.299*Rn);
				yuvoutput[k++] = cv::saturate_cast<uchar>(0.114*Bp + 0.587*Gp + 0.299*Rp);
				U += 0.5*(Bn + Bp) - 0.332*(Gn + Gp) - 0.169*(Rn + Rp) + 256;
				V += -0.0813*(Bn + Bp) - 0.419*(Gn + Gp) + 0.5*(Rn + Rp) + 256;
				++j;
				++count;
			}

			U /= 4;
			V /= 4;

			yuvoutput[pu++] = cv::saturate_cast<uchar>(U);
			yuvoutput[pv++] = cv::saturate_cast<uchar>(V);
		}
	}

	return yuvoutput;
}

void YUV420_NV122BGR(unsigned char *yuvData, cv::Mat &Dst, int height, int width, int flag)
{
	if (!yuvData) { return; }
	int frameSize = height * width;
	Dst = cv::Mat(height, width, CV_8UC3);
	int Uflag = 0, Vflag = 1;
	if (flag) { Uflag = 1; Vflag = 0; }

	int puv = frameSize;

	int i, j, k;
	for (i = 0,k=0; i < height; i += 2, k += width)
	{
		uchar *prev_DstPtr = Dst.ptr<uchar>(i);
		uchar *next_DstPtr = Dst.ptr<uchar>(i + 1);

		for (j = 0; j < width;)
		{
			int count = 0;
			while (count < 2 && j<width)
			{
				int baseJ = j * 3;

				int Y2 = int(yuvData[k + width]);
				int Y1 = int(yuvData[k++]);

				int U = int(yuvData[puv + Uflag]);
				int V = int(yuvData[puv + Vflag]);

				int B1 = Y1 + (1.7790*(U - 128));
				int G1 = Y1 - (0.3455*(U - 128) + (0.7169 * (V - 128)));
				int R1 = Y1 + (1.4075 * (V - 128));

				int B2 = Y2 + (1.7790*(U - 128));
				int G2 = Y2 - (0.3455*(U - 128) + (0.7169 * (V - 128)));
				int R2 = Y2 + (1.4075 * (V - 128));

				prev_DstPtr[baseJ] = cv::saturate_cast<uchar>(B1);
				prev_DstPtr[baseJ + 1] = cv::saturate_cast<uchar>(G1);
				prev_DstPtr[baseJ + 2] = cv::saturate_cast<uchar>(R1);

				next_DstPtr[baseJ] = cv::saturate_cast<uchar>(B2);
				next_DstPtr[baseJ + 1] = cv::saturate_cast<uchar>(G2);
				next_DstPtr[baseJ + 2] = cv::saturate_cast<uchar>(R2);

				j++;
				count++;
			}
			puv += 2;
		}
	}
}

//done
unsigned char *BGR2YUV420_NV12(cv::Mat &Src, int flag)
{
	// flag: 0->NV12 , 1->NV21
	int Uflag = 0, Vflag = 1;
	if (flag) { Uflag = 1; Vflag = 0; }

	if (Src.channels() != 3) { return NULL; }
	int height = Src.rows, width = Src.cols;

	unsigned char *yuvoutput = new unsigned char[((height*width * 3)>>1) +0.5]();

	int puv = height*width;
	int i, j, k;

	for (i = 0, k = 0; i<height-1; i+=2,k+=width )
	{
		uchar *ptr_prev = Src.ptr<uchar>(i);
		uchar *ptr_next = Src.ptr<uchar>(i + 1);
		for (j = 0; j<width;)
		{

			int count = 0;
			int U = 0, V = 0;
			while (count<2 && j<width)
			{
				int J = j * 3;
				int Bp = (int)ptr_prev[J], Gp = (int)ptr_prev[J + 1], Rp = (int)ptr_prev[J + 2];
				int Bn = (int)ptr_next[J], Gn = (int)ptr_next[J + 1], Rn = (int)ptr_next[J + 2];
				yuvoutput[k + width] = cv::saturate_cast<uchar>(0.114*Bn + 0.587*Gn + 0.299*Rn);
				yuvoutput[k++] = cv::saturate_cast<uchar>(0.114*Bp + 0.587*Gp + 0.299*Rp);
				
				U += 0.5*(Bn+Bp) - 0.332*(Gn+Gp) - 0.169*(Rn+Rp) + 256;
				V += -0.0813*(Bn+Bp) - 0.419*(Gn+Gp) + 0.5*(Rn+Rp) + 256;
		
				++j;
				++count;
			}

			U = U >> 2;
			V = V >> 2;

			yuvoutput[puv + Uflag] = cv::saturate_cast<uchar>(U);
			yuvoutput[puv + Vflag] = cv::saturate_cast<uchar>(V);
			puv += 2;
		}
	}


	return yuvoutput;
}

void YUV422_semip2BGR(unsigned char *yuvData, cv::Mat &Dst, int height, int width, int flag)
{
	if (!yuvData) { return; }
	int frameSize = height * width;
	Dst = cv::Mat(height, width, CV_8UC3);
	int Uflag, Vflag, Yflag;
	int i, j, k;
	switch (flag)
	{
		case 0:
		Uflag = 1, Vflag = 3, Yflag = 0;
		break;
		case 1:
		Uflag = 3, Vflag = 1, Yflag = 0;
		break;
		case 2:
		Uflag = 0, Vflag = 2, Yflag = 1;
		break;
		case 3:
		Uflag = 2, Vflag = 0, Yflag = 1;
		break;
		default:
		;
	}


	for (i = 0,k = 0; i < height; ++i)
	{
		uchar *DstPtr = Dst.ptr<uchar>(i);
		for (j = 0; j < width; j+=2,k+=4 )
		{
			int baseJ = j * 3;
			int Y1 = int(yuvData[k + Yflag]);
			int Y2 = int(yuvData[k + Yflag + 2]);
			int U = int(yuvData[k + Uflag]);
			int V = int(yuvData[k + Vflag]);

			int B1 = Y1 + (1.7790*(U - 128));
			int G1 = Y1 - (0.3455*(U - 128) + (0.7169 * (V - 128)));
			int R1 = Y1 + (1.4075 * (V - 128));

			int B2 = Y2 + (1.7790*(U - 128));
			int G2 = Y2 - (0.3455*(U - 128) + (0.7169 * (V - 128)));
			int R2 = Y2 + (1.4075 * (V - 128));
			
			DstPtr[baseJ] = cv::saturate_cast<uchar>(B1);
			DstPtr[baseJ+1] = cv::saturate_cast<uchar>(G1);
			DstPtr[baseJ+2] = cv::saturate_cast<uchar>(R1);
			DstPtr[baseJ+3] = cv::saturate_cast<uchar>(B2);
			DstPtr[baseJ+4] = cv::saturate_cast<uchar>(G2);
			DstPtr[baseJ+5] = cv::saturate_cast<uchar>(R2);
		}
	}
}
//done
unsigned char *BGR2YUV422_semip(cv::Mat &Src, int flag)
{

	if (Src.channels() != 3) { return NULL; }
	int Uflag, Vflag, Yflag;

	//flag: 0->YUY2(YUYV) , 1->YVYU , 2->UYVY , 3->VYUY

	switch (flag)
	{
		case 0:
		Uflag = 1, Vflag = 3, Yflag = 0;
		break;
		case 1:
		Uflag = 3, Vflag = 1, Yflag = 0;
		break;
		case 2:
		Uflag = 0, Vflag = 2, Yflag = 1;
		break;
		case 3:
		Uflag = 2, Vflag = 0, Yflag = 1;
		break;
		default:
			;
	}

	if (Src.channels() != 3) { return NULL; }

	int height = Src.rows, width = Src.cols;
	unsigned char *yuvoutput = new unsigned char[(height*width) << 1]();

	int i = 0, j = 0, k = 0;

	for (i = 0, k = 0; i<height; ++i)
	{

		uchar *ptr = Src.ptr<uchar>(i);

		for (j = 0; j<width;)
		{
			int Ys[2];
			int U = 0, V = 0;

			int count = 0;
			while (count<2 && j<width)
			{
				int J = j * 3;

				int B = (int)ptr[J], G = (int)ptr[J + 1], R = (int)ptr[J + 2];
				Ys[count] = 0.114*B + 0.587*G + 0.299*R;
				U += 0.5*B - 0.332*G - 0.169*R + 128;
				V += -0.0813*B - 0.419*G + 0.5*R + 128;

				count++;
				++j;
			}


			U /= count;
			V /= count;

			yuvoutput[k + Yflag] = cv::saturate_cast<uchar>(Ys[0]);
			yuvoutput[k + Uflag] = cv::saturate_cast<uchar>(U);
			yuvoutput[k + Yflag + 2] = cv::saturate_cast<uchar>(Ys[1]);
			yuvoutput[k + Vflag] = cv::saturate_cast<uchar>(V);
			k += 4;


		}
	}

	return yuvoutput;
}