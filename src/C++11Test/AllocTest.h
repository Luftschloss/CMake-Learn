#pragma once
#include <corecrt_malloc.h>

// void *malloc(size_t size)	:分配一块size字节数大小的内存，并返回一个指向已分配内存起始位置的指针，如果系统内存不足会返回NULL，故需要检查
// void *calloc(size_t num_elements, size_t element_size)	:相较于malloc默认会在返回内存指针前将内存初始化为0
// void *realloc(void *ptr, size_t new_size)	:用于修改一块已经分配好的内存块，如果扩大，原数据不变，后面添加新的未初始化内存，如果缩小，若无法改变大小，会重新分配一块内存并将原内存内容复制到新的块上，因此旧指针都不能用了
// void free(void *pointer)		:释放malloc、calloc、realloc等分配的内存，可以是NULL

void Test1()
{
	
}