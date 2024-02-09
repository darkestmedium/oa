#include "Speedometer.hpp"
#include "SpeedometerCmd.hpp"



// Public Data
const char* SpeedometerCmd::commandName = "speed";

// Command's Flags
const char* SpeedometerCmd::nameFlagShort = "-n";
const char* SpeedometerCmd::nameFlagLong = "-name";

const char* SpeedometerCmd::parentFlagShort = "-p";
const char* SpeedometerCmd::parentFlagLong = "-parent";

const char* SpeedometerCmd::shapeFlagShort = "-s";
const char* SpeedometerCmd::shapeFlagLong = "-shape";

const char* SpeedometerCmd::lineWidthFlagShort = "-lw";
const char* SpeedometerCmd::lineWidthFlagLong = "-lineWidth";

const char* SpeedometerCmd::helpFlagShort = "-h";
const char* SpeedometerCmd::helpFlagLong = "-help";



MSyntax SpeedometerCmd::syntaxCreator() {
	/* Creates the command's syntax object and returns it.

	Returns:
		syntax (MSyntax): Command's syntax object

	*/
	MSyntax sytnax;

	sytnax.addFlag(nameFlagShort, nameFlagLong, MSyntax::kString);
	sytnax.addFlag(parentFlagShort, parentFlagLong, MSyntax::kString);

	sytnax.addFlag(shapeFlagShort, shapeFlagLong, MSyntax::kString);
	sytnax.addFlag(lineWidthFlagShort, lineWidthFlagLong, MSyntax::kDouble);

	sytnax.addFlag(helpFlagShort, helpFlagLong);

	sytnax.setObjectType(MSyntax::kSelectionList, 0, 1);
	sytnax.useSelectionAsDefault(true);

	return sytnax;
}


void displayHelp() {
	/* Displays help for the command. */
	MString helpStr;
	helpStr += "Flags:\n";
	helpStr += "   -n  -name                 String     Name of the speed locator to create.\n";
	helpStr += "   -p  -parent               String     Name of the object that will be the parent.\n";
	helpStr += "   -s  -shape                String     Shape to be drawn: 'Locator' 'Square' 'Cube' 'Circle'.\n";
	helpStr += "   -fw -lineWidth            Double     Controls the line width of the outline.\n";
	helpStr += "   -h  -help                 N/A        Display this text.\n";
	MGlobal::displayInfo(helpStr);
}


