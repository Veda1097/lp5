
#include <iostream>
#include <omp.h>
#include <chrono>

using namespace std;

void merge(int *arr,int first, int mid, int last){
    int i=first,j=mid+1,k=0;
    int temp[last+1];
    
    while(i<=mid || j<=last){
        if(i<=mid && j<=last){
            if(arr[i]<arr[j]){
                temp[k++]=arr[i++];
            }
            else{
                temp[k++]=arr[j++];
            }
        }
        else if(i<=mid){
            temp[k++]=arr[i++];
        }
        else if(j<=last){
            temp[k++]=arr[j++];
        }
    }
    
    for(int i=first,j=0; i<=last; i++,j++){
        arr[i]=temp[j];
    }
}

void merge_sort(int *arr, int first, int last){
    if(first<last){
        int mid=(first+last)/2;
        
        #pragma omp parallel sections
        {
            #pragma omp section
            {
                merge_sort(arr,first,mid);
            }
            
            #pragma omp section
            {
                merge_sort(arr,mid+1,last);
            }
        }
        
        merge(arr,first,mid,last);
    }
    
}

void print(int *arr, int n){
    for(int i=0;i<n;i++){
        cout<<arr[i]<<" ";
    }
    
}

int main(){
    int n;
    cin>>n;
    
    int *arr=new int[n];
    
    for(int i=0;i<n;i++){
        cin>>arr[i];
    }
    
    cout<<"Merge sort: ";
    auto start_p= chrono::high_resolution_clock::now();
    merge_sort(arr,0,n-1);
    auto end_p= chrono::high_resolution_clock::now();
    chrono::duration<double,milli> time_p = end_p - start_p;
    print(arr,n);
    cout<<endl;
    cout<<"Time: "<<time_p.count()<<endl;
}
