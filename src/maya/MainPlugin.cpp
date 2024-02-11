// Nodes
#include "node/TransformShape.hpp"

#include "node/ComponentNode.hpp"
#include "node/Ctrl.hpp"
#include "node/CtrlCmd.hpp"
#include "node/SpaceSwitchNode.hpp"
#include "node/MetaDataNode.hpp"
#include "node/MetaDataCmd.hpp"
// #include "node/ShapeNode.hpp"
#include "node/ShakeNode.hpp"
#include "node/ShakeNodeRot.hpp"
#include "node/ShakeCmd.hpp"
#include "node/Speedometer.hpp"
#include "node/SpeedometerCmd.hpp"

// Solvers
#include "solve/Ik2bSolver.hpp"
#include "solve/IkCommand.hpp"
#include "solve/TwistSolver.hpp"
#include "solve/FootRollSolver.hpp"

// Temp
// #include "temp/rawfootPrintNode.hpp"

// Function Sets
#include <maya/MFnPlugin.h>




// Callback variables
static MCallbackIdArray callbackIds;
static MCallbackId afterNewCallbackId;
static MCallbackId afterOpenCallbackId;
static MCallbackId afterSaveSetMetaDataNodeCbId;



void setMelConfig(void*) {
  /* Sets the selection priority for locators to 999. */
  MGlobal::executeCommandOnIdle("cycleCheck -e 0");
  // MGlobal::executeCommandOnIdle("selectPriority -locator 999");
}


static void onSceneSaved(void* clientData) {
  MGlobal::executePythonCommand("import oa.maya.Core as omc");
  MGlobal::executePythonCommand("lm.LMMetaData().setFromSceneName()");
}



