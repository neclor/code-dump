#include <stdlib.h>
#include <stdio.h>

//#include "Sort.h"

/* ------------------------------------------------------------------------- *

   * Helper function: Merge two sorted halves of the indices array using a

   * temporary array (temp)

 * ------------------------------------------------------------------------- */

static void merge(size_t *indices,size_t *temp,size_t left,size_t mid,size_t right,

				  void *array,int (*compare)(const void *,size_t,size_t))

{

	size_t i = left; // Start index of the left half

	size_t j = mid + 1; // Start index of the right half

	size_t k = left;

	// While there are elements in both the left and right halves

	while(i <= mid && j <= right)

	{

		// If the element on the left is smaller or equal, it is in the correct place

		if(compare(array,indices[i],indices[j]) <= 0)

		{

			temp[k] = indices[i];

			i++;
		}

		else

		{

			temp[k] = indices[j];

			j++;
		}

		k++;
	}

	while(i <= mid)

	{

		temp[k] = indices[i];

		i++;

		k++;
	}

	while(j <= right)

	{

		temp[k] = indices[j];

		j++;

		k++;
	}

	for(size_t idx = left; idx <= right; idx++)

	{

		indices[idx] = temp[idx];
	}
}

/* ------------------------------------------------------------------------- *

 * Helper function: Recursive array division

 * ------------------------------------------------------------------------- */

static void mergeSortRecursive(size_t *indices,size_t *temp,size_t left,size_t right,

							   void *array,int (*compare)(const void *,size_t,size_t))

{

	// Base case: proceed if the array partition has more than 1 element

	if(left < right)

	{

		// Calculate mid in a way that avoids potential overflow of size_t

		size_t mid = left + (right - left) / 2;

		// Recursively sort the left half

		mergeSortRecursive(indices,temp,left,mid,array,compare);

		// Recursively sort the right half

		mergeSortRecursive(indices,temp,mid + 1,right,array,compare);

		// Merge the two sorted halves using the temp array

		merge(indices,temp,left,mid,right,array,compare);
	}
}

/* ------------------------------------------------------------------------- *

 * Main public function, matching the contract defined in Sort.h

 * ------------------------------------------------------------------------- */

static void sort(void *array,size_t length,

		  int (*compare)(const void *,size_t i,size_t j),

		  void (*swap)(void *array,size_t i,size_t j))

{

	// An array of 0 or 1 element is already considered sorted

	if(!array || !compare || !swap || length < 2)
	{

		return;
	}

	size_t *indices = malloc(length * sizeof(size_t));

	size_t *temp = malloc(length * sizeof(size_t));

	if(indices == NULL || temp == NULL)

	{

		if(indices != NULL)
			free(indices);

		if(temp != NULL)
			free(temp);

		return;
	}

	for(size_t i = 0; i < length; i++)

	{

		indices[i] = i;
	}

	// Start the recursive sorting from the first to the last index

	mergeSortRecursive(indices,temp,0,length - 1,array,compare);



	for(size_t i = 0; i < length; i++)
	{
		printf("temp[%d] = %d, ", indices[i], i);
		temp[indices[i]] = i;
		
	}
	printf("\n");
	
	for(size_t i = 0; i < length; i++)
	{
		printf("%d. ", temp[i]);
		while(temp[i] != i)
		{
			size_t target = temp[i];
			swap(array,i,target);
			size_t tempIndex = temp[i];
			temp[i] = temp[target];
			temp[target] = tempIndex;
		}
	}

	printf("\n");

	free(indices);

	free(temp);
}
