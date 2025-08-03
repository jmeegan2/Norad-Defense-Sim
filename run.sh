#!/bin/bash

# Define the names of your C++ source files
# The key is to include both .cpp files here
SOURCE_FILES="main.cpp missile.cpp missile_controller.cpp enemy_missile.cpp detection_system.cpp"

# Define the name of the executable you want to create
EXECUTABLE="main"

# --- Compilation Step ---
# clang++ is the compiler.
# -std=c++17 sets the C++ standard to C++17.
# -o $EXECUTABLE tells the compiler to name the output file.
# $SOURCE_FILES is the list of input files to compile.
clang++ -std=c++17 -o $EXECUTABLE $SOURCE_FILES

# Check the exit status of the last command (clang++).
# If the exit status is not 0, it means compilation failed.
if [ $? -ne 0 ]; then
    echo "----------------------------------------"
    echo "Compilation failed. Check errors above."
    echo "----------------------------------------"
    exit 1
fi

echo
echo "Compilation successful. Running program..."
echo
# --- Run the Program ---
# ./$EXECUTABLE runs the compiled program.
./$EXECUTABLE