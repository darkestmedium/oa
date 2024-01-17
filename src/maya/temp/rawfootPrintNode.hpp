//-
// ==========================================================================
// Copyright 2020 Autodesk, Inc.  All rights reserved.
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
// ==========================================================================
//+
#if _MSC_VER >= 1700
#pragma warning( disable: 4005 )
#endif

#pragma once

#include <maya/MPxLocatorNode.h>
#include <maya/MString.h>
#include <maya/MDagPath.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MColor.h>
#include <maya/M3dView.h>
#include <maya/MFnPlugin.h>
#include <maya/MDistance.h>
#include <maya/MMatrix.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFileObject.h>
#include <maya/MHWGeometry.h>
#include <maya/MEventMessage.h>
#include <maya/MEvaluationNode.h>
#include <maya/MPointArray.h>

// Viewport 2.0 includes
#include <maya/MDrawRegistry.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MSelectionContext.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MStateManager.h>
#include <maya/MShaderManager.h>
#include <maya/MRenderTargetManager.h>
#include <maya/MAnimControl.h>
#include <maya/MFnCamera.h>

#include <assert.h>
#include <stdlib.h>

// Open APi
#include "api/Utils.hpp"


// GL stuff
#if defined (__APPLE__)
    // #define GL_SILENCE_DEPRECATION
    // #include </Users/luky/My Drive/Development/3d/OpenGL/include/GLFW/glfw3.h>
    // #include </Users/luky/My Drive/Development/3d/OpenGL/include/GLFW/glfw3native.h>
    // #include "../maya/api/gl3.h"
    // #include </Users/luky/My Drive/Development/3d/OpenGL/include/macos/gl3.h>
  // #define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
  #include <OpenGL/gl3.h> // For gl*VertexArray(s) and APIENTRY

#elif defined (__linux__)
  #include <GL/glx.h> // For glXGetProcAddressARB
#endif

// #define GL_SILENCE_DEPRECATION


// Foot Data
//
static float sole[][3] = { {  0.00f, 0.0f, -0.70f },
               {  0.04f, 0.0f, -0.69f },
               {  0.09f, 0.0f, -0.65f },
               {  0.13f, 0.0f, -0.61f },
               {  0.16f, 0.0f, -0.54f },
               {  0.17f, 0.0f, -0.46f },
               {  0.17f, 0.0f, -0.35f },
               {  0.16f, 0.0f, -0.25f },
               {  0.15f, 0.0f, -0.14f },
               {  0.13f, 0.0f,  0.00f },
               {  0.00f, 0.0f,  0.00f },
               { -0.13f, 0.0f,  0.00f },
               { -0.15f, 0.0f, -0.14f },
               { -0.16f, 0.0f, -0.25f },
               { -0.17f, 0.0f, -0.35f },
               { -0.17f, 0.0f, -0.46f },
               { -0.16f, 0.0f, -0.54f },
               { -0.13f, 0.0f, -0.61f },
               { -0.09f, 0.0f, -0.65f },
               { -0.04f, 0.0f, -0.69f },
               { -0.00f, 0.0f, -0.70f } };

static float heel[][3] = { {  0.00f, 0.0f,  0.06f },
               {  0.13f, 0.0f,  0.06f },
               {  0.14f, 0.0f,  0.15f },
               {  0.14f, 0.0f,  0.21f },
               {  0.13f, 0.0f,  0.25f },
               {  0.11f, 0.0f,  0.28f },
               {  0.09f, 0.0f,  0.29f },
               {  0.04f, 0.0f,  0.30f },
               {  0.00f, 0.0f,  0.30f },
               { -0.04f, 0.0f,  0.30f },
               { -0.09f, 0.0f,  0.29f },
               { -0.11f, 0.0f,  0.28f },
               { -0.13f, 0.0f,  0.25f },
               { -0.14f, 0.0f,  0.21f },
               { -0.14f, 0.0f,  0.15f },
               { -0.13f, 0.0f,  0.06f },
               { -0.00f, 0.0f,  0.06f } };

static int soleCount = 21;
static int heelCount = 17;

// Index buffer data for drawing in Dx and Core Profile modes:
static unsigned short soleWireIndices[] = {
  0, 1,
  1, 2,
  2, 3,
  3, 4,
  4, 5,
  5, 6,
  6, 7,
  7, 8,
  8, 9,
  9, 10,
  10, 11,
  11, 12,
  12, 13,
  13, 14,
  14, 15,
  15, 16,
  16, 17,
  17, 18,
  18, 19,
  19, 20
};

static unsigned short heelWireIndices[] =
    {
      0, 1,
      1, 2,
      2, 3,
      3, 4,
      4, 5,
      5, 6,
      6, 7,
      7, 8,
      8, 9,
      9, 10,
      10, 11,
      11, 12,
      12, 13,
      13, 14,
      14, 15,
      15, 16
    };
static unsigned short soleShadedIndices[] =
    {
      0, 1, 2,
      0, 2, 3,
      0, 3, 4,
      0, 4, 5,
      0, 5, 6,
      0, 6, 7,
      0, 7, 8,
      0, 8, 9,
      0, 9, 10,
      0, 10, 11,
      0, 11, 12,
      0, 12, 13,
      0, 13, 14,
      0, 14, 15,
      0, 15, 16,
      0, 16, 17,
      0, 17, 18,
      0, 18, 19,
      0, 19, 20
    };
static unsigned short heelShadedIndices[] =
    {
      0, 1, 2,
      0, 2, 3,
      0, 3, 4,
      0, 4, 5,
      0, 5, 6,
      0, 6, 7,
      0, 7, 8,
      0, 8, 9,
      0, 9, 10,
      0, 10, 11,
      0, 11, 12,
      0, 12, 13,
      0, 13, 14,
      0, 14, 15,
      0, 15, 16
    };

// 8 Vertices for drawing the bounding box in DX and Core Profile mode
// lower and upper values of the sole and heel vertices +/- 0.1
static float bbData[][3] = {
    { -0.18f, 0.f, -0.71f},
    {  0.18f, 0.f, -0.71f},
    {  0.18f, 0.f,  0.31f},
    { -0.18f, 0.f,  0.31f},
    { -0.18f, 0.f, -0.71f},
    {  0.18f, 0.f, -0.71f},
    {  0.18f, 0.f,  0.31f},
    { -0.18f, 0.f,  0.31f}};

// Index buffer for Wireframe drawing in Dx and Core Profile
static unsigned short bbWireIndices[] = {
  0,1,
  1,2,
  2,3,
  3,0,
  4,5,
  5,6,
  6,7,
  7,4,
  0,4,
  1,5,
  2,6,
  3,7
};




//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Node implementation with standard viewport draw
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------




class rawfootPrint : public MPxLocatorNode {

public:
  static MTypeId id;
  static MString drawDbClassification;
  static MString drawRegistrantId;

  static MObject size;               // The size of the foot
  static MObject transparencySort;   // The transparent status of the foot
  static MObject transparency;       // The transparent value of the foot

  // Constructors
  rawfootPrint()
    : MPxLocatorNode()
   {};

  // Destructors
  ~rawfootPrint() override {};

  static void *  creator() {return new rawfootPrint();};
  static MStatus initialize();

  MStatus   	 compute(const MPlug& plug, MDataBlock& data) override {return MS::kUnknownParameter;};
  bool         isBounded() const override {return true;};
  MBoundingBox boundingBox() const override;

  MStatus preEvaluation(const MDGContext& context, const MEvaluationNode& evaluationNode) override;
  void    getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const override;

};


MObject rawfootPrint::size;
MObject rawfootPrint::transparencySort;
MObject rawfootPrint::transparency;
MTypeId rawfootPrint::id(0x0008002D);
MString	rawfootPrint::drawDbClassification("drawdb/geometry/rawfootPrint");
MString	rawfootPrint::drawRegistrantId("RawFootprintNodePlugin");


MStatus rawfootPrint::initialize() {
  MFnUnitAttribute unitFn;
  MFnNumericAttribute numberFn;
  
  MStatus stat;

  size = unitFn.create("size", "sz", MFnUnitAttribute::kDistance);
  unitFn.setDefault(1.0);
  
  transparencySort = numberFn.create("transparencySort", "ts", MFnNumericData::kBoolean);
  numberFn.setDefault(false);
  
  transparency = numberFn.create("transparency", "t", MFnNumericData::kFloat);
  numberFn.setDefault(1.0);
  numberFn.setMax(1.0);
  numberFn.setMin(0.0);

  addAttributes(size, transparencySort, transparency);

  // Add dependency on the localScale attribute, so that when size is modified RawFootPrintDrawOverride::transform will get called
  attributeAffects(rawfootPrint::size, MPxLocatorNode::localScale);

  return MS::kSuccess;
}


