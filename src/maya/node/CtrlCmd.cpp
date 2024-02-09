#include "Ctrl.hpp"
#include "CtrlCmd.hpp"
// #include "ShapeNode.hpp"




// Public Data
const char* CtrlCmd::commandName = "ctrl";

// Command's Flags
const char* CtrlCmd::nameFlagShort = "-n";
const char* CtrlCmd::nameFlagLong = "-name";

const char* CtrlCmd::parentFlagShort = "-p";
const char* CtrlCmd::parentFlagLong = "-parent";

const char* CtrlCmd::translateToFlagShort = "-tt";
const char* CtrlCmd::translateToFlagLong = "-translateTo";

const char* CtrlCmd::rotatetToFlagShort = "-rt";
const char* CtrlCmd::rotatetToFlagLong = "-rotateTo";

const char* CtrlCmd::localPositionFlagShort = "-lp";
const char* CtrlCmd::localPositionFlagLong = "-localPosition";

const char* CtrlCmd::localRotateFlagShort = "-lr";
const char* CtrlCmd::localRotateFlagLong = "-localRotate";

const char* CtrlCmd::localScaleFlagShort = "-ls";
const char* CtrlCmd::localScaleFlagLong = "-localScale";

const char* CtrlCmd::shapeFlagShort = "-sh";
const char* CtrlCmd::shapeFlagLong = "-shape";

const char* CtrlCmd::fillShapeFlagShort = "-fs";
const char* CtrlCmd::fillShapeFlagLong = "-fillShape";

const char* CtrlCmd::fillShapeOpacityFlagShort = "-fso";
const char* CtrlCmd::fillShapeOpacityFlagLong = "-fillShapeOpacity";

// const char* CtrlCmd::createShapeNodeFlagShort = "-csn";
// const char* CtrlCmd::createShapeNodeFlagLong = "-createShapeNode";

const char* CtrlCmd::drawLineFlagShort = "-dl";
const char* CtrlCmd::drawLineFlagLong = "-drawLine";

const char* CtrlCmd::drawSolverModeFlagShort = "-dsm";
const char* CtrlCmd::drawSolverModeFlagLong = "-drawSolverMode";

const char* CtrlCmd::solverModePositionFlagShort = "-smp";
const char* CtrlCmd::solverModePositionFlagLong = "-solverModePosition";

const char* CtrlCmd::lineWidthFlagShort = "-lw";
const char* CtrlCmd::lineWidthFlagLong = "-lineWidth";

const char* CtrlCmd::colorFlagShort = "-cl";
const char* CtrlCmd::colorFlagLong = "-color";

const char* CtrlCmd::xRayFlagShort = "-xr";
const char* CtrlCmd::xRayFlagLong = "-xRay";

const char* CtrlCmd::lockShapeAttributesFlagShort = "-lsa";
const char* CtrlCmd::lockShapeAttributesFlagLong = "-lockShapeAttributes";

const char* CtrlCmd::hideOnPlaybackFlagShort = "-hop";
const char* CtrlCmd::hideOnPlaybackFlagLong = "-hideOnPlayback";

const char* CtrlCmd::helpFlagShort = "-h";
const char* CtrlCmd::helpFlagLong = "-help";

// const char* CtrlCmd::hasDynamicAttributesFlagShort = "-hda";
// const char* CtrlCmd::hasDynamicAttributesFlagLong = "-hasDynamicAttributes";




