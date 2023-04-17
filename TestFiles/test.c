#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    printf("Hello world \n");
    int arr1[40000];
    int arr2[40000];
    int size = 40000;
    #pragma omp parallel for num_threads(5)
    for(int i=0; i<size;i++){
        arr1[i] = i;
        printf("\n %d", arr1[i]);
    }
    #pragma omp barrier
    #pragma omp parallel for nowait
    for(int i=0; i<size;i++){
        arr2[i] = i;
        printf("\n %d", arr2[i]);
    }
    
}