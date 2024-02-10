#include "Ctrl.hpp"




//--------------------------------------------------------------------------------------
//
// Ctrl Transform Node implementation with standard viewport draw
//
//--------------------------------------------------------------------------------------


// MObject Ctrl::attrOutLineMatrix	= {};
// MObject Ctrl::inputSize            = {};
MObject Ctrl::geometryChanging        = {};


// Class attributes
const MString Ctrl::typeName 	  = "ctrl";
const MTypeId Ctrl::typeId 	    = 0x6606002;
const MString Ctrl::typeDrawDb	= "drawdb/geometry/animation/ctrl";
const MString Ctrl::typeDrawId	= "ctrlPlugin";

MObject Ctrl::localPosition, Ctrl::localPositionX, Ctrl::localPositionY, Ctrl::localPositionZ;
MObject Ctrl::localRotate, Ctrl::localRotateX, Ctrl::localRotateY, Ctrl::localRotateZ;
MObject Ctrl::localScale, Ctrl::localScaleX, Ctrl::localScaleY, Ctrl::localScaleZ;

MObject Ctrl::attrIndxShape;
MObject Ctrl::attrFillShape;
MObject Ctrl::attrFillShapeOpacity;
MObject Ctrl::attrWidthLine;

MObject Ctrl::attrInDrawLine;
Attribute Ctrl::attrInLineMatrix;
Attribute Ctrl::attrOutLineMatrix;

MObject Ctrl::attrDrawSolverMode;
MObject Ctrl::attrSolverModeSize;
MObject Ctrl::attrSolverModePosition, Ctrl::attrSolverModePositionX, Ctrl::attrSolverModePositionY, Ctrl::attrSolverModePositionZ;
MObject Ctrl::attrXRay;
MObject Ctrl::attrInFkIk;
MObject Ctrl::attrComponent;



