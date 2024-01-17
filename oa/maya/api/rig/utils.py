# Built-in imports
import json
import platform
import subprocess
import logging
from collections import OrderedDict

# Third-party imports
from maya import cmds
import maya.api.OpenMaya as om
from PySide2 import QtCore as qtc

# Custom imports
import oa.maya.Core as omc



class Utils():
  """Rigging utilities.
  """

  @classmethod
  def GetVerteciesPosition(cls, object:str, precision:int=3) -> tuple:
    """Gets the vertecies position as well as edge indecies.
    """
    fnMesh = om.MFnMesh(omc.Object.GetDagPathFromString(object))

    print(fnMesh.numVertices)
    edgeIndecies = tuple(fnMesh.getVertices()[1])
    vrtxIndecies = set(edgeIndecies)

    listVertecies = []
    for vrtx in vrtxIndecies:
      vrtxPos = fnMesh.getPoint(vrtx)
      listVertecies.append((
        round(vrtxPos.x, precision),
        round(vrtxPos.y, precision),
        round(vrtxPos.z, precision)
      ))

    return tuple(listVertecies), edgeIndecies
