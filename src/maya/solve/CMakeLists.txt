# Define the project name
project(oamayasolve LANGUAGES CXX)

set(NODE_FILES
  Ik2bSolver.h
  Ik2bSolver.cpp
  IkCommand.h
  IkCommand.cpp
  TwistSolver.h
  TwistSolver.cpp
  FootRollSolver.h
  FootRollSolver.cpp
)

add_library(oamayasolve ${NODE_FILES})
target_include_directories(oamayasolve PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
