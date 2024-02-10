#include "Speedometer.hpp"




// Class attributes
const MString Speedometer::typeName("speedometer");
const MTypeId Speedometer::typeId(0x0066671);
const MString Speedometer::typeDrawDb("drawdb/geometry/animation/speedometer");
const MString Speedometer::typeDrawId("speedometerPlugin");

// Node's Input Attributes
MObject Speedometer::localPosition, Speedometer::localPositionX, Speedometer::localPositionY, Speedometer::localPositionZ;
MObject Speedometer::localRotate, Speedometer::localRotateX, Speedometer::localRotateY, Speedometer::localRotateZ;
MObject Speedometer::localScale, Speedometer::localScaleX, Speedometer::localScaleY, Speedometer::localScaleZ;
MObject Speedometer::textPosition, Speedometer::textPositionX, Speedometer::textPositionY, Speedometer::textPositionZ;

MObject Speedometer::attrIndxShape;
MObject Speedometer::attrFillShape;
MObject Speedometer::attrFillShapeOpacity;
MObject Speedometer::attrWidthLine;
MObject Speedometer::attrInDrawLine;

Attribute Speedometer::attrInLineMatrix;
Attribute Speedometer::attrOutLineMatrix;

MObject Speedometer::attrText;
MObject Speedometer::attrTextSize;
MObject Speedometer::attrXRay;

// Attribute Speedometer::attrInTransform;
MObject Speedometer::attrInTime;

MObject Speedometer::attrPrecision;
MObject Speedometer::attrUnitType;

// Nodes's Output Attributes
MObject Speedometer::updateAttr;
MObject Speedometer::outputAttr;




MStatus Speedometer::initialize() {
  /* Node Initializer.

  This method initializes the node, and should be overridden in user-defined nodes.

  Returns:
    status code (MStatus): kSuccess if the operation was successful, kFailure if an error occured
      during the operation.

  */
  MFnUnitAttribute    fnUnit;
  MFnNumericAttribute fnNum;
  MFnEnumAttribute    fnEnum;
  MFnMatrixAttribute  fnMat;
  MFnTypedAttribute   fnType;

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


  // createAttribute(attrInTransform, "inTransform", DefaultValue<MMatrix>());

  { // Text
    attrText = fnType.create("text", "txt", MFnData::kString);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);
    fnNum.setReadable(false);

    textPositionX = fnNum.create("textPositionX", "tpx", MFnNumericData::kDouble, 0.0);
    textPositionY = fnNum.create("textPositionY", "tpy", MFnNumericData::kDouble, 0.0);
    textPositionZ = fnNum.create("textPositionZ", "tpz", MFnNumericData::kDouble, 0.0);
    textPosition = fnNum.create("textPosition", "tp", textPositionX, textPositionY, textPositionZ);
    fnNum.setStorable(true);
    fnNum.setStorable(true);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);

    attrPrecision = fnNum.create("precision", "prec", MFnNumericData::kInt, 2);
    fnNum.setMin(0);
    fnNum.setMax(6);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);
    fnNum.setReadable(false);

    attrTextSize = fnNum.create("textSize", "txts", MFnNumericData::kInt, 12);
    fnNum.setKeyable(false);
    fnNum.setChannelBox(true);
    fnNum.setReadable(false);
    fnNum.setMin(9);
    fnNum.setMax(24);

    attrUnitType = fnEnum.create("unitType", "utyp");
    fnEnum.addField("km/h", 0);
    fnEnum.addField("mi/h", 1);
    fnEnum.addField("m/s", 2);
    fnEnum.setKeyable(false);
    fnEnum.setChannelBox(true);
  }


  attrInTime = fnUnit.create("inTime", "itm", MFnUnitAttribute::kTime);
  fnUnit.setKeyable(true);
  fnUnit.setReadable(false);

  updateAttr = fnNum.create("update", "upt", MFnNumericData::kDouble, 0.0);
  fnNum.setWritable(false);

  outputAttr = fnType.create("output", "out", MFnData::kString);
  fnType.setWritable(false);

  // Add attributes
  addAttributes(
    localPosition, localRotate, localScale,
    attrText, textPosition, attrPrecision, attrTextSize, attrUnitType,
    attrIndxShape, attrFillShape, attrFillShapeOpacity, attrWidthLine, attrXRay,
    attrInDrawLine, attrInLineMatrix,
    attrInTime,
    updateAttr, outputAttr
  );

  return MS::kSuccess;
}


