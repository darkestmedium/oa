# Built-in imports
import logging

# Third-party imports
from maya import cmds

# OpenAPi imports
import oa.maya.Core as omc
import oa.maya.Rig as omr




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




#--------------------------------------------------------------------------------------------------
# Components
#--------------------------------------------------------------------------------------------------




class Base():
  """Top rig component class.
  """
  asset_type = "rig"

  # Constructor 
  def __init__(self, name:str="Base") -> None:
    self.name = name
    self.comp = cmds.component(name="rig")
    self.main = omr.Ctrl(template="base", name="main", parent=self.comp)
    self.root = omr.Ctrl(template="root", name="root", parent=self.main.node)
    omr.Utils.create_space_switch(self.root.node, drivers=[(self.main.node, "Main"), (self.comp, "World")])
    self.geo = cmds.component(name="geo", parent=self.comp)

    self.setup_base_attrs()
    [omc.Attribute.lockControlChannels(obj, ["translate", "rotate", "scale", "visibility"]) for obj in [self.comp, self.geo]]


  def setup_base_attrs(self):
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
    cmds.setAttr(f"{self.displCtrls}.overrideColorR", dict_colors["lightyellow"][0])
    cmds.setAttr(f"{self.displCtrls}.overrideColorG", dict_colors["lightyellow"][1])
    cmds.setAttr(f"{self.displCtrls}.overrideColorB", dict_colors["lightyellow"][2])
    self.displGeo = cmds.createNode("displayLayer", name="geo_displ")
    self.displJnts = cmds.createNode("displayLayer", name="outs_displ")

    # Visibility
    # Main Ctrls
    self.attrCtrlsVisibility = omc.Attribute.addOnOff(self.main.node, "ctrlsVisibility")
    cmds.connectAttr(self.attrCtrlsVisibility, f"{self.displCtrls}.visibility")
    cmds.connectAttr(f"{self.displCtrls}.drawInfo", f"{self.main.node}.drawOverride", force=True)

    # Meshes
    self.attrGeoVisibility = omc.Attribute.addOnOff(self.main.node, "geoVisibility")
    cmds.connectAttr(self.attrGeoVisibility, f"{self.displGeo}.visibility")
    cmds.connectAttr(f"{self.displGeo}.drawInfo", f"{self.geo}.drawOverride", force=True)
  
    # Export Skeleton
    self.attrExportSkeletonVisibility = omc.Attribute.addOnOff(self.main.node, "exportSkeletonVisibility", False)
    cmds.connectAttr(self.attrExportSkeletonVisibility, f"{self.displJnts}.visibility")

    omc.Attribute.addSeparator(self.main.node, "__")

    # Diplay Type Overrides
    # Main Ctrl
    self.attrCtrlsDisplayType = omc.Attribute.addDisplayType(self.main.node, "ctrlsDisplayType")
    cmds.connectAttr(self.attrCtrlsDisplayType, f"{self.displCtrls}.displayType")
    cmds.connectAttr(f"{self.displCtrls}.displayType", f"{self.main.node}.overrideDisplayType")

    # Geoes
    self.attrGeoDisplayType = omc.Attribute.addDisplayType(self.main.node, "geoDisplayType", 2)
    cmds.connectAttr(self.attrGeoDisplayType, f"{self.displGeo}.displayType")
    cmds.connectAttr(f"{self.displGeo}.displayType", f"{self.geo}.overrideDisplayType")

    # Export Skeleton
    self.attrExportSkeletonDisplayType = omc.Attribute.addDisplayType(self.main.node, "exportSkeletonDisplayType", 2)
    cmds.connectAttr(self.attrExportSkeletonDisplayType, f"{self.displJnts}.displayType")

    omc.Attribute.addSeparator(self.main.node, "___")

    # Hide Ctrls On Playback
    self.attrHideCtrlsOnPlayback = omc.Attribute.addOnOff(self.main.node, "hideCtrlsOnPlayback", False)
    cmds.connectAttr(self.attrHideCtrlsOnPlayback, f"{self.displCtrls}.hideOnPlayback")
    cmds.connectAttr(f"{self.displCtrls}.hideOnPlayback", f"{self.main.node}.hideOnPlayback")
