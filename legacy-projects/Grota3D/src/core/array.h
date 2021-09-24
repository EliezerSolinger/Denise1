#ifndef GROTA_EVENT_REGISTER_H
#define GROTA_EVENT_REGISTER_H
#include <stdbool.h>
#include <glad/glad.h>

bool array_contains(int* arr,unsigned int arr_size,int value);
bool array_add_once(int* arr,unsigned int arr_size,int value);
bool array_add(int* arr,unsigned int arr_size,int value);
bool array_remove(int* arr,unsigned int arr_size,int value);
void array_clear(int* arr,unsigned int arr_size);

#endif