MStatus Speedometer::setDependentsDirty(const MPlug& plugBeingDirtied, MPlugArray& affectedPlugs) {
  /* Sets the relation between attributes and marks the specified plugs dirty.

  Args:
    plugBeingDirtied (&MPlug): Plug which is being set dirty by Maya.
    affectedPlugs (&MPlugArray): The programmer should add any plugs which they want to set dirty
      to this list.

  */
  if ( 
    // plugBeingDirtied == attrInTransform
    // || plugBeingDirtied == attrInTime
       plugBeingDirtied == attrInTime
    || plugBeingDirtied == attrText
    || plugBeingDirtied == attrPrecision
    || plugBeingDirtied == attrUnitType
  )	{
    affectedPlugs.append(MPlug(selfObject, outputAttr));
    affectedPlugs.append(MPlug(selfObject, updateAttr));
  }

  return MS::kSuccess;
}


MBoundingBox Speedometer::boundingBox() const {
  /* This method should be overridden to return a bounding box for the locator.

  If this method is overridden, then MPxLocatorNode::isBounded should also be overridden to return true.

  Returns:
    MBoundingBox: The bounding box of the locator.

  */
  SpeedometerData data;
  data.getPlugs(selfObject);
  data.getBbox(selfObject, data.matLocal);

  return data.bbox;
}


double Speedometer::getNumericFPS() {
  /* Returns the ui frame rate as a numeric value 

  Returns:
    double: Ui framerate as double ex. kFilm - 24

  */
  unsigned int uiFpsIndex = MTime::uiUnit();

  if (uiFpsIndex == 5)
  {  // kGames
    return 15;
  }
  else if (uiFpsIndex == 6)
  {  // kFilm
    return 24;
  }
  else if (uiFpsIndex == 7)
  {  // kPALFrame
    return 25;
  }
  else if (uiFpsIndex == 8)
  {  // kNTSCFrame
    return 30;
  } else if (uiFpsIndex == 9)
  {  // kShowScan
    return 48;
  } else if (uiFpsIndex == 10)
  {  // kPALField
    return 50;
  } else if (uiFpsIndex == 11)
  {  // kNTSCField
    return 60;
  }	else if (uiFpsIndex == 20)
  {  // k16FPS
    return 16;
  } else if (uiFpsIndex == 21)
  {  // k20FPS
    return 20;
  } else if (uiFpsIndex == 22)
  {  // k40FPS
    return 40;
  } else if (uiFpsIndex == 43)
  {  // k23_976FPS
    return 23.976;
  } else if (uiFpsIndex == 44)
  {  // k29_97FPS
    return 29.97;
  } else if (uiFpsIndex == 45)
  {  // k29_97DF
    return 29.97;
  } else if (uiFpsIndex == 46)
  {  // k47_952FPS
    return 47.952;
  } else {
    //  Return default 24 just so that the compute method doesn't crash maya (division by 0).
    return 24;
  }
}


void Speedometer::getSelfPosition() {
  /* Get the world position of this node. */
  // MDagPath thisPath;
  // MDagPath::getAPathTo(selfObject, thisPath);
  MMatrix matrix = selfPath.inclusiveMatrix();
  posCurrent = MPoint(matrix[3][0], matrix[3][1], matrix[3][2]);
}


void Speedometer::setDisplayUnits(unsigned int indxUnitType) {
  /* Sets the display unit for speed.

  Args:
    indxUnitType (unsigned int): 0 is km/s, 1 is m/s

  */
  if (indxUnitType == 0) {  // km/h
    if (speed != 0) {
      speed = speed * 3600 / 100000;
    }
    dispUnit = "km/h";
  }	
  else if (indxUnitType == 1)	{
    if (speed != 0) {
      speed = speed * 3600 / 160000;
    }
    dispUnit = "mi/s";
  }
  else if (indxUnitType == 2)	{
    if (speed != 0) {
      speed = speed / 100;
    }
    dispUnit = "m/s";
  }
}


std::string Speedometer::prd(const double number, unsigned int decDigits) {
  /* Set the decimal precision od doubles.
  
  Args:
    number (double): Given number
  
  Returns:
    decDigits (unsigned int): Decimal precision

  */
  std::stringstream ss;
  ss.precision(decDigits);  // set # places after decimal
  ss << std::fixed;
  ss << number;

  return ss.str();
}


double Speedometer::calculateSpeed(double distance, double timePassed) {
  /* Calculates the speed.

  Args:
    distance (double): Distance traveled
    timePassed (double): Time that has passed

  Returns:
    double: The speed in generic units

  */ 
  if (distance != 0 && timePassed != 0) {
    return (distance / timePassed) * getNumericFPS();
  }	else {
    return distance;
  }
}


