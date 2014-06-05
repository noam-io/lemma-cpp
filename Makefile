#Set this to @ to keep the makefile quiet
#SILENCE = @
#CXX=g++
BUILD := prod

#---- Outputs ----#
COMPONENT_NAME = LemmaLib
TARGET_LIB = \
	lib/lib$(COMPONENT_NAME).a

SAMPLE_TARGET = \
	$(COMPONENT_NAME)_sample

TEST_TARGET = \
	$(COMPONENT_NAME)_tests

VERIFICATION_TARGET = \
	$(COMPONENT_NAME)_verification

#--- Inputs ----#
PROJECT_HOME_DIR = .
CPPUTEST_HOME = cpputest2
CPPUTEST_CXXFLAGS  = -std=c++11 -Wno-error=unused-function
CPPUTEST_WARNINGFLAGS =  -Wshadow -Wswitch-default -Wswitch-enum -Wconversion

CPPUTEST_USE_MEM_LEAK_DETECTION.test = Y
CPPUTEST_USE_MEM_LEAK_DETECTION.prod = N
CPPUTEST_USE_MEM_LEAK_DETECTION = ${CPPUTEST_USE_MEM_LEAK_DETECTION.${BUILD}}

#SRC_DIRS is a list of source directories that make up the target library
#If test files are in these directories, their IMPORT_TEST_GROUPs need
#to be included in main to force them to be linked in.  By convention
#put them into an AllTests.h file in each directory
SRC_DIRS = \
	lemma/src \
	lemma/jansson \
	lemma/platform/unix

PLATFORM_SRC_DIRS = \
	lemma/platform/unix

# TEST_SRC is a list of files which include tests to run
# To include full directories see TEST_SRC_DIRS
# -
TEST_SRC = \
	lemmatests/src/EventFilterTests.cpp \
	lemmatests/src/HostLocatorTest.cpp \
	lemmatests/src/LemmaListDeserializerTest.cpp \
	lemmatests/src/LemmaListSerializerTest.cpp \
	lemmatests/src/LemmaListTest.cpp \
	lemmatests/src/MaestroLocatorTests.cpp \
	lemmatests/src/MessageBuilderTests.cpp \
	lemmatests/src/MessageParserTests.cpp \
	lemmatests/src/TcpReaderTests.cpp

#TEST_SRC_DIRS is a list of directories including
# - A test main (AllTests.cpp by convention)
# - OBJ files in these directories are included in the TEST_TARGET
# - Consequently - AllTests.h containing the IMPORT_TEST_GROUPS is not needed
# -
TEST_SRC_DIRS = \
	lemmatests

VERIFICATION_MAIN = verification/main.o
SAMPLE_MAIN = sampleapp/SampleApp.o
#includes for all compiles
INCLUDES =\
  -I.\
  -Ilemma/include\
  -Ilemma/platform/unix\
  -Ilemma/jansson\
  -I$(CPPUTEST_HOME)/include/

#Flags to pass to ld
#CPPUTEST_LDFLAGS = -lstdc++

include $(CPPUTEST_HOME)/build/MakefileWorker.mk

PLATFORM_SRC = $(call get_src_from_dir_list, $(PLATFORM_SRC_DIRS)) $(PLATFORM_SRC_FILES)
PLATFORM_OBJ = $(call change_o_file_location, $(call src_to_o,$(PLATFORM_SRC)))

$(VERIFICATION_TARGET): $(PRODUCTION_CODE_START) $(PLATFORM_OBJ) $(VERIFICATION_MAIN) $(TARGET_LIB) $(USER_LIBS) $(PRODUCTION_CODE_END) $(STDLIB_CODE_START)
	echo $(PLATFORM_OBJ)
	$(SILENCE)echo Linking $@
	$(SILENCE)$(LINK.o) -o $@ $^ $(LD_LIBRARIES)


$(SAMPLE_TARGET): $(PRODUCTION_CODE_START) $(PLATFORM_OBJ) $(SAMPLE_MAIN) $(TARGET_LIB) $(USER_LIBS) $(PRODUCTION_CODE_END) $(STDLIB_CODE_START)
	@echo BUILD=${BUILD}
	$(SILENCE)echo Linking $@
	$(SILENCE)$(LINK.cpp) -lstdc++ -o $@ $^ $(LD_LIBRARIES)
	./$(SAMPLE_TARGET)


verify: $(VERIFICATION_TARGET)

sample: $(SAMPLE_TARGET)
