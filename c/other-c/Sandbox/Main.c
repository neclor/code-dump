#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "Sort.h"


int* make_shuffled_array(size_t length) {
	if(length == 0) return NULL;

	int *arr = (int*)malloc(length * sizeof(int));
	if(!arr) return NULL;

	for(size_t i = 0; i < length; i++) {
		arr[i] = (int)i;
	}

	for(size_t i = length - 1; i > 0; i--) {
		size_t j = (size_t)(rand() % (i + 1));

		int tmp = arr[i];
		arr[i] = arr[j];
		arr[j] = tmp;
	}

	return arr;
}







int compare(const void *array, size_t i, size_t j) {
	int* ar = array;

	if (ar[i] < ar[j]) return -1;
	else if (ar[i] == ar[j]) return 0;
	else return 1;
}


void swap(void *array, size_t i, size_t j) {
	int* ar = array;

	int tmp = ar[i];
	ar[i] = ar[j];
	ar[j] = tmp;
}



int compare_prt(const void *array,size_t i,void *key) {
	int* ar = array;
	int k = *(int*)key;

	if(ar[i] < k) return -1;
	else if(ar[i] == k) return 0;
	else return 1;
}


size_t binarySearchLow(void *array,size_t length,void *key,int (*compare)(const void *array,size_t i,void *key)) {
	if(length < 2) return 0;
	size_t result = 0;
	size_t left = 0;
	size_t right = length;
	while(left < right) {
		size_t middle = left + (right - left) / 2;
		int comp = compare(array,middle,key);
		if(comp < 0) {
			left = middle + 1;
		} else {
			right = middle;
			if(comp == 0) {
				result = middle;
			}
		}
	}
	return result;
}

size_t binarySearchHigh(void *array,size_t length,void *key,int (*compare)(const void *array,size_t i,void *key)) {
	if(length < 2) return 0;
	size_t result = 0;
	size_t left = 0;
	size_t right = length;
	while(left < right) {
		size_t middle = left + (right - left) / 2;
		int comp = compare(array,middle,key);
		if(comp > 0) {
			right = middle;
		} else {
			left = middle + 1;
			if(comp == 0) {
				result = middle;
			}
		}
	}
	return result;
}





int main() {
	srand((unsigned int)time(NULL));
	

	//int key = 1;
	//int search[4] = {1, 1, 1, 1};

	//printf("%llu \n",binarySearchLow(search,4,&key,compare_prt));
	//printf("%llu \n",binarySearchHigh(search,4,&key,compare_prt));



	







	//return;

	int len = 10;

	for(int a = 0; a < 1000; a++) {

		int *arr = make_shuffled_array(len);

		for(size_t i = 0; i < len; i++) {
			printf("%d, ",arr[i]);
		}
		printf("\n");

		sort(arr,len,compare,swap);

		for(size_t i = 0; i < len - 1; i++) {
			printf("%d, ",arr[i]);
			if(arr[i] > arr[i + 1]) {
				printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
				printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");


			}
		}
		printf("\n");

		free(arr);
		printf("\n");
	}



	
}
