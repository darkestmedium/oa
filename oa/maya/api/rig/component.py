# Built-in imports
import logging

# Third-party imports
from maya import cmds

# OpenAPi imports
import oa.maya.Core as omc
import oa.maya.Rig as omr
# OpenAPi constants 
from oa.maya import Color
from oa.maya import Lock




#---------------------------------------------------------------------------------------------------
# Components
#---------------------------------------------------------------------------------------------------




class Component():
  """Base component class.
  """
  side = "center"
  cola = "lightyellow"
  colb = "yellow"
  suff: ...


  def getSide(self, side:str="center"):
    match side:
      case "left":
        self.suff = "_l"
        self.cola = Color.LightOrange
        self.colb = Color.Orange
      case "center":
        self.suff = ""
        self.cola = Color.LightYellow
        self.colb = Color.Yellow
      case "right":
        self.suff = "_r"
        self.cola = Color.LightBlue
        self.colb = Color.Blue


  def getCtrls(self):
    """Returns component's ctrls.
    """
    ...





class Base():
  """Top rig component class.
  """
  typeAsset = "rig"

  # Constructor
  def __init__(self, name:str="Base") -> None:
    self.name = name
    self.comp = cmds.component(name="rig")
    self.main = omr.Ctrl(template="base", name="main", parent=self.comp)
    self.root = omr.Ctrl(template="root", name="root", parent=self.main.node)
    omr.Utils.createSpaceSwitch(self.root.node, drivers=[(self.main.node, "Main"), (self.comp, "World")])
    self.geo = cmds.component(name="geo", parent=self.comp)
    [omc.Attribute.lockControlChannels(obj, Lock.TransformsVisibility) for obj in [self.comp, self.geo]]

    self.setupBaseAttributes()


  def setupBaseAttributes(self):
    """Creates visibility and display type attributes on the main cotroller and connects them.
    """
    nameAttr = "name"
    sceneObjectTypeAttr = "sceneObjectType"
    [cmds.addAttr(self.comp, longName=attr, dataType="string") for attr in [nameAttr, sceneObjectTypeAttr]]
    cmds.setAttr(f"{self.comp}.{nameAttr}", self.name, type="string", lock=True)
    cmds.setAttr(f"{self.comp}.{sceneObjectTypeAttr}", self.asset_type, type="string", lock=True)

    omc.Attribute.addSeparator(self.main.node)

    self.displCtrls = cmds.createNode("displayLayer", name="ctrls_displ")
    cmds.setAttr(f"{self.displCtrls}.enabled", True)
    cmds.setAttr(f"{self.displCtrls}.overrideRGBColors", True)
    cmds.setAttr(f"{self.displCtrls}.overrideColorR", Color.LightYellow[0])
    cmds.setAttr(f"{self.displCtrls}.overrideColorG", Color.LightYellow[1])
    cmds.setAttr(f"{self.displCtrls}.overrideColorB", Color.LightYellow[2])
    self.displGeo = cmds.createNode("displayLayer", name="geo_displ")
    self.displJnts = cmds.createNode("displayLayer", name="outs_displ")

    # Visibility
    # Main Ctrls
    self.attrVisCtrls = omc.Attribute.addOnOff(self.main.node, "ctrlsVisibility")
    cmds.connectAttr(self.attrVisCtrls, f"{self.displCtrls}.visibility")
    cmds.connectAttr(f"{self.displCtrls}.drawInfo", f"{self.main.node}.drawOverride", force=True)

    # Meshes
    self.attrVisGeo = omc.Attribute.addOnOff(self.main.node, "geoVisibility")
    cmds.connectAttr(self.attrVisGeo, f"{self.displGeo}.visibility")
    cmds.connectAttr(f"{self.displGeo}.drawInfo", f"{self.geo}.drawOverride", force=True)
  
    # Export Skeleton
    self.attrVisBones = omc.Attribute.addOnOff(self.main.node, "bonesVisibility", False)
    cmds.connectAttr(self.attrVisBones, f"{self.displJnts}.visibility")

    omc.Attribute.addSeparator(self.main.node, "__")

    # Diplay Type Overrides
    # Main Ctrl
    self.attrDtCtrls = omc.Attribute.addDisplayType(self.main.node, "ctrlsDisplayType")
    cmds.connectAttr(self.attrDtCtrls, f"{self.displCtrls}.displayType")
    cmds.connectAttr(f"{self.displCtrls}.displayType", f"{self.main.node}.overrideDisplayType")

    # Geoes
    self.attrDtGeo = omc.Attribute.addDisplayType(self.main.node, "geoDisplayType", 2)
    cmds.connectAttr(self.attrDtGeo, f"{self.displGeo}.displayType")
    cmds.connectAttr(f"{self.displGeo}.displayType", f"{self.geo}.overrideDisplayType")

    # Export Skeleton
    self.attrDtBones = omc.Attribute.addDisplayType(self.main.node, "exportSkeletonDisplayType", 2)
    cmds.connectAttr(self.attrDtBones, f"{self.displJnts}.displayType")

    omc.Attribute.addSeparator(self.main.node, "___")

    # Hide Ctrls On Playback
    self.attrHideCtrlsOnPlayback = omc.Attribute.addOnOff(self.main.node, "hideCtrlsOnPlayback", False)
    cmds.connectAttr(self.attrHideCtrlsOnPlayback, f"{self.displCtrls}.hideOnPlayback")
    cmds.connectAttr(f"{self.displCtrls}.hideOnPlayback", f"{self.main.node}.hideOnPlayback")




