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
#include <maya/MGlobal.h>
#include <maya/MDagMessage.h>
#include <maya/MEvaluationNode.h>
#include <maya/MEventMessage.h>
#include <maya/MEvaluationManager.h>
#include <maya/MEvaluationNode.h>

// Function sets
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnDependencyNode.h>

// Viewport 2.0 includes
#include <maya/MDrawRegistry.h>
#include <maya/MPxGeometryOverride.h>
#include <maya/MShaderManager.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>

// Proxies
#include <maya/MPxTransform.h>
// #include <maya/MPxTransformationMatrix.h>
// #include <maya/MPxLocatorNode.h>
// #include <maya/MPxSurfaceShape.h>
#include <maya/MPxDrawOverride.h>

// Open APi
#include "api/Shapes.hpp"
#include "api/Utils.hpp"
#include "api/Text.hpp"
#include "api/Attribute.hpp"




using namespace std;



class TransformShape : public MPxTransform {
  /* Clean transform instance with a custom type_name.
  
  Reference:
    Controls the size of the footprint geometry
    static  MObject  inputSize;  // 'sz'   Input   Distance
    static  MObject  outputSize; // 'osz'  Output  Distance: outputSize = [](inputSize) -> {return inputSize;}

    Add your renderer-required-attributes here
    static MObject inputXX;
    static MObject outputXX;

    Utility attribute for viewport
    [[maya::storable(false)]] [[maya::connectable(false)]] [[maya::hidden]]

    Attribute dependencies:
      inputSize -> outputSize
      inputSize -> geometryChanging 
    
      * inputXX -> outputXX
      * inputXX -> geometryChanging (if XX affect the geometry)
    
     "Logical" dependencies (Technique 1):
        outputSize, geometryChanging -> [renderer]
        * outputXX -> [renderer]
    
    Additional note:
    
    Q : Why is there outputSize ? (Technique 1)
    A : Input attributes, like inputSize cannot be cached by Evaluation Cache
        Check FootPrintNode::setDependentsDirty() for more details about this work-around
    
    Q : 'outputXX' is not updating in EM mode? (Technique 1)
    A : The virtual connections to [renderer] are not understand by EM.
        Add this connection to FootPrintNode::setDependentsDirty().
    
    Q : "geometryChanging" always returns 'true' ? (Technique 1.1)
    A : "geometryChanging" is a "dirty flag attribute" tracking if the node needs geometry-update
        If anything affecting it is changed, it will re-evaluate and return 'true' 
        This will notify the viewport that the geometry is changing, 
        Then, it will be reset to 'false' when geometry is updated (populateGeometry() is called)
        This allows us to track the dirty status without override setDependentsDirty() or postEvaluation()
        * Note, viewport will not reset its value in background evaluation (VP2 caching)
        Check requiresGeometryUpdate(), populateGeometry() for detail

  */

public:
  // Class attributes
  static const MString typeName;
  static const MTypeId typeId;
  static const MString typeDrawDb;
  static const MString typeDrawId;

  // Node's input attributes
  static MObject localPosition;
  static MObject   localPositionX;
  static MObject   localPositionY;
  static MObject   localPositionZ;
  static MObject localRotate;
  static MObject   localRotateX;
  static MObject   localRotateY;
  static MObject   localRotateZ;
  static MObject localScale;
  static MObject   localScaleX;
  static MObject   localScaleY;
  static MObject   localScaleZ;

  static MObject shapeIndx;
  static MObject fillShape;
  static MObject fillShapeOpacity;
  static MObject lineWidth;

  static MObject text;
  static MObject textPosition;
  static MObject   textPositionX;
  static MObject   textPositionY;
  static MObject   textPositionZ;
  static MObject textSize;

  static MObject drawLine;
  static MObject lineMatrix;
  static MObject xRay;

  // Node's output attributes

  MObject   selfObject;
  MDagPath  selfPath;

  // Constructors
  TransformShape()
    : MPxTransform()
  {};
  // Destructors
  virtual ~TransformShape() override {};

  // Class Methods
  static void *   creator() {return new TransformShape();};
  static MStatus  initialize();
  static MStatus  initializeLocalTransforms();
  static MStatus  initializeShape();
  // virtual void    postConstructor() override;

  // MStatus         setDependentsDirty(const MPlug& plugBeingDirtied, MPlugArray& affectedPlugs) override;
  MStatus         compute(const MPlug& plug, MDataBlock& dataBlock) override {return MS::kSuccess;};
  // MStatus         postEvaluation(const  MDGContext& context, const MEvaluationNode& evaluationNode, PostEvaluationType evalType) override; 

  void            getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const override;
  SchedulingType  schedulingType() const override {return SchedulingType::kParallel;}

  bool                  isBounded() const override {return true;}
  // virtual MBoundingBox  boundingBox() const override;
  // virtual bool          treatAsTransform() const override {return false;}
};





// class TransformShapeData : public MUserData {
// public:
//   MMatrix       matLocal;
//   MBoundingBox  bbox;
//   MMatrix       matLine;
//   MPoint        posLine;

//   short         indxShape;
//   bool          bFillShape;
//   float         fillShapeOpacity;
//   bool          bXRay;
//   bool          bXRayJoint;
//   unsigned int  prioDepth;

//   MPointArray   arrayVertecies;
//   MPointArray   arrayEdges;
//   MPointArray   arrayTriangles;
//   MPointArray   arrayLine;

//   float         widthLine;
//   MColor        colWireframe;
//   MColor        colShape;
//   MColor        colGrey;

//   // Fk / Ik state
//   MObject       objDrawLineTo;
//   MMatrix       matTo;
//   double        fkIk;
//   bool          bDrawline;

//   bool          bDrawSolverMode;
//   unsigned int  sizeSolverMode;
//   MPoint        posSolverMode;
//   MString       strSolverMode;

//   // Constructors
//   TransformShapeData()
//     : MUserData(false)
//     , colGrey(0.5, 0.5, 0.5)
//   {};

//   // Destructor
//   virtual ~TransformShapeData() override {};

//   virtual void getBbox(const MObject& object, MMatrix matrix);
//   virtual void getPlugs(const MObject& object);
//   virtual void getShape(const MObject& object, const MDagPath& dpObject, MMatrix matrix);
//   virtual void getText(const MObject& object);
// }