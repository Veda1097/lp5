
#include<iostream>

__global__ void matrixMultiply(float *matrixA, float *matrixB, float *matrixC, int n) {
  int row = blockIdx.y * blockDim.y + threadIdx.y;
  int col = blockIdx.x * blockDim.x + threadIdx.x;
  float sum = 0;

  if (row < n && col < n) {
    for (int i=0; i<n; ++i) {
      sum += matrixA[row * n + i] * matrixB[i * n + col];
    }
    matrixC[row * n + col] = sum;
  }
}

int main() {
  int n = 1024;
  int nxn = n*n;
  float *host_matrixA, *host_matrixB, *host_matrixC;
  float *device_matrixA, *device_matrixB, *device_matrixC;
  float elapsed_time;
  cudaEvent_t start, stop;
  size_t size = nxn * sizeof(float);

  //Allocate CPU memory for matrices
  host_matrixA = new float[size];
  host_matrixB = new float[size];
  host_matrixC = new float[size];

  //Initialize matrices
  for(int i=0; i<nxn; ++i) {
    host_matrixA[i] = i % n;
    host_matrixB[i] = i % n;
  }

  //Allocate GPU memory for matrices
  cudaMalloc(&device_matrixA, size);
  cudaMalloc(&device_matrixB, size);
  cudaMalloc(&device_matrixC, size);

  //Copy input data to device
  cudaMemcpy(device_matrixA, host_matrixA, size, cudaMemcpyHostToDevice);
  cudaMemcpy(device_matrixB, host_matrixB, size, cudaMemcpyHostToDevice);

  //Set kernel launch configuration
  dim3 blockSize(128, 128);
  dim3 gridSize((n + blockSize.x - 1) / blockSize.x, (n + blockSize.y - 1) / blockSize.y);
  
  //Create Cuda events
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  //Launch kernel
  cudaEventRecord(start);
  matrixMultiply<<<gridSize, blockSize>>>(device_matrixA, device_matrixB, device_matrixC, n); 
  cudaEventRecord(stop); 
  cudaEventSynchronize(stop); 
  
  //Copy output data to host
  cudaMemcpy(host_matrixC, device_matrixC, size, cudaMemcpyDeviceToHost);

  //Print elapsed time
  cudaEventElapsedTime(&elapsed_time, start, stop);
  std::cout<<"Elapsed Time: "<<elapsed_time<<"ms";

  //Free device memory
  cudaFree(device_matrixA); 
  cudaFree(device_matrixB);
  cudaFree(device_matrixC);
  
  //Free host memory 
  delete[] host_matrixA;
  delete[] host_matrixB;
  delete[] host_matrixC;
  
  return 0;
}
