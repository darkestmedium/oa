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




//-------------------------------------------------------------------------------------------------
//
// Ctrl Transform Node definition
//
//-------------------------------------------------------------------------------------------------

class Ctrl : public MPxTransform {
  /* Clean transform instance with a custom type_name. */

public:
  // Controls the size of the footprint geometry
  static Attribute    attr_in_line_matrix;
  static Attribute    attr_out_line_matrix;
  // static  MObject  inputSize;  // 'sz'   Input   Distance
  // static  MObject  outputSize; // 'osz'  Output  Distance: outputSize = [](inputSize) -> {return inputSize;}

  // Add your renderer-required-attributes here
  // static MObject inputXX;
  // static MObject outputXX;

  // Utility attribute for viewport
  // [[maya::storable(false)]] [[maya::connectable(false)]] [[maya::hidden]]
  static	MObject      geometryChanging;  // 'gcg' Output	Bool: geometryChanging = [](inputSize) -> {return true;} [*] check notes

  // Attribute dependencies:
  //      inputSize -> outputSize
  //      inputSize -> geometryChanging 
  //
  //      * inputXX -> outputXX
  //      * inputXX -> geometryChanging (if XX affect the geometry)
  //
  // "Logical" dependencies (Technique 1):
  //      outputSize, geometryChanging -> [renderer]
  //      * outputXX -> [renderer]
  //
  // Additional note:
  //
  // Q :  Why is there outputSize ? (Technique 1)
  // A :  Input attributes, like inputSize cannot be cached by Evaluation Cache
  //      Check FootPrintNode::setDependentsDirty() for more details about this work-around
  //
  // Q :  'outputXX' is not updating in EM mode? (Technique 1)
  // A :  The virtual connections to [renderer] are not understand by EM.
  //      Add this connection to FootPrintNode::setDependentsDirty().
  //
  // Q :  "geometryChanging" always returns 'true' ? (Technique 1.1)
  // A :  "geometryChanging" is a "dirty flag attribute" tracking if the node needs geometry-update
  //      If anything affecting it is changed, it will re-evaluate and return 'true' 
  //      This will notify the viewport that the geometry is changing, 
  //      Then, it will be reset to 'false' when geometry is updated (populateGeometry() is called)
  //      This allows us to track the dirty status without override setDependentsDirty() or postEvaluation()
  //      * Note, viewport will not reset its value in background evaluation (VP2 caching)
  //      Check requiresGeometryUpdate(), populateGeometry() for detail


public:
  // Class attributes
  static const MString typeName;
  static const MTypeId typeId;
  static const MString typeDrawDb;
  static const MString typeDrawId;

  // Node attributes
  // static MObject size;
  static MObject localPosition, localPositionX, localPositionY, localPositionZ;
  static MObject localRotate, localRotateX, localRotateY, localRotateZ;
  static MObject localScale, localScaleX, localScaleY, localScaleZ;

  static MObject attr_shape_indx;
  static MObject attr_fill_shape;
  static MObject attr_fill_shape_opacity;
  static MObject attr_line_width;
  static MObject attr_in_draw_line;

  static MObject attr_draw_solver_mode;
  static MObject attr_solver_mode_size;
  static MObject attr_solver_mode_positionX, attr_solver_mode_positionY, attr_solver_mode_positionZ, attr_solver_mode_position;
  static MObject attrInText;
  static MObject attrXRay;

  static MObject attrInFkIk;
  static MObject attr_component;

  // Use only on dynamic ctrl like fk/ik blending or pole vectors
  bool draw_line;

  MObject self_object;
  MDagPath self_path;

  // Constructors
  Ctrl()
    : MPxTransform()
  {};
  // Destructors
  virtual ~Ctrl() override {};

  // Class Methods
  static void *   creator() {return new Ctrl();};
  static MStatus  initialize();
  virtual void    postConstructor() override;

  MStatus         setDependentsDirty(const MPlug& plugBeingDirtied, MPlugArray& affectedPlugs) override;
  MStatus         compute(const MPlug& plug, MDataBlock& dataBlock) override;
  MStatus         postEvaluation(const  MDGContext& context, const MEvaluationNode& evaluationNode, PostEvaluationType evalType) override; 

