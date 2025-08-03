#!/bin/bash

# Create the new directory structure
echo "Creating project structure..."
mkdir -p src include build tests docs external

# Move header files to include/
echo "Moving header files..."
mv *.h include/ 2>/dev/null || echo "No .h files found in root"

# Move source files to src/
echo "Moving source files..."
mv *.cpp src/ 2>/dev/null || echo "No .cpp files found in root"

# Create CMakeLists.txt
echo "Creating CMakeLists.txt..."
cat > CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.16)
project(MissileDefenseSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Find all source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Optional: Add Qt support (uncomment when ready)
# find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
# target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Widgets)
EOF

# Create .gitignore
echo "Creating .gitignore..."
cat > .gitignore << 'EOF'
# Build directories
build/
cmake-build-*/

# Compiled Object files
*.o
*.obj

# Executables
*.exe
*.out
*.app

# IDE files
.vscode/
.idea/
*.vcxproj*
*.sln

# OS files
.DS_Store
Thumbs.db

# CMake
CMakeCache.txt
CMakeFiles/
cmake_install.cmake
Makefile
EOF

# Create basic README.md
echo "Creating README.md..."
cat > README.md << 'EOF'
# Missile Defense System

A real-time C++ simulation of a missile defense system with threat detection and interception capabilities.

## Features
- Real-time missile tracking
- 3D position calculations  
- Interactive command-line interface
- Threat detection and interception

## Building
```bash
mkdir build && cd build
cmake ..
make
./MissileDefenseSystem
```

## Structure
- `src/` - Source files (.cpp)
- `include/` - Header files (.h)
- `tests/` - Unit tests
- `build/` - Build artifacts
EOF

# Fix include paths in source files
echo "Updating include paths..."
find src/ -name "*.cpp" -exec sed -i 's/#include "/#include "/g' {} \; 2>/dev/null

# Create a basic test file
echo "Creating sample test..."
cat > tests/CMakeLists.txt << 'EOF'
# Tests will go here
# Example: add_executable(test_missile test_missile.cpp)
EOF

echo ""
echo "✅ Project restructured successfully!"
echo ""
echo "Your new structure:"
echo "📁 Current directory"
echo "├── 📁 src/          (all .cpp files)"
echo "├── 📁 include/      (all .h files)"  
echo "├── 📁 build/        (build artifacts)"
echo "├── 📁 tests/        (unit tests)"
echo "├── 📁 docs/         (documentation)"
echo "├── 📁 external/     (third-party libs)"
echo "├── 📄 CMakeLists.txt"
echo "├── 📄 .gitignore"
echo "└── 📄 README.md"
echo ""
echo "To build:"
echo "  cd build"
echo "  cmake .."
echo "  make"
echo ""
echo "🚀 Ready for Qt integration!"