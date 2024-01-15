# Built-in imports
from typing import final

# Third-party imports

# OAPi imports
import oa.maya.api.retarget.humanik
import oa.maya.api.retarget.ctrl
import oa.maya.api.retarget.unreal




@final
class HumanIk(oa.maya.api.retarget.humanik.HumanIk):
  """Import wrapper for the oa.maya.api.retarget.humanik.HumanIk class.
  """




@final
class Ctrl(oa.maya.api.retarget.ctrl.Ctrl):
  """Import wrapper for the oa.maya.api.retarget.ctrl.Ctrl class.
  """




@final
class Skeleton(oa.maya.api.retarget.ctrl.Skeleton):
  """Import wrapper for the oa.maya.api.retarget.ctrl.Skeleton class.
  """




@final
class MetaHuman(oa.maya.api.retarget.unreal.MetaHuman):
  """Import wrapper for the oa.maya.api.retarget.unreal.MetaHuman class.
  """




@final
class Mannequin5(oa.maya.api.retarget.unreal.Mannequin5):
  """Import wrapper for the oa.maya.api.retarget.unreal.Mannequin5 class.
  """




@final
class Mannequin4(oa.maya.api.retarget.unreal.Mannequin4):
  """Import wrapper for the oa.maya.api.retarget.unreal.Mannequin4 class.
  """


