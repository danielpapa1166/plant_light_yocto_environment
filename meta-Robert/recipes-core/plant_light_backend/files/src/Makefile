# Compiler and flags
CXX ?= $(CXX)
CXXFLAGS += -Wall -Wextra -std=c++17 -O2

# Absolute path to CSV file (⚠ see note below)
PLANT_LIGHT_ENV_MEAS_DATA_LOG_FILE := /home/pbd2bp/projects/plant_light/log/env_data_log.csv

# Inject as string macro into C++
CXXFLAGS += -DPLANT_LIGHT_ENV_MEAS_DATA_LOG_FILE=\"$(PLANT_LIGHT_ENV_MEAS_DATA_LOG_FILE)\"

# Directories
BUILD_DIR := build

# Find all .cpp files automatically
SRC := $(wildcard *.cpp)

# Convert each .cpp to .o in build directory
OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC))

# Name of the final executable
TARGET := plant_light_backend

# Default rule
all: $(TARGET)

# Link object files into the final executable (FIXED)
$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

# Ensure build directory exists before compiling
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean built files
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean
