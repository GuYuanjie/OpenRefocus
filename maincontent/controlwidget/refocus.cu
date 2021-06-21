#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <opencv2\opencv.hpp>
#include "cudarefocus.h"
//#include "cudaRefocus.h"

using namespace std;
using namespace cv;



__device__ int index(int n, int m)
{
	if (0 <= n && n < m)
		return n;
	else if (n < 0)
		return 0;
	else
		return m - 1;
}

__global__ void refocusKernel(uchar* pLFData, uchar* pRefocusLFData, uchar* pRefocusData,float alpha,
                              int imgH, int imgW, int radius)
{
    const int tidx=blockDim.x*blockIdx.x+threadIdx.x;
    const int tidy=blockDim.y*blockIdx.y+threadIdx.y;

    if(tidx == 0 && tidy == 0)
    {
       // printf("KKKKKennel\n");

    }
    int diameter=2*radius+1;
    int heightLF=imgH*diameter;
    int widthLF=imgW*diameter;
    int windowSize=diameter*diameter;
    if (tidx<imgW && tidy<imgH)
    {
        float output_color_B = 0;
		float output_color_G = 0;
		float output_color_R = 0;

		for (int j = -radius; j < radius + 1; j++)
		{
			for (int i = -radius; i < radius + 1; i++)
			{
				float x_ind = i*(1 - 1 / alpha) + tidx;  
				float y_ind = j*(1 - 1 / alpha) + tidy;

                int x_floor = floorf(x_ind);
                int y_floor = floorf(y_ind);

				int x_1 = index(x_floor, imgW); 
				int y_1 = index(y_floor, imgH);
				int x_2 = index(x_floor + 1, imgW); 
				int y_2 = index(y_floor + 1, imgH);

				float x_1_w = 1 - (x_ind - x_floor); 
				float x_2_w = 1 - x_1_w;
				float y_1_w = 1 - (y_ind - y_floor);
				float y_2_w = 1 - y_1_w;

				
				int x_1_index = i + radius + (x_1) * diameter;
				int y_1_index = j + radius + (y_1) * diameter;
				int x_2_index = i + radius + (x_2) * diameter;
				int y_2_index = j + radius + (y_2) * diameter;

				
				float interp_color_B = y_1_w * x_1_w * pLFData[y_1_index * widthLF * 3 + x_1_index * 3 + 0 ] +
									y_2_w * x_1_w * pLFData[y_1_index * widthLF * 3 + x_2_index * 3 + 0 ] +
									y_1_w * x_2_w * pLFData[y_2_index * widthLF * 3 + x_1_index * 3 + 0 ] +
									y_2_w * x_2_w * pLFData[y_2_index * widthLF * 3 + x_2_index * 3 + 0 ];

				float interp_color_G = y_1_w * x_1_w * pLFData[y_1_index * widthLF * 3 + x_1_index * 3 + 1] +
									y_2_w * x_1_w * pLFData[y_1_index * widthLF * 3 + x_2_index * 3 + 1] +
									y_1_w * x_2_w * pLFData[y_2_index * widthLF * 3 + x_1_index * 3 + 1] +
									y_2_w * x_2_w * pLFData[y_2_index * widthLF * 3 + x_2_index * 3 + 1];

				float interp_color_R = y_1_w * x_1_w * pLFData[y_1_index * widthLF * 3 + x_1_index * 3 + 2] +
									y_2_w * x_1_w * pLFData[y_1_index * widthLF * 3 + x_2_index * 3 + 2] +
									y_1_w * x_2_w * pLFData[y_2_index * widthLF * 3 + x_1_index * 3 + 2] +
									y_2_w * x_2_w * pLFData[y_2_index * widthLF * 3 + x_2_index * 3 + 2];

				// CORRESPONDENCE ANALYSIS
				
				int x_index_remap = i + radius + (tidx) * diameter;
				int y_index_remap = j + radius + (tidy) * diameter;
				
				pRefocusLFData[x_index_remap * 3 + y_index_remap * widthLF * 3 + 0] = interp_color_B;
				pRefocusLFData[x_index_remap * 3 + y_index_remap * widthLF * 3 + 1] = interp_color_G;
				pRefocusLFData[x_index_remap * 3 + y_index_remap * widthLF * 3 + 2] = interp_color_R;

				// DEFOCUS ANALYSIS
				output_color_R = interp_color_R + output_color_R;
				output_color_G = interp_color_G + output_color_G;
				output_color_B = interp_color_B + output_color_B;
			}
		}
		
		pRefocusData[tidx * 3 + tidy * imgW * 3 + 0] = output_color_B / windowSize;
		pRefocusData[tidx * 3 + tidy * imgW * 3 + 1] = output_color_G / windowSize;
		pRefocusData[tidx * 3 + tidy * imgW * 3 + 2] = output_color_R / windowSize;
    }
}

