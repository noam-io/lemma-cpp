//Copyright (c) 2014, IDEO

#include "CppUTest/CommandLineTestRunner.h"
#include "jansson.h"

void *json_malloc(size_t a) {
	return malloc(a);
}


void json_free(void * e) {
	free(e);
}

int main(int argc, const char* argv[])
{
	  json_set_alloc_funcs(json_malloc, json_free);
		int result = CommandLineTestRunner::RunAllTests(argc, (char**)argv);
    return result;
}



