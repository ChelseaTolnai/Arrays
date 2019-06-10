#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct Array {
  int capacity;  // How many elements can this array hold?
  int count;  // How many states does the array currently hold?
  char **elements;  // The string elements contained in the array
} Array;


/************************************
 *
 *   CREATE, DESTROY, RESIZE FUNCTIONS
 *
 ************************************/

/*****
 * Allocate memory for a new array
 *****/
Array *create_array (int capacity) {
  // Allocate memory for the Array struct
  Array *arr = malloc(sizeof(Array));
  // Set initial values for capacity and count
  arr->capacity = capacity;
  arr->count = 0;
  // Allocate memory for elements
  arr->elements =  malloc(capacity * sizeof(char*));

  return arr;
}


/*****
 * Free memory for an array and all of its stored elements
 *****/
void destroy_array(Array *arr) {

  // Free all elements
  for (int i=0; i < arr->count; i++){
    free(arr->elements[i]);
  }
  free(arr->elements);
  // Free array
  free(arr);
}

/*****
 * Create a new elements array with double capacity and copy elements
 * from old to new
 *****/
void resize_array(Array *arr) {
  // Create a new element storage with double capacity
  int double_capacity = arr->capacity * 2;
  char **element_storage =  malloc(double_capacity * sizeof(char*));

  // Copy elements into the new storage
  for (int i=0; i < arr->count; i++) {
    element_storage[i] = arr->elements[i];
  }

  // Free the old elements array (but NOT the strings they point to)
  free(arr->elements);

  // Update the elements and capacity to new values
  arr->elements = element_storage;
  arr->capacity = double_capacity;

}



/************************************
 *
 *   ARRAY FUNCTIONS
 *
 ************************************/

/*****
 * Return the element in the array at the given index.
 *
 * Throw an error if the index is out of range.
 *****/
char *arr_read(Array *arr, int index) {

  // Throw an error if the index is greater or equal to than the current count
  if (index >= arr->count) {
    perror("index out of range");
    return NULL;
  } else {
  // Otherwise, return the element at the given index
    return arr->elements[index];
  }
}


/*****
 * Insert an element to the array at the given index
 *
 * Store the VALUE of the given string, not the REFERENCE
 *****/
void arr_insert(Array *arr, char *element, int index) {

  // Throw an error if the index is greater than the current count
  if (index > arr->count) {
    perror("index out of range");
    return;
  } else {
  // Resize the array if the number of elements is over capacity

    if (arr->count >= arr->capacity){
      resize_array(arr);
    } 

  // Move every element after the insert index to the right one position
    for (int i=arr->count; i > index; i--) {
      arr->elements[i] = arr->elements[i-1];
    }
  // Copy the element (hint: use `strdup()`) and add it to the array
    arr->elements[index] = strdup(element);
  // Increment count by 1
    arr->count += 1;
  }
}

/*****
 * Append an element to the end of the array
 *****/
void arr_append(Array *arr, char *element) {

  // Resize the array if the number of elements is over capacity
  // or throw an error if resize isn't implemented yet.
  if (arr->count >= arr->capacity){
    resize_array(arr);
  }
  // Copy the element and add it to the end of the array
  arr->elements[arr->count] = strdup(element);
  // Increment count by 1
  arr->count += 1;
}

/*****
 * Remove the first occurence of the given element from the array,
 * then shift every element after that occurence to the left one slot.
 *
 * Throw an error if the value is not found.
 *****/
void arr_remove(Array *arr, char *element) {

  // Search for the first occurence of the element and remove it.
  for (int i=0 ; i < arr->count ; i++) {
    if (strcmp(arr->elements[i], element) == 0) {
      // Don't forget to free its memory!
      free(arr->elements[i]);
      // Shift over every element after the removed element to the left one position
      for (int j=i ; j < arr->count-1 ; j++) {
        arr->elements[j] = arr->elements[j+1];
      }
      // Decrement count by 1
      arr->count -= 1;
      break;
    }
  }
}