MBoundingBox rawfootPrint::boundingBox() const {
  // Get the size
  //
  MObject thisNode = thisMObject();
  MPlug plug(thisNode, size);
  MDistance sizeVal;
  plug.getValue(sizeVal);

  double multiplier = sizeVal.asCentimeters();

  MPoint corner1(-0.17, 0.0, -0.7);
  MPoint corner2(0.17, 0.0, 0.3);

  corner1 = corner1 * multiplier;
  corner2 = corner2 * multiplier;

  return MBoundingBox( corner1, corner2 );
}


// Called before this node is evaluated by Evaluation Manager
MStatus rawfootPrint::preEvaluation(const MDGContext& context, const MEvaluationNode& evaluationNode) {
  if (context.isNormal()) {
    // The size attribute is set to affect the localScale attribute during
    // initialization, thus no need to be checked here to trigger geometry
    // change.
    MStatus status;
    if ((evaluationNode.dirtyPlugExists(transparencySort, &status) && status) ||
      (evaluationNode.dirtyPlugExists(transparency, &status) && status))
    {
      MHWRender::MRenderer::setGeometryDrawDirty(thisMObject());
    }
  }

  return MStatus::kSuccess;
}



void rawfootPrint::getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const {
  MPxLocatorNode::getCacheSetup(evalNode, disablingInfo, cacheSetupInfo, monitoredAttributes);
  assert(!disablingInfo.getCacheDisabled());
  cacheSetupInfo.setPreference(MNodeCacheSetupInfo::kWantToCacheByDefault, true);
}




//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Viewport 2.0 override implementation
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------





class RawFootPrintData : public MUserData {

public:
  float                         fMultiplier;
  float                         fColor[4];
  bool                          fCustomBoxDraw;
  MBoundingBox                  fCurrentBoundingBox;
  MDAGDrawOverrideInfo          fDrawOV;
  const MHWRender::MBlendState* mBlendState{nullptr};
  MUint64                       mLastFrameStamp{0};

  // Destructors
  ~RawFootPrintData() override { 
    mBlendState = nullptr; 
  }

};




// Helper class declaration for the object drawing
class RawFootPrintDrawAgent {

public:
  // Constructors
  RawFootPrintDrawAgent()
    : mDrawContext(NULL)
    , mShaderOverride(NULL)
  {};
  virtual ~RawFootPrintDrawAgent() {}

  virtual void drawShaded() = 0;
  virtual void drawBoundingBox() = 0;
  virtual void drawWireframe() = 0;

  virtual void setMatrix(const MHWRender::MDrawContext& context, const MHWRender::MSelectionInfo* selectInfo) = 0;
  virtual void beginDraw(const MHWRender::MDrawContext& context, MHWRender::MShaderInstance* passShaderOverride) {
    mDrawContext = &context;
    mShaderOverride = passShaderOverride;
    // Sample code to debug geometry streams required for the override shader.
    const bool debugShader = false;
    MHWRender::MVertexBufferDescriptorList bufferList;
    if (mShaderOverride) {
      if (debugShader) {
        mShaderOverride->requiredVertexBuffers(bufferList);
        for (int i=0; i<bufferList.length(); i++) {
          MHWRender::MVertexBufferDescriptor desc;
          bufferList.getDescriptor( i, desc );
          printf("Buffer[%d][name=%s]\n", i, desc.name().asChar());
          printf("[semantic = %s\n", desc.semanticName().asChar());
          printf("[dataType = %d\n", desc.dataType());
          printf("[dataTyleSize = %d\n", desc.dataTypeSize());
          printf("[dimension = %d\n", desc.dimension());
          printf("[offset = %d\n", desc.offset());
          printf("[stride = %d\n", desc.stride());
        }
      }
      mShaderOverride->bind( *mDrawContext );
      mShaderOverride->updateParameters( *mDrawContext );
      mShaderOverride->activatePass( *mDrawContext, 0 );
    }
  }

  virtual void endDraw() {
    if (mShaderOverride) {
      mShaderOverride->unbind(*mDrawContext);
    }
    mDrawContext = NULL;
    mShaderOverride = NULL;
  }

  void setColor( const MColor& color) {
    mColor = color;
  }

protected:
  const MHWRender::MDrawContext* mDrawContext;
  MHWRender::MShaderInstance* mShaderOverride;
  MColor mColor;
};


// GL draw agent declaration
class RawFootPrintDrawAgentGL : public RawFootPrintDrawAgent {

public:
  static RawFootPrintDrawAgentGL& getDrawAgent() {
    static RawFootPrintDrawAgentGL obj;
    return obj;
  }

  void drawShaded() override;
  void drawBoundingBox() override;
  void drawWireframe() override;
  void beginDraw(const MHWRender::MDrawContext& context, MHWRender::MShaderInstance* passShaderOverride) override;
  void endDraw() override;

  void setMatrix(const MHWRender::MDrawContext& context, const MHWRender::MSelectionInfo* selectInfo) override {
    // Get  world view matrix
    MStatus status;
    mWorldViewMatrix = context.getMatrix(MHWRender::MFrameContext::kWorldViewMtx, &status);
    if (status != MStatus::kSuccess) return;

    // Get projection matrix
    mProjectionMatrix = context.getMatrix(MHWRender::MFrameContext::kProjectionMtx, &status);
    if (status != MStatus::kSuccess) return;

    // Compute a pick matrix that, when it is post-multiplied with the
    // projection matrix, will cause the picking region to fill the entire
    // viewport for OpenGL selection mode.
    if (selectInfo) {
      int view_x, view_y, view_w, view_h;
      context.getViewportDimensions(view_x, view_y, view_w, view_h);

      unsigned int sel_x, sel_y, sel_w, sel_h;
      selectInfo->selectRect(sel_x, sel_y, sel_w, sel_h);

      double center_x = sel_x + sel_w * 0.5;
      double center_y = sel_y + sel_h * 0.5;

      MMatrix pickMatrix;
      pickMatrix[0][0] = view_w / double(sel_w);
      pickMatrix[1][1] = view_h / double(sel_h);
      pickMatrix[3][0] = (view_w - 2.0 * (center_x - view_x)) / double(sel_w);
      pickMatrix[3][1] = (view_h - 2.0 * (center_y - view_y)) / double(sel_h);

      mProjectionMatrix *= pickMatrix;
    }
  }

private:
  // Constructors
  RawFootPrintDrawAgentGL() {}
  ~RawFootPrintDrawAgentGL() override {}
  RawFootPrintDrawAgentGL(const RawFootPrintDrawAgentGL& v) {}
  RawFootPrintDrawAgentGL operator = (const RawFootPrintDrawAgentGL& v) {return *this;}
  // Destructors

  MMatrix mWorldViewMatrix;
  MMatrix mProjectionMatrix;
};



// GL draw agent definition
//
void RawFootPrintDrawAgentGL::beginDraw(const MHWRender::MDrawContext& context, MHWRender::MShaderInstance* passShaderOverride) {
  RawFootPrintDrawAgent::beginDraw(context, passShaderOverride);
  if (!mShaderOverride) {
    // set world view matrix
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixd(mWorldViewMatrix.matrix[0]);

    // set projection matrix
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixd(mProjectionMatrix.matrix[0]);

    glPushAttrib( GL_CURRENT_BIT );
  }
}


void RawFootPrintDrawAgentGL::endDraw() {
  if (!mShaderOverride) {
    glPopAttrib();
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
  }
  RawFootPrintDrawAgent::endDraw();
}


void RawFootPrintDrawAgentGL::drawShaded() {
  // set color
  glColor4fv( &(mColor.r) );

  glBegin(GL_TRIANGLE_FAN);
  int i;
  int last = soleCount - 1;
  for ( i = 0; i < last; ++i ) {
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(sole[i][0], sole[i][1], sole[i][2]);
  }
  glEnd();

  glBegin(GL_TRIANGLE_FAN);
  last = heelCount - 1;
  for ( i = 0; i < last; ++i ) {
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(heel[i][0], heel[i][1], heel[i][2]);
  }
  glEnd();
}

void RawFootPrintDrawAgentGL::drawBoundingBox() {
  // set color
  glColor4fv( &(mColor.r) );

  const float* bottomLeftFront = bbData[0];
  const float* topLeftFront = bbData[4];
  const float* bottomRightFront = bbData[1];
  const float* topRightFront = bbData[5];
  const float* bottomLeftBack = bbData[3];
  const float* topLeftBack = bbData[7];
  const float* bottomRightBack = bbData[2];
  const float* topRightBack = bbData[6];

  glBegin( GL_LINES );

  // 4 Bottom lines
  //
  glVertex3fv( bottomLeftFront );
  glVertex3fv( bottomRightFront );
  glVertex3fv( bottomRightFront );
  glVertex3fv( bottomRightBack );
  glVertex3fv( bottomRightBack );
  glVertex3fv( bottomLeftBack );
  glVertex3fv( bottomLeftBack );
  glVertex3fv( bottomLeftFront );

  // 4 Top lines
  //
  glVertex3fv( topLeftFront );
  glVertex3fv( topRightFront );
  glVertex3fv( topRightFront );
  glVertex3fv( topRightBack );
  glVertex3fv( topRightBack );
  glVertex3fv( topLeftBack );
  glVertex3fv( topLeftBack );
  glVertex3fv( topLeftFront );

  // 4 Side lines
  //
  glVertex3fv( bottomLeftFront );
  glVertex3fv( topLeftFront );
  glVertex3fv( bottomRightFront );
  glVertex3fv( topRightFront );
  glVertex3fv( bottomRightBack );
  glVertex3fv( topRightBack );
  glVertex3fv( bottomLeftBack );
  glVertex3fv( topLeftBack );

  glEnd();
}


