#Set this to @ to keep the makefile quiet
SILENCE = @
CXX=g++
RANLIB = @echo

#---- Outputs ----#
COMPONENT_NAME = LemmaLib
TARGET_LIB = \
	lib/lib$(COMPONENT_NAME).a

TEST_TARGET = \
	$(COMPONENT_NAME)_tests

VERIFICATION_TARGET = \
	$(COMPONENT_NAME)_verification

#--- Inputs ----#
PROJECT_HOME_DIR = .
CPPUTEST_HOME = cpputest
CPPUNIT_HOME = ../cppunit
CPP_PLATFORM = Gcc

#CFLAGS are set to override malloc and free to get memory leak detection in C programs
CFLAGS = -Dmalloc=cpputest_malloc -Dfree=cpputest_free
CPPFLAGS =
#GCOVFLAGS = -fprofile-arcs -ftest-coverage

#SRC_DIRS is a list of source directories that make up the target library
#If test files are in these directories, their IMPORT_TEST_GROUPs need
#to be included in main to force them to be linked in.  By convention
#put them into an AllTests.h file in each directory
SRC_DIRS = \
	lemma/src \
	lemma/jansson

PLATFORM_SRC_DIRS = \
	lemma/platform/unix

#TEST_SRC_DIRS is a list of directories including
# - A test main (AllTests.cpp by convention)
# - OBJ files in these directories are included in the TEST_TARGET
# - Consequently - AllTests.h containing the IMPORT_TEST_GROUPS is not needed
# -
TEST_SRC_DIRS = \
	lemmatests/src \
	lemmatests

VERIFICATION_MAIN = verification/main.o
#includes for all compiles
INCLUDES =\
  -I.\
  -Ilemma/include\
  -Ilemma/platform/unix\
  -Ilemma/jansson\
  -I$(CPPUTEST_HOME)/include/

#Flags to pass to ld
CPPUTEST_LDFLAGS = -lstdc++

include $(CPPUTEST_HOME)/build/ComponentMakefile

PLATFORM_SRC = $(call get_src_from_dir_list, $(PLATFORM_SRC_DIRS)) $(PLATFORM_SRC_FILES)
PLATFORM_OBJ = $(call change_o_file_location, $(call src_to_o,$(PLATFORM_SRC)))

$(VERIFICATION_TARGET): $(PRODUCTION_CODE_START) $(PLATFORM_OBJ) $(VERIFICATION_MAIN) $(TARGET_LIB) $(USER_LIBS) $(PRODUCTION_CODE_END) $(STDLIB_CODE_START)
	echo $(PLATFORM_OBJ)
	$(SILENCE)echo Linking $@
	$(SILENCE)$(LINK.o) -o $@ $^ $(LD_LIBRARIES)

verify: $(VERIFICATION_TARGET)
