cmake_minimum_required(VERSION 3.28)
project(project_01_load_file)

# Set C++ standard
SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_STANDARD 14)
set(TARGET_NAME ${PROJECT_NAME})

# Set compiler flags
if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(PCL REQUIRED)
find_package(VTK REQUIRED)
find_package(Boost REQUIRED)
find_package(Eigen3 REQUIRED)

# Set dependencies
set(dependencies
        PCL
        VTK
        Boost
        Eigen3
)

# Set include directories
set(PCL_INCLUDE_DIRS /usr/local/include/pcl-1.13)

# Include directories
include_directories(
        include
        ${PCL_INCLUDE_DIRS}
        ${EIGEN3_INCLUDE_DIR}
)

# Build executable
add_executable(main src/study1.cpp)

# Link libraries
ament_target_dependencies(main ${dependencies})
target_link_libraries(main ${PCL_LIBRARIES})

# Install libraries
#install(TARGETS
#        ARCHIVE DESTINATION lib
#        LIBRARY DESTINATION lib
#        RUNTIME DESTINATION bin
#)

# Install executable
install(TARGETS main
        RUNTIME DESTINATION lib/${PROJECT_NAME})

# Install header files`
# install(DIRECTORY include
#         DESTINATION include)

# Install launch files
# install(DIRECTORY launch
#         DESTINATION share/${PROJECT_NAME})

# Export include directories
ament_export_include_directories(include)

# Export libraries
ament_export_libraries(main)

# Export dependencies
ament_export_dependencies(${dependencies})

# Install CMake config files
ament_package()

