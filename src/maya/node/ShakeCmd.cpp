#include "ShakeCmd.hpp"



// Public Data
const char* ShakeCmd::commandName = "shake";

// Command's Flags
const char* ShakeCmd::nameFlagShort = "-n";
const char* ShakeCmd::nameFlagLong = "-name";

const char* ShakeCmd::attributeFlagShort = "-a";
const char* ShakeCmd::attributeFlagLong = "-attribute";

const char* ShakeCmd::helpFlagShort = "-h";
const char* ShakeCmd::helpFlagLong = "-help";



MSyntax ShakeCmd::syntaxCreator() 
{
  /* Creates the command's syntax object and returns it.

  Returns:
    syntax (MSyntax): Command's syntax object

  */
  MSyntax sytnax;

  sytnax.addFlag(nameFlagShort, nameFlagLong, MSyntax::kString);
  sytnax.addFlag(attributeFlagShort, attributeFlagLong, MSyntax::kString);

  sytnax.setObjectType(MSyntax::kSelectionList, 0, 255);
  sytnax.useSelectionAsDefault(true);
  sytnax.enableEdit(false);
  sytnax.enableQuery(false);

  return sytnax;
}


void ShakeCmd::displayHelp() 
{
  /* Displays help for the command. */
  MString helpStr;
  helpStr += "Flags:\n";
  helpStr += "   -n -name          String     Name of the shake node to create.\n";
  helpStr += "   -a -attribute     String     Name of the attribute to shake.\n";
  helpStr += "   -h -help          N/A        Display this text.\n";
  MGlobal::displayInfo(helpStr);
}