MStatus Speedometer::parseDataBlock(MDataBlock& dataBlock) {
  /* Parse the data block and get all inputs.	*/
  MStatus status;

  // Ask for time value to force refresh on the node
  timeCurrent = dataBlock.inputValue(attrInTime, &status).asTime();

  // matTransform = dataBlock.inputValue(attrInTransform).asMatrix();
  // posCurrent = MPoint(matTransform[3][0], matTransform[3][1], matTransform[3][2]);

  // Additional attributes
  indxUnitType = dataBlock.inputValue(attrUnitType).asShort();
  text = dataBlock.inputValue(attrText).asString().asChar();
  precision = dataBlock.inputValue(attrPrecision).asInt();

  return MS::kSuccess;
}


MStatus Speedometer::getSpeed() {

  // getSelfPosition();
  posCurrent = MPoint(getTranslation(MSpace::kWorld));

  double distance = posCached.distanceTo(posCurrent);
  double timePassed = abs((timeCurrent - timeCached).value());

  speed = calculateSpeed(distance, timePassed);

  setDisplayUnits(indxUnitType);

  strSpeed = prd(speed, precision);
  strSpeed = text + " " + strSpeed + " " + dispUnit;

  return MStatus::kSuccess;
}



MStatus Speedometer::updateOutput(const MPlug& plug, MDataBlock& dataBlock) {	
  /* Sets the outputs and data block clean.

  Args:
    plug (MPlug&): Plug representing the attribute that needs to be recomputed.
    dataBlock (MDataBlock&): Data block containing storage for the node's attributes.

  Returns:
    status code (MStatus): kSuccess if the operation was successful, kFailure if an	error occured
      during the operation.

  */
  MStatus status;

  MDataHandle dhOutput = dataBlock.outputValue(outputAttr);
  dhOutput.setString(strSpeed.c_str());
  dhOutput.setClean();

  MDataHandle dhUpdate = dataBlock.outputValue(updateAttr);
  dhUpdate.setDouble(0.0);
  dhUpdate.setClean();

  dataBlock.setClean(plug);

  return MS::kSuccess;
}


MStatus Speedometer::compute(const MPlug& plug, MDataBlock& dataBlock) {
  /* This method should be overridden in user defined nodes.

  Recompute the given output based on the nodes inputs. The plug represents the data
  value that needs to be recomputed, and the data block holds the storage for all of
  the node's attributes.

  The MDataBlock will provide smart handles for reading and writing this node's
  attribute values. Only these values should be used when performing computations.

  When evaluating the dependency graph, Maya will first call the compute method for
  this node. If the plug that is provided to the compute indicates that the attribute
  was defined by the Maya parent node, the compute method should return
  MS::kUnknownParameter. When this occurs, Maya will call the internal Maya node from
  which the user-defined node is derived to compute the plug's value.

  This means that a user defined node does not need to be concerned with computing
  inherited output attributes. However, if desired, these can be safely recomputed by
  this method to change the behaviour of the node.

  Args:
    plug (MPlug&): Plug representing the attribute that needs to be recomputed.
    dataBlock (MDataBlock&): Data block containing storage for the node's attributes.

  Returns:
    status code (MStatus): kSuccess if the operation was successful, kFailure if an	error occured 
      during the operation.

  */
  MStatus status;

  // if (plug != outputAttr || plug != updateAttr) {return MS::kUnknownParameter;}
  // if (!shouldCompute(plug, attrInTime, attrInText, attrPrecision, attrUnitType)) {
  //   return MS::kUnknownParameter;
  // }

  CHECK_MSTATUS_AND_RETURN_IT(parseDataBlock(dataBlock));

  CHECK_MSTATUS_AND_RETURN_IT(getSpeed());

  CHECK_MSTATUS_AND_RETURN_IT(updateOutput(plug, dataBlock));

  // Cache time change
  posCached = posCurrent;
  timeCached = timeCurrent;

  return MS::kSuccess;
}


void Speedometer::getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const {
  /* Disables Cached Playback support by default.

  Built-in locators all enable Cached Playback by default, but plug-ins have to	explicitly enable
  it by overriding this method.
  This method should be overridden to enable Cached Playback by default for custom locators.

  Args:
    evalNode (MEvaluationNode&): This node's evaluation node, contains animated plug information.
    disablingInfo (MNodeCacheDisablingInfo&): Information about why the node disables caching to be
      reported to the user.
    cacheSetup (MNodeCacheSetupInfo&): Preferences and requirements this node has for caching.
    monitoredAttribures (MObjectArray&): Attributes impacting the behavior of this method that will
      be monitored for change.

  */
  MPxTransform::getCacheSetup(evalNode, disablingInfo, cacheSetupInfo, monitoredAttributes);
  assert(!disablingInfo.getCacheDisabled());
  cacheSetupInfo.setPreference(MNodeCacheSetupInfo::kWantToCacheByDefault, true);
}


