# Define the project name
project(oamayasolve LANGUAGES CXX)

set(NODE_FILES
  Ik2bSolver.hpp
  Ik2bSolver.cpp
  IkCommand.hpp
  IkCommand.cpp
  TwistSolver.hpp
  TwistSolver.cpp
  FootRollSolver.hpp
  FootRollSolver.cpp
)

add_library(oamayasolve ${NODE_FILES})
target_include_directories(oamayasolve PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
