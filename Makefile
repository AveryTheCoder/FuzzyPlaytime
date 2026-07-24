CXX = g++
CXXFLAGS = -std=c++17 -Wall -Isrc
LDFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11

SRCS = src/main.cpp src/save_load.cpp src/sound.cpp src/shaders.cpp src/game_time.cpp src/monster.cpp src/computer.cpp src/dialogue.cpp src/level.cpp src/story.cpp src/data_loader.cpp src/event_manager.cpp src/asset_manager.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = FuzzyPlaytime

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