  void            getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const override;
  SchedulingType  schedulingType() const override {return SchedulingType::kParallel;}

  bool            isBounded() const override {return true;}
  virtual MBoundingBox boundingBox() const override;
  virtual bool     treatAsTransform() const override {return false;}
};




//-------------------------------------------------------------------------------------------------
//
// Ctrl Draw Override definition
//
//-------------------------------------------------------------------------------------------------

class CtrlUserData : public MUserData {
public:
  MMatrix         matLocal;
  MBoundingBox    bbox;
  MMatrix         matPv;
  MPoint          posDrawPvTo;

  short           indxShape;
  bool            bFillShape;
  float           fillShapeOpacity;
  bool            bXRay;
  bool            bXRayJoint;
  unsigned int    prio_depth;

  MPointArray     arrayVertecies;
  MPointArray     arrayEdges;
  MPointArray     arrayTriangles;
  MPointArray     arrayLine;

  float           widthLine;
  MColor          colWireframe;
  MColor          colShape;
  MColor          colGrey;

  // Fk / Ik state
  MObject objDrawLineTo;
  MMatrix matTo;
  double fkIk;
  bool bDrawline;

  bool draw_solver_mode;
  unsigned int solver_mode_size;
  MPoint pos_solver_mode;
  MString str_solver_mode;

  // Constructors
  CtrlUserData()
    : MUserData(false)
    , colGrey(0.5, 0.5, 0.5)
  {};

  // Destructor
  virtual ~CtrlUserData() override {};

  virtual void getPlugs(const MObject& object);
  virtual void getShape(const MObject& object, const MDagPath& dp_object, MMatrix matrix);
  virtual void getBbox(const MObject& object, const MDagPath& dp_object, MMatrix matrix);
  virtual void getText(const MObject& object);


  MBoundingBox PopulateBoundingBox(const vector<array<float,3>>& bbox) {
    return MBoundingBox(MPoint(bbox[0][0], bbox[0][1], bbox[0][2]), MPoint(bbox[1][0], bbox[1][1], bbox[1][2]));
  }

  void PopulateVertexBuffer(
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

  void PopulateVertexBuffer(
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




class CtrlDrawOverride : public MHWRender::MPxDrawOverride {
public:
  // Destructor
  virtual ~CtrlDrawOverride() override {
    ptrCtrl = NULL; 
    if (fModelEditorChangedCbId != 0) {
      MMessage::removeCallback(fModelEditorChangedCbId);
      fModelEditorChangedCbId = 0;
    }
  };
  // Public Methods
  static MHWRender::MPxDrawOverride* creator(const MObject& obj) {return new CtrlDrawOverride(obj);}
  virtual MHWRender::DrawAPI supportedDrawAPIs() const override {return MHWRender::kAllDevices;}

  virtual bool isBounded(const MDagPath& objPath, const MDagPath& cameraPath) const override {return true;}
  virtual MBoundingBox boundingBox(
    const MDagPath& objPath,
    const MDagPath& cameraPath
  ) const override;
  virtual bool hasUIDrawables() const override {return true;}
  virtual MUserData* prepareForDraw(
    const MDagPath& objPath,
    const MDagPath& cameraPath,
    const MHWRender::MFrameContext& frameContext,
    MUserData* oldData
  ) override;
  virtual void addUIDrawables(
    const MDagPath& objPath,
    MHWRender::MUIDrawManager& drawManager,
    const MHWRender::MFrameContext& frameContext,
    const MUserData* data
  ) override;

private:
  // Constructors
  CtrlDrawOverride(const MObject& obj)
    : MHWRender::MPxDrawOverride(obj, nullptr, false)
    , ptrCtrl(nullptr)
  {
    fModelEditorChangedCbId = MEventMessage::addEventCallback("modelEditorChanged", OnModelEditorChanged, this);
    MStatus status;
    MFnDependencyNode fn_node(obj, &status);
    ptrCtrl = status ? dynamic_cast<Ctrl*>(fn_node.userNode()) : NULL;
  };

  Ctrl*   ptrCtrl;    // The node we are rendering
  MCallbackId fModelEditorChangedCbId;
  static void OnModelEditorChanged(void *clientData);
};
