# we use features in 3.5 or above
cmake_minimum_required (VERSION 3.5)

set(PROJECT_BIN_NAME "gentoc2")

# name of our project
project(${PROJECT_BIN_NAME})

# set our desired directory for install
set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

# add the executable
add_executable(${PROJECT_BIN_NAME} gentoc2.c)

# install
install (TARGETS ${PROJECT_BIN_NAME})
