#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define SIZE 10000

void swap(long* a, long* b) {
  long tmp = *a;
  *a = *b;
  *b = tmp;
}

long* lomuto_branchfree(long* first, long* last) {
  assert(first <= last);
  if (last - first < 2)  
    return first;
  --last;
  if (*first > *last)
    swap(first, last);
  long* pivot_pos = first;
  long pivot = *first;
  do {
    ++first;
  } while(*first < pivot);
  assert(first <= last);
  for(long* read = first + 1; read < last; ++read) {
    long x = *read;
    long smaller = -((int)(x < pivot));
    long delta = smaller & (read - first);
    first[delta] = *first;
    read[-delta] = x;
    first -= smaller;
  }
  assert(*first >= pivot);
  --first;
  *pivot_pos = *first;
  *first = pivot;
  return first;
}

long* lomuto_branch_busy(long* left, long* right) {
  if (right - left < 2) 
    return left;
  long* pivot_pos = left;
  long pivot = *left++;
  for (long* read = left; read < right; ++read) {
    if (*read < pivot) {
      swap(read, left++);
    }
  }
  swap(--left, pivot_pos);
  return left;
}

long* le_hoare(long* first, long* last) {
  if (last - first)
    return first;
  --last;
  if (*first > *last)
    swap(first, last);
  long* pivot_pos = first;
  long pivot = *first;
  for(;;) {
    ++first;
    long f = *first;
    while (f < pivot)
      f = *++first;
    long l = *last;
    while (pivot < l)
      l = *--last;
    if (first >= last)
      break;
    *first = l;
    *last = f;
    --last;
  }
  --first;
  swap(first, pivot_pos);
  return first;
}

void quickSortDeHoare(long* first, long* last) {
  if (first < last) {
    long* pivot = le_hoare(first, last);
    quickSortDeHoare(first, pivot);
    quickSortDeHoare(pivot + 1, last);
  }
}

void quickSortLomutoBusy(long* left, long* right) {
  if (left < right) {
    long* pivot = lomuto_branch_busy(left, right);
    quickSortLomutoBusy(left, pivot);
    quickSortLomutoBusy(pivot + 1, right);
  }
}

void quickSortLomutoFree(long* left, long* right) {
  if (left < right) {
    long* pivot = lomuto_branchfree(left, right);
    quickSortLomutoFree(left, pivot);
    quickSortLomutoFree(pivot + 1, right);
  }
}

long* createAndFillArray() {
  long* arr = malloc(sizeof(long) * SIZE);
  for (size_t i = 0; i < SIZE; i++) {
    arr[i] = rand() % 100;
  }
  return arr;
}

void printArr(long* arr) {
  for(size_t i = 0; i < SIZE; i++) {
    printf("%ld ", arr[i]);
  }
}

float sampleMean(float* times, size_t size) {
  float holder = 0;
  for (size_t i = 0; i < size; i++) {
    holder += times[i];
  }
  return (1. / size) * holder;
}

float geometricMean(float* times, size_t size) {
  float holder = 1;
  for (size_t i = 0; i < size; i++) {
    holder *= times[i];
  }
  return pow(holder, 1.0 / size);
}

int main() {
  float* branchFree = malloc(sizeof(float) * 5);
  float* hoare = malloc(sizeof(float) * 5);
  clock_t time;
  float* branchBusy = malloc(sizeof(float) * 5);
  for (size_t i = 0; i < 5; i++) {
    srand(clock());
    long* arr = createAndFillArray();
    time = clock();
    lomuto_branchfree(arr, arr + SIZE);
    time = clock() - time;
    branchFree[i] = time;
    free(arr);
  }
  for (size_t i = 0; i < 5; i++) {
    srand(clock());
    long* arr = createAndFillArray();
    time = clock();
    lomuto_branch_busy(arr, arr + SIZE);
    time = clock() - time;
    branchBusy[i] = time;
    free(arr);
  }
  for (size_t i = 0; i < 5; i++) {
    srand(clock());
    long* arr = createAndFillArray();
    time = clock();
    le_hoare(arr, arr + SIZE);
    time = clock() - time;
    hoare[i] = time;
    free(arr);
  }
  float brfreesamplemean = sampleMean(branchFree, 5);
  float brbusysamplemean = sampleMean(branchBusy, 5);
  float hoaresamplemean = sampleMean(hoare, 5);
  float brfreegeommean = geometricMean(branchFree, 5);
  float brbusygeommean = geometricMean(branchBusy, 5);
  float hoaregeommean = geometricMean(hoare, 5);
  printf("Lomuto branch free:\nSample mean: %f\nGeometric mean: %f\n", brfreesamplemean, brfreegeommean);
  printf("Lomuto naive:\nSample mean: %f\nGeometric mean: %f\n", brbusysamplemean, brbusygeommean);
  printf("Hoare:\nSample mean: %f\nGeometric mean: %f\n", hoaresamplemean, hoaregeommean);
}