MStatus Ctrl::initialize() {
  /* Node Initializer.

  This method initializes the node, and should be overridden in user-defined nodes.
  
  Returns:
    status code (MStatus): kSuccess if the operation was successful, kFailure if an	error occured
      during the operation.

  */
  MFnUnitAttribute    fnUnit;
  MFnNumericAttribute fnNum;
  MFnEnumAttribute    fnEnum;
  MFnMatrixAttribute  fnMat;
  MFnTypedAttribute   fnType;
  MFnMessageAttribute fnMess;

  { // Local Transforms
    localPositionX = fnNum.create("localPositionX", "lpx", MFnNumericData::kFloat);
    localPositionY = fnNum.create("localPositionY", "lpy", MFnNumericData::kFloat);
    localPositionZ = fnNum.create("localPositionZ", "lpz", MFnNumericData::kFloat);
    localPosition = fnNum.create("localPosition", "lp", localPositionX, localPositionY, localPositionZ);
    fnNum.setStorable(true);
    fnNum.setStorable(true);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);

    localRotateX = fnUnit.create("localRotateX", "lrx", MFnUnitAttribute::kAngle);
    localRotateY = fnUnit.create("localRotateY", "lry", MFnUnitAttribute::kAngle);
    localRotateZ = fnUnit.create("localRotateZ", "lrz", MFnUnitAttribute::kAngle);
    localRotate = fnNum.create("localRotate", "lr", localRotateX, localRotateY, localRotateZ);
    fnNum.setStorable(true);
    fnNum.setStorable(true);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);

    localScaleX = fnNum.create("localScaleX", "lsx", MFnNumericData::kFloat, 1.0);
    localScaleY = fnNum.create("localScaleY", "lsy", MFnNumericData::kFloat, 1.0);
    localScaleZ = fnNum.create("localScaleZ", "lsz", MFnNumericData::kFloat, 1.0);
    localScale = fnNum.create("localScale", "ls", localScaleX, localScaleY, localScaleZ);
    fnNum.setStorable(true);
    fnNum.setStorable(true);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);
  }

  { // Shape
    attrIndxShape = fnEnum.create("shape", "shp");
    fnEnum.addField("Cube", 0);
    fnEnum.addField("Square", 1);
    fnEnum.addField("Cylinder", 2);
    fnEnum.addField("Cone", 3);
    fnEnum.addField("Circle", 4);
    fnEnum.addField("Sphere", 5);
    fnEnum.addField("Dome", 6);
    fnEnum.addField("Diamond", 7);
    fnEnum.addField("Pyramid", 8);
    fnEnum.addField("Triangle", 9);
    fnEnum.addField("Prism", 10);
    fnEnum.addField("Locator", 11);
    fnEnum.addField("Frame", 12);
    fnEnum.addField("Arrow", 13);
    fnEnum.addField("Arrow2Way", 14);
    fnEnum.addField("Circle4Arrows", 15);
    fnEnum.addField("Hip", 16);
    fnEnum.addField("CircleHalfDouble", 17);
    fnEnum.addField("PinRound", 18);
    fnEnum.addField("Clavicle", 19);
    fnEnum.addField("Pointer2Way", 20);
    fnEnum.addField("Pointer2WayArc", 21);
    fnEnum.addField("Cross", 22);
    fnEnum.addField("CrossShort", 23);
    fnEnum.addField("None", 24);
    fnEnum.setKeyable(false);
    fnEnum.setStorable(true);
    fnEnum.setChannelBox(true);

    attrFillShape = fnNum.create("fillShape", "fp", MFnNumericData::kBoolean, false);
    fnNum.setStorable(true);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);

    attrFillShapeOpacity = fnNum.create("fillShapeOpacity", "fso", MFnNumericData::kFloat, 0.1);
    fnNum.setMin(0.01);
    fnNum.setMax(1);
    fnNum.setStorable(true);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);
  
    attrWidthLine = fnNum.create("lineWidth", "lw", MFnNumericData::kDouble);
    fnNum.setMin(0.5);
    fnNum.setDefault(1.0);
    fnNum.setMax(5);
    fnNum.setStorable(true);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);

    attrInDrawLine = fnNum.create("drawLine", "dl", MFnNumericData::kBoolean, false);
    fnNum.setStorable(true);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);

    createAttribute(attrInLineMatrix, "drawLineTo", DefaultValue<MMatrix>());

    attrXRay = fnNum.create("xRay", "xr", MFnNumericData::kBoolean, false);
    fnNum.setStorable(true);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);
  }

  { // Text
    attrDrawSolverMode = fnNum.create("drawSolverMode", "dsm", MFnNumericData::kBoolean, false);
    fnNum.setStorable(true);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);

    attrSolverModePositionX = fnNum.create("solverModePositionX", "smpx", MFnNumericData::kDouble, 0.0);
    attrSolverModePositionY = fnNum.create("solverModePositionY", "smpy", MFnNumericData::kDouble, 0.0);
    attrSolverModePositionZ = fnNum.create("solverModePositionZ", "smpz", MFnNumericData::kDouble, 0.0);
    attrSolverModePosition = fnNum.create("solverModePosition", "smp", attrSolverModePositionX, attrSolverModePositionY, attrSolverModePositionZ);
    fnNum.setStorable(true);
    fnNum.setStorable(true);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);

    attrSolverModeSize = fnNum.create("solverModeSize", "sms", MFnNumericData::kInt, 14);
    fnNum.setStorable(true);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);
    fnNum.setMin(9);
    fnNum.setMax(24);
  }

  attrInFkIk = fnNum.create("fkIk", "fkIk", MFnNumericData::kDouble);
  fnNum.setStorable(true);
  fnNum.setKeyable(false);
  fnNum.setChannelBox(true);
  fnNum.setSoftMin(0.0);
  fnNum.setSoftMax(100.0);

  attrComponent = fnMess.create("component", "component");
  fnMess.setWritable(false);
  fnMess.setReadable(true);

  geometryChanging = fnNum.create("geometryChanging", "gcg", MFnNumericData::kBoolean, true);
  fnNum.setStorable(false);
  fnNum.setHidden(true);
  fnNum.setConnectable(false);

  // Add attributes
  addAttributes(
    localPosition, localRotate, localScale,
    attrIndxShape, attrFillShape, attrFillShapeOpacity, attrWidthLine,
    attrInDrawLine, attrInLineMatrix,
    attrDrawSolverMode,
    attrSolverModeSize,
    attrSolverModePosition,
    attrInFkIk,
    attrXRay,
    geometryChanging,
    attrComponent
  );

  attributeAffects(Ctrl::attrInLineMatrix, Ctrl::geometryChanging);

  return MS::kSuccess;
}


