# Built-in imports
import json
import platform
import logging

# Third-party imports

# Maya imports
import maya.api.OpenMaya as om

from PySide2 import QtCore as qtc

# Open APi imports




class Object(om.MObject):
  """Wrapper class with MObjects utils.
  """

  log = logging.getLogger("Object")


  @classmethod
  def getObjFromString(cls, object:str) -> om.MObject:
    """Gets the MObject for given object.

    Python Api 1.0 Reference:
      listSelection = om.MSelectionList()
      listSelection.add(object)
      mObject = om.MObject()
      listSelection.getDependNode(0, mObject)

    """
    return om.MSelectionList().add(object).getDependNode(0)


  @classmethod
  def getDagPathFromString(cls, object:str) -> om.MDagPath:
    """Gets the MDagPath for the given object.

    Python Api 1.0 Reference:
      listSelection = om.MSelectionList()
      listSelection.add(object)
      dpObject = om.MDagPath()
      listSelection.getDagPath(0, dpObject)

    """
    return om.MSelectionList().add(object).getDagPath(0)