#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct array_list
{
    char *arrayType;
    size_t arraySize;
    void *address;
    size_t arrayLen;
    char typeSize;
} Array_list;

char parseSizeBasedOnType(char *arrayType)
{
    if (strcmp(arrayType, "int") == 0)
    {
        return sizeof(int);
    }

    if (strcmp(arrayType, "long") == 0)
    {
        return sizeof(unsigned long long);
    }

    if (strcmp(arrayType, "char") == 0)
    {
        return sizeof(char);
    }

    if (strcmp(arrayType, "char*") == 0)
    {
        return sizeof(char *);
    }

    return 0;
}

void resizeArray(Array_list *arr)
{ // << 2  is equiv to *2 but faster
    void *new_cu_array = (void *)malloc(arr->typeSize * (arr->arraySize << 2));
    memcpy(new_cu_array, arr->address, arr->arrayLen * arr->typeSize);
    free(arr->address);
    arr->address = new_cu_array;
    arr->arraySize = arr->arraySize << 2;
}

Array_list *createArray(char *arrayType, int arraySize)
{
    char sizeBasedOnType = parseSizeBasedOnType(arrayType);
    if (sizeBasedOnType == 0)
    {
        return NULL;
    }
    Array_list *myArray = (Array_list *)malloc(sizeof(Array_list));
    myArray->address = (void *)malloc(arraySize * sizeBasedOnType);
    myArray->arrayType = arrayType;
    myArray->arraySize = arraySize;
    myArray->typeSize = sizeBasedOnType;
    myArray->arrayLen = 0;
    return myArray;
}

void push(Array_list *arr, void *data)
{

    if (arr->arrayLen == arr->arraySize)
    {
        // return a bigger array
        // create new Array_list with bigger size and copy all data to it
        resizeArray(arr);
        arr->arraySize = arr->arraySize << 2;
    }

    void *slot = arr->address + arr->arrayLen * arr->typeSize;
    memcpy(slot, data, arr->typeSize);
    arr->arrayLen = arr->arrayLen + 1;
}

void printArray(Array_list *arr)
{

    if (arr->arrayLen == 0)
    {
        printf("{}\n");
        return;
    }

    if (strcmp(arr->arrayType, "int") == 0)
    {

        for (size_t i = 0; i < arr->arrayLen; i++)
        {
            printf("%d\n", ((int *)arr->address)[i]);
        }
    }

    else if (strcmp(arr->arrayType, "long") == 0)
    {

        for (size_t i = 0; i < arr->arrayLen; i++)
        {
            printf("%llu\n", ((unsigned long long *)arr->address)[i]);
        }
    }

    else if (strcmp(arr->arrayType, "char") == 0)
    {

        for (size_t i = 0; i < arr->arrayLen; i++)
        {
            printf("%c\n", ((char *)arr->address)[i]);
        }
    }

    else if (strcmp(arr->arrayType, "char*") == 0)
    {

        for (size_t i = 0; i < arr->arrayLen; i++)
        {
            printf("%s\n", ((char **)arr->address)[i]);
        }
    }
}

void pop(Array_list *arr)
{
    if (arr->arrayLen == 0)
    {
        return;
    }

    arr->arrayLen--;

    if ((float)arr->arraySize / (float)arr->arrayLen >= 5.0)
    {
        printf("old size : %zu\n", arr->arraySize);
        arr->arraySize = (size_t)ceil((float)((arr->arraySize) / 2.0));
        printf("new size : %zu\n", arr->arraySize);
    }
}

ptrdiff_t getIndexOf(Array_list *arr, void *val)
{

    if (strcmp(arr->arrayType, "char*") == 0)
    {
        for (int i = 0; i < arr->arrayLen; i++)
        {
            if (strcmp(((char **)arr->address)[i], val) == 0)
            {
                return i;
            }
        }
        return -1;
    }
    else
    {
        void *ptr = arr->address;
        for (int i = 0; i < arr->arrayLen; i++)
        {
            if (memcmp(ptr, val, arr->typeSize) == 0)
            {
                return i;
            }
            ptr = ptr + arr->typeSize;
        }
    }
    return -1;
}

void *getElementByIndex(Array_list *arr, size_t index)
{
    return arr->address + (arr->typeSize * index);
}

size_t getLen(Array_list *arr)
{
    return arr->arrayLen;
}

void clear(Array_list *arr)
{
    if (arr->arrayLen == 0)
    {
        return;
    }

    free(arr->address);
    size_t proposedNewSize = (size_t)ceil((float)arr->arrayLen / 2);
    // at least 10 elements minimum
    if (proposedNewSize < 10)
    {
        proposedNewSize = 10;
    }
    arr->arraySize = proposedNewSize;
    arr->arrayLen = 0;
}

bool elementExist(Array_list *arr, size_t index)
{
    if (arr->arrayLen <= index)
    {
        return false;
    }
    return true;
}

void set(Array_list *arr, size_t index, void *newVal)
{

    if (!elementExist(arr, index))
    {
        return;
    }
    memcpy(arr->address + (arr->typeSize * index), newVal, arr->typeSize);
}

void removeElmByIndex(Array_list *arr, size_t index)
{

    if (!elementExist(arr, index))
    {
        return;
    }

    // remove from buttom
    if ((arr->arrayLen - 1) == index)
    {
        pop(arr);
        return;
    }

    void *new_arr = (void *)malloc(arr->typeSize * arr->arraySize);

    // remove from top
    if (index == 0)
    {
        memcpy(new_arr, arr->address + arr->typeSize, arr->typeSize * arr->arrayLen - 1);
    }
    else // remove from middle
    {
        // middle and elements are greater than 3 elements
        memcpy(new_arr, arr->address, arr->typeSize * index);
        memcpy(new_arr + (arr->typeSize * index), arr->address + (arr->typeSize * (index + 1)), (arr->arrayLen - 1 - (index)) * arr->typeSize);
    }

    free(arr->address);
    arr->address = new_arr;
    arr->arrayLen--;
}

int main()
{
    Array_list *myArr = createArray("int", 5);
    int val = 11;
    push(myArr, &val);

    val = 22;
    push(myArr, &val);

    val = 33;
    push(myArr, &val);

    // fprintf(stderr, "index of 'abc efg h'? %d\n", getIndexOf(myArr, "abc efg h"));
    // fprintf(stderr, "index of 'hello world'? %d\n", getIndexOf(myArr, "hello world"));
    // fprintf(stderr, "index of 'blah'? %d\n", getIndexOf(myArr, "blah"));
    // fprintf(stderr, "index of 'zewww'? %d\n", getIndexOf(myArr, "zewwww"));

    int newVal = 999;
    set(myArr, 2, &newVal);

    val = 44;
    push(myArr, &val);

    val = 55;
    push(myArr, &val);

    printArray(myArr);
    printf("---------------\n");
    removeElmByIndex(myArr, 0);
    printArray(myArr);
}