void Speedometer::postConstructor() {
  /* Post constructor.

  Internally maya creates two objects when a user defined node is created, the internal
  MObject and the user derived object. The association between the these two objects is
  not made until after the MPxNode constructor is called. This implies that no MPxNode
  member function can be called from the MPxNode constructor. The postConstructor will
  get called immediately after the constructor when it is safe to call any MPxNode
  member function.

  */
  selfObject = thisMObject();
  MDagPath::getAPathTo(selfObject, selfPath);

  MFnDependencyNode fnThis(selfObject);

  // fnShape.setName(Speedometer::typeName);

  MPlug plugOverrideEnabled = fnThis.findPlug("overrideEnabled", false);
  plugOverrideEnabled.setValue(1);
  MPlug plugOverrideRGBColors = fnThis.findPlug("overrideRGBColors", false);
  plugOverrideRGBColors.setValue(1);

  MPlug plugOverrideColorR = fnThis.findPlug("overrideColorR", false);
  plugOverrideColorR.setValue(1.0);
  MPlug plugOverrideColorG = fnThis.findPlug("overrideColorG", false);
  plugOverrideColorG.setValue(1.0);
  MPlug plugOverrideColorB = fnThis.findPlug("overrideColorB", false);
  plugOverrideColorB.setValue(0.0);

  LMScene::connectSceneTime(selfObject, "inTime", modDg);

  modDg.doIt();
}











void SpeedometerData::getPlugs(const MObject& object) {
  /* Get all the necessary data from the attributes of the locator.

  Args:
    object (MObject&): Object passed to query the attrbutes from

  */
  float tx = MPlug(object, Speedometer::localPositionX).asFloat();
  float ty = MPlug(object, Speedometer::localPositionY).asFloat();
  float tz = MPlug(object, Speedometer::localPositionZ).asFloat();

  float rx = MPlug(object, Speedometer::localRotateX).asFloat();
  float ry = MPlug(object, Speedometer::localRotateY).asFloat();
  float rz = MPlug(object, Speedometer::localRotateZ).asFloat();

  float sx = MPlug(object, Speedometer::localScaleX).asFloat();
  float sy = MPlug(object, Speedometer::localScaleY).asFloat();
  float sz = MPlug(object, Speedometer::localScaleZ).asFloat();

  this->matLocal = MEulerRotation(rx, ry, rz).asMatrix();
  this->matLocal[3][0] = tx; this->matLocal[3][1] = ty;	this->matLocal[3][2] = tz;
  this->matLocal[0][0] *= sx; this->matLocal[0][1] *= sx;	this->matLocal[0][2] *= sx;
  this->matLocal[1][0] *= sy; this->matLocal[1][1] *= sy;	this->matLocal[1][2] *= sy;
  this->matLocal[2][0] *= sz; this->matLocal[2][1] *= sz;	this->matLocal[2][2] *= sz;

  float textPositionX = MPlug(object, Speedometer::textPositionX).asFloat();
  float textPositionY = MPlug(object, Speedometer::textPositionY).asFloat();
  float textPositionZ = MPlug(object, Speedometer::textPositionZ).asFloat();
  posText = MPoint(textPositionX+tx, textPositionY+ty, textPositionZ+tz);

  indxShape =         MPlug(object, Speedometer::attrIndxShape).asShort();
  bFillShape =        MPlug(object, Speedometer::attrFillShape).asBool();
  fillShapeOpacity =  MPlug(object, Speedometer::attrFillShapeOpacity).asFloat();
  widthLine =         MPlug(object, Speedometer::attrWidthLine).asFloat();
  bDrawline =         MPlug(object, Speedometer::attrInDrawLine).asBool();
  bXRay =             MPlug(object, Speedometer::attrXRay).asBool();
  textToDraw =        MPlug(object, Speedometer::outputAttr).asString();
  sizeText =          MPlug(object, Speedometer::attrTextSize).asInt();

  matPv = MDataHandle(MPlug(object, Speedometer::attrInLineMatrix).asMDataHandle()).asMatrix();
  posDrawPvTo = MPoint(matPv[3][0], matPv[3][1], matPv[3][2]);
}



