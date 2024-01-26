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






class Attribute():
  """Wrapper class for attribute utilities.
  """
  log = logging.getLogger("Attribute")
  dgMod = om.MDGModifier()


  @classmethod
  def CopyTransformsToOPM(cls, object:str):
    """Copies the translation and rotation values to the offset parent matrix attribute.
    """
    arrayLocalMatrix = cmds.xform(object, query=True, matrix=True, worldSpace=False)
    cmds.setAttr(f"{object}.offsetParentMatrix", arrayLocalMatrix, type="matrix")
    cmds.xform(object, translation=(0,0,0), rotation=(0,0,0))


  @classmethod
  def hide(cls, name:str) -> None:
    """Hides the specified attribute from channel box.

    Args:
      name (string): Name of the attribute to be locked.

    """
    cmds.setAttr(name, lock=True, keyable=False, channelBox=False)


  @classmethod
  def addDisplayType(cls, object:str, name:str, defaultValue:int=0) -> str:
    """Adds a display type attribute on the given object.
    """
    attrName = f"{object}.{name}"
    cmds.addAttr(object, longName=name, attributeType="enum", enumName="normal=0:template=1:reference=2", keyable=False, defaultValue=defaultValue)
    cmds.setAttr(attrName, channelBox=True)
    # cmds.setAttr(f"{object}.overrideEnabled", True)
    return attrName


  @classmethod
  def addFloat(cls, object:str, name:str, defaultValue:float=0.0) -> str:
    """Adds a float attribute on the given object.
    """
    attrName = f"{object}.{name}"
    cmds.addAttr(object, longName=name, attributeType="float", keyable=True, defaultValue=defaultValue)
    return attrName


  @classmethod
  def addOnOff(cls, object:str, name:str, defaultValue:bool=True) -> str:
    """Adds a on / off attribute on the given object.
    """
    attrName = f"{object}.{name}"
    cmds.addAttr(object, longName=name, attributeType="enum", enumName="off=0:on=1", keyable=False, defaultValue=defaultValue)
    cmds.setAttr(attrName, channelBox=True)
    return attrName


  @classmethod
  def addSeparator(cls, object:str, name:str="_") -> str:
    """Adds a separator attribute on the given object.
    """
    attrName = f"{object}.{name}"
    cmds.addAttr(object, longName=name, attributeType="enum", enumName=" =0:", defaultValue=False, keyable=False)
    cmds.setAttr(attrName, lock=True, channelBox=True)
    return attrName


  @classmethod
  def addFloatFkIk(cls, object:str, name:str, minValue:float=0.0, maxValue:float=100.0, defaultValue:float=0.0) -> str:
    """Adds a float attribute on the given object.
    """
    attrName = f"{object}.{name}"
    cmds.addAttr(object, longName=name, attributeType="float", minValue=minValue, maxValue=maxValue, keyable=True, defaultValue=defaultValue)
    return attrName


  @classmethod
  def addFkIkMode(cls, object:str, name:str, defaultValue:int=0) -> str:
    """Adds a display type attribute on the given object.
    """
    # attrName = f"{object}.{name}"
    cmds.addAttr(object, longName=name, attributeType="enum", enumName="Fk=0:Ik=1", keyable=True, defaultValue=defaultValue)
    # cmds.setAttr(attrName, channelBox=True)
    cmds.setAttr(f"{object}.overrideEnabled", True)
    return f"{object}.{name}"


  @classmethod
  def addMessage(cls, object:str, name:str="component", isArray:bool=False):
    cmds.addAttr(object, longName=name, attributeType="message", multi=isArray)
    return f"{object}.{name}"


  @classmethod
  def addCtrls(cls, object:str, name:str="ctrls", addFk:bool=True, addIk:bool=True) -> str:
    attr_name = f"{object}.{name}"
    cmds.addAttr(object, longName=name, attributeType="compound", numberOfChildren=2)
    if addFk: cmds.addAttr(object, longName="fk", attributeType="message", multi=True, parent=name)
    if addIk: cmds.addAttr(object, longName="ik", attributeType="message", multi=True, parent=name)
    return (
      attr_name,
      f"{attr_name}.fk",
      f"{attr_name}.ik",
    )


  @classmethod
  def lockControlChannels(cls, object:str, lockChannels:list):
    """Locks the given attributes.
    """
    if lockChannels != []:
      singleAttributeLockList = []
      for lockChannel in lockChannels:
        if lockChannel in ["translate", "rotate", "scale"]:
          [singleAttributeLockList.append(f"{lockChannel}{axis}") for axis in ["X", "Y", "Z"]]
        else:
          singleAttributeLockList.append(lockChannel)
      [cmds.setAttr(f"{object}.{attr}", channelBox=False, keyable=False, lock=True) for attr in singleAttributeLockList]


  @classmethod
  def lockControlPlugs(cls, object:str, lockChannels:list):
    """Locks the given attributes.
    """
    mObj = omc.Core.GetObjFromString(object)
    fnObj = om.MFnDependencyNode(mObj)

    if lockChannels != []:
      singleAttributeLockList = []
      for channel in lockChannels:
        if channel in ["translate", "rotate", "scale"]:
          [singleAttributeLockList.append(f"{channel}{axis}") for axis in ["X", "Y", "Z"]]
        else:
          singleAttributeLockList.append(channel)

      for attr in singleAttributeLockList:
        plug = fnObj.findPlug(attr, False)
        plug.setKeyable(False)
        plug.setChannelBox(False)
        plug.setLocked(True)
      # [cmds.setAttr(f"{object}.{attr}", lock=True, keyable=False, channelBox=False) for attr in singleAttributeLockList]


  @classmethod
  def lockTransforms(cls, object:str, lockChannels:list=["translate", "rotate", "scale", "shear", "rotateOrder", "rotateAxis", "inheritsTransform", "offsetParentMatrix", "visibility"]):
    """Locks all transform attributes on the given object.
    """
    cls.lockControlChannels(object, lockChannels)


  @classmethod
  def editLocked(cls, name:str, value:float, relock:bool=True) -> None:
    """Allows to edit a locked attribute.
    
    Unlocks the specified attribute, edits it and then locks it again.

    Args:
      name (string): Name of the attribute.
      value (float): Value for the attribute.
    
    """
    cls.unlockIfLocked(name)
    cmds.setAttr(name, value)
    if relock: cmds.setAttr(name, lock=True)


  @classmethod
  def isLocekd(cls, name:str):
    """Unlocks the given attribute if it is locked.

    TODO:
      Replace with the api method to unlock Attributes from referenced files 8-)

    """
    return cmds.getAttr(name, lock=True)


  @classmethod
  def unlockIfLocked(cls, name:str):
    """Unlocks the given attribute if it is locked.

    TODO:
      Replace with the api method to unlock Attributes from referenced files 8-)

    """
    if cmds.getAttr(name, lock=True): cmds.setAttr(name, lock=False)



  @classmethod
  def ConnectSceneTime(cls, object:str, plug:str="inTime", doIt:bool=True) -> None:
    """Connects the scene's default time1 node to the given target.
    """
    fnTarget = om.MFnDependencyNode(omc.Object.GetObjFromString(object))
    plugInTime = fnTarget.findPlug(plug, False)
    plugTime1Out = om.MFnDependencyNode(omc.Object.GetObjFromString("time1")).findPlug("outTime", False)
    cls.dgMod.connect(plugTime1Out, plugInTime)

    if doIt: cls.dgMod.doIt()