MSyntax CtrlCmd::syntaxCreator() {
  /* Creates the command's syntax object and returns it.

  Returns:
    syntax (MSyntax): Command's syntax object

  */
  MSyntax sytnax;

  // Main flags
  sytnax.addFlag(nameFlagShort, nameFlagLong, MSyntax::kString);
  sytnax.addFlag(parentFlagShort, parentFlagLong, MSyntax::kString);

  sytnax.addFlag(translateToFlagShort, translateToFlagLong, MSyntax::kString);
  sytnax.addFlag(rotatetToFlagShort, rotatetToFlagLong, MSyntax::kString);

  // Local flags
  sytnax.addFlag(localPositionFlagShort, localPositionFlagLong, MSyntax::kDouble, MSyntax::kDouble, MSyntax::kDouble);
  sytnax.addFlag(localRotateFlagShort, localRotateFlagLong, MSyntax::kDouble, MSyntax::kDouble, MSyntax::kDouble);
  sytnax.addFlag(localScaleFlagShort, localScaleFlagLong, MSyntax::kDouble, MSyntax::kDouble, MSyntax::kDouble);

  // Visual flags
  // sytnax.addFlag(createShapeNodeFlagShort, createShapeNodeFlagLong, MSyntax::kBoolean);
  sytnax.addFlag(shapeFlagShort, shapeFlagLong, MSyntax::kString);
  sytnax.addFlag(fillShapeFlagShort, fillShapeFlagLong, MSyntax::kBoolean);
  sytnax.addFlag(fillShapeOpacityFlagShort, fillShapeOpacityFlagLong, MSyntax::kDouble);

  sytnax.addFlag(drawLineFlagShort, drawLineFlagLong, MSyntax::kBoolean);
  sytnax.addFlag(lineWidthFlagShort, lineWidthFlagLong, MSyntax::kDouble);
  sytnax.addFlag(colorFlagShort, colorFlagLong, MSyntax::kString);
  sytnax.addFlag(xRayFlagShort, xRayFlagLong, MSyntax::kBoolean);

  sytnax.addFlag(drawSolverModeFlagShort, drawSolverModeFlagLong, MSyntax::kBoolean);
  sytnax.addFlag(solverModePositionFlagShort, solverModePositionFlagLong, MSyntax::kDouble, MSyntax::kDouble, MSyntax::kDouble);

  sytnax.addFlag(lockShapeAttributesFlagShort, lockShapeAttributesFlagLong, MSyntax::kBoolean);
  sytnax.addFlag(hideOnPlaybackFlagShort, hideOnPlaybackFlagLong, MSyntax::kBoolean);
  
  // sytnax.addFlag(hasDynamicAttributesFlagShort, hasDynamicAttributesFlagLong, MSyntax::kBoolean);

  sytnax.addFlag(helpFlagShort, helpFlagLong, MSyntax::kBoolean);

  sytnax.setObjectType(MSyntax::kSelectionList, 0, 1);
  sytnax.useSelectionAsDefault(true);

  return sytnax;
}