void SpeedometerData::getBbox(const MObject& object, MMatrix matrix) {
  /* Gets the bounding box from the shapesDefinition.h file

  Args:
    obj (MObject &): Object passed to query the attrbutes from
    matrix (MMatrix): Matrix used to transform the bounding box

  */
  indxShape = MPlug(object, Speedometer::attrIndxShape).asShort();
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
}


void SpeedometerData::getShape(const MObject& object,  const MDagPath& dpObject, MMatrix matrix) {
  /* Get the points for each line used for drawing the shape

  Args:
    obj (MObject &): Object passed to query the attrbutes from
    matrix (MMatrix): Matrix used to transform the line points

  */
  MStatus status;
  indxShape = MPlug(object, Speedometer::attrIndxShape).asShort();

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


// void SpeedometerData::getText(const MObject& object) {
//   bDrawSolverMode = MPlug(object, Ctrl::attrDrawSolverMode).asBool();
//   posSolverMode = MPoint(
//     MPlug(object, Ctrl::attrSolverModePositionX).asFloat(),
//     MPlug(object, Ctrl::attrSolverModePositionY).asFloat(),
//     MPlug(object, Ctrl::attrSolverModePositionZ).asFloat()
//   );
//   fkIk = MPlug(object, Ctrl::attrInFkIk).asDouble();
//   if (fkIk == 0.0) {
//     strSolverMode = MString("Fk");
//   } else if (fkIk > 0.0 && fkIk < 100.0) {
//     MString strFkIk = LMText::doublePrecision(MPlug(object, Ctrl::attrInFkIk).asDouble(), 0).c_str();
//     strSolverMode = MString("FkIk " + strFkIk);
//   } else if (fkIk == 100.0) {
//     strSolverMode = MString("Ik");
//   }
// }
















MBoundingBox SpeedometerDrawOverride::boundingBox(const MDagPath& objPath, const MDagPath& cameraPath) const {
  /* Called by Maya whenever the bounding box of the drawable object is needed.

  This method should return the object space bounding box for the object to be drawn.
  Note that this method will not be called if the isBounded() method returns a value of
  false.
  Default implementation returns a huge bounding box which will never cull the object.

  Args:
    objPath (MDagPath &): The path to the object being drawn
    cameraPath (MDagPath &): The path to the camera that is being used to draw

  Returns:
    MBoundingBox: The object space bounding box of object drawn in the draw callback

  */
  SpeedometerData data;
  MObject node = objPath.node();

  data.getPlugs(node);
  data.getBbox(node, data.matLocal);

  return data.bbox;
}


MUserData* SpeedometerDrawOverride::prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* oldData)
{
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

  Returns
    MUserData*: Pointer to data to be passed to the draw callback method

  */
  MStatus status;

  SpeedometerData* data = dynamic_cast<SpeedometerData*>(oldData);
  MObject object = objPath.node(&status);

  if (!data) {data = new SpeedometerData;}

  data->getPlugs(object);
  data->getShape(object, objPath, data->matLocal);
  // data->getText(object);

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


void SpeedometerDrawOverride::addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data)
{
  /* Provides access to the MUIDrawManager, which can be used to queue up operations to draw simple
  UI shapes like lines, circles, text, etc.

  This method will only be called when this override is dirty and its hasUIDrawables()
  is overridden to return true. This method is called after prepareForDraw() and carries
  the same restrictions on the sorts of operations it can perform.

  Args:
    objPath (MDagPath&): The path to the object being drawn.
    drawManager (MHWRender::MUIDrawManager&): The UI draw manager, it can be used to draw some simple
      geometry including text.
    frameContext (MHWRender::MFrameContext&): Frame level context information.
    data (MUserData*): Data cached by prepareForDraw().

  */
  SpeedometerData* pTransformData = (SpeedometerData*)data;
  if (!pTransformData) {return;}

  drawManager.beginDrawable();

  drawManager.setDepthPriority(MHWRender::MRenderItem::sDormantFilledDepthPriority);

  // If XRay Joints Draw in XRay Mode
  if (pTransformData->bXRayJoint) {
    if (pTransformData->bXRay) {drawManager.beginDrawInXray();}
  }

  // Draw edges
  drawManager.setColor(pTransformData->colWireframe);
  drawManager.setLineWidth(pTransformData->widthLine);
  drawManager.mesh(MHWRender::MUIDrawManager::kLines, pTransformData->arrayEdges);

  // Draw text
  drawManager.setFontSize(pTransformData->sizeText);
  drawManager.setFontWeight(MHWRender::MUIDrawManager::kWeightLight);
  drawManager.text(pTransformData->posText, pTransformData->textToDraw, drawManager.kCenter);


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
