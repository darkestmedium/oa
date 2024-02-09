#pragma once

#include "Speedometer.hpp"

// System Includes

// Maya General Includes
#include <maya/MGlobal.h>
#include <maya/MPlug.h>
#include <maya/MSyntax.h>
#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MSelectionList.h>
#include <maya/MTime.h>
#include <maya/MAnimControl.h>
#include <maya/MString.h>
#include <maya/MDagModifier.h>

// Function Sets
#include <maya/MFnDependencyNode.h>

// Iterators
#include <maya/MItSelectionList.h>

// Proxies
#include <maya/MPxCommand.h>



class SpeedometerCmd : public MPxCommand {

public:
  // Constructors
  SpeedometerCmd() : MPxCommand() {};

  // Destructor
  virtual ~SpeedometerCmd() override {};

  // Public Methods
  static void* creator() {return new SpeedometerCmd();}
  virtual bool isUndoable() const override {return true;}
  static MSyntax syntaxCreator();

  virtual MStatus doIt(const MArgList& argList) override;
  virtual MStatus redoIt() override;
  virtual MStatus undoIt() override;

  // Public Data
  static const char* commandName;

  // Command's Flags
  static const char* nameFlagShort;
  static const char* nameFlagLong;

  static const char* parentFlagShort;
  static const char* parentFlagLong;

  static const char* shapeFlagShort;
  static const char* shapeFlagLong;

  static const char* lineWidthFlagShort;
  static const char* lineWidthFlagLong;

  static const char* helpFlagShort;
  static const char* helpFlagLong;

private:
  // Private Methods
  MStatus parseArguments(const MArgList& argList);

  // Private Data
  MObject objShape;
  MObject objTransform;

  MString name = commandName;
  MString parent;
  short _shape = 0;
  double _lineWidth = 1.0;

  MSelectionList _selList;
  MDagModifier _dagMod;

friend void displayHelp();
};
