#include "CppUTest/CommandLineTestRunner.h"
#include "jansson.h"

void *json_malloc(size_t a) {
	malloc(a);
}


void json_free(void * e) {
	free(e);
}

int main( int ac, char** av )
{
	  json_set_alloc_funcs(json_malloc, json_free);
		CommandLineTestRunner::RunAllTests(ac, av);
}


