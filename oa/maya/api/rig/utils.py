# Built-in imports
import json
import logging

# Third-party imports
from maya import cmds
import maya.api.OpenMaya as om

# OpenAPi imports
import oa.maya.Core as omc




class Utils():
  """Rigging utilities.
  """

  log = logging.getLogger("Utils")


  @classmethod
  def getPoleVectorPosition(cls, start:str, mid:str, end:str, space:str="world") -> om.MVector:
    """Gets the pole vector position from the input of three objects.
    """
    # Get world position from strings
    posStart = cmds.xform(start, q=True, ws=True, t=True)
    posMid = cmds.xform(mid, q=True, ws=True, t=True)
    posEnd = cmds.xform(end, q=True, ws=True, t=True)
    # Get vectors
    vecA = om.MVector(posStart[0], posStart[1], posStart[2])
    vecB = om.MVector(posMid[0], posMid[1], posMid[2])
    vecC = om.MVector(posEnd[0], posEnd[1], posEnd[2])

    vecAC = vecC - vecA
    vecAB = vecB - vecA
    vecBC = vecC - vecB

    valScale = (vecAC * vecAB) / (vecAC * vecAC)
    vecProjection = vecAC * valScale + vecA
    lenABC = vecAB.length() + vecBC.length()

    posPv = ((vecB - vecProjection).normal() * lenABC)

    if space == "world": return posPv + vecB
    return posPv


  @classmethod
  def createMatrixConstraint(cls, parent, child, offset, sourceNode:str=None) -> str:

    nodeMultMatrix = cmds.createNode("multMatrix", name=f"{child}_matCnst")
    if sourceNode:
      arrayLocalMatrix = cmds.xform(sourceNode, query=True, matrix=True, worldSpace=False),
      cmds.setAttr(f"{nodeMultMatrix}.matrixIn[0]", arrayLocalMatrix[0], type="matrix")

    cmds.connectAttr(f"{parent}.worldMatrix[0]", f"{nodeMultMatrix}.matrixIn[1]")
    cmds.connectAttr(f"{offset}.worldInverseMatrix[0]", f"{nodeMultMatrix}.matrixIn[2]")

    cmds.connectAttr(f"{nodeMultMatrix}.matrixSum", f"{child}.offsetParentMatrix")

    return nodeMultMatrix


  @classmethod
  def create_space_switch(cls, node, drivers, switch_attribute:str="space"):
    """Creates a space switch network.

    The network uses the offsetParentMatrix attribute and does not create any
    constraints or new dag nodes.

    :param node: Transform to drive
    :param drivers: List of tuples: [(driver1, "spaceName1"), (driver2, "spaceName2")]
    :param switch_attribute: Name of the switch attribute to create on the target node.

    """
    if cmds.objExists(f"{node}.{switch_attribute}"): cmds.deleteAttr(node, at=switch_attribute)

    names = [d[1] for d in drivers]
    cmds.addAttr(node, ln=switch_attribute, at="enum", en=":".join(names), keyable=True)

    spaceswitch = cmds.spaceSwitch(name=f"{node}_spaceswitch")
    # Get the current offset parent matrix. This is used as the starting blend point
    mat = om.MMatrix(cmds.getAttr("{}.offsetParentMatrix".format(node)))
    cmds.setAttr(f"{spaceswitch}.spaces[0].offsetMatrix", list(mat), type="matrix")

    parent = cmds.listRelatives(node, parent=True, path=True)
    to_parent_local = f"{parent[0]}.worldInverseMatrix[0]" if parent else None

    for indx, driver in enumerate(drivers):
      driver = driver[0]
      cls._connect_driver_matrix_network(spaceswitch, node, driver, indx, to_parent_local)
      target_attr = f"{spaceswitch}.target[{indx}]"

    cmds.connectAttr(f"{spaceswitch}.outputMatrix", f"{node}.offsetParentMatrix")
    cmds.connectAttr(f"{node}.{switch_attribute}", f"{spaceswitch}.space")


  @classmethod
  def _connect_driver_matrix_network(cls, spaceswitch, node, driver, index, to_parent_local):
    # The multMatrix node will calculate the transformation to blend to when driven
    # by this driver transform
    offset = (
      om.MSelectionList().add(node).getDagPath(0).exclusiveMatrix() * om.MMatrix(cmds.getAttr(f"{driver}.worldInverseMatrix[0]"))
    )
    cmds.setAttr(f"{spaceswitch}.spaces[{index}].offsetMatrix", list(offset), type="matrix")
    cmds.connectAttr(f"{driver}.worldMatrix[0]", f"{spaceswitch}.spaces[{index}].driverMatrix")

    if to_parent_local:	
      cmds.connectAttr(to_parent_local, f"{spaceswitch}.spaces[{index}].driverInverseMatrix")


  @classmethod
  def switch_space(cls, node, attribute, space, create_keys=False):
    """Seamlessly switch between spaces

    :param node: Node to switch
    :param attribute: Space switching attribute on node
    :param space: Space index in the space attribute
    :param create_keys: True to create switching keys
    """
    m = cmds.xform(node, q=True, ws=True, m=True)
    cmds.setAttr("{}.{}".format(node, attribute), space)
    cmds.xform(node, ws=True, m=m)


  @classmethod
  def save_rig_look(cls, file_path:str):
    jsonRigLook = {}
    attrs = [
      "localPositionX", "localPositionY", "localPositionZ",
      "localRotateX", "localRotateY", "localRotateZ",
      "localScaleX", "localScaleY", "localScaleZ",
      "shape", "lineWidth", "drawLine", "drawSolverMode",
      "solverModeSize",
      "solverModePositionX", "solverModePositionY", "solverModePositionZ",
      "fkIk"
    ]
    nodes = cmds.listRelatives("rig", typ="ctrl", allDescendents=True)
    cmds.select(nodes)

    for ctrl in nodes:
      jsonRigLook[ctrl] = {}
      for attr in attrs:
        jsonRigLook[ctrl][attr] = cmds.getAttr(f"{ctrl}.{attr}")

    with open(file_path, "w") as file:
      json.dump(jsonRigLook, file, indent=2, sort_keys=True)


  # @classmethod
  # def apply_rig_look(cls, file_path:str):
  #   # Set rig look:
  #   jsonRigLook = nom.MFileIO.load_json(file_path)
  #   for ctrl in jsonRigLook:
  #     if not cmds.objExists(ctrl): continue
  #     for attr in jsonRigLook[ctrl]:
  #       attrNs = f'{ctrl}.{attr}'
  #       if cmds.getAttr(attrNs, settable=True):
  #         cmds.setAttr(attrNs, jsonRigLook[ctrl][attr])
  #       nom.MAttribute.lockControlChannels(ctrl, [attr])


  @classmethod
  def GetVerteciesPosition(cls, object:str, precision:int=3):
    fnMesh = om.MFnMesh(omc.Object.GetDagPathFromString(object))
    edgeIndecies = tuple(fnMesh.getVertices()[1])
    vrtxIndecies = set(edgeIndecies)
    listVertecies = []
    for vrtx in vrtxIndecies:
      vrtxPos = fnMesh.getPoint(vrtx)
      listVertecies.append((
        round(vrtxPos.x, precision),
        round(vrtxPos.y, precision),
        round(vrtxPos.z, precision),
      ))
      listVertecies.append((vrtxPos.x, vrtxPos.y, vrtxPos.z))
    return tuple(listVertecies), edgeIndecies
