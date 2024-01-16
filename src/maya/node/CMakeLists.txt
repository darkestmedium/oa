# Define the project name
project(oamayanode LANGUAGES CXX)

set(NODE_FILES
  ComponentNode.h
	CtrlNode.h
	CtrlNode.cpp
	CtrlCmd.h
	CtrlCmd.cpp
  SpaceSwitchNode.h
  ShapesDefinition.h
)

# add_library(oamayanode ${NODE_FILES})
# target_include_directories(oamayanode PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