/*****
 * Utility function to print an array.
 *****/
void arr_print(Array *arr) {
  printf("[");
  for (int i = 0 ; i < arr->count ; i++) {
    printf("%s", arr->elements[i]);
    if (i != arr->count - 1) {
      printf(",");
    }
  }
  printf("]\n");
}


/************************************
 *
 *   STRETCH -- ARRAY FUNCTIONS
 *
 ************************************/

void arr_clear(Array *arr) {
  for (int i=0; i < arr->count; i++){
    free(arr->elements[i]);
  }
  free(arr->elements);
  arr->count = 0;
}

Array *arr_copy(Array *arr) {
  Array *arr_dup = create_array(arr->capacity);
  arr_dup->count = arr->count;
  for (int i=0; i < arr_dup->count; i++) {
    arr_dup->elements[i] = strdup(arr->elements[i]);
  }
  return arr_dup;
}

void arr_extend(Array *arr, Array *arr2) {
  for (int i=0; i < arr2->count; i++) {
    arr_append(arr, arr2->elements[i]);
  }
}

int arr_index(Array *arr, char *element) {
  for (int i=0 ; i < arr->count ; i++) {
    if (strcmp(arr->elements[i], element) == 0) {
      return i;
    }
  }
  return -1;
}

char *arr_pop(Array *arr, int index) {
  if ((index >= arr->count) || (-index > arr->count) ) {
    perror("index out of range");
    return NULL;
  } else {
    int i = index >= 0 ? index : arr->count + index;

    char *element = arr->elements[i];
    free(arr->elements[i]);
    for (int j=i ; j < arr->count-1 ; j++) {
      arr->elements[j] = arr->elements[j+1];
    }
    arr->count -= 1;
    return element;
  }
}

void arr_reverse(Array *arr) {
  Array *arr_temp = arr_copy(arr);
  for (int i=0; i < arr->count; i++) {
    arr->elements[i] = strdup(arr_temp->elements[arr->count-1-i]);
  }
  destroy_array(arr_temp);
}


#ifndef TESTING
int main(void)
{

  Array *arr = create_array(1);

  arr_insert(arr, "STRING1", 0);
  arr_append(arr, "STRING4");
  arr_insert(arr, "STRING2", 0);
  arr_insert(arr, "STRING3", 1);
  arr_print(arr);
  arr_remove(arr, "STRING3");
  arr_print(arr);

  Array *arr_dup = arr_copy(arr);
  printf("Array: ");
  arr_print(arr);
  printf("Array Copy: ");
  arr_print(arr_dup);

  arr_clear(arr_dup);
  printf("Array Copy Clear: ");
  arr_print(arr_dup);

  Array *arr_2 = arr_copy(arr);
  printf("Array: ");
  arr_print(arr);
  printf("Array Copy: ");
  arr_print(arr_2);
  arr_extend(arr, arr_2);
  printf("Array Extend Array Copy: ");
  arr_print(arr);

  int index_exists = arr_index(arr, "STRING1");
  printf("Index of STRING1 should be 1: %d\n", index_exists);
  int index_no_exists = arr_index(arr, "STRING3");
  printf("Index of STRING3 should be -1: %d\n", index_no_exists);

  char* pop_out_range_neg = arr_pop(arr, -8);
  printf("%s\n", pop_out_range_neg);
  char*  pop_out_range_pos = arr_pop(arr, 10);
  printf("%s\n", pop_out_range_pos);
  char*  pop_in_range_mid = arr_pop(arr, 3);
  printf("%s\n", pop_in_range_mid); 
  char*  pop_in_range_last = arr_pop(arr, (arr->count-1));
  printf("%s\n", pop_in_range_last);   
  arr_pop(arr, 0);
  arr_pop(arr, (arr->count-1));
  arr_insert(arr, "STRING2", 1);
  printf("Array after Pops: ");
  arr_print(arr);

  arr_reverse(arr);
  printf("Array after Reverse: ");
  arr_print(arr);

  destroy_array(arr);

  return 0;
}
#endif
