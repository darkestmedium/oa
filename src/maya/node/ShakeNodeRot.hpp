#pragma once

#include "ShakeNode.hpp"

// System Includes
#include <cassert>
#include <math.h>

// Maya Api Includes
#include <maya/MPxNode.h>
#include <maya/MGlobal.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MDataHandle.h>
#include <maya/MEvaluationNode.h>
#include <maya/MNodeCacheDisablingInfo.h>
#include <maya/MNodeCacheSetupInfo.h>
#include <maya/MObjectArray.h>

// Function Sets
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnCompoundAttribute.h>



class ShakeNodeRot : public ShakeNode {
public:
  // Constructors
  ShakeNodeRot(): ShakeNode() {};

  // Destructor
  virtual ~ShakeNodeRot() override {};

  // Public Methods
  static void* creator() {return new ShakeNodeRot();}
  static MStatus initialize();
  bool isPassiveOutput(const MPlug& plug)	const override;
  virtual MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override;
  void getCacheSetup(
    const MEvaluationNode& evalNode,
    MNodeCacheDisablingInfo& disablingInfo,
    MNodeCacheSetupInfo& cacheSetupInfo,
    MObjectArray& monitoredAttributes
  ) const override;

  SchedulingType schedulingType() const override {return SchedulingType::kParallel;}

  // Node's attributes
  static const MString typeName;
  static const MTypeId typeId;

  // Node's input attributes
  static MObject enableAttr;
  static MObject inTimeAttr;
  static MObject weightAttr;
  static MObject seedAttr;
  static MObject frequencyAttr;
  static MObject strengthAttrX;
  static MObject strengthAttrY;
  static MObject strengthAttrZ;
  static MObject strengthAttr;
  static MObject fractalAttr;
  static MObject roughnessAttr;
  static MObject shakeAttr;

  // Node's output attributes
  static MObject outputAttrX;
  static MObject outputAttrY;
  static MObject outputAttrZ;
  static MObject outputAttr;


private:
  // Private Methods
  double radians(double degrees);

  // Private Data
  static PerlinNoise __ipNoise;
};
