#include "Speedometer.hpp"




// Public Data
const MString Speedometer::typeName("speedometer");
const MTypeId Speedometer::typeId(0x0066671);
const MString Speedometer::drawDbClassification("drawdb/geometry/speedometer");
const MString Speedometer::drawRegistrationId("speedometerPlugin");

// Nodes' Input Attributes
Attribute Speedometer::attrInTransform;
MObject Speedometer::attrInTime;

MObject Speedometer::textOffsetAttr;
MObject Speedometer::shapeAttr;
MObject Speedometer::textAttr;
MObject Speedometer::textSizeAttr;
MObject Speedometer::precisionAttr;
MObject Speedometer::unitTypeAttr;
MObject Speedometer::lineWidthAttr;

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
  MFnMatrixAttribute mAttr;
  MFnNumericAttribute nAttr;
  MFnTypedAttribute tAttr;
  MFnEnumAttribute eAttr;
  MFnUnitAttribute uAttr;

  createAttribute(attrInTransform, "inTransform", DefaultValue<MMatrix>());

  attrInTime = uAttr.create("inTime", "itm", MFnUnitAttribute::kTime);
  uAttr.setKeyable(true);
  uAttr.setReadable(false);

  textOffsetAttr = nAttr.createPoint("textOffset", "tof");
  nAttr.setKeyable(false);
  nAttr.setChannelBox(true);
  nAttr.setReadable(false);

  textAttr = tAttr.create("text", "txt", MFnData::kString);
  nAttr.setKeyable(false);
  nAttr.setChannelBox(true);
  nAttr.setReadable(false);

  textSizeAttr = nAttr.create("textSize", "txts", MFnNumericData::kInt, 12);
  nAttr.setKeyable(false);
  nAttr.setChannelBox(true);
  nAttr.setReadable(false);
  nAttr.setMin(9);
  nAttr.setMax(24);

  precisionAttr = nAttr.create("precision", "prec", MFnNumericData::kInt, 2);
  nAttr.setMin(0);
  nAttr.setMax(6);
  nAttr.setKeyable(false);
  nAttr.setChannelBox(true);
  nAttr.setReadable(false);

  unitTypeAttr = eAttr.create("unitType", "utyp");
  eAttr.addField("km/h", 0);
  eAttr.addField("mi/h", 1);
  eAttr.addField("m/s", 2);
  eAttr.setKeyable(false);
  eAttr.setChannelBox(true);

  shapeAttr = eAttr.create("shape", "shp");
  eAttr.addField("Locator", 0);
  eAttr.addField("Square", 1);
  eAttr.addField("Cube", 2);
  eAttr.addField("Circle", 3);
  eAttr.addField("None", 4);
  eAttr.setKeyable(false);
  eAttr.setChannelBox(true);

  lineWidthAttr = nAttr.create("lineWidth", "lw", MFnNumericData::kDouble);
  nAttr.setMin(0.5);
  nAttr.setDefault(1.0);
  nAttr.setMax(5);
  nAttr.setStorable(true);
  nAttr.setKeyable(false);
  nAttr.setChannelBox(true);

  updateAttr = nAttr.create("update", "upt", MFnNumericData::kDouble, 0.0);
  nAttr.setWritable(false);

  outputAttr = tAttr.create("output", "out", MFnData::kString);
  tAttr.setWritable(false);

  // Add attributes
  addAttributes(
    attrInTransform, attrInTime, textAttr, precisionAttr, unitTypeAttr,
    textOffsetAttr, textSizeAttr, shapeAttr, lineWidthAttr,
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
  if ( plugBeingDirtied == attrInTransform
    || plugBeingDirtied == attrInTime
    || plugBeingDirtied == textAttr
    || plugBeingDirtied == precisionAttr
    || plugBeingDirtied == unitTypeAttr
  )	{
    affectedPlugs.append(MPlug(objSelf, outputAttr));
    affectedPlugs.append(MPlug(objSelf, updateAttr));
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

  data.getPlugs(objSelf);
  data.getBBox(objSelf, data.matrix);

  return data.bBox;
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
  // MDagPath::getAPathTo(objSelf, thisPath);
  MMatrix matrix = dpShape.inclusiveMatrix();
  posCurrent = MPoint(matrix[3][0], matrix[3][1], matrix[3][2]);
}


void Speedometer::setDisplayUnits(unsigned int unitTypeIndex) {
  /* Sets the display unit for speed.

  Args:
    unitTypeIndex (unsigned int): 0 is km/s, 1 is m/s

  */
  if (unitTypeIndex == 0) {  // km/h
    if (speed != 0) {
      speed = speed * 3600 / 100000;
    }
    dispUnit = "km/h";
  }	
  else if (unitTypeIndex == 1)	{
    if (speed != 0) {
      speed = speed * 3600 / 160000;
    }
    dispUnit = "mi/s";
  }
  else if (unitTypeIndex == 2)	{
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
  if (distance != 0 && timePassed != 0) 	{
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

  matTransform = dataBlock.inputValue(attrInTransform).asMatrix();
  // posCurrent = MPoint(matTransform[3][0], matTransform[3][1], matTransform[3][2]);

  // Additional attributes
  unitTypeIndex = dataBlock.inputValue(unitTypeAttr).asShort();
  text = dataBlock.inputValue(textAttr).asString().asChar();
  precision = dataBlock.inputValue(precisionAttr).asInt();

  return MS::kSuccess;
}


MStatus Speedometer::getSpeed() {

  getSelfPosition();

  double distance = posCached.distanceTo(posCurrent);
  double timePassed = abs((timeCurrent - timeCached).value());

  speed = calculateSpeed(distance, timePassed);

  setDisplayUnits(unitTypeIndex);

  strSpeed = prd(speed, precision);
  strSpeed = text + " " + strSpeed + " " + dispUnit;

  // MGlobal::displayWarning(std::to_string(distance).c_str());
  // MGlobal::displayWarning(std::to_string(timePassed).c_str());
  // MGlobal::displayWarning(std::to_string(speed).c_str());
  // MGlobal::displayWarning(strSpeed.c_str());

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
  if (!shouldCompute(plug, attrInTransform, attrInTime, textAttr, precisionAttr, unitTypeAttr)) {
    return MS::kUnknownParameter;
  }

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
  MPxLocatorNode::getCacheSetup(evalNode, disablingInfo, cacheSetupInfo, monitoredAttributes);
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
  objSelf = thisMObject();
  MDagPath::getAPathTo(objSelf, dpShape);
  MFnDagNode fnShape(dpShape);

  MObject objTransform = fnShape.parent(0);
  MDagPath dpTransform;
  MDagPath::getAPathTo(objTransform, dpTransform);
  MFnDagNode fnTransform(dpTransform);

  // MGlobal::displayWarning("Shape name " + fnShape.name());
  // MGlobal::displayWarning("Transform name " + fnTransform.name());

  fnShape.setName(Speedometer::typeName + "Shape");

  MPlug overrideEnabledPlug = fnShape.findPlug("overrideEnabled", false);
  overrideEnabledPlug.setBool(1);

  MPlug overrideRGBColorsPlug = fnShape.findPlug("overrideRGBColors", false);
  overrideRGBColorsPlug.setBool(1);

  MPlug overrideColorRPlug = fnShape.findPlug("overrideColorR", false);
  overrideColorRPlug.setFloat(1.0);
  MPlug overrideColorGPlug = fnShape.findPlug("overrideColorG", false);
  overrideColorGPlug.setFloat(1.0);
  MPlug overrideColorBPlug = fnShape.findPlug("overrideColorB", false);
  overrideColorBPlug.setFloat(0.0);

  // LMScene::connectSceneTime(objSelf, "inTime", modDg);

  // modDg.doIt();
}











void SpeedometerData::getPlugs(const MObject& obj)
{
  /* Get all the necessary data from the attributes of the locator.

  Args:
    obj (MObject&): Object passed to query the attrbutes from

  */
  float tx = MPlug(obj, Speedometer::localPositionX).asFloat();
  float ty = MPlug(obj, Speedometer::localPositionY).asFloat();
  float tz = MPlug(obj, Speedometer::localPositionZ).asFloat();

  float sx = MPlug(obj, Speedometer::localScaleX).asFloat();
  float sy = MPlug(obj, Speedometer::localScaleY).asFloat();
  float sz = MPlug(obj, Speedometer::localScaleZ).asFloat();

  MMatrix newMatrix;
  this->matrix = newMatrix;
  this->matrix[0][0] *= sx;
  this->matrix[0][1] *= sx;
  this->matrix[0][2] *= sx;

  this->matrix[1][0] *= sy;
  this->matrix[1][1] *= sy;
  this->matrix[1][2] *= sy;

  this->matrix[2][0] *= sz;
  this->matrix[2][1] *= sz;
  this->matrix[2][2] *= sz;

  this->matrix[3][0] = tx;
  this->matrix[3][1] = ty;
  this->matrix[3][2] = tz;

  MPlug textOffsetPlug(obj, Speedometer::textOffsetAttr);
  double textOffsetX = textOffsetPlug.child(0).asDouble();
  double textOffsetY = textOffsetPlug.child(1).asDouble();
  double textOffsetZ = textOffsetPlug.child(2).asDouble();
  this->_textOffset = MPoint(textOffsetX + tx, textOffsetY + ty, textOffsetZ + tz);

  this->_textSize = MPlug(obj, Speedometer::textSizeAttr).asInt();

  this->_lineWidth = MPlug(obj, Speedometer::lineWidthAttr).asFloat();

  this->__drawText = MPlug(obj, Speedometer::outputAttr).asString();
}


void SpeedometerData::getBBox(const MObject& obj, MMatrix matrix) {
  /* Gets the bounding box from the shapesDefinition.h file.

  Args:
    obj (MObject&): Object passed to query the attrbutes from.
    matrix (MMatrix): Matrix used to transform the bounding box.

  */
  unsigned int shapeIndex = MPlug(obj, Speedometer::shapeAttr).asInt();

  this->bBox.transformUsing(matrix);
}


void SpeedometerData::getShpae(const MObject& obj, MMatrix matrix) {
  /* Get the points for each line used for drawing the shape

  Args:
    obj (MObject &): Object passed to query the attrbutes from
    matrix (MMatrix): Matrix used to transform the line points

  */
  MStatus status;

  unsigned int shapeIndex = MPlug(obj, Speedometer::shapeAttr).asInt();

  this->fTransformedList.clear();
  this->fLineList.clear();

}

















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
  data.getBBox(node, data.matrix);

  return data.bBox;
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
  MObject oNode = objPath.node(&status);

  if (!data) {data = new SpeedometerData;}

  data->getPlugs(oNode);
  data->getShpae(oNode, data->matrix);

  data->_wfColor = MHWRender::MGeometryUtilities::wireframeColor(objPath);

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
  SpeedometerData* pSpeedometerData = (SpeedometerData*)data;
  if (!pSpeedometerData) {return;}

  drawManager.beginDrawable();

  drawManager.setDepthPriority(MHWRender::MRenderItem::sDormantFilledDepthPriority);

  // Draw the outline
  drawManager.setColor(pSpeedometerData->_wfColor);
  drawManager.setLineWidth(pSpeedometerData->_lineWidth);
  drawManager.mesh(MHWRender::MUIDrawManager::kLines,	pSpeedometerData->fLineList);

  // Draw text
  drawManager.setFontSize(pSpeedometerData->_textSize);
  drawManager.text(
    pSpeedometerData->_textOffset,
    pSpeedometerData->__drawText,
    MUIDrawManager::kCenter,
    NULL,
    NULL,
    false
  );

  drawManager.endDrawable();
}
