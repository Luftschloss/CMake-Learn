#pragma once
#include <corecrt_malloc.h>

// void *malloc(size_t size)	:����һ��size�ֽ�����С���ڴ棬������һ��ָ���ѷ����ڴ���ʼλ�õ�ָ�룬���ϵͳ�ڴ治��᷵��NULL������Ҫ���
// void *calloc(size_t num_elements, size_t element_size)	:�����mallocĬ�ϻ��ڷ����ڴ�ָ��ǰ���ڴ��ʼ��Ϊ0
// void *realloc(void *ptr, size_t new_size)	:�����޸�һ���Ѿ�����õ��ڴ�飬�������ԭ���ݲ��䣬��������µ�δ��ʼ���ڴ棬�����С�����޷��ı��С�������·���һ���ڴ沢��ԭ�ڴ����ݸ��Ƶ��µĿ��ϣ���˾�ָ�붼��������
// void free(void *pointer)		:�ͷ�malloc��calloc��realloc�ȷ�����ڴ棬������NULL

void Test1()
{
	
}