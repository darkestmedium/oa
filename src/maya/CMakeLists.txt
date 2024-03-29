set(MAYA_FILES
# APi
  api/Shapes.cpp
# Nodes
  # Transform
  node/TransformShape.hpp
  node/TransformShape.cpp
  node/ComponentNode.hpp
  node/Ctrl.hpp
  node/Ctrl.cpp
  node/CtrlCmd.hpp
  node/CtrlCmd.cpp
  node/ShapeNode.hpp
  node/SpaceSwitchNode.hpp
  node/MetaDataCmd.hpp
  node/MetaDataCmd.cpp
  node/MetaDataNode.hpp
  node/MetaDataNode.cpp
  # ShakeNode
  node/PerlinNoise.hpp
  node/PerlinNoise.cpp
  node/ShakeNode.hpp
  node/ShakeNode.cpp
  node/ShakeNodeRot.hpp
  node/ShakeNodeRot.cpp
  node/ShakeCmd.hpp
  node/ShakeCmd.cpp
  # Speedometer
  node/Speedometer.hpp
  node/Speedometer.cpp
  node/SpeedometerCmd.hpp
  node/SpeedometerCmd.cpp
# Solvers
  solve/Ik2bSolver.hpp
  solve/Ik2bSolver.cpp
  solve/IkCommand.hpp
  solve/IkCommand.cpp
  solve/TwistSolver.cpp
  solve/TwistSolver.hpp
  solve/FootRollSolver.hpp
  solve/FootRollSolver.cpp
# Temp
  # temp/rawfootPrintNode.hpp
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
# add_subdirectory(node)
# add_subdirectory(temp)


# target_link_libraries(oam PRIVATE oamayanode)

# add_subdirectory(solve)
# target_link_libraries(oam PRIVATE oamayasolve)