MStatus ShakeCmd::gatherFlagArguments(const MArgList& argList) {
  /* Parses the commands's flag arguments.

  Args:
    argList (MArglist): List of arguments passed to the command.

  Returns:
    status code (MStatus): kSuccess if the command was successful, kFailure if an error occured
      during the command.

  */
  MStatus status;

  MArgDatabase argData(syntax(), argList);
  argData.getObjects(_selList);

  if (argData.isFlagSet(nameFlagShort)) {
    _shakeName = argData.flagArgumentString(nameFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }

  if (argData.isFlagSet(attributeFlagShort)) {
    _shakeAttribute = argData.flagArgumentString(attributeFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }

  if (argData.isFlagSet(helpFlagShort)) {
    displayHelp();
    return MS::kSuccess;
  }

  return MS::kSuccess;
}


MStatus ShakeCmd::_getTime1Output() {
  /* Gets the scene's default time1 output plug.

  Returns:
    status code (MStatus): kSuccess if the command was successful, kFailure if an error occured
      during the command.

  */
  MStatus status;

  MSelectionList selectionList;
  selectionList.add("time1");
  MObject timeObj;
  status = selectionList.getDependNode(0, timeObj);
  CHECK_MSTATUS_AND_RETURN_IT(status);
  MFnDependencyNode nodeFn(timeObj, &status);
  CHECK_MSTATUS_AND_RETURN_IT(status);
  _timeOutPlug = nodeFn.findPlug("outTime", false, &status);
  CHECK_MSTATUS_AND_RETURN_IT(status);

  return MS::kSuccess;
}


MStatus ShakeCmd::_validateNodes() {
  /* Checks if we have at least one node.

  Returns:
    status code (MStatus): kSuccess if the command was successful, kFailure if an error occured
      during the command.

  */
  MStatus status;
  if (_selList.length() == 0) {
    return MS::kFailure;
  }

  return MS::kSuccess;
}


MStatus ShakeCmd::_createShakeNode(MString name, MString output="angular") {
  /* Creates the specified shakeNode.

  Args:
    name (string): Name of the shake node
    output (string): Type of the shake node to create - with linear or angular output.
      Default is angular.

  Returns:
    status code (MStatus): kSuccess if the command was successful, kFailure if an error occured
      during the command.

  */
  MStatus status;

  if (output == "angular") { 
    _shakeObj = _dgMod.createNode("shakeNodeRot");
    _dgMod.renameNode(_shakeObj, name);
  }
  else if (output == "linear") {
    _shakeObj = _dgMod.createNode("shakeNode");
    _dgMod.renameNode(_shakeObj, name);
  }
  else
  {
    MGlobal::displayError("Shake output can either be 'angular' or 'linear'.");
    return MS::kFailure;
  }

  return MS::kSuccess;
}


MStatus ShakeCmd::_setupShake()
{
  /* Core function for creating and setting up the shake and plugs.

  Returns:
    status code (MStatus): kSuccess if the command was successful, kFailure if an error occured
      during the command.

  */
  MStatus status;

  MFnDependencyNode nodeFn(_nodeObj);

  if (_shakeAttribute == "rotate")
  {
    // Check if given attribute is not connected
    _nodeInPlug = nodeFn.findPlug("rotateAxis", false, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status)
    if (_nodeInPlug.isConnected())
    {
      return MS::kFailure;
    }
    // Check if _shakeName was specified in the flag name
    if (_shakeName == "Shake")
    {
      _shakeName = nodeFn.name() + "Rot" + _shakeName;
    }
    status = _createShakeNode(_shakeName);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MFnDependencyNode shakeFn(_shakeObj);
    _shakeInPlug = shakeFn.findPlug("inTime", false);
    _shakeOutPlug = shakeFn.findPlug("output", false);
  }
  else
  {
    _nodeInPlug = nodeFn.findPlug(_shakeAttribute, false);
    if (!_nodeInPlug || _nodeInPlug.isConnected())
    {
      return MS::kFailure;
    }
    // Check if _shakeName was specified in the flag name
    if (_shakeName == "Shake") {
      std::string shakeAttrShort = _shakeAttribute.substring(0, 2).asChar();
      shakeAttrShort[0] = std::toupper(shakeAttrShort[0]);
      _shakeName = nodeFn.name() + shakeAttrShort.c_str() + _shakeName;
    }
    status = _createShakeNode(_shakeName, "linear");
    CHECK_MSTATUS_AND_RETURN_IT(status);
    MFnDependencyNode shakeFn(_shakeObj);
    _shakeInPlug = shakeFn.findPlug("inTime", false);
    _shakeOutPlug = shakeFn.findPlug("output", false);
  }

  return MS::kSuccess;
}


MStatus ShakeCmd::doIt(const MArgList& argList)
{
  /* Command's doIt method.

  This method should perform a command by setting up internal class data and then
  calling the redoIt method.

  The actual action performed by the command should be done in the redoIt method.
  This is a pure virtual method, and must be overridden in derived classes.

  Args:
    argList (MArgList): List of arguments passed to the command

  Returns:
    status code (MStatus): kSuccess if the command was successful, kFailure if an error occured
      during the command

  */
  MStatus status;

  status = gatherFlagArguments(argList);
  CHECK_MSTATUS_AND_RETURN_IT(status);

  status = _validateNodes();
  CHECK_MSTATUS_AND_RETURN_IT(status);

  status = _getTime1Output();
  CHECK_MSTATUS_AND_RETURN_IT(status);

  // For Loop
  MItSelectionList itSelList(_selList, MFn::kDependencyNode);
  while (!itSelList.isDone())
  {
    itSelList.getDependNode(_nodeObj);

    status = _setupShake();
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // Connect the attributes
    _dgMod.connect(_timeOutPlug, _shakeInPlug);
    _dgMod.connect(_shakeOutPlug, _nodeInPlug);

    itSelList.next();
  }

  return redoIt();
}


MStatus ShakeCmd::redoIt()
{
  /* Command's redoIt method.

  This method should do the actual work of the command based on the internal class data only.
  Internal class data should be set in the doIt method.

  Returns:
    status code (MStatus): kSuccess if the command was successful, kFailure if an error occured
      during the command

  */
  MStatus status;

  status = _dgMod.doIt();
  CHECK_MSTATUS_AND_RETURN_IT(status);

  // Select the noise node
  MGlobal::select(_shakeObj, MGlobal::ListAdjustment::kReplaceList);

  // Sets command's output result in mel / python
  clearResult();
  appendToResult(MFnDependencyNode(_shakeObj).name());

  return MS::kSuccess;
}


MStatus ShakeCmd::undoIt() 
{
  /* Command's undoIt method.

  This method should undo the work done by the redoIt method based on the internal
  class data only.

  Returns:
    status code (MStatus): kSuccess if the command was successful, kFailure if an error occured
      during the command.

  */
  MStatus status;

  // Restore the initial state
  status = _dgMod.undoIt();
  CHECK_MSTATUS_AND_RETURN_IT(status);

  return MS::kSuccess;
}
