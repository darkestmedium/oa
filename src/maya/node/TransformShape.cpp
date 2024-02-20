#include "TransformShape.hpp"




// Class attributes
const MString TransformShape::typeName 	  = "transformShape";
const MTypeId TransformShape::typeId 	    = 0x66600000;
const MString TransformShape::typeDrawDb	= "drawdb/geometry/transformShape";
const MString TransformShape::typeDrawId	= "transformShapePlugin";

// Node's input attributes
MObject TransformShape::localPosition;
MObject   TransformShape::localPositionX;
MObject   TransformShape::localPositionY;
MObject   TransformShape::localPositionZ;
MObject TransformShape::localRotate;
MObject   TransformShape::localRotateX;
MObject   TransformShape::localRotateY;
MObject   TransformShape::localRotateZ;
MObject TransformShape::localScale;
MObject   TransformShape::localScaleX;
MObject   TransformShape::localScaleY;
MObject   TransformShape::localScaleZ;

MObject TransformShape::shapeIndx;
MObject TransformShape::fillShape;
MObject TransformShape::fillShapeOpacity;
MObject TransformShape::lineWidth;

MObject TransformShape::text;
MObject TransformShape::textPosition;
MObject   TransformShape::textPositionX;
MObject   TransformShape::textPositionY;
MObject   TransformShape::textPositionZ;
MObject TransformShape::textSize;

MObject TransformShape::drawLine;
MObject TransformShape::lineMatrix;
MObject TransformShape::xRay;




MStatus TransformShape::initializeLocalTransforms() {
  MFnNumericAttribute fnNum;
  MFnUnitAttribute    fnUnit;

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

  return MS::kSuccess;
}


MStatus TransformShape::initializeShape() {
  MFnNumericAttribute fnNum;
  // MFnUnitAttribute    fnUnit;
  MFnEnumAttribute    fnEnum;
  MFnMatrixAttribute  fnMat;
  // MFnMatrixAttribute  fnMat;
  // MFnTypedAttribute   fnType;
  // MFnMessageAttribute fnMess;

  shapeIndx = fnEnum.create("shape", "shp");
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

  fillShape = fnNum.create("fillShape", "fp", MFnNumericData::kBoolean, false);
  fnNum.setStorable(true);
  fnNum.setKeyable(false);
  fnNum.setChannelBox(true);

  fillShapeOpacity = fnNum.create("fillShapeOpacity", "fso", MFnNumericData::kFloat, 0.1);
  fnNum.setMin(0.01);
  fnNum.setMax(1);
  fnNum.setStorable(true);
  fnNum.setKeyable(false);
  fnNum.setChannelBox(true);

  lineWidth = fnNum.create("lineWidth", "lw", MFnNumericData::kDouble, 2.0);
  fnNum.setMin(0.5);
  fnNum.setMax(5);
  fnNum.setStorable(true);
  fnNum.setKeyable(false);
  fnNum.setChannelBox(true);

  drawLine = fnNum.create("drawLine", "dl", MFnNumericData::kBoolean, false);
  fnNum.setStorable(true);
  fnNum.setKeyable(false);
  fnNum.setChannelBox(true);

  lineMatrix = fnMat.create("lineMatrix", "lmat");
  fnMat.setStorable(true);

  xRay = fnNum.create("xRay", "xr", MFnNumericData::kBoolean, false);
  fnNum.setStorable(true);
  fnNum.setKeyable(false);
  fnNum.setChannelBox(true);

  return MS::kSuccess;
}


MStatus TransformShape::initialize() {
  /* Node Initializer.

  This method initializes the node, and should be overridden in user-defined nodes.
  
  Returns:
    status code (MStatus): kSuccess if the operation was successful, kFailure if an	error occured
      during the operation.ransformShapeData : public MUserData {
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

  */
  MFnNumericAttribute fnNum;
  MFnUnitAttribute    fnUnit;
  MFnEnumAttribute    fnEnum;
  MFnMatrixAttribute  fnMat;
  MFnTypedAttribute   fnType;
  MFnMessageAttribute fnMess;

  initializeLocalTransforms();

  initializeShape();

  // Add attributes
  addAttributes(
    localPosition, localRotate, localScale,
    shapeIndx, fillShape, fillShapeOpacity, lineWidth, drawLine, lineMatrix, xRay
  );

  return MS::kSuccess;
}



void TransformShape::getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const {
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