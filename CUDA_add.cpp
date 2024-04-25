#include<iostream>

// CUDA kernel for vector addition
__global__ void vectorAdd(int *a, int *b, int *c, int n) {
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < n) {
    c[i] = a[i] + b[i];
  }
}

int main() {
  int n = 10000;
  int *host_vectorA, *host_vectorB, *host_vectorC;
  int *device_vectorA, *device_vectorB, *device_vectorC;
  float elapsed_time;
  cudaEvent_t start, stop;
  size_t size = n * sizeof(int);

  // Allocate memory for host vectors
  host_vectorA = new int[size];
  host_vectorB = new int[size];
  host_vectorC = new int[size];

  // Initialize host vectors 
  for(int i=0; i<n; i++) {
    host_vectorA[i] = i;
    host_vectorB[i] = i;
  }

  // Allocate memory for device vectors
  cudaMalloc(&device_vectorA, size);
  cudaMalloc(&device_vectorB, size);
  cudaMalloc(&device_vectorC, size);

  //Copy host vectors to device vectors
  cudaMemcpy(device_vectorA, host_vectorA, size, cudaMemcpyHostToDevice);
  cudaMemcpy(device_vectorB, host_vectorB, size, cudaMemcpyHostToDevice);

  // Define block size and grid size
  int blockSize = 256;
  int gridSize = (n + blockSize - 1) / blockSize;

  //Create Cuda events
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  //Launch kernel
  cudaEventRecord(start);
  vectorAdd<<<gridSize, blockSize>>>(device_vectorA, device_vectorB, device_vectorC, n);
  cudaEventRecord(stop);
  cudaEventSynchronize(stop);

  //Copy device result vector to host result vector
  cudaMemcpy(host_vectorC, device_vectorC, size, cudaMemcpyDeviceToHost);

  //Evaluate result
  for(int j=0; j<n; j++) {
    if (host_vectorC[j] != 2*j) {
      std::cout<<"Error Evaluating Result!"<<std::endl;
      break;
    }
  }
  cudaEventElapsedTime(&elapsed_time, start, stop);
  std::cout<<"Elapsed Time: "<<elapsed_time<<"ms";

  // Free device memory
  cudaFree(device_vectorA); 
  cudaFree(device_vectorB);
  cudaFree(device_vectorC);

  // Free host memory 
  delete[] host_vectorA;
  delete[] host_vectorB;
  delete[] host_vectorC;

  return 0;
}