class Pelvis(Component):
  """Class for building the fk pelvis component.
  """

  def __init__(self, compBase, listJoints:dict) -> None:
    """Class constructor.

    Args:
      parent (string): Parent of the component to be parented to.

    """
    self.comp = cmds.component(name="pelvis_comp", parent=compBase.main.node)
    omc.Attribute.lockTransforms(self.comp)

    # Main chain
    self.ctrlPelvis = omr.Ctrl(
      name="pelvis_ctrl",
      template="pelvis",
      parent=self.comp,
      translateTo=listJoints["Root"],
      rotateTo=listJoints["Root"],
      localScale=(1.75, 1.75, 1.75)
    )
    self.ctrlPelvisRot = omr.Ctrl(
      name="{}_rot_ctrl".format(listJoints["Root"]),
      parent=self.ctrlPelvis.node,
      translateTo=listJoints["Root"],
      rotateTo=listJoints["Root"],
      localScale=(1.75, 1.75, 1.75)
    )
    [omc.Attribute.copyTransformsToOPM(ctrl.node) for ctrl in self.getCtrls()]

    omr.Utils.createSpaceSwitch(
      self.ctrlPelvis.node,
      drivers=[
        (compBase.root.node, "Root Motion"),
        (compBase.main.node, "Main"),
        (compBase.comp, "World"),
      ],
    )

    # Lock attributes
    omc.Attribute.lockControlChannels(self.ctrlPelvis.node, lockChannels=["scale", "visibility"])
    omc.Attribute.lockControlChannels(self.ctrlPelvisRot.node, lockChannels=["translate", "scale", "visibility"])

    # Message Attrs Setup
    [cmds.connectAttr(f"{ctrl.node}.component", f"{self.comp}.ctrls.fk[{indx}]") for ctrl, indx in zip(self.getCtrls(), range(self.getCtrls().__len__()))]
    cmds.setAttr(f"{self.comp}.ctrls", lock=True)


  def getCtrls(self):
    """Returns component's ctrls.
    """
    return (self.ctrlPelvis, self.ctrlPelvisRot)




#---------------------------------------------------------------------------------------
# FK Components
#---------------------------------------------------------------------------------------




class FkArray(Component):
  """Class for building the fk chains component.
  """

  def __init__(self, parent, listJoints, side:str="center") -> None:
    """Class constructor.

    Notes:
      You can have Spine1, Spine2, Spine3 ... and the rest missing but not
      Spine1, Spine2 - does not exist, Spine4 ...

    Args:
      parent (string): Parent of the component to be parented to.

    """
    self.getSide(side)
    self.ctrls = []
    for indx, dict in enumerate(listJoints.items()):
      if cmds.objExists(dict[1]):
        self.ctrl = omr.Ctrl(
          name=f"{dict[1]}{self.suff}_ctrl",
          parent=self.ctrls[indx-1].node if indx > 0 else parent,
          translateTo=dict[1],
          rotateTo=dict[1],
          color=self.cola
        )
        self.ctrls.append(self.ctrl)

    [omc.Attribute.copyTransformsToOPM(ctrl.node) for ctrl in self.ctrls]

