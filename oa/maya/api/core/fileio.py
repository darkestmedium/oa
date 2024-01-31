# Built-in imports
import json
import platform
import logging

# Third-party imports

# Maya imports
from maya import cmds
from maya import mel
import maya.OpenMaya as om
import maya.OpenMayaAnim as oma
from PySide2 import QtCore as qtc

# Open APi imports
import oa.maya.Core as omc




class FileIO():
  """Wrapper for file operations."""
  log = logging.getLogger("FileIO")


  @classmethod
  def new(cls, force:bool=False) -> bool:
    """Open a new file.
    """
    if not force:
      if cmds.file(query=True, modified=True):
        inputDialog = cls.dialog(
          message=f"Save changes to '{om.MFileIO.currentFile()}' ?",
          title="Save Changes",
          icon="question",
          buttons=["Save", "Don't Save", "Cancel"],
          cancelButton="Cancel",
        )
        if inputDialog == "Save":
          if not cmds.file(query=True, sceneName=True, shortName=False):
            mel.eval("SaveSceneAs;")
            return True
          om.MFileIO.save()
        if inputDialog == "Cancel":
          return False

    # if force and file is not modified:	
    om.MFileIO.newFile(True)
    return True


  @classmethod
  def load(cls, filePath:str, nameSpace:str=None, reference:bool=True):
    """Import file wrapper method for fbx and maya native file formats.
    
    If we are importing an fbx file we don't want a namespace in order to import / update
    the animation in the scene. Since importing fbx files with a namespace will bring in
    a new set of joints it is unwanted behaviour for this case. 
    For the maya files on the other hands we need a namespace rig as the target.

    """
    if reference:
      om.MFileIO.reference(filePath, False, False, nameSpace)
      return True

    # Legacy batch retarget support
    if filePath.endswith('.fbx'):
      om.MFileIO.importFile(filePath)
    if filePath.endswith('.ma') or filePath.endswith('.mb'):
      cls.reference(filePath, nameSpace)


  @classmethod
  def importFile(cls, filePath:str, nameSpace:str, importTimeRange:str="combine", importFrameRate:bool=False):
    """Imports the specified file into the current scene.
    """
    cmds.file(
      filePath,
      namespace=nameSpace,
      i=True,
      importTimeRange=importTimeRange,
      importFrameRate=importFrameRate,
    )


  @classmethod
  def reference(cls, filePath:str, nameSpace:str, lockReference:bool=False, deferReference:bool=False) -> str:
  # def reference(cls, filePath:str, nameSpace:str) -> str:
    """Adds the speciefied file as a reference to the current scene.

    Args:
      filePath (string): name of the file to add as a reference.
      nameSpace (string):	name space to add the contents of the referenced file to. If no namespace is provided, name-clashes will be resolved by prefixing with the filename.
      lockReference (bool):	If true, all nodes and attributes from the referenced file will be locked.
      deferReference (bool): boolean to indicate whether loading has to be deferred.

    """
    om.MFileIO.reference(filePath, deferReference, lockReference, nameSpace)

    return cmds.file(filePath, query=True, referenceNode=True)


  @classmethod
  def importReference(cls, filePath:str):
    """Ïmports the specified reference.
    """
    cmds.file(filePath, importReference=True)


  @classmethod
  def removeReference(cls, filePath:str, deleteNamespaceContent:bool=True):
    """Removes the specified reference from the scene.
    """
    if deleteNamespaceContent:
      om.MFileIO.removeReference(filePath, om.MFileIO.kForceDeleteNamespaceContent)
      return

    om.MFileIO.removeReference(filePath)


  @classmethod
  def getReferenceNodesByType(cls, filePath:str, nodeType:str="joint") -> list:
    """Returns the nodes of a specified type from the given reference node.

    """
    listReferenceNodes = []
    om.MFileIO.getReferenceNodes(filePath, listReferenceNodes)
    # Filter out node type
    listNodesByTpe = []
    [listNodesByTpe.append(node) for node in listReferenceNodes if cmds.nodeType(node) == nodeType]

    return listNodesByTpe


  @classmethod
  def stageFromFile(cls, filePath:str):
    """Loads a usd stage from the specified file path.
    """
    nodeUsd = cmds.createNode("mayaUsdProxyShape", name="stageShape", skipSelect=True)
    cmds.setAttr(f"{nodeUsd}.filePath", filePath, type="string")
    # cmds.connectAttr(f"time1.outTime", f"{nodeUsd}.time")
    omc.Attribute.ConnectSceneTime(nodeUsd, "time")

    return nodeUsd


  @classmethod
  def loadDataFromJson(cls, filePath:str) -> dict:
    """Loads data from a json file.
    """
    with open(filePath, "r") as file:
      return json.loads(file.read())


  @classmethod
  def importDialog(cls,
      startingDirectory:str=qtc.QStandardPaths.writableLocation(qtc.QStandardPaths.DesktopLocation),
      fileFilter:str="FBX ( .fbx ) (*.fbx)",
    ) -> str:
    return cmds.fileDialog2(
      fileMode=4,
      dialogStyle=2,
      okCaption="Import",
      fileFilter=fileFilter,
      startingDirectory=startingDirectory,
    )


  @classmethod
  def exportDialog(cls,
      startingDirectory:str=qtc.QStandardPaths.writableLocation(qtc.QStandardPaths.DesktopLocation),
      fileFilter:str="FBX ( .fbx ) (*.fbx)",
    ) -> str:
    return cmds.fileDialog2(
      fileMode=0,
      dialogStyle=2,
      okCaption="Export",
      fileFilter=fileFilter,
      startingDirectory=startingDirectory,
    )


  @classmethod
  def characterDialog(cls) -> str or None:
    result = cmds.promptDialog(
      title='Character Namespace',
      message='Enter Namespace:',
      button=['OK', 'Cancel'],
      defaultButton='OK',
      cancelButton='Cancel',
      dismissString='Cancel',
    )
    if result == 'OK':
      return cmds.promptDialog(query=True, text=True)
    
    om.MGlobal.displayWarning("Import operation was canceled")
    return None


  @classmethod
  def dialog(cls,
      message="Default Message",
      title="Default Title",
      icon="question",
      buttons=["Install", "Cancel"],
      cancelButton="Cancel"
    ) -> str:
    """Convinience wrapper method for creating confirm dialogs.

    Returns:
      str: Input from user as string e.g. "Install" or "Cancel".

    """
    return cmds.confirmDialog(
      title=title,
      message=message,
      icon=icon,
      button=buttons,
      cancelButton=cancelButton,
      dismissString=cancelButton
    )

