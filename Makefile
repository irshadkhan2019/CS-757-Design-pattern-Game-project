CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
FLTK_FLAGS = `fltk-config --cxxflags --ldflags`

# Target
TARGET = BallBricksGame

# Source files
SOURCES = main.c++ Game.c++ GameObjects.c++ Flyweight.c++ Memento.c++ Performance.c++

OBJECTS = $(SOURCES:.c++=.o)

# Header files 
HEADERS = Game.h GameObjects.h Flyweight.h Memento.h GameConfig.h Performance.h

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJECTS) -o $(TARGET) $(FLTK_FLAGS)
	@echo "Build completed successfully!"

# Compile source files to object files
%.o: %.c++ $(HEADERS)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) $(FLTK_FLAGS) -c $< -o $@

# Cleaning artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean completed!"

# Install FLTK 
install-deps:
	@echo "Installing FLTK development libraries"
	sudo apt-get update
	sudo apt-get install libfltk1.3-dev


Game.o: Game.c++ Game.h GameConfig.h GameObjects.h Flyweight.h Memento.h
GameObjects.o: GameObjects.c++ GameObjects.h GameConfig.h
Flyweight.o: Flyweight.c++ Flyweight.h GameObjects.h
Memento.o: Memento.c++ Memento.h
Performance.o: Performance.c++ Performance.h
main.o: main.c++ Game.h GameConfig.h Performance.h