void RawFootPrintDrawAgentGL::drawWireframe() {
  // set color
  glColor4fv( &(mColor.r) );

  // draw wire
  glBegin(GL_LINES);
  int i;
  int last = soleCount - 1;
  for (i = 0; i < last; ++i) {
    glVertex3f( sole[i][0],
      sole[i][1],
      sole[i][2]);
    glVertex3f( sole[i+1][0],
      sole[i+1][1],
      sole[i+1][2]);
  }
  last = heelCount - 1;
  for (i = 0; i < last; ++i) {
    glVertex3f( heel[i][0],
      heel[i][1],
      heel[i][2]);
    glVertex3f( heel[i+1][0],
      heel[i+1][1],
      heel[i+1][2]);
  }
  glEnd();
}































// GL Core Profile functions:
class GLCP {
public:
  // For more extensive OpenGL coding you will definitively want to
  // use a good extension loader framework. I recommend looking at:
  // https://www.opengl.org/wiki/OpenGL_Loading_Library 
  // to find the one that will fit your needs the best.
  // Also note that MGLFunctionTable stops at GL 2.1, so a lot
  // of functions will be missing.

#if !defined (__APPLE__)
  typedef char GLchar;
  typedef ptrdiff_t GLsizeiptr;
#endif

  typedef void (APIENTRY *Tp_glBindVertexArray) (GLuint array);
  static Tp_glBindVertexArray BindVertexArray;

  typedef void (APIENTRY *Tp_glGenVertexArrays) (GLsizei n, GLuint *arrays);
  static Tp_glGenVertexArrays GenVertexArrays;

  typedef void (APIENTRY *Tp_glDeleteVertexArrays ) (GLsizei n, const GLuint *arrays);
  static Tp_glDeleteVertexArrays DeleteVertexArrays;

  typedef void (APIENTRY *Tp_glUniform4f) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
  static Tp_glUniform4f Uniform4f;

  typedef void (APIENTRY *Tp_glUniformMatrix4fv) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
  static Tp_glUniformMatrix4fv UniformMatrix4fv;

  typedef GLint (APIENTRY *Tp_glGetUniformLocation) (GLuint program, const GLchar *name);
  static Tp_glGetUniformLocation GetUniformLocation;

  typedef GLint (APIENTRY *Tp_glGetAttribLocation) (GLuint program, const GLchar *name);
  static Tp_glGetAttribLocation GetAttribLocation;

  typedef GLuint (APIENTRY *Tp_glCreateShader ) (GLenum type);
  static Tp_glCreateShader CreateShader;

