# Makefile

# Compiler
CXX = g++

# Compiler flags
# -std=c++17: Use C++17 standard
# -Wall:      Enable all common warnings
# -g:         Include debugging information
CXXFLAGS = -std=c++17 -Wall -g

# Source files
SRCS = main.cpp employee.cpp availability.cpp shift.cpp schedule.cpp TimeUtility.cpp

# Object files (derived from source files)
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = scheduler

# Default rule: build the executable
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile source files into object files
%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include header dependencies (optional but good practice)
# %.h:

# Clean rule: remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets (targets that don't represent files)
.PHONY: all clean
