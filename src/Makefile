.PHONY: all clean

TARGET := fsmos
OBJS := main.o fsmos.o
DEPS := $(wildcard *.d)

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS) $(DEPS)

$(TARGET): $(OBJS)

CC := $(CXX)

override CPPFLAGS += -MMD -Wall
include $(DEPS)
