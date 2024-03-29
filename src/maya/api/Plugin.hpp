#pragma once

#include "MathUtility.hpp"

// System Includes
#include <algorithm>
#include <cmath>
#include <limits>
#include <string>
#include <type_traits>
#include <vector>

// Maya General Includes
#include <maya/MSelectionList.h>
#include <maya/MAngle.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MEulerRotation.h>
#include <maya/MGlobal.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MQuaternion.h>
#include <maya/MDGModifier.h>
#include <maya/MDagPath.h>

// Function Sets
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
// #include <maya/MFnPlugin.h>

// Iterators

// Proxies
#include <maya/MPxNode.h>

// Custom
#include "Utils.hpp"
#include "Object.hpp"




namespace LMPlugin {
  /* LMPlugin
   * Lunar Maya Plugin Utilities
   */


  inline MStatus parseTransformInput(MDataBlock& dataBlock, const MObject& obj, MFnTransform& fnObj, const MObject& objAttribute) {
    /* Gets the dag path from the given input and attaches to a function set.
    */
    MStatus status;
    MDagPath dpObj;
    status = MDagPath::getAPathTo(
      Attr::getSourceObjFromPlug(obj, dataBlock.inputValue(objAttribute).attribute()),
      dpObj
    );
    if (status == MS::kSuccess) {
      fnObj.setObject(dpObj);
    } else {
      return MS::kFailure;
    }
    return MS::kSuccess;
  }


  inline MStatus parseTransformInput(MDataBlock& dataBlock, const MObject& obj, MFnTransform& fnObj, Attribute& objAttribute) {
    /* Gets the dag path from the given input and attaches to a function set.
    */
    MStatus status;
    MDagPath dpObj;
    status = MDagPath::getAPathTo(
      Attr::getSourceObjFromPlug(obj, dataBlock.inputValue(objAttribute).attribute()),
      dpObj
    );
    if (status == MS::kSuccess) {
      fnObj.setObject(dpObj);
    } else {
      return MS::kFailure;
    }
    return MS::kSuccess;
  }


  inline MStatus parseTransformOutput(MDataBlock& dataBlock, const MObject& obj, MFnTransform& fnObj, MObject& objAttribute) {
    /* Gets the dag path from the given input and attaches to a function set.
    */
    MStatus status;
    MDagPath dpObj;
    MPlugArray destinations = Attr::getDestinationObjFromPlug(obj, dataBlock.outputValue(objAttribute).attribute());
    if (destinations.length() > 0) {
      status = MDagPath::getAPathTo(
        destinations[0].node(),
        dpObj
      );
      if (status == MS::kSuccess) {
        fnObj.setObject(dpObj);
        return MS::kSuccess;
      }
    }
    return MS::kFailure;
  }

};

