#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// swaps two values using a temp value
void swap (int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

// heapify a subtree with size of n
void heapify(int arr[], int N, int i)
{
	// declaration for biggest, left, and right
	int biggest = i, l = 2 * i + 1, r = 2 * i + 2;

	// child is larger
	if(l < N && arr[l] > arr[biggest])
		biggest = l;

	// right child is larger
	if(r < N && arr[r] > arr[biggest])
		biggest = r;

	// swap and recall heapify if largest is not root
	if (biggest != i)
	{
		swap(&arr[i], &arr[biggest]);

		heapify(arr, N, biggest);
	}

}


// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{
	int i;

	// builds a max heap
	for(i = n / 2 - 1; i >= 0; i--)
		heapify(arr, n, i);

	// heap sort
	for(i = n - 1; i >= 0; i--)
	{
		swap(&arr[0], &arr[i]);

		// heapify to get largest element at root
		heapify(arr, i, 0);
	}
}

void merge(int pData[], int l, int m, int r)
{
    int i, j , k;
    int n1 = m - l + 1;
    int n2 = r - m;

	// create temp arrays and update memory allocated
    int *L = (int*)malloc(n1*sizeof(int));
	int *R = (int*)malloc(n2*sizeof(int));

	extraMemoryAllocated += (((r - l) + 1) * sizeof(int));

	// copy data to temp arrays L and R
    for (i = 0; i < n1; i++)
        L[i] = pData[l + i];
    for (j = 0; j < n2; j++)
        R[j] = pData[m + 1 + j];

	// initialize index of subarrays
    i = 0;
    j = 0;
    k = l;

	// merge temp arrays back into pData
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            pData[k] = L[i];
            i++;
        }
        else {
            pData[k] = R[j];
            j++;
        }
        k++;
    }
  
	// copy any remaining elements of L
    while (i < n1) {
        pData[k] = L[i];
        i++;
        k++;
    }
  
	// copy any remaining elements of R
    while (j < n2) {
        pData[k] = R[j];
        j++;
        k++;
    }
	free(L);
	free(R);
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
    if (l < r)
	{
		// calculates mid point
        int m = l + (r - l) / 2;

		// sort first and second halves
        mergeSort(pData, l, m);
        mergeSort(pData, m + 1, r);

        merge(pData, l, m, r);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		if(i>=dataSz){
			printf("\n\n");
			return;
		}
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}