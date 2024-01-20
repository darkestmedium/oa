// ==========================================================================
// Copyright 2019 Autodesk, Inc.  All rights reserved.
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
// ==========================================================================
#pragma once

// System includes
#include <array>
#include <math.h>
#include <cassert>
#include <memory>
#include <unordered_map>
#include <atomic>


#include <maya/MUserData.h>
#include <maya/MMatrix.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MColor.h>
#include <maya/MDistance.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MGlobal.h>
#include <maya/MDagMessage.h>
#include <maya/MEvaluationNode.h>
#include <maya/MEventMessage.h>
#include <maya/MEvaluationManager.h>
#include <maya/MEvaluationNode.h>
#include <maya/MDagModifier.h>

// Function sets
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMessageAttribute.h>

// Viewport 2.0 includes
#include <maya/MDrawRegistry.h>
#include <maya/MPxGeometryOverride.h>
#include <maya/MShaderManager.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>

// Proxies
#include <maya/MPxTransform.h>
#include <maya/MPxTransformationMatrix.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MPxDrawOverride.h>

// Open APi

#include "api/Utils.hpp"
#include "api/LMText.hpp"
#include "api/LMAttribute.hpp"



using namespace std;




//-------------------------------------------------------------------------------------------------
//
// Ctrl Transform Node definition
//
//-------------------------------------------------------------------------------------------------

class Ctrl : public MPxTransform {
  /* Clean transform instance with a custom type_name. */

public:
  // Class attributes
  static const MString typeName;
  static const MTypeId typeId;
  static const MString typeDrawDb;
  static const MString typeDrawId;

  // Node attributes

  MDagModifier modDag;
  MObject selfObject;
  MDagPath selfPath;

  MObject selfShape;

  // Constructors
  Ctrl()
    : MPxTransform()
  {};
  // Destructors
  virtual ~Ctrl() override {};

  // Class Methods
  static void *   creator() {return new Ctrl();}
  static MStatus  initialize() {return MS::kSuccess;}
  virtual void    postConstructor() override;

  void getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const override;
  SchedulingType schedulingType() const override {return SchedulingType::kParallel;}
};




// Class attributes
inline const MString Ctrl::typeName   = "ctrl";
inline const MTypeId Ctrl::typeId     = 0x6606002;
inline const MString Ctrl::typeDrawDb = "drawdb/geometry/animation/ctrl";
inline const MString Ctrl::typeDrawId = "ctrlPlugin";




inline void Ctrl::getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const {
	/* Disables Cached Playback support by default.

	Built-in locators all enable Cached Playback by default, but plug-ins have to
	explicitly enable it by overriding this method.
	This method should be overridden to enable Cached Playback by default for custom locators.

	Args:
		evalNode (MEvaluationNode&): This node's evaluation node, contains animated plug information
		disablingInfo (MNodeCacheDisablingInfo&): Information about why the node disables caching to be reported to the user
		cacheSetup (MNodeCacheSetupInfo&): Preferences and requirements this node has for caching
		monitoredAttribures (MObjectArray&): Attributes impacting the behavior of this method that will be monitored for change

	*/
	MPxTransform::getCacheSetup(evalNode, disablingInfo, cacheSetupInfo, monitoredAttributes);
	assert(!disablingInfo.getCacheDisabled());
	cacheSetupInfo.setPreference(MNodeCacheSetupInfo::kWantToCacheByDefault, true);
}


inline void Ctrl::postConstructor() {

  selfObject=thisMObject();
  MDagPath::getAPathTo(selfObject, selfPath);
  MFnDependencyNode selfFn(selfObject);

  selfFn.findPlug("shear", false).setLocked(1);
  selfFn.findPlug("rotateAxis", false).setLocked(1);

  // // Check if ctrl command already created a shape node, if it didn't create one (node editor case)
  // unsigned int numShapes;
  // selfPath.numberOfShapesDirectlyBelow(numShapes);
  // if (numShapes == 0) {
  //   selfShape = modDag.createNode("ctrlShape", selfObject);
  //   modDag.doIt();
  // }

}