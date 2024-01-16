# Built-in imports
import json
import platform
import logging

# Third-party imports

# Maya imports
from maya import cmds
from maya import mel
import maya.api.OpenMaya as om

from PySide2 import QtCore as qtc

# Open APi imports





class Namespace(om.MNamespace):
  """Wrapper for the MNamespace class.
  """
  log = logging.getLogger("Namespace")


  @classmethod
  def GetNamespaceFromName(cls, name:str, include_colon:bool=True, *args) -> str:
    """Overrides getNamespaceFromName to return an empty string if there is no namespace in the name.

    Note:
      Maya's default getNamespaceFromName returns (kFailure): Unexpected Internal Failure if an
        empty string is passed.

    Returns:
      str: "namespace" or "asd:asd" or "namespace:" based on args, if no namespace was found and empty
        string will be returned instead "".

    """
    if name != "":
      name = om.MNamespace.getNamespaceFromName(name)
      if name != "" and include_colon:
        name = f"{name}:"
    return name


  @classmethod
  def GetNameWIthNamespace(cls, name:str, namespace:str, *args) -> str:
    """Returns name with the give namespace, this method doesn't create the namespace in the scene.

    Args:
      name (str): Name of the node/object.
      namespace (str): Name of the node/object.

    Returns:
      str: Name with namespace will be returned if namespace is not empty, otherwise just the name.

    """
    if namespace != "":	return f"{namespace}:{name}"
    return name


  @classmethod
  def GetNameWithoutNamespace(cls, name:str, *args) -> str:
    """Returns name without the specified namesapce, this method doesn't delete the namespace in the scene.

    Args:
      name (str): Name of the node/object.

    Returns:
      str: If the namespace is not an empty string, name with namespace will be returned. Otherwise
        just the name.

    """
    if name != "": name = om.MNamespace.stripNamespaceFromName(name)
    return name


  @classmethod
  def GetSelectionNamespace(cls) -> str or None:
    """Returns the namespace found in the current selection."""
    selection = cmds.ls(selection=True)
    if selection:
      namespace = cls.get_namespace_from_name(selection[0])
      # namespace = Namespace.get_namespace_from_name(selection[0])
      cls.log.warning("Selection has no namespace, an empty string will be returned")
      return namespace
    cls.log.warning("Nothing is currently selected.")
    return None
