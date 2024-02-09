#pragma once

// System Includes
#include <string>

// Maya General Includes
#include <maya/MGlobal.h>
#include <maya/MPlug.h>
#include <maya/MSyntax.h>
#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MSelectionList.h>
#include <maya/MDGModifier.h>
#include <maya/MDagModifier.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MString.h>
#include <maya/MDagModifier.h>

// Function Sets
#include <maya/MFnDependencyNode.h>
#include <maya/MFnTransform.h>

// Iterators
#include <maya/MItSelectionList.h>

// Proxies
#include <maya/MPxCommand.h>

// Lunar
#include "Ctrl.hpp"
#include "api/Utils.hpp"
#include "api/Attribute.hpp"
#include "api/Object.hpp"



class CtrlCmd : public MPxCommand {
public:
  enum CommandMode {kCommandCreate, kCommandHelp};
  CommandMode command;

  // Public Data
  static const char* commandName;

  // Command's Flags
  static const char* nameFlagShort;
  static const char* nameFlagLong;

  static const char* parentFlagShort;
  static const char* parentFlagLong;

  static const char* translateToFlagShort;
  static const char* translateToFlagLong;

  static const char* rotatetToFlagShort;
  static const char* rotatetToFlagLong;

  static const char* localPositionFlagShort;
  static const char* localPositionFlagLong;

  static const char* localRotateFlagShort;
  static const char* localRotateFlagLong;
  
  static const char* localScaleFlagShort;
  static const char* localScaleFlagLong;

  // Visual flags
  static const char* shapeFlagShort;
  static const char* shapeFlagLong;

  static const char* fillShapeFlagShort;
  static const char* fillShapeFlagLong;

  static const char* fillShapeOpacityFlagShort;
  static const char* fillShapeOpacityFlagLong;

  // static const char* createShapeNodeFlagShort;
  // static const char* createShapeNodeFlagLong;

  static const char* drawLineFlagShort;
  static const char* drawLineFlagLong;

  static const char* drawSolverModeFlagShort;
  static const char* drawSolverModeFlagLong;

  static const char* solverModePositionFlagShort;
  static const char* solverModePositionFlagLong;

  static const char* lineWidthFlagShort;
  static const char* lineWidthFlagLong;

  static const char* colorFlagShort;
  static const char* colorFlagLong;

  static const char* xRayFlagShort;
  static const char* xRayFlagLong;

  static const char* lockShapeAttributesFlagShort;
  static const char* lockShapeAttributesFlagLong;

  static const char* hideOnPlaybackFlagShort;
  static const char* hideOnPlaybackFlagLong;

  static const char* helpFlagShort;
  static const char* helpFlagLong;

  MString name;
  MString parent;

  bool bTranslateTo;
  bool bRotateTo;
  MString strTranslateTo;
  MString strRotateTo;

  MVector localPosition;
  MVector localRotate;
  MVector localScale;
  MVector solverModePosition;

  MString strColor;
  MColor  colorOverride;

  // bool    bCreateShapeNode;
  short   indxShape;
  bool    bFillShape;
  float   fillShapeOpacity;
  bool    bXRay;
  bool    bDrawSolverMode;
  bool    bDrawLine;
  double  widthLine;
  bool    bLockShapeAttributes;
  bool    bHideOnPlayback;

  // Constructors
  CtrlCmd()
    : MPxCommand()
    , name(Ctrl::typeName)
    , bTranslateTo(false)
    , bRotateTo(false)
    , localPosition(0.0, 0.0, 0.0)
    , localRotate(0.0, 0.0, 0.0)
    , localScale(1.0, 1.0, 1.0)
    , solverModePosition(0.0, 0.0, 0.0)
    // , bCreateShapeNode(true)
    , indxShape(0)
    , bFillShape(false)
    , fillShapeOpacity(0.1)
    , bDrawLine(false)
    , bXRay(false)
    , bDrawSolverMode(false)
    , widthLine(2.0)
    , strColor("yellow")
    , colorOverride(1.0, 1.0, 0.25)
    , bLockShapeAttributes(false)
    , bHideOnPlayback(false)
    , command(kCommandCreate)
  {};

  // Public Methods
  static void* creator() {return new CtrlCmd();}
  virtual bool isUndoable() const override {return command == kCommandCreate;}
  static MSyntax syntaxCreator();

  virtual MStatus doIt(const MArgList& argList) override;
  virtual MStatus redoIt() override;
  virtual MStatus undoIt() override;

  MStatus parseArguments(const MArgList& argList);

private:
  // Private Data
  MObject objThisTransform;
  MDagPath dpThisTransform;

  MObject objThisShape;

  MDagPath dpTargetTranslation;
  MDagPath dpTargetRotation;
  MVector posTarget;
  MQuaternion rotTarget;

  MSelectionList listSelection;
  MDagModifier modDag;
};