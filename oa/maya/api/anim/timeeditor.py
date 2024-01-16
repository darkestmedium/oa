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





class TimeEditor():
  """Wrapper class for the time editor.

  TODO:
    Get all TimeEditor nodes

  """
  composition = "Composition1"


  def __init__(self) -> None:
    if not cmds.ls(type="timeEditorTracks"): cmds.timeEditorComposition(self.composition, createTrack=True)


  @classmethod
  def CreateClip(cls, objects:list, name:str, startFrame:int=None) -> None:
    """Creates a time editor clip for the ctrl rig.

    Args:
      name (str): Name of the animation clip.
      startFrame (int): Starting frame of the animation clip

    """
    if not startFrame: start_frame = oma.MAnimControl.minTime().value()

    list_attrs = []
    for object in objects:
      list_keyable = [attr.split("|")[-1] for attr in cmds.listAnimatable(object)]
      [list_attrs.append(attr) for attr in list_keyable]

    cmds.timeEditorClip(
      name,
      addObjects=";".join(list_attrs),
      startTime=start_frame,
      removeSceneAnimation=True,
      includeRoot=False,
      recursively=False,
      track=f"{cmds.timeEditorComposition(query=True, active=True)}:-1"
    )