MStatus Ctrl::setDependentsDirty(const MPlug& plug, MPlugArray& affectedPlugs) {
  /* Sets the relation between attributes and marks the specified plugs dirty.

  Args:
    plugBeingDirtied (&MPlug): Plug which is being set dirty by Maya.
    affectedPlugs (&MPlugArray): The programmer should add any plugs which they want to set dirty
      to this list.

  */
  bDrawLine = MPlug(selfObject, attrInDrawLine).asBool();
  if (bDrawLine) MHWRender::MRenderer::setGeometryDrawDirty(selfObject);

  // Update the shape attrs for editing / rig creation - draw - line has to be on for
  // performance optimizations
  if (MEvaluationManager::graphConstructionActive()) {
    if ( plug == attrInLineMatrix
      || plug == localPosition
      || plug == localRotate
      || plug == localScale
      || plug == attrIndxShape
      || plug == attrFillShape
      || plug == attrFillShapeOpacity
      || plug == attrWidthLine
      || plug == attrInDrawLine
      || plug == attrOutLineMatrix
      || plug == attrDrawSolverMode
      || plug == attrSolverModeSize
      || plug == attrSolverModePosition
      || plug == attrInFkIk
      || plug == attrXRay
    ) {affectedPlugs.append(MPlug(selfObject, geometryChanging));}
  }

  return MS::kSuccess;
}


MStatus Ctrl::compute(const MPlug& plug, MDataBlock& dataBlock) {
  // Check documentation in "class FootPrintNode" for descriptions about the attributes here
  if (plug == geometryChanging) {
    MDataHandle boolHandle = dataBlock.outputValue(geometryChanging);
    boolHandle.setBool(true);
    MHWRender::MRenderer::setGeometryDrawDirty(selfObject);
  }	else {
    return MS::kUnknownParameter;
  }
  return MStatus::kSuccess;
}


// Called before this node is evaluated by Evaluation Manager
MStatus Ctrl::postEvaluation(const MDGContext& context, const MEvaluationNode& evaluationNode, PostEvaluationType evalType) {
  // For cache restoration only
  // This method is responsible for fixing the 'geometryChanging' flag in cache restore frames
  // Because in cache store phase,
  // PopulateGeometry & Viewport-Caching happens before Evaluation-Cache store
  // The value of 'geometryChanging' will always be set to 'false' (it is already used by render)
  // Thus, we have to fix the geometryChanging attribute to the correct value 
  MStatus status;
  if (evalType == PostEvaluationEnum::kEvaluatedDirectly && evaluationNode.dirtyPlugExists(geometryChanging, &status) && status) {
    MDataBlock data = forceCache();
    MDataHandle boolHandle = data.outputValue(geometryChanging, &status);
    if (status != MStatus::kSuccess) return status;
    boolHandle.setBool(true);
    boolHandle.setClean();
  }
  return MPxTransform::postEvaluation(context, evaluationNode, evalType);
}