  typedef void (APIENTRY *Tp_glShaderSource ) (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
  static Tp_glShaderSource ShaderSource;

  typedef void (APIENTRY *Tp_glCompileShader ) (GLuint shader);
  static Tp_glCompileShader CompileShader;

  typedef void (APIENTRY *Tp_glGetShaderiv ) (GLuint shader, GLenum pname, GLint *params);
  static Tp_glGetShaderiv GetShaderiv;

  typedef void (APIENTRY *Tp_glGetShaderInfoLog ) (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
  static Tp_glGetShaderInfoLog GetShaderInfoLog;

  typedef GLuint (APIENTRY *Tp_glCreateProgram ) ();
  static Tp_glCreateProgram CreateProgram;

  typedef void (APIENTRY *Tp_glAttachShader ) (GLuint program, GLuint shader);
  static Tp_glAttachShader AttachShader;

  typedef void (APIENTRY *Tp_glDeleteShader ) (GLuint shader);
  static Tp_glDeleteShader DeleteShader;

  typedef void (APIENTRY *Tp_glLinkProgram ) (GLuint program);
  static Tp_glLinkProgram LinkProgram;

  typedef void (APIENTRY *Tp_glGetProgramiv ) (GLuint program, GLenum pname, GLint *params);
  static Tp_glGetProgramiv GetProgramiv;

  typedef void (APIENTRY *Tp_glUseProgram ) (GLuint program);
  static Tp_glUseProgram UseProgram;

  typedef void (APIENTRY *Tp_glDeleteProgram ) (GLuint program);
  static Tp_glDeleteProgram DeleteProgram;

  typedef void (APIENTRY *Tp_glGetProgramInfoLog ) (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
  static Tp_glGetProgramInfoLog GetProgramInfoLog;

  typedef void (APIENTRY *Tp_glGenBuffers ) (GLsizei n, GLuint *buffers);
  static Tp_glGenBuffers GenBuffers;

  typedef void (APIENTRY *Tp_glDeleteBuffers ) (GLsizei n, const GLuint *buffers);
  static Tp_glDeleteBuffers DeleteBuffers;

  typedef void (APIENTRY *Tp_glBindBuffer ) (GLenum target, GLuint buffer);
  static Tp_glBindBuffer BindBuffer;

  typedef void (APIENTRY *Tp_glBufferData ) (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
  static Tp_glBufferData BufferData;

  typedef void (APIENTRY *Tp_glEnableVertexAttribArray ) (GLuint index);
  static Tp_glEnableVertexAttribArray EnableVertexAttribArray;
  
  typedef void (APIENTRY *Tp_glVertexAttribPointer ) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
  static Tp_glVertexAttribPointer VertexAttribPointer;

  static void initGLFunctionsCoreProfile();

};


GLCP::Tp_glBindVertexArray GLCP::BindVertexArray = 0;
GLCP::Tp_glGenVertexArrays GLCP::GenVertexArrays = 0;
GLCP::Tp_glDeleteVertexArrays GLCP::DeleteVertexArrays = 0;
GLCP::Tp_glUniform4f GLCP::Uniform4f = 0;
GLCP::Tp_glUniformMatrix4fv GLCP::UniformMatrix4fv = 0;
GLCP::Tp_glGetUniformLocation GLCP::GetUniformLocation = 0;
GLCP::Tp_glGetAttribLocation GLCP::GetAttribLocation = 0;
GLCP::Tp_glCreateShader GLCP::CreateShader = 0;
GLCP::Tp_glShaderSource GLCP::ShaderSource = 0;
GLCP::Tp_glCompileShader GLCP::CompileShader = 0;
GLCP::Tp_glGetShaderiv GLCP::GetShaderiv = 0;
GLCP::Tp_glGetShaderInfoLog GLCP::GetShaderInfoLog = 0;
GLCP::Tp_glCreateProgram GLCP::CreateProgram = 0;
GLCP::Tp_glAttachShader GLCP::AttachShader = 0;
GLCP::Tp_glDeleteShader GLCP::DeleteShader = 0;
GLCP::Tp_glLinkProgram GLCP::LinkProgram = 0;
GLCP::Tp_glGetProgramiv GLCP::GetProgramiv = 0;
GLCP::Tp_glUseProgram GLCP::UseProgram = 0;
GLCP::Tp_glDeleteProgram GLCP::DeleteProgram = 0;
GLCP::Tp_glGetProgramInfoLog GLCP::GetProgramInfoLog = 0;
GLCP::Tp_glGenBuffers GLCP::GenBuffers = 0;
GLCP::Tp_glDeleteBuffers GLCP::DeleteBuffers = 0;
GLCP::Tp_glBindBuffer GLCP::BindBuffer = 0;
GLCP::Tp_glBufferData GLCP::BufferData = 0;
GLCP::Tp_glEnableVertexAttribArray GLCP::EnableVertexAttribArray = 0;
GLCP::Tp_glVertexAttribPointer GLCP::VertexAttribPointer = 0;


void GLCP::initGLFunctionsCoreProfile() {
#if defined (__APPLE__)
  #define INIT_GLPOINTER(funcName) funcName = gl##funcName
#else
  #define INIT_GLPOINTER(funcName) if (!funcName) funcName = (Tp_gl##funcName)::glXGetProcAddress((const GLubyte*)"gl" #funcName)
#endif

  INIT_GLPOINTER(BindVertexArray);
  INIT_GLPOINTER(GenVertexArrays);
  INIT_GLPOINTER(DeleteVertexArrays);
  INIT_GLPOINTER(Uniform4f);
  INIT_GLPOINTER(UniformMatrix4fv);
  INIT_GLPOINTER(GetUniformLocation);
  INIT_GLPOINTER(GetAttribLocation);
  INIT_GLPOINTER(CreateShader);
  INIT_GLPOINTER(ShaderSource);
  INIT_GLPOINTER(CompileShader);
  INIT_GLPOINTER(GetShaderiv);
  INIT_GLPOINTER(GetShaderInfoLog);
  INIT_GLPOINTER(CreateProgram);
  INIT_GLPOINTER(AttachShader);
  INIT_GLPOINTER(DeleteShader);
  INIT_GLPOINTER(LinkProgram);
  INIT_GLPOINTER(GetProgramiv);
  INIT_GLPOINTER(UseProgram);
  INIT_GLPOINTER(DeleteProgram);
  INIT_GLPOINTER(GetProgramInfoLog);
  INIT_GLPOINTER(GenBuffers);
  INIT_GLPOINTER(DeleteBuffers);
  INIT_GLPOINTER(BindBuffer);
  INIT_GLPOINTER(BufferData);
  INIT_GLPOINTER(EnableVertexAttribArray);
  INIT_GLPOINTER(VertexAttribPointer);

#undef INIT_GLPOINTER
}

#if !defined (__APPLE__)
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_STATIC_DRAW                    0x88E4
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_INFO_LOG_LENGTH                0x8B84
#endif









































// Core Profile draw agent declaration
class RawFootPrintDrawAgentCoreProfile : public RawFootPrintDrawAgent {

public:
  static RawFootPrintDrawAgentCoreProfile& getDrawAgent() {
    static RawFootPrintDrawAgentCoreProfile obj;
    return obj;
  }

  void drawShaded() override;
  void drawBoundingBox() override;
  void drawWireframe() override;
  void beginDraw( const MHWRender::MDrawContext& context, MHWRender::MShaderInstance* passShaderOverride) override;
  void endDraw() override;

  void setMatrix(const MHWRender::MDrawContext& context, const MHWRender::MSelectionInfo* selectInfo) override {
    // Get world view projection matrix
    MStatus status;
    mWorldViewProjMatrix = context.getMatrix(MHWRender::MFrameContext::kWorldViewProjMtx, &status);
  }

  bool releaseCoreProfileResources();

private:
  GLuint mShaderProgram;
  GLint mWVPIndex;
  GLint mColorIndex;
  GLint mVtxAttrib;

  GLuint mBBoxVAO;
  GLuint mSoleWireframeVAO;
  GLuint mHeelWireframeVAO;
  GLuint mSoleShadedVAO;
  GLuint mHeelShadedVAO;

  GLuint mBoundingboxVertexBuffer;
  GLuint mBoundingboxIndexBuffer;
  GLuint mSoleVertexBuffer;
  GLuint mSoleNormalBuffer;
  GLuint mHeelVertexBuffer;
  GLuint mHeelNormalBuffer;
  GLuint mSoleWireIndexBuffer;
  GLuint mSoleShadedIndexBuffer;
  GLuint mHeelWireIndexBuffer;
  GLuint mHeelShadedIndexBuffer;

  MMatrix mWorldViewProjMatrix;
  bool mInitialized;
  bool mValid;
  bool initShadersCoreProfile();
  bool initBuffersCoreProfile();
  void setupUniforms();

private:
  // Constructors
  RawFootPrintDrawAgentCoreProfile()
    // Shader program data
    : mShaderProgram(0)
    , mWVPIndex(0)
    , mColorIndex(0)
    , mVtxAttrib(0)
    // VAOs
    , mBBoxVAO(0)
    , mSoleWireframeVAO(0)
    , mHeelWireframeVAO(0)
    , mSoleShadedVAO(0)
    , mHeelShadedVAO(0)
    // Draw buffers
    , mBoundingboxVertexBuffer(0)
    , mBoundingboxIndexBuffer(0)
    , mSoleVertexBuffer(0)
    , mSoleNormalBuffer(0)
    , mHeelVertexBuffer(0)
    , mHeelNormalBuffer(0)
    , mSoleWireIndexBuffer(0)
    , mSoleShadedIndexBuffer(0)
    , mHeelWireIndexBuffer(0)
    , mHeelShadedIndexBuffer(0)
    , mInitialized(false)
    , mValid(false)
  {};
  ~RawFootPrintDrawAgentCoreProfile() override {};
  RawFootPrintDrawAgentCoreProfile(const RawFootPrintDrawAgentCoreProfile& v) {};
  RawFootPrintDrawAgentCoreProfile operator = (const RawFootPrintDrawAgentCoreProfile& v){return *this;}
  // Destructors
};



void RawFootPrintDrawAgentCoreProfile::beginDraw(const MHWRender::MDrawContext& context, MHWRender::MShaderInstance* passShaderOverride) {
  // Init common data:
  if ( !mInitialized ){
    // Set up all the one-time data:
    GLCP::initGLFunctionsCoreProfile();
    mValid = initShadersCoreProfile() && initBuffersCoreProfile();
    mInitialized = true;
  }

  if (!mValid)
    return;

  RawFootPrintDrawAgent::beginDraw(context, passShaderOverride);
  if (!mShaderOverride) {
    GLCP::UseProgram(mShaderProgram);
  }
}


void RawFootPrintDrawAgentCoreProfile::endDraw() {
  if (!mValid)
    return;

  if (!mShaderOverride) {
    GLCP::UseProgram(0);
  }
  GLCP::BindVertexArray(0);

  RawFootPrintDrawAgent::endDraw();
}


void RawFootPrintDrawAgentCoreProfile::setupUniforms() {
  if( mShaderOverride == NULL ) {
    float fMatrix[4][4];
    for (size_t i = 0; i < 4; ++i)
      for (size_t j = 0; j < 4; ++j)
        fMatrix[i][j] = (float)mWorldViewProjMatrix.matrix[i][j];

    GLCP::UniformMatrix4fv(mWVPIndex, 1, GL_FALSE, (float*)fMatrix);

    GLCP::Uniform4f(mColorIndex, mColor.r, mColor.g, mColor.b, mColor.a);
  }
}


void RawFootPrintDrawAgentCoreProfile::drawShaded() {
  if (!mValid)
    return;

  // Set uniforms
  setupUniforms();

  GLCP::BindVertexArray(mSoleShadedVAO);
  glDrawElements(GL_TRIANGLES, 3 * (soleCount-2), GL_UNSIGNED_SHORT, 0);
  GLCP::BindVertexArray(mHeelShadedVAO);
  glDrawElements(GL_TRIANGLES, 3 * (heelCount-2), GL_UNSIGNED_SHORT, 0);
}


void RawFootPrintDrawAgentCoreProfile::drawBoundingBox() {
  if (!mValid)
    return;

  // Set uniforms
  setupUniforms();

  GLCP::BindVertexArray(mBBoxVAO);
  glDrawElements(GL_LINES, 2 * 12, GL_UNSIGNED_SHORT, 0);
}


void RawFootPrintDrawAgentCoreProfile::drawWireframe() {
  if (!mValid)
    return;

  // Set uniforms
  setupUniforms();

  GLCP::BindVertexArray(mSoleWireframeVAO);
  glDrawElements(GL_LINES, 2 * (soleCount-1), GL_UNSIGNED_SHORT, 0);
  GLCP::BindVertexArray(mHeelWireframeVAO);
  glDrawElements(GL_LINES, 2 * (heelCount-1), GL_UNSIGNED_SHORT, 0);
}



































class RawFootPrintDrawOverride : public MHWRender::MPxDrawOverride {

public:
  // Constructors
  // By setting isAlwaysDirty to false in MPxDrawOverride constructor, the
  // draw override will be updated (via prepareForDraw()) only when the node
  // is marked dirty via DG evaluation or dirty propagation. Additional
  // callback is also added to explicitly mark the node as being dirty (via
  // MRenderer::setGeometryDrawDirty()) for certain circumstances.
  RawFootPrintDrawOverride(const MObject& obj)
    : MHWRender::MPxDrawOverride(obj, RawFootPrintDrawOverride::draw, false)
    // We want to perform custom bounding box drawing
    // so return true so that the internal rendering code
    // will not draw it for us.
    , mCustomBoxDraw(true)
    , mExcludeFromPostEffects(false)
    , mTransparencySort(false)
    , mBlendState(NULL)
    , mLastFrameStamp(0)
    , fRawFootPrint(obj)
    {
      fModelEditorChangedCbId = MEventMessage::addEventCallback("modelEditorChanged", OnModelEditorChanged, this);
    };

  // Destructor
  ~RawFootPrintDrawOverride() override {
    if (mBlendState) {
      MHWRender::MStateManager::releaseBlendState(mBlendState);
      mBlendState = NULL;
    }
    if (fModelEditorChangedCbId != 0) {
      MMessage::removeCallback(fModelEditorChangedCbId);
      fModelEditorChangedCbId = 0;
    }
  };

  static MHWRender::MPxDrawOverride* Creator(const MObject& obj) {return new RawFootPrintDrawOverride(obj);}
  MHWRender::DrawAPI supportedDrawAPIs() const override {return (MHWRender::kOpenGL | MHWRender::kOpenGLCoreProfile);};

  bool isBounded(const MDagPath& objPath, const MDagPath& cameraPath) const override {return true;};
  MMatrix transform(const MDagPath& objPath, const MDagPath& cameraPath) const override;
  MBoundingBox boundingBox(const MDagPath& objPath, const MDagPath& cameraPath) const override;

  bool disableInternalBoundingBoxDraw() const override {return mCustomBoxDraw;};
  bool excludedFromPostEffects() const override {return mExcludeFromPostEffects;};
  bool isTransparent() const override {return mTransparencySort;};

  MUserData* prepareForDraw(
    const MDagPath& objPath,
    const MDagPath& cameraPath,
    const MHWRender::MFrameContext& frameContext,
    MUserData* oldData) override;

  bool hasUIDrawables() const override { return true; }

  void addUIDrawables(
    const MDagPath& objPath,
    MHWRender::MUIDrawManager& drawManager,
    const MHWRender::MFrameContext& frameContext,
    const MUserData* data) override;

  // The general trace sequence we will get is:
  //
  // At creation and during geometry chagnes (size attribute)
  //
  // rawFootPrintNode: Start draw override DG bounding box update for object:: |transform1|rawfootPrint1
  // rawFootPrintNode: End draw override DG bounding box update for object:: |transform1|rawfootPrint1
  //

  // At creation and during attribute or transform modification:
  //
  // rawFootPrintNode: Start draw override DG node transform update for object: |transform1|rawfootPrint1
  // rawFootPrintNode: End draw override DG node transform update for object: |transform1|rawfootPrint1
  //

  // Every refresh:
  //
  // rawFootPrintNode: Start draw override render item update for object: |transform1|rawfootPrint1
  // rawFootPrintNode:  - call draw override prepareForDraw()
  // rawFootPrintNode: End draw override render item update for object: |transform1|rawfootPrint1
  // rawFootPrintNode: Start draw override queuing of UI drawables for object: |transform1|rawfootPrint1
  // rawFootPrintNode:  - call draw override addUIDrawables()
  // rawFootPrintNode: End draw override queuing of UI drawables for object: |transform1|rawfootPrint1
  //

  // Every draw:
  //
  // rawFootPrintNode: Start draw override draw function: |transform1|rawfootPrint1
  // rawFootPrintNode: End draw override draw function: |transform1|rawfootPrint1
  //

  // Return true if internal tracing is desired.
  bool traceCallSequence() const override {return false;}
  void handleTraceMessage( const MString &message ) const override {
    // Some simple custom message formatting.
    fputs("rawFootPrintNode: ", stderr);
    fputs(message.asChar(), stderr);
    fputs("\n", stderr);
  }

  bool wantUserSelection() const override {
    // Perform user selection when the viewport renderer is using legacy
    // OpenGL API which supports OpenGL selection mode
    MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
    return theRenderer && theRenderer->drawAPI() == MHWRender::kOpenGL;
  }

  bool userSelect(
    const MHWRender::MSelectionInfo& selectInfo,
    const MHWRender::MDrawContext& context,
    const MDagPath& objPath,
    const MUserData* data,
    MSelectionList& selectionList,
    MPointArray& worldSpaceHitPts) override
  {
    // This is the user selection function that will be invoked only if
    // wantUserSelection() returns true. The OpenGL selection mode is used
    // here for simplicity reason, however, a plug-in can choose other
    // GPU-based approaches like occlusion query, or CPU-based approaches
    // which perform hit test on object shapes.

    // Set up selection buffer
    const GLsizei BUF_SIZE = 10;
    GLuint selectBuf[BUF_SIZE];
    glSelectBuffer(BUF_SIZE, selectBuf);

    // Enter OpenGL selection mode
    glRenderMode(GL_SELECT);

    // Call into draw code with a name pushed onto the name stack and the
    // select info passed into the method for adjusting projection matrix.
    glInitNames();
    glPushName(0);
    drawImpl(context, &selectInfo, data);
    glPopName();

    // Exit OpenGL selection mode.
    GLint hits = glRenderMode(GL_RENDER);

    // Return false if nothing is hit.
    if (hits <= 0) return false;

    // Otherwise, calculate the hit point in world space. The depth is the
    // average of minimum and maximum, so it isn't the accurate depth at
    // the cursor position.
    GLuint minZ = selectBuf[1];
    GLuint maxZ = selectBuf[2];
    double depth = (minZ*0.5 + maxZ*0.5) / 0xFFFFFFFF;

    MFnCamera camera(context.getCurrentCameraPath());
    if (!camera.isOrtho())
    {
      const double Zn = camera.nearClippingPlane();
      const double Zf = camera.farClippingPlane();
      depth *= Zn / (Zf - depth * (Zf - Zn));
    }

    unsigned int x, y, w, h;
    selectInfo.selectRect(x, y, w, h);

    MPoint nearPw, farPw;
    context.viewportToWorld(x + w*0.5, y + h*0.5, nearPw, farPw);

    const MPoint hitPoint = nearPw + depth * (farPw - nearPw);

    selectionList.add(objPath);
    worldSpaceHitPts.append(hitPoint);

    return true;
  }

  static void drawImpl(const MHWRender::MDrawContext& context, const MHWRender::MSelectionInfo* selectInfo, const MUserData* data);
  static void draw(const MHWRender::MDrawContext& context, const MUserData* data) {drawImpl(context, nullptr, data);}

protected:
  MBoundingBox mCurrentBoundingBox;
  bool		mCustomBoxDraw;
  bool		mExcludeFromPostEffects;
  bool		mTransparencySort;
  MUint64	mLastFrameStamp;
private:
  // RawFootPrintDrawOverride(const MObject& obj);

  float	getMultiplier(const MDagPath& objPath) const;
  bool	isTransparentSort(const MDagPath& objPath) const;
  float	getTransparency(const MDagPath& objPath) const;

  static void OnModelEditorChanged(void *clientData);

  MCallbackId fModelEditorChangedCbId;
  MObject fRawFootPrint;

  const MHWRender::MBlendState* mBlendState;
};





void RawFootPrintDrawOverride::OnModelEditorChanged(void *clientData) {
  // Mark the node as being dirty so that it can update on display appearance
  // switch among wireframe and shaded.
  RawFootPrintDrawOverride *ovr = static_cast<RawFootPrintDrawOverride*>(clientData);
  if (ovr) MHWRender::MRenderer::setGeometryDrawDirty(ovr->fRawFootPrint);
}


float RawFootPrintDrawOverride::getMultiplier(const MDagPath& objPath) const {
  // Retrieve value of the size attribute from the node
  MStatus status;
  MObject rawfootprintNode = objPath.node(&status);
  if (status)
  {
    MPlug plug(rawfootprintNode, rawfootPrint::size);
    if (!plug.isNull())
    {
      MDistance sizeVal;
      if (plug.getValue(sizeVal))
      {
        return (float)sizeVal.asCentimeters();
      }
    }
  }

  return 1.0f;
}

bool RawFootPrintDrawOverride::isTransparentSort(const MDagPath& objPath) const
{
  MStatus status;
  MObject rawfootprintNode = objPath.node(&status);
  if (status)
  {
    MPlug plug(rawfootprintNode, rawfootPrint::transparencySort);
    if (!plug.isNull())
    {
      bool isTransparent;
      if (plug.getValue(isTransparent))
      {
        return isTransparent;
      }
    }
  }

  return false;
}

float RawFootPrintDrawOverride::getTransparency(const MDagPath& objPath) const
{
  // Retrieve value of the transparency attribute from the node
  MStatus status;
  MObject rawfootprintNode = objPath.node(&status);
  if (status)
  {
    MPlug plug(rawfootprintNode, rawfootPrint::transparency);
    if (!plug.isNull())
    {
      float transparencyVal;
      if (plug.getValue(transparencyVal))
      {
        return transparencyVal;
      }
    }
  }

  return 1.0f;
}


MMatrix RawFootPrintDrawOverride::transform(
  const MDagPath& objPath,
  const MDagPath& cameraPath) const
{
  // Update the transform matrix with the multiplier value,
  // this way, the matrices of the draw context will already contain the multiplier factor.
  // That way the WorldViewProjection uniform of the selection shader - which we do not have access to -
  // will be set properly.

  MMatrix transform = objPath.inclusiveMatrix();
  float multiplier = getMultiplier(objPath);

  transform[0][0] *= multiplier;
  transform[1][1] *= multiplier;
  transform[2][2] *= multiplier;

  return transform;
}

MBoundingBox RawFootPrintDrawOverride::boundingBox(
  const MDagPath& objPath,
  const MDagPath& cameraPath) const
{
  MPoint corner1( -0.17, 0.0, -0.7 );
  MPoint corner2( 0.17, 0.0, 0.3 );

  float multiplier = getMultiplier(objPath);
  corner1 = corner1 * multiplier;
  corner2 = corner2 * multiplier;

  RawFootPrintDrawOverride *nonConstThis = (RawFootPrintDrawOverride *)this;
  nonConstThis->mCurrentBoundingBox.clear();
  nonConstThis->mCurrentBoundingBox.expand( corner1 );
  nonConstThis->mCurrentBoundingBox.expand( corner2 );

  return mCurrentBoundingBox;
}


MUserData* RawFootPrintDrawOverride::prepareForDraw(
  const MDagPath& objPath,
  const MDagPath& cameraPath,
  const MHWRender::MFrameContext& frameContext,
  MUserData* oldData)
{
  MDagPath cameraPathToo = frameContext.getCurrentCameraPath();
  MString ufeCamera = frameContext.getCurrentUfeCameraPath();
  
  // We only care about post effects when in shaded mode
  const unsigned int displayStyle = frameContext.getDisplayStyle();
  bool shadedMode = (displayStyle & MHWRender::MFrameContext::kGouraudShaded) ||
            (displayStyle & MHWRender::MFrameContext::kFlatShaded);
  static bool sExcludedFromPostEffects =
    (getenv("MAYA_RAWFOOTPRINT_EXCLUDEDFROMPOSTEFFECTS") != NULL);
  mExcludeFromPostEffects = !shadedMode || sExcludedFromPostEffects;

  mTransparencySort = isTransparentSort(objPath);

  // Retrieve data cache (create if does not exist)
  RawFootPrintData* data = dynamic_cast<RawFootPrintData*>(oldData);
  if (!data)
  {
    data = new RawFootPrintData();
    data->mLastFrameStamp = 0;
  }

  // compute data and cache it
  data->fMultiplier = getMultiplier(objPath);

  MHWRender::DisplayStatus displayStatus =
    MHWRender::MGeometryUtilities::displayStatus(objPath);
  if ((displayStatus == MHWRender::kLead) ||
    (displayStatus == MHWRender::kActive) ||
    (displayStatus == MHWRender::kHilite) ||
    (displayStatus == MHWRender::kActiveComponent))
  {
    MColor color = MHWRender::MGeometryUtilities::wireframeColor(objPath);
    data->fColor[0] = color.r;
    data->fColor[1] = color.g;
    data->fColor[2] = color.b;
  }
  else
  {
    data->fColor[0] = 0.6f;
    data->fColor[1] = 0.6f;
    data->fColor[2] = 0.6f;
  }
  data->fColor[3] = getTransparency(objPath);

  data->fCustomBoxDraw = mCustomBoxDraw;
  data->fCurrentBoundingBox = mCurrentBoundingBox;

  // Get the draw override information
  data->fDrawOV = objPath.getDrawOverrideInfo();

  // Get a blend state override
  if (!mBlendState)
  {
    MHWRender::MBlendStateDesc blendStateDesc;
    for(int i = 0; i < (blendStateDesc.independentBlendEnable ? MHWRender::MBlendState::kMaxTargets : 1); ++i)
    {
      blendStateDesc.targetBlends[i].blendEnable = true;
      blendStateDesc.targetBlends[i].sourceBlend = MHWRender::MBlendState::kSourceAlpha;
      blendStateDesc.targetBlends[i].destinationBlend = MHWRender::MBlendState::kInvSourceAlpha;
      blendStateDesc.targetBlends[i].blendOperation = MHWRender::MBlendState::kAdd;
      blendStateDesc.targetBlends[i].alphaSourceBlend = MHWRender::MBlendState::kOne;
      blendStateDesc.targetBlends[i].alphaDestinationBlend = MHWRender::MBlendState::kInvSourceAlpha;
      blendStateDesc.targetBlends[i].alphaBlendOperation = MHWRender::MBlendState::kAdd;
    }

    blendStateDesc.blendFactor[0] = 1.0f;
    blendStateDesc.blendFactor[1] = 1.0f;
    blendStateDesc.blendFactor[2] = 1.0f;
    blendStateDesc.blendFactor[3] = 1.0f;

    mBlendState = MHWRender::MStateManager::acquireBlendState(blendStateDesc);
  }
  data->mBlendState = mBlendState;

  return data;
}


void RawFootPrintDrawOverride::addUIDrawables(
  const MDagPath& objPath,
  MHWRender::MUIDrawManager& drawManager,
  const MHWRender::MFrameContext& frameContext,
  const MUserData* data)
{
  // Draw a text "Foot"
  MPoint pos( 0.0, 0.0, 0.0 ); // Position of the text
  MColor textColor( 0.1f, 0.8f, 0.8f, 1.0f ); // Text color

  drawManager.beginDrawable();

  drawManager.setColor( textColor );
  drawManager.setFontSize( MHWRender::MUIDrawManager::kSmallFontSize );
  drawManager.text( pos,  MString("rawFootprint"), MHWRender::MUIDrawManager::kCenter );

  drawManager.endDrawable();
}


//
// Main drawing routine
//
void RawFootPrintDrawOverride::drawImpl(
  const MHWRender::MDrawContext& context,
  const MHWRender::MSelectionInfo* selectInfo,
  const MUserData* data)
{
  // Get renderer
  MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
  if (!theRenderer)
  {
    return;
  }

  // Get user draw data
  const RawFootPrintData* footData = dynamic_cast<const RawFootPrintData*>(data);
  if (!footData)
  {
    return;
  }

  // Debug code to trace frame stamp numbering
  static bool traceFrameStamp = false;
  if (traceFrameStamp)
  {
    const MHWRender::MPassContext & passCtx = context.getPassContext();
    const MStringArray & passSem = passCtx.passSemantics();
    MUint64 currentFrameStamp = context.getFrameStamp();
    if (currentFrameStamp != footData->mLastFrameStamp)
    {
      bool updateColorPassFrameStamp = true;
      if (passSem.length() == 1 && 
        (passSem[0] == MHWRender::MPassContext::kSelectionPassSemantic ||
        passSem[0] == MHWRender::MPassContext::kShadowPassSemantic))
      {
        updateColorPassFrameStamp = false;
      }
      if (updateColorPassFrameStamp)
      {
        printf("RawFootPrintDrawOverride: frame stamp change from %ld to %ld\n", 					
          (long)footData->mLastFrameStamp, (long)currentFrameStamp);
      }
      ((RawFootPrintData*)footData)->mLastFrameStamp = currentFrameStamp;
    }
  }

  // Get DAG object draw override
  const MDAGDrawOverrideInfo& objectOverrideInfo = footData->fDrawOV;

  // Just return and draw nothing, if it is overridden invisible
  if ( objectOverrideInfo.fOverrideEnabled && !objectOverrideInfo.fEnableVisible )
  {
    return;
  }

  // If in playback but hidden in playback, skip drawing
  bool animPlay = MAnimControl::isPlaying();
  bool animScrub = MAnimControl::isScrubbing();
  if (!objectOverrideInfo.fPlaybackVisible &&
    (animPlay || animScrub))
  {
    return;
  }
  

  // Check display styles to determine what modes to draw with
  //
  const unsigned int displayStyle = context.getDisplayStyle();

  bool drawAsBoundingbox =
    (displayStyle & MHWRender::MFrameContext::kBoundingBox) ||
    (footData->fDrawOV.fLOD == MDAGDrawOverrideInfo::kLODBoundingBox);

  // Require shadeded draw
  bool drawShaded = ((displayStyle & MHWRender::MFrameContext::kGouraudShaded) ||
            (displayStyle & MHWRender::MFrameContext::kFlatShaded));
  // Require wireframe draw
  bool drawWireframe = (displayStyle & MHWRender::MFrameContext::kWireFrame) > 0;

  // Templated, only draw wirefame and it is not selectale
  bool overideTemplated = objectOverrideInfo.fOverrideEnabled &&
    (objectOverrideInfo.fDisplayType == MDAGDrawOverrideInfo::kDisplayTypeTemplate);
  if (overideTemplated)
  {
    drawWireframe = true;
  }

  // Override no shaded, only show wireframe
  bool overrideNoShaded = objectOverrideInfo.fOverrideEnabled && !objectOverrideInfo.fEnableShading;
  if (overideTemplated || overrideNoShaded)
  {
    drawShaded = false;
  }

  // If we don't want to draw the bounds within this plugin
  // manually, then skip drawing altogether in bounding box mode
  // since the bounds draw is handled by the renderer and
  // doesn't need to be drawn here.
  //
  if ( drawAsBoundingbox && !footData->fCustomBoxDraw )
  {
    drawAsBoundingbox = false;
  }

  // For any viewport interactions switch to bounding box mode,
  // except when we are in playback. Uncomment to enable usage.
  if (context.inUserInteraction() || context.userChangingViewContext())
  {
    if (!animPlay && !animScrub)
    {
      ; //drawAsBoundingbox = true;
    }
  }

  // Check to see if we are drawing with a shader override,
  // and look for any specific branching required for different passes
  // (e.g. for selection)
  //
  bool inSelection = false;
  bool inBeautyPass = false;
  bool inTransparencyPass = false;
  const MString beautyPassSemantic("shadedBeautyGraphSemantic");
  const MHWRender::MPassContext & passCtx = context.getPassContext();
  const MStringArray & passSem = passCtx.passSemantics();
  MHWRender::MShaderInstance* passShaderOverride = NULL;
  for (unsigned int i=0; i<passSem.length(); i++)
  {
    if (passSem[i] == MHWRender::MPassContext::kSelectionPassSemantic)
    {
      inSelection = true;
    }
    else if (passSem[i] == beautyPassSemantic)
    {
      inBeautyPass = true;
    }

    // Do not use the shader override in selection drawing code path
    if (selectInfo == nullptr && passCtx.hasShaderOverride())
    {
      passShaderOverride = passCtx.shaderOverrideInstance();

      // Update single-sided state and normal multiplier as needed
      passShaderOverride->setParameter("isSingleSided", (displayStyle & MHWRender::MFrameContext::kTwoSidedLighting) == 0);
      passShaderOverride->setParameter("mayaNormalMultiplier", -1.0f);
    }

    if (passSem[i] == MHWRender::MPassContext::kCullBackSemantic ||
      passSem[i] == MHWRender::MPassContext::kCullFrontSemantic)
    {
      inTransparencyPass = true;
    }
  }

  // Get cached data
  float color[4] = {
      footData->fColor[0] * footData->fColor[3],
      footData->fColor[1] * footData->fColor[3],
      footData->fColor[2] * footData->fColor[3],
      footData->fColor[3]
  };

  bool requireBlending = false;

  if (!inSelection) {
    // Use some monotone version of color to show "default material mode"
    //
    //if (displayStyle & MHWRender::MFrameContext::kDefaultMaterial)
    //{
    //	color[0] = color[1] = color[2] = (color[0] + color[1] + color[2]) / 3.0f;
    //}
    // Do some alpha blending if in x-ray mode
    //
    if (displayStyle & MHWRender::MFrameContext::kXray)
    {
      // If in beauty pass should still allow blending e.g. kMaterialOverrideSemantic
      requireBlending = !passShaderOverride;
      color[3] = 0.3f;
    }
  }

  //================================================
  // Set blend and raster state
  //================================================
  MHWRender::MStateManager* stateMgr = context.getStateManager();
  const MHWRender::MBlendState* pOldBlendState = NULL;
  const MHWRender::MRasterizerState* pOldRasterState = NULL;
  const MHWRender::MRasterizerState* rasterState = NULL;

  if(stateMgr && ((displayStyle & MHWRender::MFrameContext::kGouraudShaded) ||
           (displayStyle & MHWRender::MFrameContext::kFlatShaded)))
  {
    // draw filled, and with blending if required
    if (stateMgr && requireBlending)
    {		
      if (footData->mBlendState)
      {
        pOldBlendState = stateMgr->getBlendState();
        stateMgr->setBlendState(footData->mBlendState);
      }
    }

    // If the object is not draw as transparency, 
    // override culling mode since we always want double-sided
    //
    if (!inTransparencyPass)
    {
      pOldRasterState = stateMgr ? stateMgr->getRasterizerState() : NULL;
      if (pOldRasterState)
      {

        MHWRender::MRasterizerStateDesc desc( pOldRasterState->desc() );
        // It's also possible to change this to kCullFront or kCullBack if we
        // wanted to set it to that.
        MHWRender::MRasterizerState::CullMode cullMode = MHWRender::MRasterizerState::kCullNone;
        if (desc.cullMode != cullMode)
        {
          // Just override the cullmode
          desc.cullMode = cullMode;
          rasterState = stateMgr->acquireRasterizerState(desc);
          if (rasterState)
          {
            stateMgr->setRasterizerState(rasterState);
          }
        }
      }
    }
  }

  //================================================
  // Start the draw work
  //================================================

  // Prepare draw agent, default using OpenGL
  RawFootPrintDrawAgentGL& drawAgentRef = RawFootPrintDrawAgentGL::getDrawAgent();
  RawFootPrintDrawAgent* drawAgentPtr = &drawAgentRef;

  if ( theRenderer->drawAPI() == MHWRender::kOpenGLCoreProfile )
  {
    RawFootPrintDrawAgentCoreProfile& drawAgentRef = RawFootPrintDrawAgentCoreProfile::getDrawAgent();
    drawAgentPtr = &drawAgentRef;
  }

  assert( drawAgentPtr );

  if (drawAgentPtr) {

    // Set color
    drawAgentPtr->setColor(MColor(color[0], color[1], color[2], color[3]));
    // Set matrix
    drawAgentPtr->setMatrix( context, selectInfo);

    drawAgentPtr->beginDraw( context, passShaderOverride );

    if (drawAsBoundingbox)
    {
      // If it is in bounding bode, draw only bounding box wireframe, nothing else
      drawAgentPtr->drawBoundingBox();
    }
    else {
      if (drawWireframe || overideTemplated || overrideNoShaded) 
      {
        drawAgentPtr->drawWireframe();
      }
      if (!overideTemplated && !overrideNoShaded  && drawShaded)
      {
        drawAgentPtr->drawShaded();
      }
    }

    drawAgentPtr->endDraw();
  }

  //================================================
  // End the draw work
  //================================================


  if ( passShaderOverride ) {
    theRenderer->getShaderManager()->releaseShader( passShaderOverride );
  }


  //================================================
  // Restore old blend state and old raster state
  //================================================
  if(stateMgr && (displayStyle & MHWRender::MFrameContext::kGouraudShaded  ||
          displayStyle & MHWRender::MFrameContext::kFlatShaded))
  {
    if (stateMgr && pOldBlendState) {
      stateMgr->setBlendState(pOldBlendState);
      stateMgr->releaseBlendState(pOldBlendState);
    }
    if (pOldRasterState) {
      stateMgr->setRasterizerState(pOldRasterState);
      stateMgr->releaseRasterizerState(pOldRasterState);
    }
    if (rasterState) {
      stateMgr->releaseRasterizerState(rasterState);
      rasterState = NULL;
    }
  }
}




























// By setting isAlwaysDirty to false in MPxDrawOverride constructor, the
// draw override will be updated (via prepareForDraw()) only when the node
// is marked dirty via DG evaluation or dirty propagation. Additional
// callback is also added to explicitly mark the node as being dirty (via
// MRenderer::setGeometryDrawDirty()) for certain circumstances.
// RawFootPrintDrawOverride::RawFootPrintDrawOverride(const MObject& obj)
// : MHWRender::MPxDrawOverride(obj, RawFootPrintDrawOverride::draw, false)
// // We want to perform custom bounding box drawing
// // so return true so that the internal rendering code
// // will not draw it for us.
// , mCustomBoxDraw(true)
// , mExcludeFromPostEffects(false)
// , mTransparencySort(false)
// , mBlendState(NULL)
// , mLastFrameStamp(0)
// , fRawFootPrint(obj)
// {
//   fModelEditorChangedCbId = MEventMessage::addEventCallback(
//     "modelEditorChanged", OnModelEditorChanged, this);
// }





bool RawFootPrintDrawAgentCoreProfile::initShadersCoreProfile() {
  static const char* vertexShaderText = 
    "#version 330  \n"
    "in vec3 Pm;  \n"
    "uniform mat4 MVP;  \n"
    "void main() {  \n"
    "	gl_Position = MVP * vec4(Pm,1.0);  \n"
    "}";
  GLuint vertexShaderID = GLCP::CreateShader(GL_VERTEX_SHADER);
  GLCP::ShaderSource(vertexShaderID, 1, &vertexShaderText , NULL);
  GLCP::CompileShader(vertexShaderID);

  // Check for errors:
  GLint result;
  GLCP::GetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
  if(result==GL_FALSE){
    GLint errLength;
    GLCP::GetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &errLength);
    char* errorMessage = (char*)malloc(errLength+1);
    GLCP::GetShaderInfoLog(vertexShaderID, errLength, NULL, &errorMessage[0]);
    fprintf(stdout, "Compilation of vertex shader failed:\n%s\n", &errorMessage[0]);
    free(errorMessage);
    return false;
  }

  static const char* fragmentShaderText = 
    "#version 330  \n"
    "uniform vec4 color = vec4( 1.0f, 1.0f, 1.0f, 1.0f );  \n"
    "out vec4 colorOut; \n"
    "void main() {  \n"
    "	colorOut = color;  \n"
    "}";

  GLuint fragmentShaderID = GLCP::CreateShader(GL_FRAGMENT_SHADER);
  GLCP::ShaderSource(fragmentShaderID, 1, &fragmentShaderText , NULL);
  GLCP::CompileShader(fragmentShaderID);

  GLCP::GetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
  if(result==GL_FALSE){
    GLint errLength;
    GLCP::GetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &errLength);
    char* errorMessage = (char*)malloc(errLength+1);
    GLCP::GetShaderInfoLog(fragmentShaderID, errLength, NULL, &errorMessage[0]);
    fprintf(stdout, "Compilation of fragment shader failed:\n%s\n", &errorMessage[0]);
    free(errorMessage);
    return false;
  }

  mShaderProgram = GLCP::CreateProgram();
  GLCP::AttachShader(mShaderProgram, vertexShaderID);
  GLCP::AttachShader(mShaderProgram, fragmentShaderID);
  GLCP::LinkProgram(mShaderProgram);

  GLCP::GetProgramiv(mShaderProgram, GL_LINK_STATUS, &result);
  if(result==GL_FALSE){
    GLint errLength;
    GLCP::GetProgramiv(mShaderProgram, GL_INFO_LOG_LENGTH, &errLength);
    char* errorMessage = (char*)malloc(errLength+1);
    GLCP::GetProgramInfoLog(mShaderProgram, errLength, NULL, &errorMessage[0]);
    fprintf(stdout, "Linking of shader program failed:\n%s\n", &errorMessage[0]);
    free(errorMessage);
    return false;
  }

  mWVPIndex = GLCP::GetUniformLocation(mShaderProgram, "MVP");
  mColorIndex = GLCP::GetUniformLocation(mShaderProgram, "color");
  mVtxAttrib = GLCP::GetAttribLocation( mShaderProgram, "Pm" );

  GLCP::DeleteShader(vertexShaderID);
  GLCP::DeleteShader(fragmentShaderID);

  return true;
}


bool RawFootPrintDrawAgentCoreProfile::initBuffersCoreProfile() {
  GLCP::GenVertexArrays(1, &mBBoxVAO);
  GLCP::BindVertexArray(mBBoxVAO);

  GLCP::GenBuffers(1, &mBoundingboxVertexBuffer);
  GLCP::BindBuffer(GL_ARRAY_BUFFER, mBoundingboxVertexBuffer);
  GLCP::BufferData(GL_ARRAY_BUFFER, sizeof(bbData), (void*)bbData, GL_STATIC_DRAW);

  GLCP::GenBuffers(1, &mBoundingboxVertexBuffer);
  GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBoundingboxVertexBuffer);
  GLCP::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bbWireIndices), (void*)bbWireIndices, GL_STATIC_DRAW);

  GLCP::EnableVertexAttribArray(mVtxAttrib);
  GLCP::VertexAttribPointer(mVtxAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  GLCP::BindVertexArray(0);


  GLCP::GenBuffers(1, &mSoleVertexBuffer);
  GLCP::BindBuffer(GL_ARRAY_BUFFER, mSoleVertexBuffer);
  GLCP::BufferData(GL_ARRAY_BUFFER, sizeof(sole), (void*)sole, GL_STATIC_DRAW);

  GLCP::GenBuffers(1, &mHeelVertexBuffer);
  GLCP::BindBuffer(GL_ARRAY_BUFFER, mHeelVertexBuffer);
  GLCP::BufferData(GL_ARRAY_BUFFER, sizeof(heel), (void*)heel, GL_STATIC_DRAW);

  GLCP::GenBuffers(1, &mSoleWireIndexBuffer);
  GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSoleWireIndexBuffer);
  GLCP::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(soleWireIndices), (void*)soleWireIndices, GL_STATIC_DRAW);

  GLCP::GenBuffers(1, &mHeelWireIndexBuffer);
  GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHeelWireIndexBuffer);
  GLCP::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(heelWireIndices), (void*)heelWireIndices, GL_STATIC_DRAW);

  GLCP::GenBuffers(1, &mSoleShadedIndexBuffer);
  GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSoleShadedIndexBuffer);
  GLCP::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(soleShadedIndices), (void*)soleShadedIndices, GL_STATIC_DRAW);

  GLCP::GenBuffers(1, &mHeelShadedIndexBuffer);
  GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHeelShadedIndexBuffer);
  GLCP::BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(heelShadedIndices), (void*)heelShadedIndices, GL_STATIC_DRAW);

  GLCP::UseProgram(mShaderProgram);

  // Setup ALL VAO Combinations here for further usage:
  GLCP::GenVertexArrays(1, &mSoleWireframeVAO);
  GLCP::BindVertexArray(mSoleWireframeVAO);
  GLCP::BindBuffer(GL_ARRAY_BUFFER, mSoleVertexBuffer);
  GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSoleWireIndexBuffer);
  GLCP::EnableVertexAttribArray(mVtxAttrib);
  GLCP::VertexAttribPointer(mVtxAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  GLCP::BindVertexArray(0);

  GLCP::GenVertexArrays(1, &mHeelWireframeVAO);
  GLCP::BindVertexArray(mHeelWireframeVAO);
  GLCP::BindBuffer(GL_ARRAY_BUFFER, mHeelVertexBuffer);
  GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHeelWireIndexBuffer);
  GLCP::EnableVertexAttribArray(mVtxAttrib);
  GLCP::VertexAttribPointer(mVtxAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  GLCP::BindVertexArray(0);

  GLCP::GenVertexArrays(1, &mSoleShadedVAO);
  GLCP::BindVertexArray(mSoleShadedVAO);
  GLCP::BindBuffer(GL_ARRAY_BUFFER, mSoleVertexBuffer);
  GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mSoleShadedIndexBuffer);
  GLCP::EnableVertexAttribArray(mVtxAttrib);
  GLCP::VertexAttribPointer(mVtxAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  GLCP::BindVertexArray(0);

  GLCP::GenVertexArrays(1, &mHeelShadedVAO);
  GLCP::BindVertexArray(mHeelShadedVAO);
  GLCP::BindBuffer(GL_ARRAY_BUFFER, mHeelVertexBuffer);
  GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, mHeelShadedIndexBuffer);
  GLCP::EnableVertexAttribArray(mVtxAttrib);
  GLCP::VertexAttribPointer(mVtxAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
  GLCP::BindVertexArray(0);

  GLCP::UseProgram(0);
  GLCP::BindBuffer(GL_ARRAY_BUFFER, 0);
  GLCP::BindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  return true;
}