MStatus SpeedometerCmd::parseArguments(const MArgList& argList) {
	/* Parses the commands's flag arguments.

	Args:
		argList (MArglist): List of arguments passed to the command.

	Returns:
		status code (MStatus): kSuccess if the command was successful,
			kFailure if an error occured during the command

	*/
	MStatus status;

	MArgDatabase argData(syntax(), argList);
	argData.getObjects(_selList);

	if (argData.isFlagSet(helpFlagShort)) {
		displayHelp();
		return MS::kSuccess;
	}

	// Name Flag
	if (argData.isFlagSet(nameFlagShort)) {
		name = argData.flagArgumentString(nameFlagShort, 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}

	// Parent Flag
	if (argData.isFlagSet(parentFlagShort)) {
		_selList.add(argData.flagArgumentString(parentFlagShort, 0, &status));
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}

	// Shape Flag
	if (argData.isFlagSet(shapeFlagShort)) {
		MString shape = argData.flagArgumentString(shapeFlagShort, 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);
		if (shape == "Locator") {
			_shape = 0;
		} else if (shape == "Square") {
			_shape = 1;
		} else if (shape == "Cube") {
			_shape = 2;
		} else if (shape == "Circle") {
			_shape = 3;
		}
	}

	// Line Width Flag
	if (argData.isFlagSet(lineWidthFlagShort)) {
		_lineWidth = argData.flagArgumentDouble(lineWidthFlagShort, 0, &status);
		CHECK_MSTATUS_AND_RETURN_IT(status);
	}

	return MS::kSuccess;
}


MStatus SpeedometerCmd::doIt(const MArgList& argList) {
	/* Command's doIt method.

	This method should perform a command by setting up internal class data and then
	calling the redoIt method.

	The actual action performed by the command should be done in the redoIt method.
	This is a pure virtual method, and must be overridden in derived classes.

	Args:
		argList (MArgList): List of arguments passed to the command

	Returns:
		status code (MStatus): kSuccess if the command was successful,
			kFailure if an error occured during the command

	*/
	MStatus status;

	status = parseArguments(argList);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	objTransform = _dagMod.createNode("transform", MObject::kNullObj);
	objShape = _dagMod.createNode(Speedometer::typeName, objTransform);


	// If name equals to "rigController" rename only the transform node as the shape node will be
	// renamed in the rigController.RigController::postConstructor method.
	if (name == SpeedometerCmd::commandName) {
		_dagMod.renameNode(objTransform, name);
	} else {
		_dagMod.renameNode(objTransform, name);
		_dagMod.renameNode(objShape, name + "Shape");
	}

	// Parent under the transform node if the selection is not empty and / or parent was specified
	int numItems = _selList.length();
	if (numItems == 1) {
		MObject parentObj;
		_selList.getDependNode(0, parentObj);
		_dagMod.reparentNode(objTransform, parentObj);
	} else if (numItems == 2) {
		MObject parentObj;
		_selList.getDependNode(1, parentObj);
		_dagMod.reparentNode(objTransform, parentObj);
	}

	return redoIt();
}


MStatus SpeedometerCmd::redoIt() {
	/* Command's redoIt method.

	This method should do the actual work of the command based on the internal class
	data only. Internal class data should be set in the doIt method

	Returns:
		status code (MStatus): kSuccess if the command was successful,
			kFailure if an error occured during the command

	*/
	MStatus status;
	status = _dagMod.doIt();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	// MFnDependencyNode shapeFn(objShape);
	MDagPath dpShape;
	MDagPath::getAPathTo(objShape, dpShape);
	MFnDagNode fnShape(dpShape);

	MDagPath dpTransform;
	MDagPath::getAPathTo(objTransform, dpTransform);
	MFnDagNode fnTransform(dpTransform);


	// Sets the plugs values based on the flag arguments
	MPlug shapePlug = fnShape.findPlug("shape", false);
	shapePlug.setShort(_shape);

	MPlug lineWidthPlug = fnShape.findPlug("lineWidth", false);
	lineWidthPlug.setDouble(_lineWidth);

	MPlug plugOutTransformWorldMatrix0 = fnTransform.findPlug("worldMatrix", false).elementByLogicalIndex(0);
	MPlug plugInTransform = fnShape.findPlug("inTransform", false);

	MPlug plugOutShapeUpdate = fnShape.findPlug("update", false);
	MPlug plugInRotatePivotX = fnTransform.findPlug("rotatePivotX", false);

	MGlobal::displayWarning(plugOutTransformWorldMatrix0.name());
	MGlobal::displayWarning(plugInTransform.name());
	MGlobal::displayWarning(plugOutShapeUpdate.name());
	MGlobal::displayWarning(plugInRotatePivotX.name());

	// Connect plugs
	MDGModifier modDg;
	modDg.connect(plugOutTransformWorldMatrix0, plugInTransform);
	LMScene::connectSceneTime(objShape, "inTime", modDg);
	modDg.connect(plugOutShapeUpdate, plugInRotatePivotX);
	modDg.doIt();

	// Sets command's output result in mel / python
	clearResult();
	appendToResult(fnTransform.name());
	appendToResult(fnShape.name());

	return MS::kSuccess;
}


MStatus SpeedometerCmd::undoIt() {
	/* Command's undoIt method.

	This method should undo the work done by the redoIt method based on the internal
	class data only.

	Returns:
		status code (MStatus): kSuccess if the command was successful,
			kFailure if an error occured during the command

	*/
	MStatus status;

	// Restore the initial state
	status = _dagMod.undoIt();
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}