void Ctrl::getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const {
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


void Ctrl::postConstructor() {
  selfObject = thisMObject();
  MDagPath::getAPathTo(selfObject, selfPath);
  MFnDependencyNode fnThis(selfObject);

  fnThis.findPlug("shear", false).setLocked(1);
  fnThis.findPlug("rotateAxis", false).setLocked(1);
  // Set color
  fnThis.findPlug("overrideEnabled", false).setBool(1);
  fnThis.findPlug("overrideRGBColors", false).setBool(1);
  fnThis.findPlug("overrideColorR", false).setDouble(1.0);
  fnThis.findPlug("overrideColorG", false).setDouble(1.0);
  fnThis.findPlug("overrideColorB", false).setDouble(0.25);

  bDrawLine = MPlug(selfObject, Ctrl::attrInDrawLine).asBool();
}


MBoundingBox Ctrl::boundingBox() const {
  // Get the size
  CtrlUserData data;
  data.getPlugs(selfObject);
  data.getBbox(selfObject, data.matLocal);

  return data.bbox;
}


void CtrlUserData::getPlugs(const MObject& object) {
  /* Get all the necessary data from the attributes of the locator.

  Args:
    obj (MObject &): Object passed to query the attrbutes from

  */
  float tx = MPlug(object, Ctrl::localPositionX).asFloat();
  float ty = MPlug(object, Ctrl::localPositionY).asFloat();
  float tz = MPlug(object, Ctrl::localPositionZ).asFloat();

  float rx = MPlug(object, Ctrl::localRotateX).asFloat();
  float ry = MPlug(object, Ctrl::localRotateY).asFloat();
  float rz = MPlug(object, Ctrl::localRotateZ).asFloat();

  float sx = MPlug(object, Ctrl::localScaleX).asFloat();
  float sy = MPlug(object, Ctrl::localScaleY).asFloat();
  float sz = MPlug(object, Ctrl::localScaleZ).asFloat();

  this->matLocal = MEulerRotation(rx, ry, rz).asMatrix();
  this->matLocal[3][0] = tx; this->matLocal[3][1] = ty;	this->matLocal[3][2] = tz;
  this->matLocal[0][0] *= sx; this->matLocal[0][1] *= sx;	this->matLocal[0][2] *= sx;
  this->matLocal[1][0] *= sy; this->matLocal[1][1] *= sy;	this->matLocal[1][2] *= sy;
  this->matLocal[2][0] *= sz; this->matLocal[2][1] *= sz;	this->matLocal[2][2] *= sz;

  indxShape = MPlug(object, Ctrl::attrIndxShape).asShort();
  bFillShape = MPlug(object, Ctrl::attrFillShape).asBool();
  fillShapeOpacity = MPlug(object, Ctrl::attrFillShapeOpacity).asFloat();
  widthLine = MPlug(object, Ctrl::attrWidthLine).asFloat();
  bDrawline = MPlug(object, Ctrl::attrInDrawLine).asBool();
  bXRay = MPlug(object, Ctrl::attrXRay).asBool();
  sizeSolverMode = MPlug(object, Ctrl::attrSolverModeSize).asInt();

  matPv = MDataHandle(MPlug(object, Ctrl::attrInLineMatrix).asMDataHandle()).asMatrix();
  posDrawPvTo = MPoint(matPv[3][0], matPv[3][1], matPv[3][2]);
}


void CtrlUserData::getBbox(const MObject& object, MMatrix matrix) {
  /* Gets the bounding box from the shapesDefinition.h file

  Args:
    obj (MObject &): Object passed to query the attrbutes from
    matrix (MMatrix): Matrix used to transform the bounding box

  */
  indxShape = MPlug(object, Ctrl::attrIndxShape).asShort();
  switch(indxShape) {
    case 0: // Cube
      this->bbox = populateBoundingBox(bboxCube);
      break;
    case 1: // Square
      this->bbox = populateBoundingBox(bboxSquare);
      break;
    case 2: // Cylinder
      this->bbox = populateBoundingBox(bboxCylinder);
      break;
    case 3: // Cone
      this->bbox = populateBoundingBox(bboxCone);
      break;
    case 4: // Circle
      this->bbox = populateBoundingBox(bboxCircle);
      break;
    case 5: // Sphere
      this->bbox = populateBoundingBox(bboxSphere);
      break;
    case 6: // Dome
      this->bbox = populateBoundingBox(bboxCircle);
      break;
    case 7: // Diamond
      this->bbox = populateBoundingBox(bboxDiamond);
      break;
    case 8: // Pyramid
      this->bbox = populateBoundingBox(bboxPyramid);
      break;
    case 9: // Triangle
      this->bbox = populateBoundingBox(bboxTriangle);
      break;
    case 10: // Prism
      this->bbox = populateBoundingBox(bboxPrism);
      break;
    case 11: // Locator
      this->bbox = populateBoundingBox(bboxLocator);
      break;
    case 12: // Frame
      this->bbox = populateBoundingBox(bboxFrame);
      break;
    case 13: // Arrow
      this->bbox = populateBoundingBox(bboxArrow);
      break;
    case 14: // Arrow2Way
      this->bbox = populateBoundingBox(bboxArrow2Way);
      break;
    case 15: // Circle4Arrows
      this->bbox = populateBoundingBox(bboxCircle4Arrows);
      break;
    case 16: // Hip
      this->bbox = populateBoundingBox(bboxHip);
      break;
    case 17: // CircleHalfDouble
      this->bbox = populateBoundingBox(bboxCircleHalfDouble);
      break;
    case 18: // PinRound
      this->bbox = populateBoundingBox(bboxPinRound);
      break;
    case 19: // Clavicle
      this->bbox = populateBoundingBox(bboxClavicle);
      break;
    case 20: // Pointer2Way
      this->bbox = populateBoundingBox(bboxPointer2Way);
      break;
    case 21: // Pointer2WayArc
      this->bbox = populateBoundingBox(bboxPointer2WayArc);
      break;
    case 22: // Cross
      this->bbox = populateBoundingBox(bboxCross);
      break;
    case 23: // CrossShort
      this->bbox = populateBoundingBox(bboxCrossShort);
      break;
  }

  this->bbox.transformUsing(matrix);
  // this->bbox.expand(this->posDrawPvTo);
}


void CtrlUserData::getShape(const MObject& object, const MDagPath& dpObject, MMatrix matrix) {
  /* Get the points for each line and triangle used for drawing the shape.

  Do not reorder the triangle append order since it will flip normals.

  Args:
    obj (MObject &): Object passed to query the attrbutes from
    matrix (MMatrix): Matrix used to transform the line points

  */
  MStatus status;

  indxShape = MPlug(object, Ctrl::attrIndxShape).asShort();

  this->arrayVertecies.clear();
  this->arrayEdges.clear();
  this->arrayTriangles.clear();
  this->arrayLine.clear();  // for pole vector line only

  switch(indxShape) {
    case 0:  // Cube
      populateVertexBuffer(pointsCube, idxEdgesCube, idxTrianglesCube, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 1:  // Square
      populateVertexBuffer(pointsSquare, idxEdgesSquare, idxTrianglesSquare, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 2:  // Cylinder
      populateVertexBuffer(pointsCylinder, idxEdgesCylinder, idxTrianglesCylinder, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 3:  // Cone
      populateVertexBuffer(pointsCone, idxEdgesCone, idxTrianglesCone, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 4:  // Circle
      populateVertexBuffer(pointsCircle, idxEdgesCircle, idxTrianglesCircle, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 5:  // Sphere
      populateVertexBuffer(pointsSphere, idxEdgesSphere, arrayVertecies, arrayEdges, matrix);
      break;
    case 6:  // Dome
      populateVertexBuffer(pointsDome, idxEdgesDome, arrayVertecies, arrayEdges, matrix);
      break;
    case 7:  // Diamond
      populateVertexBuffer(pointsDiamond, idxEdgesDiamond, idxTrianglesDiamond, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 8:  // Pyramid
      populateVertexBuffer(pointsPyramid, idxEdgesPyramid, idxTrianglesPyramid, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 9:  // Triangle
      populateVertexBuffer(pointsTriangle, idxEdgesTriangle, idxTrianglesTriangle, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 10:  // Prism
      populateVertexBuffer(pointsPrism, idxEdgesPrism, idxTrianglesPrism, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 11:  // Locator
      populateVertexBuffer(pointsLocator, idxEdgesLocator, arrayVertecies, arrayEdges, matrix);
      break;
    case 12:  // Frame
      populateVertexBuffer(pointsFrame, idxEdgesFrame, arrayVertecies, arrayEdges, matrix);
      break;
    case 13:  // Arrow
      populateVertexBuffer(pointsArrow, idxEdgesArrow, idxTrianglesArrow, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 14:  // Arrow2Way
      populateVertexBuffer(pointsArrow2Way, idxEdgesArrow2Way, idxTrianglesArrow2Way, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 15:  // Circle4Arrows
      populateVertexBuffer(pointsCircle4Arrows, idxEdgesCircle4Arrows, idxTrianglesCircle4Arrows, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 16:  // Hip
      populateVertexBuffer(pointsHip, idxEdgesHip, arrayVertecies, arrayEdges, matrix);
      break;
    case 17:  // CircleHalfDouble
      populateVertexBuffer(pointsCircleHalfDouble, idxEdgesCircleHalfDouble, arrayVertecies, arrayEdges, matrix);
      break;
    case 18:  // PinRound
      populateVertexBuffer(pointsPinRound, idxEdgesPinRound, idxTrianglesPinRound, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 19:  // Clavicle
      populateVertexBuffer(pointsClavicle, idxEdgesClavicle, arrayVertecies, arrayEdges, matrix);
      break;
    case 20:  // Pointer2Way
      populateVertexBuffer(pointsPointer2Way, idxEdgesPointer2Way, idxTrianglesPointer2Way, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 21:  // Pointer2WayArc
      populateVertexBuffer(pointsPointer2WayArc, idxEdgesPointer2WayArc, idxTrianglesPointer2WayArc, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 22:  // Cross
      populateVertexBuffer(pointsCross, idxEdgesCross, idxTrianglesCross, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
    case 23:  // CrossShort
      populateVertexBuffer(pointsCrossShort, idxEdgesCrossShort, idxTrianglesCrossShort, arrayVertecies, arrayEdges, arrayTriangles, matrix);
      break;
  };

  // Draw line for pole vectors
  if (bDrawline) { 
    // MMatrix matDrawLineTo = MDataHandle(MPlug(object, Ctrl::attrInLineMatrix).asMDataHandle()).asMatrix();
    arrayLine.append(MPoint() * matrix);
    arrayLine.append(MPoint(posDrawPvTo[0], posDrawPvTo[1], posDrawPvTo[2]) * dpObject.inclusiveMatrixInverse());
  }
}


void CtrlUserData::getText(const MObject& object) {
  bDrawSolverMode = MPlug(object, Ctrl::attrDrawSolverMode).asBool();
  posSolverMode = MPoint(
    MPlug(object, Ctrl::attrSolverModePositionX).asFloat(),
    MPlug(object, Ctrl::attrSolverModePositionY).asFloat(),
    MPlug(object, Ctrl::attrSolverModePositionZ).asFloat()
  );
  fkIk = MPlug(object, Ctrl::attrInFkIk).asDouble();
  if (fkIk == 0.0) {
    strSolverMode = MString("Fk");
  } else if (fkIk > 0.0 && fkIk < 100.0) {
    MString strFkIk = LMText::doublePrecision(MPlug(object, Ctrl::attrInFkIk).asDouble(), 0).c_str();
    strSolverMode = MString("FkIk " + strFkIk);
  } else if (fkIk == 100.0) {
    strSolverMode = MString("Ik");
  }
}


void CtrlDrawOverride::OnModelEditorChanged(void *clientData) {
  // Mark the node as being dirty so that it can update on display appearance
  // switch among wireframe and shaded.
  CtrlDrawOverride *ovr = static_cast<CtrlDrawOverride*>(clientData);
  if (ovr && ovr->ptrCtrl) {MHWRender::MRenderer::setGeometryDrawDirty(ovr->ptrCtrl->thisMObject());}
}


MBoundingBox CtrlDrawOverride::boundingBox(const MDagPath& objPath, const MDagPath& cameraPath) const {
  /* Called by Maya whenever the bounding box of the drawable object is needed.

  This method should return the object space bounding box for the object to be drawn.
  Note that this method will not be called if the isBounded() method returns a value of
  false.
  Default implementation returns a huge bounding box which will never cull the object.

  Args:
    objPath (MDagPath&): The path to the object being drawn
    cameraPath (MDagPath&): The path to the camera that is being used to draw

  Returns:
    MBoundingBox: The object space bounding box of object drawn in the draw callback

  */
  CtrlUserData data;
  MObject node = objPath.node();

  data.getPlugs(node);
  data.getBbox(node, data.matLocal);

  return data.bbox;
}


MUserData* CtrlDrawOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* oldData) {
  /* Called by Maya whenever the object is dirty and needs to update for draw.

  Any data needed from the Maya dependency graph must be retrieved and cached in this
  stage. It is invalid to pull data from the Maya dependency graph in the draw callback
  method and Maya may become unstable if that is attempted.

  Implementors may allow Maya to handle the data caching by returning a pointer to the
  data from this method. The pointer must be to a class derived from MUserData. This
  same pointer will be passed to the draw callback. On subsequent draws, the pointer
  will also be passed back into this method so that the data may be modified and reused
  instead of reallocated. If a different pointer is returned Maya will delete the old
  data. If the cache should not be maintained between draws, set the delete after use
  flag on the user data and Maya will delete it in a certain stage depending on whether
  the draw callback exists. If exists, the user data will be deleted after both
  addUIDrawables() and the draw callback are called; otherwise, the user data will be
  deleted immediately after addUIDrawables() is called. In all cases, the lifetime and
  ownership of the user data is handled by Maya and the user cshould not try to delete
  the data themselves. Data caching occurs per-instance of the associated DAG object.
  The lifetime of the user data can be longer than the associated node, instance or
  draw override. Due to internal caching, the user data can be deleted after an
  arbitrary long time. One should therefore be careful to not access stale objects from
  the user data destructor. If it is not desirable to allow Maya to handle data caching,
  simply return NULL in this method and ignore the user data parameter in the draw
  callback method.

  Args:
    objPath (MDagPath&): The path to the object being drawn
    cameraPath (MDagPath&): The path to the camera that is being used to draw
    frameContext (MHWRender::MFrameContext&): Frame level context information
    oldData (MUserData*) Data cached by the previous draw of the instance

  Returns:
    MUserData*: Pointer to data to be passed to the draw callback method

  */
  MStatus status;

  CtrlUserData* data = dynamic_cast<CtrlUserData*>(oldData);
  MObject object = objPath.node();

  if (!data) {data=new CtrlUserData;}

  data->getPlugs(object);
  data->getShape(object, objPath, data->matLocal);
  data->getText(object);

  data->colWireframe = MHWRender::MGeometryUtilities::wireframeColor(objPath);
  data->colShape = MColor(data->colWireframe.r, data->colWireframe.g, data->colWireframe.b, data->fillShapeOpacity);

  // If XRay Joints Draw in XRay Mode
  if (frameContext.getDisplayStyle() & MHWRender::MFrameContext::kXrayJoint) {data->bXRayJoint = true;}
  else {data->bXRayJoint = false;}

  switch (MHWRender::MGeometryUtilities::displayStatus(objPath)) {
    case MHWRender::kLead:
    case MHWRender::kActive:
    case MHWRender::kHilite:
    case MHWRender::kActiveComponent:
      data->prioDepth = MHWRender::MRenderItem::sActiveWireDepthPriority;
      break;
    default:
      data->prioDepth = MHWRender::MRenderItem::sDormantFilledDepthPriority;
      break;
  }
  return data;
}


void CtrlDrawOverride::addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data) {
  /* Provides access to the MUIDrawManager, which can be used to queue up operations
  to draw simple UI shapes like lines, circles, text, etc.

  This method will only be called when this override is dirty and its hasUIDrawables()
  is overridden to return true. This method is called after prepareForDraw() and carries
  the same restrictions on the sorts of operations it can perform.

  Args:
    objPath (MDagPath&): The path to the object being drawn
    drawManager (MHWRender::MUIDrawManager&): The UI draw manager, it can be used to
      draw some simple geometry including text
    frameContext (MHWRender::MFrameContext&): Frame level context information
    data (MUserData*) Data cached by prepareForDraw()

  */
  CtrlUserData* pTransformData = (CtrlUserData*)data;
  if (!pTransformData) {return;}

  // Define selectability - make it unselectable if shape is none
  if (pTransformData->indxShape == 24) {
    drawManager.beginDrawable(MHWRender::MUIDrawManager::kNonSelectable);
  } else {
    drawManager.beginDrawable();
  }
  // drawManager.beginDrawable();

  drawManager.setDepthPriority(pTransformData->prioDepth);

  // If XRay Joints Draw in XRay Mode
  if (pTransformData->bXRayJoint) {
    if (pTransformData->bXRay) {drawManager.beginDrawInXray();}
  }

  // Draw edges
  drawManager.setColor(pTransformData->colWireframe);
  drawManager.setLineWidth(pTransformData->widthLine);
  drawManager.mesh(MHWRender::MUIDrawManager::kLines, pTransformData->arrayEdges);

  // Fk Ik State
  if (pTransformData->bDrawSolverMode) {
    drawManager.setFontSize(pTransformData->sizeSolverMode);
    drawManager.setFontWeight(MHWRender::MUIDrawManager::kWeightLight);
    drawManager.text(pTransformData->posSolverMode, pTransformData->strSolverMode, drawManager.kCenter);
  }

  // Draw fill shape
  if (pTransformData->bFillShape) {
    drawManager.setColor(pTransformData->colShape);
    drawManager.mesh(MHWRender::MUIDrawManager::kTriangles, pTransformData->arrayTriangles);
  }

  if (pTransformData->bDrawline) {
    drawManager.setColor(pTransformData->colGrey);
    drawManager.setLineStyle(MHWRender::MUIDrawManager::kShortDashed);
    drawManager.line(pTransformData->arrayLine[0], pTransformData->arrayLine[1]);
  }

  // End drawable
  if (pTransformData->bXRayJoint) {
    if (pTransformData->bXRay) {drawManager.endDrawInXray();}
  }

  drawManager.endDrawable();
}


