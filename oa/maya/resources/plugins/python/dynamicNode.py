import maya.OpenMaya as om
import maya.OpenMayaMPx as ompx


class DynamicNode(ompx.MPxNode):
  """Template for user-defined nodes.

  Quick and clean template for creating custom nodes in maya. Python API 1 and 2 can
  be mixed together. For Viewport 2 overrides the core of the plugin must be written
  in Python API 2.

  """
  # Class variables
  typeName = "dynamicNode"
  typeId = om.MTypeId(0x0111)

  # Node's input attributes
  aInTime = om.MObject()
  aWeight = om.MObject()
  aGoal = om.MObject()
  aDamping = om.MObject()
  aStiffness = om.MObject()
  aParentInverse = om.MObject()

  # Node's output attributes
  aOutput = om.MObject()


  def __init__(self):
    super(DynamicNode, self).__init__()
    self._initialized = False
    self._currentPosition = om.MPoint()
    self._previousPosition = om.MPoint()
    self._previousTime = om.MTime()


  def compute(self, plug, dataBlock):
    """Recompute the given output based on the nodes inputs.

    The plug represents the	data value that needs to be recomputed, and the data
    block holds the storage for all of the node"s attributes.

    Args:
      plug (MPlug): plug representing the attribute that needs to be recomputed
      dataBlock (MDataBlock): The node"s datablock

    """
    if plug != DynamicNode.aOutput:
      return om.kUnknownParameter

    # Get the inputs
    weight = dataBlock.inputValue(self.aWeight).asFloat()
    currentTime = dataBlock.inputValue(self.aInTime).asTime()
    damping = dataBlock.inputValue(self.aDamping).asFloat()
    stiffness = dataBlock.inputValue(self.aStiffness).asFloat()
    goal = om.MPoint(dataBlock.inputValue(self.aGoal).asFloatVector())
    parentInverse = dataBlock.inputValue(self.aParentInverse).asMatrix()

    if not self._initialized:
      self._previousTime = currentTime
      self._currentPosition = goal
      self._previousPosition = goal
      self._initialized = True

    # Check if the timestep is just 1 frame since we want a stable simulation
    timeDifference = currentTime.value() - self._previousTime.value()
    if timeDifference > 1.0 or timeDifference < 0.0:
      self._initialized = False
      self._previousTime = currentTime
      dataBlock.setClean(plug)
      return

    velocity = (self._currentPosition - self._previousPosition) * (1.0 - damping)
    newPosition = self._currentPosition + velocity
    goalForce = (goal - newPosition) * stiffness
    newPosition += goalForce

    # Store the states for the next computation
    self._previousPosition = om.MPoint(self._currentPosition)
    self._currentPosition = om.MPoint(newPosition)
    self._previousTime = om.MTime(currentTime)

    newPosition = goal + ((newPosition - goal) * weight)

    # Put in the output local space
    newPosition *= parentInverse

    hOutput = dataBlock.outputValue(DynamicNode.aOutput)
    outVector = om.MFloatVector(newPosition.x, newPosition.y, newPosition.z)
    hOutput.setMFloatVector(outVector)
    hOutput.setClean()
    dataBlock.setClean(plug)


  @classmethod
  def creator(cls):
    """Creator of the node.

    Method called by Maya to create an instance of this class.

    Returns:
      Class (Instance): Instance of the class that has been created

    """
    return ompx.asMPxPtr(DynamicNode())


  @classmethod
  def initialize(cls):
    """Node initializer.

    This method initializes the node, and should be overridden in user-defined nodes.

    Returns:
      bool: True if the operation was successful, otherwise False

    """
    nAttr = om.MFnNumericAttribute()
    uAttr = om.MFnUnitAttribute()
    mAttr = om.MFnMatrixAttribute()
    
    cls.aWeight = nAttr.create("weight", "wgt", om.MFnNumericData.kFloat, 1.0)
    nAttr.setKeyable(True)
    nAttr.setMin(0.0)
    nAttr.setMax(1.0)

    cls.aInTime = uAttr.create("inTime", "itm", om.MFnUnitAttribute.kTime, 0.0)
    nAttr.setKeyable(True)
    nAttr.setReadable(False)

    cls.aGoal = nAttr.createPoint("goal", "gol")

    cls.aStiffness = nAttr.create("stiffness", "stf", om.MFnNumericData.kFloat, 0.1)
    nAttr.setKeyable(True)
    nAttr.setMin(0.0)
    nAttr.setMax(1.0)

    cls.aDamping = nAttr.create("damping", "dmp", om.MFnNumericData.kFloat, 0.1)
    nAttr.setKeyable(True)
    nAttr.setMin(0.0)
    nAttr.setMax(1.0)

    cls.aParentInverse = mAttr.create("parentInverseMatrix", "pim")

    cls.aOutput = nAttr.createPoint("output", "out")
    nAttr.setWritable(False)
    nAttr.setStorable(False)

    cls.addAttribute(cls.aWeight)
    cls.addAttribute(cls.aInTime)
    cls.addAttribute(cls.aGoal)
    cls.addAttribute(cls.aStiffness)
    cls.addAttribute(cls.aDamping)
    cls.addAttribute(cls.aParentInverse)
    cls.addAttribute(cls.aOutput)

    cls.attributeAffects(cls.aWeight, cls.aOutput)
    cls.attributeAffects(cls.aInTime, cls.aOutput)
    cls.attributeAffects(cls.aGoal, cls.aOutput)
    cls.attributeAffects(cls.aStiffness, cls.aOutput)
    cls.attributeAffects(cls.aParentInverse, cls.aOutput)
    cls.attributeAffects(cls.aDamping, cls.aOutput)





def initializePlugin(pluginObj):
  """Initialize the script plug-in.

  Args:
    pluginObj (MObject): The plugin to initialize

  """
  fnPlugin = ompx.MFnPlugin(pluginObj, "Lukasz Biernat", "1.0.0")
  fnPlugin.registerNode(
    DynamicNode.typeName,
    DynamicNode.typeId,
    DynamicNode.creator,
    DynamicNode.initialize,
    ompx.MPxNode.kDependNode
  )


def uninitializePlugin(pluginObj):
  """Unintialize the script plug-in.

  Args:
    pluginObj (MObject): The plugin to uninitialize

  """
  fnPlugin = ompx.MFnPlugin(pluginObj)
  fnPlugin.deregisterNode(DynamicNode.typeId)




def testScene(pluginName):
  """Example test code to be executed in evalDeferred.

  Args:
    pluginName(str): Name of the plugin to load
  """
  cmds.file(new=True, force=True)
  # If plugin is loaded - unload it
  if cmds.pluginInfo(pluginName, query=True, loaded=True):
    cmds.unloadPlugin(pluginName)
  # If plugin is not loaded - load it 
  if not cmds.pluginInfo(pluginName, query=True, loaded=True):
    cmds.loadPlugin(pluginName)

  loc = cmds.spaceLocator()[0]
  dynNode = cmds.createNode("dynamicNode")
  sphere = cmds.polySphere()

  cmds.connectAttr(f"{loc}.translate", f"{dynNode}.goal")
  cmds.connectAttr("time1.outTime", f"{dynNode}.inTime")
  cmds.connectAttr(f"{dynNode}.output", f"{sphere[0]}.translate")



if __name__ == "__main__":
  """For Development Only.

  Test code to accelerate reloading and testing the plugin.

  """
  from maya import cmds
  cmds.evalDeferred('testScene(pluginName="dynamicNode.py")')
