set(MAYA_FILES
# Nodes
  node/ComponentNode.h
  node/CtrlNode.h
  node/CtrlNode.cpp
  node/CtrlCmd.h
  node/CtrlCmd.cpp
  node/SpaceSwitchNode.h
  node/ShapesDefinition.h
  node/MetaDataCmd.h
  node/MetaDataCmd.cpp
  node/MetaDataNode.h
  node/MetaDataNode.cpp
# Solvers
  solve/Ik2bSolver.h
  solve/Ik2bSolver.cpp
  solve/IkCommand.h
  solve/IkCommand.cpp
  solve/TwistSolver.cpp
  solve/TwistSolver.h
  solve/FootRollSolver.h
  solve/FootRollSolver.cpp
# Main
  MainPlugin.cpp
)

find_package(Maya REQUIRED)

add_library(oam SHARED ${MAYA_FILES})

# Link any dependencies needed for the root project
target_link_libraries(oam PRIVATE Maya::Maya)

# target_link_libraries(oam PRIVATE "src/maya/solve")

target_include_directories(oam 
  PRIVATE Maya::Maya
  PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}"
)
MAYA_PLUGIN(oam)


# # Add the subdirectory for the nested project
add_subdirectory(api)

add_subdirectory(node)
# target_link_libraries(oam PRIVATE oamayanode)

# add_subdirectory(solve)
# target_link_libraries(oam PRIVATE oamayasolve)

