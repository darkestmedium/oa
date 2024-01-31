# Built-in imports
import logging

# Third-party imports
from maya import cmds
import maya.api.OpenMaya as om

# OpenAPi imports
import oa.maya.Core as omc




class Build():
  """Base template class for building control rigs.
  """

  def __init__(self, name:str, filePath:str, template:dict): ...


  def build(): ...


  def createCtrls(): ...
  
