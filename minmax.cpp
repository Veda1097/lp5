
#include <iostream>
#include <omp.h>
#include <climits>

using namespace std;

void min_reduction(int list[], int n) {
  int min = INT_MAX;

  #pragma omp parallel for reduction(min: min)
  for (int i = 0; i < n; i++) {
    if (list[i] < min) {
      min = list[i];
    }
  }

  cout<<"Minimum Value: "<<min<<endl;
}

void max_reduction(int list[], int n) {
  int max = INT_MIN;

  #pragma omp parallel for reduction(max: max)
  for (int i=0; i<n; i++) {
    if (list[i] > max) {
      max = list[i];
    }
  }

  cout<<"Maximum value: "<<max<<endl;
}

void sum_reduction(int list[], int n) {
  int sum = 0;

  #pragma omp parallel for reduction(+: sum)
  for (int i=0; i<n; i++) {
	  sum += list[i];
  }

  cout<<"Sum: "<<sum<<endl;
}

void avg_reduction(int list[], int n) {
  int sum = 0;

  #pragma omp parallel for reduction(+: sum)
  for (int i=0; i<n; i++) {
	  sum += list[i];
  }

  cout<<"Average: "<<(double)sum/(n-1)<<endl;
}

int main() {
  int *list, n;

  cout<<"Enter No of Elements: ";
  cin>>n;
  list = new int[n];

  cout<<"Enter Elements:"<<endl;
  for(int i=0; i<n; i++) {
    cin>>list[i];
  }

  min_reduction(list, n);
  max_reduction(list, n);
  sum_reduction(list, n);
  avg_reduction(list, n);
}
