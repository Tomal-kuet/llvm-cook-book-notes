# the compiler: gcc for C program, define as g++ for C++
CC = clang++
 
  # compiler flags:
  #  -g     - this flag adds debugging information to the executable file
  #  -Wall  - this flag is used to turn on most compiler warnings
CFLAGS  = -o3
INCLUDEDIR=/home/tomal/llvm_all/llvm-project/llvm/include/
INCLUEDDIR2=/home/tomal/llvm_all/llvm-project/build/include
  # The build target 
TARGET = toy

all: $(TARGET)

$(TARGET): $(TARGET).cpp
		$(CC) -I$(INCLUDEDIR) -I$(INCLUEDDIR2) $(TARGET).cpp $(CFLAGS) -o $(TARGET)

clean:
		$(RM) $(TARGET)