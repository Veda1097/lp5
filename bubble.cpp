
#include<iostream>
#include<omp.h>
#include<chrono>

using namespace std;

void swap(int &a, int &b) {
    a = a+b;
    b = a-b;
    a = a-b;
}

void serial_bubble(int *arr, int n) {
    for (int i=0; i<n-1; i++) {
        bool swapped = false;
        for (int j=0; j<n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                swap(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        if(!swapped) {
            break;
        }
    }
}

void parallel_bubble(int *arr, int n) {

    for (int i=0; i<n-1; i++) {
        bool swapped = false;

        #pragma omp parallel for shared(arr,swapped)
        for (int j=0; j<n-i-1; j++) {
            #pragma omp critical
            {
                if (arr[j] > arr[j+1]) {
                    swap(arr[j], arr[j+1]);
                    swapped = true;
                }
            }
        }
        
        #pragma omp barrier
        if(!swapped) {
            break;
        }
        
        
    }
}

void print_list(int *arr, int n) {
    for(int i=0; i<n; i++) {
        cout<<arr[i]<<" ";
    }
}

int main() {
    int *list1, *list2, e, n;
    cout<<"Enter No of Elements: ";
    cin>>n;
    list1 = list2 = new int[n];

    cout<<"Enter Elements: "<<endl;
    for(int i=0; i<n; i++) {
   	    cin>>e;
        list1[i] = list2[i] = e;
    }

    auto start_s = chrono::high_resolution_clock::now();
    serial_bubble(list1, n);
    auto end_s = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_s = end_s - start_s;
    
    auto start_p = chrono::high_resolution_clock::now();
    parallel_bubble(list2, n);
    auto end_p = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> time_p = end_p - start_p;

    cout<<"Serial Bubble Sort: ";
    print_list(list1, n);
    cout<<"\nTime: "<<time_s.count()<<"ms"<<endl;
    cout<<"Parallel Bubble Sort: ";
    print_list(list2, n);
    cout<<"\nTime: "<<time_p.count()<<"ms"<<endl;

    return 0;
}