MStatus CtrlCmd::parseArguments(const MArgList &argList) {
  /* Parses the commands's flag arguments.

  Args:
    argList (MArglist): List of arguments passed to the command.

  Returns:
    status code (MStatus): kSuccess if the command was successful, kFailure if an error occured
      during the command.

  */
  MStatus status;

  MArgDatabase argData(syntax(), argList);
  argData.getObjects(listSelection);

  // Display Help
  if (argData.isFlagSet(helpFlagShort))	{
    command = kCommandHelp;
    MString strHelp;
    strHelp += "Flags:\n";
    strHelp += "   -n     -name                 String     Name of the rig controller to create.\n";
    strHelp += "   -p     -parent               String     Name of the object that will be the parent.\n";
    strHelp += "   -tt    -translateTo	        String     Name of the object that the controller will be translated to.\n";
    strHelp += "   -rt    -rotateTo             String     Name of the object that the controller will be rotated to.\n";
    strHelp += "   -lp    -localPosition        Double3    Local Position of the controller.\n";
    strHelp += "   -lr    -localRotate          Double3    Local Rotate of the controller.\n";
    strHelp += "   -ls    -localScale           Double3    Local Scale of the controller.\n";
    // strHelp += "   -csn   -createShapeNode      Bool       Whether or not to create the shape node.\n";
    strHelp += "   -sh    -shape                String     Shape to be drawn: 'cube' 'sphere' cross' 'diamond' 'square' 'circle' 'locator' 'none'.\n";
    strHelp += "   -fs    -fillShape            Bool       Whether or not to draw the fill shape.\n";
    strHelp += "   -fso   -fillShapeOpacity     Float      Opacity of the fill shape.\n";
    strHelp += "   -dl    -drawLine             Bool       Whether or not you want to display a line from the object center to a target.\n";
    strHelp += "   -dsm   -drawSolverMode       Bool       Whether or not you want to display te fk / ik mode.\n";
    strHelp += "   -smp   -solverModePosition   Double3    Local Position of the fk / ik mode.\n";
    strHelp += "   -iad   -hasDynamicAttributes Bool       Forces redraw of the ctrl shape in the viewport, use for dynamic attributes like polevectors or fk/ik state display\n";
    strHelp += "   -lw    -lineWidth            Double     Controls the line width of the outline.\n";
    strHelp += "   -cl    -color                String     Viewport display color of the controller: 'lightyellow' 'yellow' 'lightorange' 'orange' 'lightblue' 'blue' 'magenta' 'green'.\n";
    strHelp += "   -xr    -xRay                 Bool       Whether or not to enable X-Ray drawing.\n";
    strHelp += "   -lsa   -lockShapeAttributes  Bool       Locks all the shpae attributes node after creation.\n";
    strHelp += "   -hop   -hideOnPlayback       Bool       Wheter or not to hide the ctrl shapes on playback.\n";
    strHelp += "   -h     -help                 N/A        Display this text.\n";
    MGlobal::displayInfo(strHelp);
    return MS::kSuccess;
  }
  // Name Flag
  if (argData.isFlagSet(nameFlagShort))	{
    name = argData.flagArgumentString(nameFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // Parent Flag
  if (argData.isFlagSet(parentFlagShort)) {
    listSelection.add(argData.flagArgumentString(parentFlagShort, 0, &status));
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // TranslateTo Flag
  if (argData.isFlagSet(translateToFlagShort)) {
    strTranslateTo = argData.flagArgumentString(translateToFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    if (strTranslateTo != "") {
      bTranslateTo = true;
      status = LMObject::getDagPathFromString(strTranslateTo, dpTargetTranslation);
      if (status == MS::kSuccess) {
        MFnTransform targetFn(dpTargetTranslation);
        posTarget = targetFn.getTranslation(MSpace::kWorld);
      }	else {
        return MS::kFailure;
      }
    }
  }
  // RotateTo Flag
  if (argData.isFlagSet(rotatetToFlagShort)) {
    strRotateTo = argData.flagArgumentString(rotatetToFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    if (strRotateTo != "") {
      bRotateTo = true;
      status = LMObject::getDagPathFromString(strRotateTo, dpTargetRotation);
      if (status == MS::kSuccess)	{
        MFnTransform targetFn(dpTargetRotation);
        targetFn.getRotation(rotTarget, MSpace::kWorld);
      }	else {
        return MS::kFailure;
      }
    }
  }
  // Local Position Flag
  if (argData.isFlagSet(localPositionFlagShort)) {
    localPosition.x = argData.flagArgumentDouble(localPositionFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    localPosition.y = argData.flagArgumentDouble(localPositionFlagShort, 1, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    localPosition.z = argData.flagArgumentDouble(localPositionFlagShort, 2, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // Local Rotate Flag
  if (argData.isFlagSet(localRotateFlagShort)) {
    localRotate.x = argData.flagArgumentDouble(localRotateFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    localRotate.y = argData.flagArgumentDouble(localRotateFlagShort, 1, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    localRotate.z = argData.flagArgumentDouble(localRotateFlagShort, 2, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // Local Scale Flag
  if (argData.isFlagSet(localScaleFlagShort))	{
    localScale.x = argData.flagArgumentDouble(localScaleFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    localScale.y = argData.flagArgumentDouble(localScaleFlagShort, 1, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    localScale.z = argData.flagArgumentDouble(localScaleFlagShort, 2, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // // Create Shape Node Flag
  // if (argData.isFlagSet(createShapeNodeFlagShort)) {
  //   bCreateShapeNode = argData.flagArgumentBool(createShapeNodeFlagShort, 0, &status);
  //   CHECK_MSTATUS_AND_RETURN_IT(status);
  // }
  // Shape Flag
  if (argData.isFlagSet(shapeFlagShort)) {
    MString strShape = argData.flagArgumentString(shapeFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    if (strShape == "cube")	{
      indxShape = 0;
    } else if (strShape == "square") {
      indxShape = 1;
    }	else if (strShape == "cylinder") {
      indxShape = 2;
    } else if (strShape == "cone") {
      indxShape = 3;
    } else if (strShape == "circle") {
      indxShape = 4;
    } else if (strShape == "sphere") {
      indxShape = 5;
    }	else if (strShape == "dome") {
      indxShape = 6;
    } else if (strShape == "diamond") {
      indxShape = 7;
    } else if (strShape == "pyramid") {
      indxShape = 8;
    } else if (strShape == "triangle") {
      indxShape = 9;
    } else if (strShape == "prism") {
      indxShape = 10;
    } else if (strShape == "locator") {
      indxShape = 11;
    } else if (strShape == "frame") {
      indxShape = 12;
    } else if (strShape == "arrow") {
      indxShape = 13;
    } else if (strShape == "arrow2way") {
      indxShape = 14;
    } else if (strShape == "circle4arrows") {
      indxShape = 15;
    } else if (strShape == "hip") {
      indxShape = 16;
    } else if (strShape == "circlehalfdouble") {
      indxShape = 17;
    } else if (strShape == "pinround") {
      indxShape = 18;
    } else if (strShape == "clavicle") {
      indxShape = 19;
    } else if (strShape == "pointer2way") {
      indxShape = 20;
    } else if (strShape == "pointer2wayarc") {
      indxShape = 21;
    } else if (strShape == "cross") {
      indxShape = 22;
    } else if (strShape == "crossshort") {
      indxShape = 23;
    } else if (strShape == "none") {
      indxShape = 24;
    }	else {
      indxShape = 0;
    }
  }
  // Fill Shape Flag
  if (argData.isFlagSet(fillShapeFlagShort)) {
  	bFillShape = argData.flagArgumentBool(fillShapeFlagShort, 0, &status);
  	CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // Fill Shape Opacity Flag
  if (argData.isFlagSet(fillShapeOpacityFlagShort)) {
  	fillShapeOpacity = argData.flagArgumentDouble(fillShapeOpacityFlagShort, 0, &status);
  	CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // Draw Line Flag
  if (argData.isFlagSet(drawLineFlagShort)) {
    bDrawLine = argData.flagArgumentBool(drawLineFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // Draw X-Ray
  if (argData.isFlagSet(xRayFlagShort)) {
    bXRay = argData.flagArgumentBool(xRayFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // Draw Solver Mode Flag
  if (argData.isFlagSet(drawSolverModeFlagShort)) {
    bDrawSolverMode = argData.flagArgumentBool(drawSolverModeFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // Solver Mode Position Flag
  if (argData.isFlagSet(solverModePositionFlagShort)) {
    solverModePosition.x = argData.flagArgumentDouble(solverModePositionFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    solverModePosition.y = argData.flagArgumentDouble(solverModePositionFlagShort, 1, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    solverModePosition.z = argData.flagArgumentDouble(solverModePositionFlagShort, 2, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // // Fill Transparency Flag
  // if (argData.isFlagSet(fillTransparencyFlagShort)) {
  // 	fillTransparency = argData.flagArgumentDouble(fillTransparencyFlagShort, 0, &status);
  // 	CHECK_MSTATUS_AND_RETURN_IT(status);
  // }
  // Line Width Flag
  if (argData.isFlagSet(lineWidthFlagShort)) {
    widthLine = argData.flagArgumentDouble(lineWidthFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // Color Flag
  if (argData.isFlagSet(colorFlagShort)) {
    strColor = argData.flagArgumentString(colorFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    if (strColor == "lightyellow")	{
      colorOverride.r = 1.0; colorOverride.g = 1.0; colorOverride.b = 0.25;
    } else if (strColor == "yellow") {
      colorOverride.r = 1.0; colorOverride.g = 0.6;	colorOverride.b = 0.1;
    } else if (strColor == "lightorange") {
      colorOverride.r = 1.0; colorOverride.g = 0.467;	colorOverride.b = 0.2;
    } else if (strColor == "orange") {
      colorOverride.r = 0.8; colorOverride.g = 0.25; colorOverride.b = 0.05;
    }	else if (strColor == "lightblue")	{
      colorOverride.r = 0.4; colorOverride.g = 0.8; colorOverride.b = 1.0;
    }	else if (strColor == "blue") {
      colorOverride.r = 0.05;	colorOverride.g = 0.25; colorOverride.b = 0.8;
    }	else if (strColor == "magenta") {
      colorOverride.r = 0.6; colorOverride.g = 0.2; colorOverride.b = 0.4;
    }	else if (strColor == "green") {
      colorOverride.r = 0.2;	colorOverride.g = 0.8; colorOverride.b = 0.4;
    }
  }
  // Lock shape attributes
  if (argData.isFlagSet(lockShapeAttributesFlagShort)) {
    bLockShapeAttributes = argData.flagArgumentBool(lockShapeAttributesFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // Hide shapes on playback
  if (argData.isFlagSet(hideOnPlaybackFlagShort))	{
    bHideOnPlayback = argData.flagArgumentBool(hideOnPlaybackFlagShort, 0, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }
  // // Is Always Dirty Flag
  // if (argData.isFlagSet(hasDynamicAttributesFlagShort)) {
  // 	has_dynamic_attributes = argData.flagArgumentBool(hasDynamicAttributesFlagShort, 0, &status);
  // 	CHECK_MSTATUS_AND_RETURN_IT(status);
  // }

  return MS::kSuccess;
}


MStatus CtrlCmd::doIt(const MArgList& argList) {
  /* Command's doIt method.

  This method should perform a command by setting up internal class data and then	calling the
  redoIt method.

  The actual action performed by the command should be done in the redoIt method.	This is a pure
  virtual method, and must be overridden in derived classes.

  Args:
    argList (MArgList): List of arguments passed to the command.

  Returns:
    status code (MStatus): kSuccess if the command was successful, kFailure if an error occured
      during the command.

  */
  MStatus status;

  status = parseArguments(argList);
  CHECK_MSTATUS_AND_RETURN_IT(status);

  // Command create mode
  if (command == kCommandCreate) {
    objThisTransform = modDag.createNode(Ctrl::typeName, MObject::kNullObj);

    // if (bCreateShapeNode) {  // Experimental
    //   objThisShape = modDag.createNode("ctrlShape", objThisTransform);
    // }

    // If __name equals to "rigController" rename only the transform node as the shape node will be
    // renamed in the rigController.RigController::postConstructor method.
    if (name != Ctrl::typeName) {modDag.renameNode(objThisTransform, name);}

    // Parent under the transform node if the selection is not empty and / or parent was specified
    int numItems = listSelection.length();
    if (numItems == 1) {
      MObject parentObj;
      listSelection.getDependNode(0, parentObj);
      modDag.reparentNode(objThisTransform, parentObj);
    }	else if (numItems == 2)	{
      MObject parentObj;
      listSelection.getDependNode(1, parentObj);
      modDag.reparentNode(objThisTransform, parentObj);
    }
  }

  return redoIt();
}


MStatus CtrlCmd::redoIt() {
  /* Command's redoIt method.

  This method should do the actual work of the command based on the internal class data only.	Internal class data should be set in the doIt method.

  Returns:
    status code (MStatus): kSuccess if the command was successful, kFailure if an error occured
      during the command.

  */
  // Command create mode
  if (command == kCommandCreate) {
    MStatus status;
    // We need to init the MFnTransform with a dag path, mobjects do not work with transformations
    // even if the object has a MFn::kTransform
    MDagPath::getAPathTo(objThisTransform, dpThisTransform);
    MFnTransform fnTransform(dpThisTransform);

    status = modDag.doIt();
    CHECK_MSTATUS_AND_RETURN_IT(status);

    // TRANSFORM NODE
    if (bTranslateTo == true) {fnTransform.setTranslation(posTarget, MSpace::kWorld);}
    if (bRotateTo == true) {fnTransform.setRotation(rotTarget, MSpace::kWorld);}

    // SHAPE PLUGS
    // Sets the plugs values based on the flag arguments
    MPlug plugShape = fnTransform.findPlug("shape", false);
    plugShape.setShort(indxShape);

    // Sets local position values
    MPlug plugLocalPosition = fnTransform.findPlug("localPosition", false);
    MPlug plugLocalPositionX = fnTransform.findPlug("localPositionX", false);
    plugLocalPositionX.setValue(localPosition.x);
    MPlug plugLocalPositionY = fnTransform.findPlug("localPositionY", false);
    plugLocalPositionY.setValue(localPosition.y);
    MPlug plugLocalPositionZ = fnTransform.findPlug("localPositionZ", false);
    plugLocalPositionZ.setValue(localPosition.z);

    // Sets local rotate values
    MPlug plugLocalRotate = fnTransform.findPlug("localRotate", false);
    MPlug plugLocalRotateX = fnTransform.findPlug("localRotateX", false);
    plugLocalRotateX.setValue(radians(localRotate.x));
    MPlug plugLocalRotateY = fnTransform.findPlug("localRotateY", false);
    plugLocalRotateY.setValue(radians(localRotate.y));
    MPlug plugLocalRotateZ = fnTransform.findPlug("localRotateZ", false);
    plugLocalRotateZ.setValue(radians(localRotate.z));

    // Sets local scale values
    MPlug plugLocalScale = fnTransform.findPlug("localScale", false);
    MPlug plugLocalScaleX = fnTransform.findPlug("localScaleX", false);
    plugLocalScaleX.setValue(localScale.x);
    MPlug plugLocalScaleY = fnTransform.findPlug("localScaleY", false);
    plugLocalScaleY.setValue(localScale.y);
    MPlug plugLocalScaleZ = fnTransform.findPlug("localScaleZ", false);
    plugLocalScaleZ.setValue(localScale.z);

    // Visual flags
    MPlug plugFillShape = fnTransform.findPlug("fillShape", false);
    plugFillShape.setValue(bFillShape);

    MPlug plugFillShapeOpacity = fnTransform.findPlug("fillShapeOpacity", false);
    plugFillShapeOpacity.setValue(fillShapeOpacity);

    MPlug plugDrawLine = fnTransform.findPlug("drawLine", false);
    plugDrawLine.setValue(bDrawLine);

    // Fk Ik State
    MPlug plugDrawSolverMode = fnTransform.findPlug("drawSolverMode", false);
    plugDrawSolverMode.setValue(bDrawSolverMode);
    MPlug plugSolverModeSize = fnTransform.findPlug("solverModeSize", false);
    // plugDrawFkIkStateSize.setValue(draw_solver_mode);
    MPlug plugSolverModePosition = fnTransform.findPlug("solverModePosition", false);
    MPlug plugSolverModePositionX = fnTransform.findPlug("solverModePositionX", false);
    plugSolverModePositionX.setValue(solverModePosition.x);
    MPlug plugSolverModePositionY = fnTransform.findPlug("solverModePositionY", false);
    plugSolverModePositionY.setValue(solverModePosition.y);
    MPlug plugSolverModePositionZ = fnTransform.findPlug("solverModePositionZ", false);
    plugSolverModePositionZ.setValue(solverModePosition.z);

    MPlug plugInFkIk = fnTransform.findPlug("fkIk", false);

    MPlug plugLineWidth = fnTransform.findPlug("lineWidth", false);
    plugLineWidth.setValue(widthLine);

    MPlug plugXRay = fnTransform.findPlug("xRay", false);
    plugXRay.setValue(bXRay);

    // Set color
    MPlug plugOverrideColorR = fnTransform.findPlug("overrideColorR", false);
    plugOverrideColorR.setValue(colorOverride.r);
    MPlug plugOverrideColorG = fnTransform.findPlug("overrideColorG", false);
    plugOverrideColorG.setValue(colorOverride.g);
    MPlug plugOverrideColorB = fnTransform.findPlug("overrideColorB", false);
    plugOverrideColorB.setValue(colorOverride.b);
  

    // MPlug plug_has_dynamic_attributes = fnTransform.findPlug("hasDynamicAttributes", false);
    // plug_has_dynamic_attributes.setValue(has_dynamic_attributes);

    // Lock shape attributes
    if (bLockShapeAttributes == true) {
      // optimize with a for loop and MPlugArray  
      Attr::lockAndHideAttr(plugLocalPosition);
      Attr::lockAndHideAttr(plugLocalPositionX);
      Attr::lockAndHideAttr(plugLocalPositionY);
      Attr::lockAndHideAttr(plugLocalPositionZ);
      // Local rotate
      Attr::lockAndHideAttr(plugLocalRotate);
      Attr::lockAndHideAttr(plugLocalRotateX);
      Attr::lockAndHideAttr(plugLocalRotateY);
      Attr::lockAndHideAttr(plugLocalRotateZ);
      // Local scale
      Attr::lockAndHideAttr(plugLocalScale);
      Attr::lockAndHideAttr(plugLocalScaleX);
      Attr::lockAndHideAttr(plugLocalScaleY);
      Attr::lockAndHideAttr(plugLocalScaleZ);
      // Shape attrs
      Attr::lockAndHideAttr(plugShape);
      Attr::lockAndHideAttr(plugFillShape);
      Attr::lockAndHideAttr(plugFillShapeOpacity);
      Attr::lockAndHideAttr(plugDrawLine);

      Attr::lockAndHideAttr(plugDrawSolverMode);
      Attr::lockAndHideAttr(plugSolverModeSize);

      Attr::lockAndHideAttr(plugSolverModePosition);
      Attr::lockAndHideAttr(plugSolverModePositionX);
      Attr::lockAndHideAttr(plugSolverModePositionY);
      Attr::lockAndHideAttr(plugSolverModePositionZ);

      Attr::lockAndHideAttr(plugLineWidth);
      Attr::lockAndHideAttr(plugInFkIk);
      Attr::lockAndHideAttr(plugXRay);

      // Attr::lockAndHideAttr(plug_has_dynamic_attributes);
    }

    // Set hide on playback
    fnTransform.findPlug("hideOnPlayback", false).setValue(bHideOnPlayback);

    // Sets command's output result in mel / python
    clearResult();
    setResult(fnTransform.name());
    // appendToResult(fnTransform.name());
  }

  return MS::kSuccess;
}


MStatus CtrlCmd::undoIt() {
  /* Command's undoIt method.

  This method should undo the work done by the redoIt method based on the internal class data only.

  Returns:
    status code (MStatus): kSuccess if the command was successful, kFailure if an error occured
      during the command.

  */
  MStatus status;

  // Restore the initial state
  status = modDag.undoIt();
  CHECK_MSTATUS_AND_RETURN_IT(status);

  return MS::kSuccess;
}
