#include "../../../lib/c4lib/c4lib.h"


// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int* arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temp arrays
    int *L = c4_malloc(n1);
    int *R = c4_malloc(n2);

    int i = 0;
    int j = 0;
    // Copy data to temp arrays L[] and R[]
    while ( i < n1 ){
        L[i] = arr[l + i];
        i=i+1;
    }
    while ( j < n2 ){
        R[j] = arr[m + 1 + j];
        j=j+1;
    }

    // Merge the temp arrays back into arr[l..r]

    // Initial index of first subarray
    i = 0;

    // Initial index of second subarray
    j = 0;

    // Initial index of merged subarray
    int k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i=i+1;
        }
        else {
            arr[k] = R[j];
            j=j+1;
        }
        k=k+1;
    }

    // Copy the remaining elements of
    // L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i=i+1;
        k=k+1;
    }

    // Copy the remaining elements of
    // R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j=j+1;
        k=k+1;
    }

    c4_free(L);
    c4_free(R);
}

// l is for left index and r is
// right index of the sub-array
// of arr to be sorted */
void mergeSort(int* arr,int l,int r){
    if(l>=r){
        return;//returns recursively
    }
    int m = (l+r-1) >> 1;
    mergeSort(arr,l,m);
    mergeSort(arr,m+1,r);
    merge(arr,l,m,r);
}

// UTILITY FUNCTIONS
// Function to print an array
void printArray(int* A, int size)
{
    int i = 0;
    while ( i < size ){
        if(i > 0){
            c4_print_str(" ");
        }
        c4_print_int(A[i]);
        i=i+1;
    }
}

// Driver code
int main(void)
{
    int* arr = c4_malloc(6);

    arr[0] = 12;
    arr[1] = 11;
    arr[2] = 13;
    arr[3] = 5;
    arr[4] = 6;
    arr[5] = 7;

    int arr_size = 6;

    c4_print_str( "Given array is\n");
    printArray(arr, arr_size);

    mergeSort(arr, 0, arr_size - 1);

    c4_print_str( "\nSorted array is\n");
    printArray(arr, arr_size);
    return 0;
}