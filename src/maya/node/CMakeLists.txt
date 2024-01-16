# Define the project name
project(oamayanode LANGUAGES CXX)

set(NODE_FILES
  ComponentNode.hpp
  CtrlNode.hpp
  CtrlNode.cpp
  CtrlCmd.hpp
  CtrlCmd.cpp
  SpaceSwitchNode.hpp
  ShapesDefinition.hpp
  MetaDataNode.hpp
  MetaDataNode.cpp
  MetaDataCmd.hpp
  MetaDataCmd.cpp
)

# add_library(oamayanode ${NODE_FILES})
# target_include_directories(oamayanode PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
