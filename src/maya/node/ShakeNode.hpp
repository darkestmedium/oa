#pragma once

#include "PerlinNoise.hpp"

// System Includes
#include <cassert>

// Maya General Includes
#include <maya/MGlobal.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MDataHandle.h>

// Function Sets
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnAttribute.h>

// Proxies
#include <maya/MPxNode.h>



class ShakeNode : public MPxNode
{
public:
  // Constructors
  ShakeNode() : MPxNode() {};

  // Destructor
  virtual ~ShakeNode() override {};

  // Public Methods
  static void* creator() {return new ShakeNode();}
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
  // Private Data
  static PerlinNoise __ipNoise;
};
