#!/bin/bash

SOURCE_FILES="src/main.cpp src/interceptor_missile.cpp src/interceptor_controller.cpp src/enemy_missile.cpp src/detection_system.cpp src/terminal_ui.cpp"
EXECUTABLE="main"

clang++ -std=c++17 -Iinclude -o $EXECUTABLE $SOURCE_FILES

if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

./$EXECUTABLE