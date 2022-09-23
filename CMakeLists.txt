cmake_minimum_required(VERSION 3.17)
project(3Dsurface_repair)

add_executable(3Dsurface_repair
  main.cpp
)
set_target_properties(3Dsurface_repair PROPERTIES
  CXX_STANDARD 17
)

# Make the executable a default target to build & run in Visual Studio
set_property(DIRECTORY ${PROJECT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 3Dsurface_repair)

