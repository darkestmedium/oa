""" template rig build script
"""

# Built-in imports
import sys
import importlib
import platform

# Third-party imports
from maya import cmds
import maya.OpenMaya as om

# OpenAPi imports
import oa.maya.Core as omc
import oa.maya.Rig as omr
import oa.maya.Retarget as omrtg
import oa.maya.api.template as omat

# External
# sys.path.append("~/Dropbox/code/legacy/3d/Maya/codeReference/cmt-master/scripts")
# import cmt.rig.spaceswitch as spaceswitch


# Reloads 
[importlib.reload(module) for module in [omc, omr, omrtg]]



class BuildMannequinUe5(omr.Build):


  # Constructor
  def __init__(self, filePath:str, name:str, template, upAxis:str="Y") -> None:
    self.name = name
    self.filePath = filePath
    self.template = template
  

  def build(): ...










if __name__ == "__main__":
  """For Development Only.

  Test code to accelerate reloading and testing the plugin.

  """
  import importlib
  from maya import cmds
  from base import mannequinue5

  # OpenAPi imports
  import oa.maya.Core as omc
  import oa.maya.Rig as omr
  import oa.maya.Retarget as omrtg

  # Reloads
  [importlib.reload(module) for module in [omc, omr, omrtg]]
  importlib.reload(mannequinue5)

  cmds.file(new=True, force=True)

  rig = BuildMannequinUe5(
    filePath="/home/darkest/Bambaa/Content/Sinners/Characters/Manny/Meshes/SKM_Manny_Simple.FBX",
    name="Manny",
    template=omat.MannequinUe5,
  )

  cmds.viewFit(all=True)
