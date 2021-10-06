#ifndef ARRAY_H
#define ARRAY_H

#include <cstring>

template<typename T>
void u_InsertElement(T*& arr, T& insert, int size, int pos) {
	T* temp = new T[size+1];
	int edit=0;

	for (int i=0; i<=size; i++) {
		if (i == pos) {temp[i] = insert; edit=1; continue;}
		temp[i] = arr[i-edit];
	}

	delete[] arr;
	arr = temp;
}

template<typename T>
void u_InsertElementEnd(T*& arr, T& insert, int size) {
	T* temp = new T[size+1];

	for (int i=0; i<size; i++) {
		temp[i] = arr[i];
	}

	temp[size] = insert;
	delete[] arr;
	arr = temp;
}

template<typename T>
void u_Dealloc2DArray(T**& arr, int size1, int size2) {
	// TODO
}

#endif
