# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -Wall -g 

# Target executable
TARGET = Renderer

# For deleting the target
TARGET_DEL = Renderer

# Source files
SRCS = main.cpp projection.cpp points.cpp glad.cpp shapes.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o  $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


# Clean rule to remove generated files
clean:
	rm $(TARGET_DEL) $(OBJS)