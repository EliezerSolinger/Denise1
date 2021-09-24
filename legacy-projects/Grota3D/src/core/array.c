#include "array.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

bool array_contains(int* arr,unsigned int arr_size,int value) {
	if(value==0) return false;
	for(int c=0;c<arr_size;c++) {
		if(arr[c]==value) return true;
	}
	return false;
}

bool array_add_once(int* arr,unsigned int arr_size,int value) {
	if(!array_contains(arr,arr_size,value)) 
		return array_add(arr,arr_size,value);
	else return false;
}
bool array_add(int* arr,unsigned int arr_size,int value) {
	if(value==0) return false;
	for(int c=0;c<arr_size;c++) {
		if(arr[c]==0) {
			arr[c]=value;
			return true;
		}
	}
	return false;
}

bool array_remove(int* arr,unsigned int arr_size,int value) {
	if(value==0) return false;
	for(int c=0;c<arr_size;c++) {
		if(arr[c]==value) {
			arr[c]=0;
			return true;
		}
	}
	return false;
}

void array_clear(int* arr,unsigned int arr_size) {
	for(int c=0;c<arr_size;c++) arr[c]=0;
}
