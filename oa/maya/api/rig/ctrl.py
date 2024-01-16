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
import oa.maya.resources.templates.ctrls as omrtc





dict_colors = {
  "lightyellow":  [1.0, 1.0, 0.25],
  "yellow":       [1.0, 0.6, 0.1],
  "lightorange":  [1.0, 0.467, 0.2],
  "orange":       [0.8, 0.25, 0.05],
  "lightblue":    [0.4, 0.8, 1.0],
  "blue":         [0.05, 0.25, 0.8],
  "magenta":      [0.6, 0.2, 0.4],
  "green":        [0.2, 0.8, 0.4]
}




class Ctrl():
  """Class for building the rig controller.
  """
  def __init__(self,
    template:str=None,
    name:str="new_ctrl",
    parent="",
    translateTo="",
    rotateTo="",
    localPosition=(0.0,	0.0, 0.0),
    localRotate=(0.0, 0.0, 0.0),
    localScale=(1.0, 1.0, 1.0),
    shape="circle",
    drawSolverMode=False,
    solverModePosition=(0.0, 0.0, 0.0),
    drawLine=False,
    drawLineTo="",
    lineWidth=2,
    color="lightyellow",
    lockShapeAttributes=True,
    lockChannels=["scale", "visibility"]
  ):
    """Init method of the Controller class.

    Args:
      name (string): Name of the controller.
      localScale (float): Scale of the rigController - edits localScaleXYZ attrs on the shape node.
      translateTo (string): Reference object the controller's position.
      rotateTo (string): Reference object for the controller's orientation.
      parent (string): Name of the parent object of the controller.
      shape (string): Name of the display shape of the controller.
      fillShape (bool): Whether or not you want to draw a full shape or just the wireframe outline.
      fillTransparency (float): Transparency of the fill shape.
      lineWidth (float): Line width of the wireframe outline.
      lockChannels (list[string]): List with attribute names to be locked.

    """
    if template and template in omrtc.templates:
      # if template in omrtc.templates:
      self.node = cmds.ctrl(
        name=name,
        parent=parent,
        translateTo=translateTo,
        rotateTo=rotateTo,
        localPosition=omrtc.templates[template]["localPosition"],
        localRotate=omrtc.templates[template]["localRotate"],
        localScale=omrtc.templates[template]["localScale"],
        shape=omrtc.templates[template]["shape"],
        drawSolverMode=omrtc.templates[template]["drawSolverMode"],
        solverModePosition=omrtc.templates[template]["solverModePosition"],
        drawLine=omrtc.templates[template]["drawLine"],
        drawLineTo=drawLineTo,
        lineWidth=omrtc.templates[template]["lineWidth"],
        color=omrtc.templates[template]["color"],
        lockShapeAttributes=omrtc.templates[template]["lockShapeAttributes"],
      )
    else:
      self.node = cmds.ctrl(
        name=name,
        parent=parent,
        translateTo=translateTo,
        rotateTo=rotateTo,
        localPosition=localPosition,
        localRotate=localRotate,
        localScale=localScale,
        shape=shape,
        drawSolverMode=drawSolverMode,
        solverModePosition=solverModePosition,
        drawLine=drawLine,
        drawLineTo=drawLineTo,
        lineWidth=lineWidth,
        color=color,
        lockShapeAttributes=lockShapeAttributes,
      )