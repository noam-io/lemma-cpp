//Copyright (c) 2014, IDEO

#include "CppUTest/CommandLineTestRunner.h"
#include "CppUTest/MemoryLeakDetectorMallocMacros.h"
#include "jansson.h"

int main(int argc, const char* argv[])
{
		int result = CommandLineTestRunner::RunAllTests(argc, (char**)argv);
    return result;
}



