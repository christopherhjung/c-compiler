#include "../../../lib/c4lib/c4lib.h"

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
int partition (int* arr, int low, int high)
{
    int pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element
    int j = low;
    while (j <= high- 1)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot)
        {
            i=i+1;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }

        j=j+1;
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
void quickSort(int* arr, int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

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

    quickSort(arr, 0, arr_size - 1);

    c4_print_str( "\nSorted array is\n");
    printArray(arr, arr_size);
    return 0;
}