MStatus initializePlugin(MObject obj) {
  // // Plugin variables  // Constructors
  // const char* author = "Darkest Medium";
  // const char* version = "0.0.1";
  // const char* requiredApiVersion = "Any";

  MStatus status;
  // MFnPlugin fnPlugin(obj, author, version, requiredApiVersion);
  MFnPlugin fnPlugin(
    obj,
    "Darkest Medium",
    "0.0.1",
    "Any"
  );


  { // Transform
    status = fnPlugin.registerTransform(
      TransformShape::typeName,
      TransformShape::typeId, 
      &TransformShape::creator, 
      &TransformShape::initialize,
      &MPxTransformationMatrix::creator,
      MPxTransformationMatrix::baseTransformationMatrixId,
      &TransformShape::typeDrawDb
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    // status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
    //   Transform::typeDrawDb,
    //   Transform::typeDrawId,
    //   TransformDrawOverride::creator
    // );
    // CHECK_MSTATUS_AND_RETURN_IT(status);

    // status = fnPlugin.registerCommand(
    //   TransformCmd::commandName,
    //   TransformCmd::creator,
    //   TransformCmd::syntaxCreator
    // );
    // CHECK_MSTATUS_AND_RETURN_IT(status);
  }


  { // Component
    status = fnPlugin.registerTransform(
      ComponentNode::typeName,
      ComponentNode::typeId, 
      &ComponentNode::creator, 
      &ComponentNode::initialize,
      &MPxTransformationMatrix::creator,
      MPxTransformationMatrix::baseTransformationMatrixId,
      &ComponentNode::typeDrawDb
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = fnPlugin.registerCommand(
      ComponentCmd::command_name,
      ComponentCmd::creator,
      ComponentCmd::syntaxCreator
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }


  { // Ctrl
    status = fnPlugin.registerTransform(
      Ctrl::typeName,
      Ctrl::typeId, 
      &Ctrl::creator, 
      &Ctrl::initialize,
      &MPxTransformationMatrix::creator,
      MPxTransformationMatrix::baseTransformationMatrixId,
      &Ctrl::typeDrawDb
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
      Ctrl::typeDrawDb,
      Ctrl::typeDrawId,
      CtrlDrawOverride::creator
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = fnPlugin.registerCommand(
      CtrlCmd::commandName,
      CtrlCmd::creator,
      CtrlCmd::syntaxCreator
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }


  { // SpaceSwitchNode
    status = fnPlugin.registerNode(
      SpaceSwitchNode::typeName,
      SpaceSwitchNode::typeId,
      SpaceSwitchNode::creator,
      SpaceSwitchNode::initialize,
      MPxNode::kDependNode
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = fnPlugin.registerCommand(
      SpaceSwitchCmd::command_name,
      SpaceSwitchCmd::creator,
      SpaceSwitchCmd::syntaxCreator
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }


  { // Ik2bSolver
    status = fnPlugin.registerNode(
      Ik2bSolver::typeName,
      Ik2bSolver::typeId,
      Ik2bSolver::creator,
      Ik2bSolver::initialize,
      MPxNode::kDependNode
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = fnPlugin.registerCommand(
      IkCommand::commandName,
      IkCommand::creator,
      IkCommand::syntaxCreator
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }


  { // MetaDataNode
    status = fnPlugin.registerTransform(
      MetaDataNode::type_name,
      MetaDataNode::type_id, 
      &MetaDataNode::creator, 
      &MetaDataNode::initialize,
      &MPxTransformationMatrix::creator,
      MPxTransformationMatrix::baseTransformationMatrixId,
      &MetaDataNode::type_drawdb
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
      MetaDataNode::type_drawdb,
      MetaDataNode::type_drawid,
      MetaDataDrawOverride::creator
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = fnPlugin.registerCommand(
      MetaDataCmd::commandName,
      MetaDataCmd::creator,
      MetaDataCmd::syntaxCreator
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }


  { // ShakeNode
    status = fnPlugin.registerNode(
      ShakeNode::typeName,
      ShakeNode::typeId,
      ShakeNode::creator,
      ShakeNode::initialize,
      MPxNode::kDependNode
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = fnPlugin.registerNode(
      ShakeNodeRot::typeName,
      ShakeNodeRot::typeId,
      ShakeNodeRot::creator,
      ShakeNodeRot::initialize,
      MPxNode::kDependNode
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = fnPlugin.registerCommand(
      ShakeCmd::commandName,
      ShakeCmd::creator,
      ShakeCmd::syntaxCreator
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    // // Add main menu items
    // if (MGlobal::mayaState() == MGlobal::kInteractive)
    // {
    //   MGlobal::executePythonCommandOnIdle("from shakeNodeMainMenu import ShakeNodeMainMenu");
    //   MGlobal::executePythonCommandOnIdle("ShakeNodeMainMenu().createMenuItems()");
    // }
  }

  
  { // Speedometer
    status = fnPlugin.registerTransform( 
      Speedometer::typeName,
      Speedometer::typeId,
      &Speedometer::creator,
      &Speedometer::initialize,
      &MPxTransformationMatrix::creator,
      MPxTransformationMatrix::baseTransformationMatrixId,
      &Speedometer::typeDrawDb
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
      Speedometer::typeDrawDb,
      Speedometer::typeDrawId,
      SpeedometerDrawOverride::creator
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    status = fnPlugin.registerCommand(
      SpeedometerCmd::commandName,
      SpeedometerCmd::creator,
      SpeedometerCmd::syntaxCreator
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
    // // Add main menu items
    // if (MGlobal::mayaState() == MGlobal::kInteractive) {
    //   MGlobal::executePythonCommandOnIdle("from speedLocatorMainMenu import SpeedometerMainMenu");
    //   MGlobal::executePythonCommandOnIdle("SpeedometerMainMenu().createMenuItems()");
    // }
  }


  { //TwistSolver
    status = fnPlugin.registerNode(
      TwistSolver::typeName,
      TwistSolver::typeId,
      TwistSolver::creator,
      TwistSolver::initialize,
      MPxNode::kDependNode
    );
    CHECK_MSTATUS_AND_RETURN_IT(status);
  }



  // // Register FootRoll node
  // status = fnPlugin.registerNode(
  // 	FootRollSolver::typeName,
  // 	FootRollSolver::typeId,
  // 	FootRollSolver::creator,
  // 	FootRollSolver::initialize,
  // 	MPxNode::kDependNode
  // );
  // CHECK_MSTATUS_AND_RETURN_IT(status);


  // TEMP
  // Globals = new GlobalVariables();
  // status = fnPlugin.registerNode(
  //   gPluginNodeName,
  //   FootPrintNode::id,
  //   &FootPrintNode::creator,
  //   &FootPrintNode::initialize,
  //   MPxNode::kLocatorNode,
  //   &FootPrintNode::drawDbClassification
  // );
  // CHECK_MSTATUS_AND_RETURN_IT(status);
  // status = MHWRender::MDrawRegistry::registerGeometryOverrideCreator(
  //   FootPrintNode::drawDbClassification,
  //   FootPrintNode::drawRegistrantId,
  //   FootPrintGeometryOverride::Creator
  // );
  // CHECK_MSTATUS_AND_RETURN_IT(status);
  // MSelectionMask::registerSelectionType(gPluginSelectionMask, 2);
  // char cmd[256];
	// snprintf(cmd, sizeof(cmd), "selectType -byName \"%s\" 1", gPluginSelectionMask);
	// status = MGlobal::executeCommand(cmd);




  if (MGlobal::mayaState() == MGlobal::kInteractive) {
    // Register callback to set selection priority on locators to 999
    setMelConfig(NULL);

    afterNewCallbackId = MSceneMessage::addCallback(MSceneMessage::kAfterNew, setMelConfig, NULL, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    callbackIds.append(afterNewCallbackId);

    afterOpenCallbackId = MSceneMessage::addCallback(MSceneMessage::kAfterOpen, setMelConfig, NULL, &status);
    CHECK_MSTATUS_AND_RETURN_IT(status);
    callbackIds.append(afterOpenCallbackId);
  
    // afterSaveSetMetaDataNodeCbId = MSceneMessage::addCallback(MSceneMessage::kBeforeSave, onSceneSaved, NULL, &status);
    // CHECK_MSTATUS_AND_RETURN_IT(status);
    // callbackIds.append(afterSaveSetMetaDataNodeCbId);

    // // Creates the maya main menu items
    // MGlobal::executePythonCommandOnIdle("from lunar.maya.resources.scripts.ctrlMainMenu import CtrlMainMenu");
    // MGlobal::executePythonCommandOnIdle("CtrlMainMenu().createMenuItems()");
  }


  return status;
}




MStatus uninitializePlugin(MObject obj) {
  MStatus status;
  MFnPlugin fnPlugin(obj);

  MMessage::removeCallbacks(callbackIds);


  { // Transform
    // fnPlugin.deregisterCommand(CtrlCmd::commandName);
    fnPlugin.deregisterNode(TransformShape::typeId);
    // MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(Transform::typeDrawDb, Ctrl::typeDrawId);
  }


  { // Component
    fnPlugin.deregisterCommand(ComponentCmd::command_name);
    fnPlugin.deregisterNode(ComponentNode::typeId);
  }


  { // Ctrl
    fnPlugin.deregisterCommand(CtrlCmd::commandName);
    fnPlugin.deregisterNode(Ctrl::typeId);
    MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(Ctrl::typeDrawDb, Ctrl::typeDrawId);
  }


  { // SpaceSwitchNode
    fnPlugin.deregisterCommand(SpaceSwitchCmd::command_name);
    fnPlugin.deregisterNode(SpaceSwitchNode::typeId);
  }


  { // ShakeNode
    fnPlugin.deregisterCommand(ShakeCmd::commandName);
    fnPlugin.deregisterNode(ShakeNodeRot::typeId);
    fnPlugin.deregisterNode(ShakeNode::typeId);
    // // Remove main menu items
    // if (MGlobal::mayaState() == MGlobal::kInteractive) {
    //   MGlobal::executePythonCommandOnIdle("ShakeNodeMainMenu().deleteMenuItems()");
    // }
  }


  { // Speedometer
    fnPlugin.deregisterCommand(SpeedometerCmd::commandName);
    MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
      Speedometer::typeDrawDb,
      Speedometer::typeDrawId
    );
    fnPlugin.deregisterNode(Speedometer::typeId);
    // // Remove main menu items
    // if (MGlobal::mayaState() == MGlobal::kInteractive) {
    //   MGlobal::executePythonCommandOnIdle("SpeedometerMainMenu().deleteMenuItems()");
    // }
  }


  { // MetaDataNode
    fnPlugin.deregisterCommand(IkCommand::commandName);
    MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
      MetaDataNode::type_drawdb,
      MetaDataNode::type_drawid
    );
    fnPlugin.deregisterNode(MetaDataNode::type_id);
  }


  // Deregister IkCommand
  fnPlugin.deregisterCommand(IkCommand::commandName);
  // Deregister Ik2bSolver
  fnPlugin.deregisterNode(Ik2bSolver::typeId);

  // Deregister TwistSolver
  fnPlugin.deregisterNode(TwistSolver::typeId);

  // Deregister Footroll Node
  // fnPlugin.deregisterNode(FootRollSolver::typeId);


  // // Deletes the maya main menu items
  // if (MGlobal::mayaState() == MGlobal::kInteractive)
  // {
  // MGlobal::executePythonCommandOnIdle("CtrlMainMenu().deleteMenuItems()");
  // }


  // delete Globals;
  // MHWRender::MDrawRegistry::deregisterGeometryOverrideCreator(
  //   FootPrintNode::drawDbClassification,
  //   FootPrintNode::drawRegistrantId
  // );
  // fnPlugin.deregisterNode(FootPrintNode::id);

  return status;
}