bool RawFootPrintDrawAgentCoreProfile::releaseCoreProfileResources() {
  if (mShaderProgram) {
    GLCP::DeleteProgram(mShaderProgram);
    mShaderProgram = 0;
  }

  if (mBBoxVAO) {
    GLCP::DeleteVertexArrays(1, &mBBoxVAO);
    mBBoxVAO = 0;
  }

  if (mSoleWireframeVAO) {
    GLCP::DeleteVertexArrays(1, &mSoleWireframeVAO);
    mSoleWireframeVAO = 0;
  }

  if (mHeelWireframeVAO) {
    GLCP::DeleteVertexArrays(1, &mHeelWireframeVAO);
    mHeelWireframeVAO = 0;
  }

  if (mSoleShadedVAO) {
    GLCP::DeleteVertexArrays(1, &mSoleShadedVAO);
    mSoleShadedVAO = 0;
  }

  if (mHeelShadedVAO) {
    GLCP::DeleteVertexArrays(1, &mHeelShadedVAO);
    mHeelShadedVAO = 0;
  }

  if (mBoundingboxVertexBuffer) {
    GLCP::DeleteBuffers(1, &mBoundingboxVertexBuffer);
    mBoundingboxVertexBuffer = 0;
  }

  if (mBoundingboxIndexBuffer) {
    GLCP::DeleteBuffers(1, &mBoundingboxIndexBuffer);
    mBoundingboxIndexBuffer = 0;
  }

  if (mSoleVertexBuffer) {
    GLCP::DeleteBuffers(1, &mSoleVertexBuffer);
    mSoleVertexBuffer = 0;
  }

  if (mHeelVertexBuffer) {
    GLCP::DeleteBuffers(1, &mHeelVertexBuffer);
    mHeelVertexBuffer = 0;
  }

  if (mSoleWireIndexBuffer) {
    GLCP::DeleteBuffers(1, &mSoleWireIndexBuffer);
    mSoleWireIndexBuffer = 0;
  }

  if (mSoleShadedIndexBuffer) {
    GLCP::DeleteBuffers(1, &mSoleShadedIndexBuffer);
    mSoleShadedIndexBuffer = 0;
  }

  if (mHeelWireIndexBuffer) {
    GLCP::DeleteBuffers(1, &mHeelWireIndexBuffer);
    mHeelWireIndexBuffer = 0;
  }

  if (mHeelShadedIndexBuffer) {
    GLCP::DeleteBuffers(1, &mHeelShadedIndexBuffer);
    mHeelShadedIndexBuffer = 0;
  }

  return true;
}

