set(MAYA_FILES
# Nodes
  node/ComponentNode.h
  node/CtrlNode.h
  node/CtrlNode.cpp
  node/CtrlCmd.h
  node/CtrlCmd.cpp
  node/SpaceSwitchNode.h
  node/ShapesDefinition.h
# Solvers
  FootRollSolver.h
  FootRollSolver.cpp
  Ik2bSolver.h
  Ik2bSolver.cpp
  IkCommand.h
  IkCommand.cpp
  MetaDataCmd.h
  MetaDataCmd.cpp
  MetaDataNode.h
  MetaDataNode.cpp
  TwistSolver.cpp
  TwistSolver.h
# Main
  MainPlugin.cpp
)

find_package(Maya REQUIRED)

add_library(oam SHARED ${MAYA_FILES})

# Link any dependencies needed for the root project
target_link_libraries(oam PRIVATE Maya::Maya)
target_include_directories(oam 
  PRIVATE Maya::Maya
  PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(oam)


# # Add the subdirectory for the nested project
add_subdirectory(api)

# add_subdirectory(node)
# target_link_libraries(oam PRIVATE oamayanode)

