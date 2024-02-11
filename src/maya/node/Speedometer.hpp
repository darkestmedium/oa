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
#include "api/Shapes.hpp"
#include "api/Utils.hpp"
#include "api/Text.hpp"
#include "api/Attribute.hpp"




using namespace std;




class Speedometer : public MPxTransform {
public:
  // Class attributes
  static const MString typeName;
  static const MTypeId typeId;
  static const MString typeDrawDb;
  static const MString typeDrawId;

  // Node's Input Attributes
  static MObject localPosition, localPositionX, localPositionY, localPositionZ;
  static MObject localRotate, localRotateX, localRotateY, localRotateZ;
  static MObject localScale, localScaleX, localScaleY, localScaleZ;

  static MObject attrIndxShape;
  static MObject attrFillShape;
  static MObject attrFillShapeOpacity;
  static MObject attrWidthLine;
  static MObject attrInDrawLine;

  static Attribute attrInLineMatrix;
  static Attribute attrOutLineMatrix;

  static MObject attrText;
  static MObject textPosition, textPositionX, textPositionY, textPositionZ;
  static MObject attrTextSize;
  static MObject attrXRay;
  static MObject attrPrecision;
  static MObject attrUnitType;

  // static Attribute attrInTransform;
  static MObject attrInTime;

  // Nodes's Output Attributes
  static	MObject geometryChanging;
  static	MObject attrOutText;

  // Public Data
  MMatrix matTransform;
  MPoint  posCurrent, posCached;

  unsigned int indxUnitType;
  string  text;
  int     precision;
  double  speed;
  string  strSpeed;
  string  dispUnit;

  MTime       timeCurrent, timeCached;
  MObject     selfObject;
  MDagPath    selfPath;
  MDGModifier modDg;

  // Use only on dynamic ctrl like fk/ik blending or pole vectors
  bool      bDrawLine;

  // Constructors
  Speedometer() 
    : MPxTransform()
    , posCached(MPoint(0.0, 0.0, 0.0))
    , timeCached(MTime(0.0, MTime::uiUnit()))
  {};

  // Destructor
  virtual ~Speedometer() override {};

  // Public methods - overrides
  static void*    creator() {return new Speedometer();}
  static MStatus  initialize();
  virtual void    postConstructor() override;

  virtual MStatus setDependentsDirty(const MPlug& plug, MPlugArray& affectedPlugs) override;
  virtual MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override;
  // MStatus         postEvaluation(const  MDGContext& context, const MEvaluationNode& evaluationNode, PostEvaluationType evalType) override; 

  void            getCacheSetup(const MEvaluationNode& evalNode,	MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const override;
  SchedulingType  schedulingType() const override {return SchedulingType::kParallel;}

  virtual bool          isBounded() const override {return true;}
  virtual MBoundingBox  boundingBox() const override;

  // Custom Methods
  virtual void getSelfPosition();
  double getNumericFPS();
  double calculateSpeed(double distance, double timePassed);
  virtual void setDisplayUnits(unsigned int indxUnitType);
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
  MMatrix       matLocal;
  MBoundingBox  bbox;
  MMatrix       matPv;
  MPoint        posDrawPvTo;

  short         indxShape;
  bool          bFillShape;
  float         fillShapeOpacity;
  bool          bXRay;
  bool          bXRayJoint;
  unsigned int  prioDepth;

  MPointArray   arrayVertecies;
  MPointArray   arrayEdges;
  MPointArray   arrayTriangles;
  MPointArray   arrayLine;

  float         widthLine;
  MColor        colWireframe;
  MColor        colShape;
  MColor        colGrey;

  MObject       objDrawLineTo;
  MMatrix       matTo;
  bool          bDrawline;

  MString       textToDraw;
  unsigned int  sizeText;
  MPoint        posText;


  // Constructors
  SpeedometerData()
    : MUserData()
    , colGrey(0.5, 0.5, 0.5)
  {};

  // Destructor
  virtual ~SpeedometerData() override {};

  virtual void getBbox(const MObject& object, MMatrix matrix);
  virtual void getPlugs(const MObject& object);
  virtual void getShape(const MObject& object, const MDagPath& dpObject, MMatrix matrix);
  // virtual void getText(const MObject& object);

  MBoundingBox populateBoundingBox(const vector<array<float,3>>& bbox) {
    return MBoundingBox(MPoint(bbox[0][0], bbox[0][1], bbox[0][2]), MPoint(bbox[1][0], bbox[1][1], bbox[1][2]));
  }

  void populateVertexBuffer(
    const vector<array<float,3>>& points,
    const vector<array<int,2>>& idxEdges,
    MPointArray& vertecies,
    MPointArray& edges,
    MMatrix& matrix
  ) {
    // Multiply the points by the matrix first
    for (int i=0; i<points.size(); i++) {
      vertecies.append(MPoint(points[i][0], points[i][1], points[i][2]) * matrix);
    }
    // Fill edges based on vertecies
    for (int i=0; i<idxEdges.size(); i++) {
      edges.append(vertecies[idxEdges[i][0]]);
      edges.append(vertecies[idxEdges[i][1]]);
    }
  };

  void populateVertexBuffer(
    const vector<array<float,3>>& points,
    const vector<array<int,2>>& idxEdges,
    const vector<array<int,3>>& idxTriangles,
    MPointArray& vertecies,
    MPointArray& edges,
    MPointArray& triangles,
    MMatrix& matrix
  ) {
    // Multiply the points by the matrix first
    for (int i=0; i<points.size(); i++) {
      vertecies.append(MPoint(points[i][0], points[i][1], points[i][2]) * matrix);
    }
    // Fill edges based on vertecies
    for (int i=0; i<idxEdges.size(); i++) {
      edges.append(vertecies[idxEdges[i][0]]);
      edges.append(vertecies[idxEdges[i][1]]);
    }
    // Fill triangles
    for (int i=0; i<idxTriangles.size(); i++) {
      triangles.append(vertecies[idxTriangles[i][0]]);
      triangles.append(vertecies[idxTriangles[i][1]]);
      triangles.append(vertecies[idxTriangles[i][2]]);
    }
  };
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
