#pragma once

// System includes
#include <array>
#include <math.h>
#include <cassert>
#include <memory>
#include <unordered_map>
#include <atomic>

// Maya
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
#include <maya/MAnimControl.h>
#include <maya/MDagMessage.h>
#include <maya/MEvaluationNode.h>
#include <maya/MEventMessage.h>
#include <maya/MEvaluationManager.h>
#include <maya/MEvaluationNode.h>

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
#include "api/Text.hpp"
#include "api/Attribute.hpp"




using namespace std;




class Speedometer : public MPxLocatorNode {
public:
  // Class attributes
  static const MString typeName;
  static const MTypeId typeId;
  static const MString drawDbClassification;
  static const MString drawRegistrationId;

  // Node's Input Attributes
  static MObject localPosition, localPositionX, localPositionY, localPositionZ;
  static MObject localRotate, localRotateX, localRotateY, localRotateZ;
  static MObject localScale, localScaleX, localScaleY, localScaleZ;

  static MObject attrShapeIndx;
  static Attribute attrInTransform;
  static MObject attrInTime;
  static MObject textOffsetAttr;
  static MObject textAttr;
  static MObject textSizeAttr;
  static MObject precisionAttr;
  static MObject unitTypeAttr;
  static MObject lineWidthAttr;

  // Nodes's Output Attributes
  static MObject updateAttr;
  static MObject outputAttr;

  // Public Data
  MMatrix matTransform;
  MPoint posCurrent, posCached;

  unsigned int unitTypeIndex;
  std::string text;
  int precision;
  double speed;
  std::string strSpeed;

  std::string dispUnit;
  // MAnimControl animCtl;
  MDGModifier modDg;

  MTime timeCurrent, timeCached;
  MDagPath dpShape;
  MObject objSelf;

  // Constructors
  Speedometer() 
    : MPxLocatorNode()
    , posCached(MPoint(0.0, 0.0, 0.0))
    , timeCached(MTime(0.0, MTime::uiUnit()))
  {};

  // Destructorstatic	MObject      geometryChanging;
  virtual ~Speedometer() override {};

  // Public methods - overrides
  static void*    creator() {return new Speedometer();}
  static MStatus  initialize();
  virtual void    postConstructor() override;

  virtual MStatus setDependentsDirty(const MPlug& plugBeingDirtied, MPlugArray& affectedPlugs) override;
  virtual MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override;

  void            getCacheSetup(const MEvaluationNode& evalNode,	MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const override;
  SchedulingType  schedulingType() const override {return SchedulingType::kParallel;}

  virtual bool          isBounded() const override {return true;}
  virtual MBoundingBox  boundingBox() const override;

  // Custom Methods
  virtual void getSelfPosition();
  double getNumericFPS();
  double calculateSpeed(double distance, double timePassed);
  virtual void setDisplayUnits(unsigned int unitTypeIndex);
  std::string prd(const double number, unsigned int decDigits);

  MStatus parseDataBlock(MDataBlock& dataBlock);
  MStatus getSpeed();
  MStatus updateOutput(const MPlug& plug, MDataBlock& dataBlock);
};




//-------------------------------------------------------------------------------------------------
//
// Draw Override definition
//
//-------------------------------------------------------------------------------------------------




class SpeedometerData : public MUserData {
public:
	// Constructors
	SpeedometerData()
    : MUserData()
  {};

	// Destructor
	virtual ~SpeedometerData() override {};

	// Public Methods
	virtual void getPlugs(const MObject& obj);
	virtual void getBBox(const MObject& obj, MMatrix matrix);
	virtual void getShpae(const MObject& obj, MMatrix matrix);

	// Public Data
	MBoundingBox bBox;
	MMatrix matrix;

	MPointArray fTransformedList;
	MPointArray fLineList;

	float _lineWidth;
	MColor _wfColor;
	MString __drawText;
	MPoint _textOffset;
	unsigned int _textSize;
};




class SpeedometerDrawOverride : public MHWRender::MPxDrawOverride {
public:
	// Destructor
	virtual ~SpeedometerDrawOverride() override {};

	// Public Methods
	static MHWRender::MPxDrawOverride* creator(const MObject& obj) {return new SpeedometerDrawOverride(obj);}
	virtual MHWRender::DrawAPI supportedDrawAPIs() const override {return MHWRender::kAllDevices;}

	virtual bool isBounded(const MDagPath& objPath, const MDagPath& cameraPath) const override {return true;}
	virtual MBoundingBox boundingBox(
		const MDagPath& objPath,
		const MDagPath& cameraPath
	) const override;

	virtual bool hasUIDrawables() const override {return true;}
	virtual void addUIDrawables(
		const MDagPath& objPath,
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext,
		const MUserData* data
	) override;

	virtual MUserData* prepareForDraw(
		const MDagPath& objPath,
		const MDagPath& cameraPath,
		const MHWRender::MFrameContext& frameContext,
		MUserData* oldData
	) override;


private:
	// Constructors
	SpeedometerDrawOverride(const MObject& obj)
    : MHWRender::MPxDrawOverride(obj, nullptr)
  {};

};
