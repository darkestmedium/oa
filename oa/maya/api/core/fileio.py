# Built-in imports
import json
import platform
import logging

# Third-party imports

# Maya imports
from maya import cmds
from maya import mel
import maya.api.OpenMaya as om
import maya.api.OpenMayaAnim as oma
from PySide2 import QtCore as qtc

# Open APi imports
import oa.maya.Core as omc




class FileIO():
  """Wrapper for file operations."""
  log = logging.getLogger("FileIO")


  @classmethod
  def LoadJson(cls, filaPath:str):
    """Loads data from a json file."""
    with open(filaPath, "r") as file: return json.loads(file.read())


  @classmethod
  def StageFromFile(cls, filePath:str):
    """Loads a usd stage from the specified file path.
    """
    nodeUsd = cmds.createNode("mayaUsdProxyShape", name="stageShape", skipSelect=True)
    cmds.setAttr(f"{nodeUsd}.filePath", filePath, type="string")
    # cmds.connectAttr(f"time1.outTime", f"{nodeUsd}.time")
    omc.Attribute.ConnectSceneTime(nodeUsd, "time")

    return nodeUsd