#include <iostream>
#include <cstdlib>

// CUDA kernel for vector addition
__global__ void vectorAdd(int *a, int *b, int *result, int n) {
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    if (tid < n) {
        result[tid] = a[tid] + b[tid];
    }
}

int main() {
    int *a, *b, *c;
    int *a_dev, *b_dev, *c_dev;
    int n = 1 << 24; // Example large number

    a = new int[n];
    b = new int[n];
    c = new int[n];
    int size = n * sizeof(int);

    cudaMalloc(&a_dev, size);
    cudaMalloc(&b_dev, size);
    cudaMalloc(&c_dev, size);

    // Initialize arrays with values
    for (int i = 0; i < n; i++) {
        a[i] = 1;
        b[i] = 2;
    }

    // Copy data from host to device
    cudaMemcpy(a_dev, a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(b_dev, b, size, cudaMemcpyHostToDevice);

    int threads = 1024;
    int blocks = (n + threads - 1) / threads;

    // Execute kernel on GPU
    vectorAdd<<<blocks, threads>>>(a_dev, b_dev, c_dev, n);

    // Copy result back to host
    cudaMemcpy(c, c_dev, size, cudaMemcpyDeviceToHost);

    // Verify the result
    bool success = true;
    for (int i = 0; i < n; i++) {
        if (c[i] != a[i] + b[i]) {
            success = false;
            std::cout << "Error at position " << i << std::endl;
            break;
        }
    }

    if (success) {
        std::cout << "Vector addition successful!" << std::endl;
    }

    // Free memory
    cudaFree(a_dev);
    cudaFree(b_dev);
    cudaFree(c_dev);
    delete[] a;
    delete[] b;
    delete[] c;

    return 0;
}

// IF LOCALLY :
// Command to compile -> nvcc -o vectorAddition vectorAddition.cu
// Command to execute -> ./vectorAddition

// IF IN COLAB :
// Command to first install nvcc_plugin -> !pip install git+https://github.com/afnan47/cuda.git
// Then -> %load_ext nvcc_plugin
// Then write %%cu at the top and then copy the above entire code and run it.