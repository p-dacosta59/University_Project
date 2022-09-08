# cmake ../
# cmake --build ./
# Minimun to CMake
cmake_minimum_required(VERSION 3.5)

# Name your project
project(voitureProjetWeb)
set(EXECUTABLE_NAME "voitureProjetWeb")


# Use the standar for C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add external library
find_package(Poco REQUIRED COMPONENTS Foundation Data DataMySQL)
include_directories(${POCO_INCLUDE_DIR})

find_package(SFML 2.5.1 COMPONENTS network system REQUIRED)
include_directories(${SFML_INCLUDE_DIR})
link_directories(${SFML_LIB_DIR})

# Include your files
file(
  GLOB_RECURSE
  source_files
  src/*
)
# Name the programme and add files to your project
add_executable(
  ${EXECUTABLE_NAME}
  ${source_files}
  /usr/include/wiringPi.h
)

# End the includes
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${Poco_LIBRARIES} wiringPi sfml-system sfml-network ${THREADS_LIBRARY})
