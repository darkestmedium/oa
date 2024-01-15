# Built-in imports

# Third-party imports

# OAPi imports
import oa.maya.api.retarget.humanik




class MetaHuman(oa.maya.api.retarget.humanik.HumanIk):
  """Class for setting up the MetaHuman skeleton in Maya.

  Contains full hik joint definition which propagates down to the UE5 and UE4 Skeleton
  TODO replace print with logger
  TODO Bake to Ik and Fk?
  TODO split the metahuman rig into body and rig file

  """
  # minimalDefinition = lmrrue.templateMH["minimalDefinition"]
  # definition = lmrrue.templateMH["definition"]
  # hikTemplate = "MetaHuman"
  # tPose = lmrrue.templateMH["tPose"]
  # aPose = lmrrue.templateMH["aPose"]




class Mannequin5(MetaHuman):
  """Class for setting up the ue5 mannequin skeleton.
  """




class Mannequin4(MetaHuman):
  """Class for setting up the ue4 mannequin skeleton.
  """
