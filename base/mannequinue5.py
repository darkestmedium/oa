"""
template rig build script

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

# External
# sys.path.append("~/Dropbox/code/legacy/3d/Maya/codeReference/cmt-master/scripts")
# import cmt.rig.spaceswitch as spaceswitch


# Reloads 
[importlib.reload(module) for module in [omc, omr, omrtg]]




def build(rigName="MannequinUe5"):
  rig = omr.CompBase(rigName)




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

  mannequinue5.build()

  cmds.viewFit(all=True)
  # cmds.modelEditor("modelPanel4", edit=True, jointXray=True)
