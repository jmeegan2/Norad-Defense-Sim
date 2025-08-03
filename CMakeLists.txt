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
