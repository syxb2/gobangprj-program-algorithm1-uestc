# 定义编译器
CC = g++

# 定义编译选项
CFLAGS = -Wall -g

# 定义目标文件
TARGET = game.exe

# 定义源文件
SRCS = ./src/game.cpp \
	   ./src/computerPlayer.cpp \
	   ./src/humanPlayer.cpp \
	   ./src/userInterface.cpp \
	   ./src/main.cpp \

# 定义头文件
HEADERS = game.h

# 定义对象文件
OBJS = $(SRCS:.cpp=.o)

# 生成目标文件
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) -lgraphics -lconio -lwinmm

# 生成对象文件
%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理
clean:
	rm -f $(OBJS) $(TARGET)

# 伪目标
.PHONY: clean
