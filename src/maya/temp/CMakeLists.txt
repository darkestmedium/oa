# Define the project name
project(oamayatest LANGUAGES CXX)

set(NODE_FILES
  DisplayNode.h
  footPrintNodeGeoOverride.h
  rawfootPrintNode.h
)

add_library(oamayatest ${NODE_FILES})
target_include_directories(oamayatest PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