void myprint(uint64_t* avail, uint64_t* total )
{

    cudaMemGetInfo(avail, total);

}




void cudaRefocus(cv::Mat &LFRemapImg, cv::Mat &refocusImgLF, cv::Mat &refocusImg, float alpha,
                 bool& isFirst, int imgHeightLF, int imgWidthLF, int imgHeight, int imgWidth,
                 int radius, uchar *pLFImg, uchar *pRefocusLFImg, uchar *pRefocusImg,cudaErr * Err)
{
    const char* errStr;

    int sizeLF=imgHeightLF*imgWidthLF*sizeof(uchar3);
    int size=imgHeight*imgWidth*sizeof(uchar3);
    cudaError err;


    uchar* p1, *p2, *p3;

    cudaMalloc((void**)&p1, sizeLF);
    cudaMalloc((void**)&p2, sizeLF);
    cudaMalloc((void**)&p3, size);

    cudaMemcpy(p1, LFRemapImg.data, sizeLF,cudaMemcpyHostToDevice);


    dim3 block(8,8);
    dim3 grid( (imgWidth+block.x-1)/block.x, (imgHeight+block.y-1)/block.y);
    refocusKernel<<<grid, block>>>(p1, p2, p3, alpha, imgHeight, imgWidth, radius);
    cudaThreadSynchronize();


    cudaMemcpy(refocusImg.data, p3, imgWidth*imgHeight*sizeof(uchar3), cudaMemcpyDeviceToHost);

    cudaFree(p1);
    cudaFree(p2);
    cudaFree(p3);

    err=cudaGetLastError();
    errStr = cudaGetErrorString(err);
    Err->cudaError = errStr;
    Err->errNum = err;
}

void cudaRefocus_bk(cv::Mat &LFRemapImg, cv::Mat &refocusImgLF, cv::Mat &refocusImg, float alpha,
                 bool& isFirst, int imgHeightLF, int imgWidthLF, int imgHeight, int imgWidth,
                 int radius, uchar *pLFImg, uchar *pRefocusLFImg, uchar *pRefocusImg,cudaErr * Err)
{
    const char* errStr;

    int sizeLF=imgHeightLF*imgWidthLF*sizeof(uchar3);
    int size=imgHeight*imgWidth*sizeof(uchar3);
    cudaError err;

    if (true==isFirst)
    {
        err=cudaMalloc((void**)&pLFImg,sizeLF);
        err=cudaMalloc((void**)&pRefocusLFImg, sizeLF);
        err=cudaMalloc((void**)&pRefocusImg, size);
        err=cudaMemcpy(pLFImg, LFRemapImg.data, sizeLF,cudaMemcpyHostToDevice);
        isFirst=false;
    }



    //memcpy
    err = cudaMemcpy(pLFImg, LFRemapImg.data, sizeLF, cudaMemcpyHostToDevice);


    dim3 block(8,8);
    dim3 grid( (imgWidth+block.x-1)/block.x, (imgHeight+block.y-1)/block.y);
    refocusKernel<<<grid, block>>>(pLFImg, pRefocusLFImg, pRefocusImg,alpha, imgHeight, imgWidth, radius);
    cudaThreadSynchronize();

    if (err!=cudaSuccess)
    {
        cout<<" err="<<err<<errStr<<endl;
    }

    err=cudaMemcpy(refocusImg.data, pRefocusImg, imgWidth*imgHeight*sizeof(uchar3), cudaMemcpyDeviceToHost);

    err=cudaGetLastError();
    errStr = cudaGetErrorString(err);
    Err->cudaError = errStr;
    Err->errNum = err;
}
