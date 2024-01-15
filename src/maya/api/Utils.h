// Copyright (c) 2018-2021 Serguei Kalentchouk et al. All rights reserved.
// Modified from :- git@github.com:serguei-k/maya-math-nodes.git, Serguei Kalentchouk.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "MathUtility.h"

// System Includes
#include <algorithm>
#include <cmath>
#include <limits>
#include <string>
#include <type_traits>
#include <vector>

// Maya General Includes
#include <maya/MAngle.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MEulerRotation.h>
#include <maya/MGlobal.h>
#include <maya/MMatrix.h>
#include <maya/MVector.h>
#include <maya/MQuaternion.h>

// Function Sets
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
// #include <maya/MFnPlugin.h>

// Iterators

// Proxies
#include <maya/MPxNode.h>



#ifdef __APPLE__
	#define TEMPLATE_PARAMETER_LINKAGE constexpr
#else
	#define TEMPLATE_PARAMETER_LINKAGE extern const
#endif

struct Attribute
{
	MObject attr;
	MObject attrX;
	MObject attrY;
	MObject attrZ;
	MObject attrW;
	
	operator const MObject&() const {return attr;}
	
	Attribute& operator=(const MObject& object)
	{
		this->attr = object;
		return *this;
	}
};

// Default value templates
// Note that complex types are always defaulted to zero/identity
template <typename TType>
inline TType DefaultValue()
{
	static TType defaultT;
	return defaultT;
}

template <>
inline MMatrix DefaultValue()
{
	return MMatrix::identity;
}

template <>
inline MEulerRotation DefaultValue()
{
	return MEulerRotation::identity;
}

template <>
inline MQuaternion DefaultValue()
{
	return MQuaternion::identity;
}

template <typename TType>
inline TType DefaultValue(double x, double y, double z);

template <>
inline MVector DefaultValue(double x, double y, double z)
{
	return MVector(x, y, z);
}

template <>
inline MEulerRotation DefaultValue(double x, double y, double z)
{
	return MEulerRotation(x, y, z);
}


// Math helper functions
template <typename TType>
inline bool almostEquals(TType a, TType b)
{
	return std::abs(a - b) <= std::numeric_limits<TType>::epsilon() * std::max(TType(1.0), std::abs(a + b));
}

template <>
inline bool almostEquals(int a, int b)
{
	return a == b;
}

template <>
inline bool almostEquals(MAngle a, MAngle b)
{
	return std::abs(a.asRadians() - b.asRadians()) <= std::numeric_limits<double>::epsilon() *
			 std::max(1.0, std::abs(a.asRadians() + b.asRadians()));
}


template <typename T>
inline T psign(T value)
{
	return (0.0 <= value) - (value < 0.0);
}


template <typename T>
inline T clamp(T inValue, T minValue, T maxValue)
{
	/* Clamp function. */
	return std::max(minValue, std::min(inValue, maxValue));
}


template <typename T>
inline T sqr(T value) 
{
	/* Square function. */
	return value * value;
}

template <typename T>
inline T radians(T degrees)
{
	/* Converts degrees to radians function. */
	return degrees * PI / 180;
}


template <typename T>
inline T getAngle(T valueA, T valueB, T valueC)
{
	return std::acos(clamp((sqr(valueA) + sqr(valueB) - sqr(valueC)) / (2.0 * valueA * valueB), -1.0, 1.0));
}


//template <typename T>
inline double makeNonZero(double value, double multiplier=KINDA_SMALL_NUMBER)
{
	return std::max(std::abs(value), multiplier) * psign(value);
}

//template <typename T>
inline MVector makeNonZero(MVector value, double multiplier=KINDA_SMALL_NUMBER)
{
	return MVector(
		std::max(std::abs(value.x), multiplier) * psign(value.x),
		std::max(std::abs(value.y), multiplier) * psign(value.y),
		std::max(std::abs(value.z), multiplier) * psign(value.z)
	);
}
// template <typename T>
// MVector Lerp(MVector a, MVector b, T w)
// {
// 	return a + w * (b - a);
// }


// Overloads for createAttribute
inline void createAttribute(Attribute& attr, const char* name, double value, bool isInput=true, bool isArray=false)
{
	MFnNumericAttribute attrFn;
	attr.attr = attrFn.create(name, name, MFnNumericData::kDouble, value);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	attrFn.setArray(isArray);
	attrFn.setUsesArrayDataBuilder(isArray);
}

inline void createAttribute(Attribute& attr, const char* name, int value, bool isInput = true, bool isArray = false)
{
	MFnNumericAttribute attrFn;
	attr.attr = attrFn.create(name, name, MFnNumericData::kInt, value);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	attrFn.setArray(isArray);
	attrFn.setUsesArrayDataBuilder(isArray);
}

inline void createAttribute(Attribute& attr, const char* name, bool value, bool isInput = true, bool isArray = false)
{
	MFnNumericAttribute attrFn;
	attr.attr = attrFn.create(name, name, MFnNumericData::kBoolean, value);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	attrFn.setArray(isArray);
	attrFn.setUsesArrayDataBuilder(isArray);
}

inline void createAttribute(Attribute& attr, const char* name, const MAngle& value, bool isInput = true, bool isArray = false)
{
	MFnUnitAttribute attrFn;
	attr.attr = attrFn.create(name, name, value);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	attrFn.setArray(isArray);
	attrFn.setUsesArrayDataBuilder(isArray);
}

inline void createAttribute(Attribute& attr, const char* name, const MVector& value, bool isInput = true, bool isArray = false)
{
	MFnNumericAttribute attrFn;
	
	const std::string attrXName = (std::string(name) + "X");
	attr.attrX = attrFn.create(attrXName.c_str(), attrXName.c_str(), MFnNumericData::kDouble, value.x);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	
	const std::string attrYName = (std::string(name) + "Y");
	attr.attrY = attrFn.create(attrYName.c_str(), attrYName.c_str(), MFnNumericData::kDouble, value.y);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	
	const std::string attrZName = (std::string(name) + "Z");
	attr.attrZ = attrFn.create(attrZName.c_str(), attrZName.c_str(), MFnNumericData::kDouble, value.z);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	
	attr.attr = attrFn.create(name, name, attr.attrX, attr.attrY, attr.attrZ);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	attrFn.setArray(isArray);
	attrFn.setUsesArrayDataBuilder(isArray);
}

inline void createAttribute(Attribute& attr, const char* name, const MPoint& value, bool isInput = true, bool isArray = false)
{
	createAttribute(attr, name, MVector(value), isInput, isArray);
}

inline void createAttribute(Attribute& attr, const char* name, const MMatrix& value, bool isInput = true, bool isArray = false)
{
	MFnMatrixAttribute attrFn;
	attr.attr = attrFn.create(name, name);
	attrFn.setDefault(value);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	attrFn.setArray(isArray);
	attrFn.setUsesArrayDataBuilder(isArray);
}

inline void createAttribute(Attribute& attr, const char* name, const MQuaternion& value, bool isInput = true, bool isArray = false)
{
	MFnCompoundAttribute cAttrFn;
	MFnNumericAttribute attrFn;
	
	const std::string attrXName = (std::string(name) + "X");
	attr.attrX = attrFn.create(attrXName.c_str(), attrXName.c_str(), MFnNumericData::kDouble, value.x);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	
	const std::string attrYName = (std::string(name) + "Y");
	attr.attrY = attrFn.create(attrYName.c_str(), attrYName.c_str(), MFnNumericData::kDouble, value.y);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	
	const std::string attrZName = (std::string(name) + "Z");
	attr.attrZ = attrFn.create(attrZName.c_str(), attrZName.c_str(), MFnNumericData::kDouble, value.z);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	
	const std::string attrWName = (std::string(name) + "W");
	attr.attrW = attrFn.create(attrWName.c_str(), attrWName.c_str(), MFnNumericData::kDouble, value.w);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	
	attr.attr = cAttrFn.create(name, name);
	cAttrFn.addChild(attr.attrX);
	cAttrFn.addChild(attr.attrY);
	cAttrFn.addChild(attr.attrZ);
	cAttrFn.addChild(attr.attrW);
	cAttrFn.setKeyable(isInput);
	cAttrFn.setStorable(isInput);
	cAttrFn.setWritable(isInput);
	cAttrFn.setArray(isArray);
	cAttrFn.setUsesArrayDataBuilder(isArray);
}

inline void createAttribute(Attribute& attr, const char* name, const MEulerRotation& value, bool isInput = true, bool isArray = false)
{
	MFnNumericAttribute attrFn;
	MFnUnitAttribute uAttrFn;
	
	const std::string attrXName = (std::string(name) + "X");
	attr.attrX = uAttrFn.create(attrXName.c_str(), attrXName.c_str(), MAngle(value.x));
	uAttrFn.setKeyable(isInput);
	uAttrFn.setStorable(isInput);
	uAttrFn.setWritable(isInput);
	
	const std::string attrYName = (std::string(name) + "Y");
	attr.attrY = uAttrFn.create(attrYName.c_str(), attrYName.c_str(), MAngle(value.y));
	uAttrFn.setKeyable(isInput);
	uAttrFn.setStorable(isInput);
	uAttrFn.setWritable(isInput);
	
	const std::string attrZName = (std::string(name) + "Z");
	attr.attrZ = uAttrFn.create(attrZName.c_str(), attrZName.c_str(), MAngle(value.z));
	uAttrFn.setKeyable(isInput);
	uAttrFn.setStorable(isInput);
	uAttrFn.setWritable(isInput);
	
	attr.attr = attrFn.create(name, name, attr.attrX, attr.attrY, attr.attrZ);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	attrFn.setArray(isArray);
	attrFn.setUsesArrayDataBuilder(isArray);
}

inline void createAttribute(Attribute& attr, const char* name, MFnData::Type type, bool isInput = true, bool isArray = false)
{
	MFnTypedAttribute attrFn;
	attr.attr = attrFn.create(name, name, type);
	attrFn.setKeyable(isInput);
	attrFn.setStorable(isInput);
	attrFn.setWritable(isInput);
	attrFn.setArray(isArray);
	attrFn.setUsesArrayDataBuilder(isArray);
}

inline void createCompoundAttribute(Attribute& attr, const std::vector<Attribute>& children, const char* name, bool isInput = true, bool isArray = false)
{
	MFnCompoundAttribute cAttrFn;
	attr.attr = cAttrFn.create(name, name);
	
	for (const Attribute& child : children)
	{
		cAttrFn.addChild(child);
	}
	
	cAttrFn.setKeyable(isInput);
	cAttrFn.setStorable(isInput);
	cAttrFn.setWritable(isInput);
	cAttrFn.setArray(isArray);
	cAttrFn.setUsesArrayDataBuilder(isArray);
}

inline void createRotationOrderAttribute(Attribute& attr)
{
	MFnEnumAttribute eAttrFn;
	attr.attr = eAttrFn.create("rotationOrder", "rotationOrder", 0);
	
	eAttrFn.addField("xyz", 0);
	eAttrFn.addField("yzx", 1);
	eAttrFn.addField("zxy", 2);
	eAttrFn.addField("xzy", 3);
	eAttrFn.addField("yxz", 4);
	eAttrFn.addField("zyx", 5);
	
	eAttrFn.setStorable(true);
	eAttrFn.setWritable(true);
	eAttrFn.setChannelBox(true);
}

inline void createAxisAttribute(Attribute& attr, const char* name, int value=0)
{
	MFnEnumAttribute eAttrFn;
	attr.attr = eAttrFn.create(name, name, value);
	
	eAttrFn.addField("x", 0);
	eAttrFn.addField("y", 1);
	eAttrFn.addField("z", 2);
	
	eAttrFn.setStorable(true);
	eAttrFn.setWritable(true);
	eAttrFn.setChannelBox(true);
}

inline MVector getVectorFromAxisAttribute(short index)
{
	if (index == 0)
	{
		return MVector::xAxis;
	}
	else if (index == 1)
	{
		return MVector::yAxis;
	}
	else if (index == 2)
	{
		return MVector::zAxis;
	}
	else
	{
		return MVector::zero;
	}
}


// Template specializations for getAttribute
template <typename TType>
inline TType getAttribute(MDataBlock& dataBlock, const Attribute& attribute);

template <typename TType>
inline TType getAttribute(MDataBlock& dataBlock, const Attribute& attribute, const Attribute& childAttribute);

template <>
inline double getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	MDataHandle handle = dataBlock.inputValue(attribute);
	return handle.asDouble();
}

template <>
inline std::vector<double> getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	std::vector<double> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		out[index] = arrayHandle.inputValue().asDouble();
		arrayHandle.next();
	}
	
	return out;
}

template <>
inline std::vector<double> getAttribute(MDataBlock& dataBlock, const Attribute& attribute, const Attribute& childAttribute)
{
	std::vector<double> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		MDataHandle handle = arrayHandle.inputValue();
		out[index] = handle.child(childAttribute).asDouble();
		arrayHandle.next();
	}
	
	return out;
}

template <>
inline int getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	MDataHandle handle = dataBlock.inputValue(attribute);
	return handle.asInt();
}

template <>
inline std::vector<int> getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	std::vector<int> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		out[index] = arrayHandle.inputValue().asInt();
		arrayHandle.next();
	}
	
	return out;
}

template <>
inline std::vector<int> getAttribute(MDataBlock& dataBlock, const Attribute& attribute, const Attribute& childAttribute)
{
	std::vector<int> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		MDataHandle handle = arrayHandle.inputValue();
		out[index] = handle.child(childAttribute).asInt();
		arrayHandle.next();
	}
	
	return out;
}

template <>
inline bool getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	MDataHandle handle = dataBlock.inputValue(attribute);
	return handle.asBool();
}

template <>
inline MAngle getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	MDataHandle handle = dataBlock.inputValue(attribute);
	return handle.asAngle();
}

template <>
inline std::vector<MAngle> getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	std::vector<MAngle> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		out[index] = arrayHandle.inputValue().asAngle();
		arrayHandle.next();
	}
	
	return out;
}

template <>
inline std::vector<MAngle> getAttribute(MDataBlock& dataBlock, const Attribute& attribute, const Attribute& childAttribute)
{
	std::vector<MAngle> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		MDataHandle handle = arrayHandle.inputValue();
		out[index] = handle.child(childAttribute).asAngle();
		arrayHandle.next();
	}
	
	return out;
}

template <>
inline MVector getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	MDataHandle handle = dataBlock.inputValue(attribute);
	return handle.asVector();
}

template <>
inline MPoint getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	MDataHandle handle = dataBlock.inputValue(attribute);
	return MPoint(handle.asVector());
}

template <>
inline std::vector<MVector> getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	std::vector<MVector> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		out[index] = arrayHandle.inputValue().asVector();
		arrayHandle.next();
	}
	
	return out;
}

template <>
inline std::vector<MVector> getAttribute(MDataBlock& dataBlock, const Attribute& attribute, const Attribute& childAttribute)
{
	std::vector<MVector> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		MDataHandle handle = arrayHandle.inputValue();
		out[index] = handle.child(childAttribute).asVector();
		arrayHandle.next();
	}
	
	return out;
}

template <>
inline MMatrix getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	MDataHandle handle = dataBlock.inputValue(attribute);
	return handle.asMatrix();
}

template <>
inline std::vector<MMatrix> getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	std::vector<MMatrix> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		out[index] = arrayHandle.inputValue().asMatrix();
		arrayHandle.next();
	}
	
	return out;
}

template <>
inline std::vector<MMatrix> getAttribute(MDataBlock& dataBlock, const Attribute& attribute, const Attribute& childAttribute)
{
	std::vector<MMatrix> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		MDataHandle handle = arrayHandle.inputValue();
		out[index] = handle.child(childAttribute).asMatrix();
		arrayHandle.next();
	}
	
	return out;
}

template <>
inline MEulerRotation getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	MDataHandle handle = dataBlock.inputValue(attribute.attr);
	
	const double x = handle.child(attribute.attrX).asAngle().asRadians();
	const double y = handle.child(attribute.attrY).asAngle().asRadians();
	const double z = handle.child(attribute.attrZ).asAngle().asRadians();
	
	return MEulerRotation(x, y, z);
}

template <>
inline std::vector<MEulerRotation> getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	std::vector<MEulerRotation> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		MDataHandle handle = arrayHandle.inputValue();
		
		const double x = handle.child(attribute.attrX).asAngle().asRadians();
		const double y = handle.child(attribute.attrY).asAngle().asRadians();
		const double z = handle.child(attribute.attrZ).asAngle().asRadians();
		
		out[index] = MEulerRotation(x, y, z);
		arrayHandle.next();
	}
	
	return out;
}

template <>
inline std::vector<MEulerRotation> getAttribute(MDataBlock& dataBlock, const Attribute& attribute, const Attribute& childAttribute)
{
	std::vector<MEulerRotation> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		MDataHandle handle = arrayHandle.inputValue();
		MDataHandle childHandle = handle.child(childAttribute);
		
		const double x = childHandle.child(childAttribute.attrX).asAngle().asRadians();
		const double y = childHandle.child(childAttribute.attrY).asAngle().asRadians();
		const double z = childHandle.child(childAttribute.attrZ).asAngle().asRadians();
		
		out[index] = MEulerRotation(x, y, z);
		arrayHandle.next();
	}
	
	return out;
}

template <>
inline MQuaternion getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	MDataHandle handle = dataBlock.inputValue(attribute.attr);
	
	const double x = handle.child(attribute.attrX).asDouble();
	const double y = handle.child(attribute.attrY).asDouble();
	const double z = handle.child(attribute.attrZ).asDouble();
	const double w = handle.child(attribute.attrW).asDouble();
	
	return MQuaternion(x, y, z, w);
}

template <>
inline std::vector<MQuaternion> getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	std::vector<MQuaternion> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		MDataHandle handle = arrayHandle.inputValue();
		
		const double x = handle.child(attribute.attrX).asDouble();
		const double y = handle.child(attribute.attrY).asDouble();
		const double z = handle.child(attribute.attrZ).asDouble();
		const double w = handle.child(attribute.attrW).asDouble();
		
		out[index] = MQuaternion(x, y, z, w);
		arrayHandle.next();
	}
	
	return out;
}

template <>
inline std::vector<MQuaternion> getAttribute(MDataBlock& dataBlock, const Attribute& attribute, const Attribute& childAttribute)
{
	std::vector<MQuaternion> out;
	MArrayDataHandle arrayHandle = dataBlock.inputArrayValue(attribute);
	
	out.resize(arrayHandle.elementCount());
	for (unsigned index = 0u; index < arrayHandle.elementCount(); ++index)
	{
		MDataHandle handle = arrayHandle.inputValue();
		MDataHandle childHandle = handle.child(childAttribute);
		
		const double x = childHandle.child(childAttribute.attrX).asDouble();
		const double y = childHandle.child(childAttribute.attrY).asDouble();
		const double z = childHandle.child(childAttribute.attrZ).asDouble();
		const double w = childHandle.child(childAttribute.attrW).asDouble();
		
		out[index] = MQuaternion(x, y, z, w);
		arrayHandle.next();
	}
	
	return out;
}

inline MObject getAttribute(MDataBlock& dataBlock, const Attribute& attribute, MFnData::Type type)
{
	MDataHandle handle = dataBlock.inputValue(attribute);
	
	switch (type)
	{       
		case MFnData::kMesh:
			return handle.asMesh();
		case MFnData::kNurbsCurve:
			return handle.asNurbsCurve();
		case MFnData::kNurbsSurface:
			return handle.asNurbsSurface();
		default:
			return MObject();
	}
}

template <>
inline MTransformationMatrix::RotationOrder getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	MDataHandle handle = dataBlock.inputValue(attribute);
	return MTransformationMatrix::RotationOrder(handle.asShort() + 1);
}

template <>
inline MEulerRotation::RotationOrder getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
	MDataHandle handle = dataBlock.inputValue(attribute);
	return MEulerRotation::RotationOrder(handle.asShort());
}


// Template specializations for setAttribute
template <typename TType>
inline void setAttribute(MDataBlock& dataBlock, const Attribute& attribute, TType value)
{
	MDataHandle handle = dataBlock.outputValue(attribute);
	handle.set(value);
	handle.setClean();
}

template <>
inline void setAttribute(MDataBlock& dataBlock, const Attribute& attribute, MPoint value)
{
	MDataHandle handle = dataBlock.outputValue(attribute);
	handle.set(MVector(value));
	handle.setClean();
}

template <>
inline void setAttribute(MDataBlock& dataBlock, const Attribute& attribute, MEulerRotation value)
{
	MDataHandle outputXHandle = dataBlock.outputValue(attribute.attrX);
	outputXHandle.set(MAngle(value.x));
	outputXHandle.setClean();
	
	MDataHandle outputYHandle = dataBlock.outputValue(attribute.attrY);
	outputYHandle.set(MAngle(value.y));
	outputYHandle.setClean();
	
	MDataHandle outputZHandle = dataBlock.outputValue(attribute.attrZ);
	outputZHandle.set(MAngle(value.z));
	outputZHandle.setClean();
}

template <>
inline void setAttribute(MDataBlock& dataBlock, const Attribute& attribute, MQuaternion value)
{
	MDataHandle outputXHandle = dataBlock.outputValue(attribute.attrX);
	outputXHandle.set(value.x);
	outputXHandle.setClean();
	
	MDataHandle outputYHandle = dataBlock.outputValue(attribute.attrY);
	outputYHandle.set(value.y);
	outputYHandle.setClean();
	
	MDataHandle outputZHandle = dataBlock.outputValue(attribute.attrZ);
	outputZHandle.set(value.z);
	outputZHandle.setClean();
	
	MDataHandle outputWHandle = dataBlock.outputValue(attribute.attrW);
	outputWHandle.set(value.w);
	outputWHandle.setClean();
}

template <typename TType>
inline void setAttribute(MDataBlock& dataBlock, const Attribute& attribute, const std::vector<TType>& values)
{
	MArrayDataHandle handle = dataBlock.outputArrayValue(attribute);
	MArrayDataBuilder builder(&dataBlock, attribute, unsigned(values.size()));
	
	for (const auto& value : values)
	{
		MDataHandle itemHandle = builder.addLast();
		itemHandle.set(value);
	}
	
	handle.set(builder);
	handle.setAllClean();
}


template <typename ...Ts>
inline static bool shouldCompute(const MPlug& _plug, Ts&&... _attrs)
{
  bool compute = false;
  auto _={(compute = compute || (_plug == _attrs), 0)...};
  (void)_;
  return compute;
}

template <typename ...Ts>
inline static void addAttributes(Ts&&... _attrs)
{
  auto _={(MPxNode::addAttribute(_attrs), 0)...};
  (void)_;
}

template <typename ...Ts>
inline static void setAffects(const Attribute& _affector, Ts&&... _affected)
{
  auto _={(MPxNode::attributeAffects(_affector, _affected), 0)...};
  (void)_;
}

template <typename ...Ts>
inline static void setAffects(std::initializer_list<std::reference_wrapper<Attribute>> _affectors, Ts&&... _affected)
{
  for (const auto tor : _affectors)
  {
    setAffects(tor.get(), std::forward<Ts>(_affected)...);
  }
}


// Maya types operator overloads
// MAngle operator+(const MAngle& a, const MAngle& b)
// {
// 	return MAngle(a.asRadians() + b.asRadians());
// }

// MAngle operator-(const MAngle& a, const MAngle& b)
// {
// 	return MAngle(a.asRadians() - b.asRadians());
// }

// MAngle operator*(const MAngle& a, const MAngle& b)
// {
// 	return MAngle(a.asRadians() * b.asRadians());
// }

// MAngle operator*(const MAngle& a, double b)
// {
// 	return MAngle(a.asRadians() * b);
// }

// MAngle operator*(const MAngle& a, int b)
// {
// 	return MAngle(a.asRadians() * b);
// }

// MAngle operator/(const MAngle& a, double b)
// {
// 	return MAngle(a.asRadians() / b);
// }

// MAngle operator/(const MAngle& a, int b)
// {
// 	return MAngle(a.asRadians() / b);
// }

// MAngle operator/(const MAngle& a, const MAngle& b)
// {
// 	return MAngle(a.asRadians() / b.asRadians());
// }

// MAngle operator-(const MAngle& a)
// {
// 	return MAngle(-a.asRadians());
// }

// bool operator<(const MAngle& a, const MAngle& b)
// {
// 	return a.asRadians() < b.asRadians();
// }

// bool operator>(const MAngle& a, const MAngle& b)
// {
// 	return a.asRadians() > b.asRadians();
// }

// MQuaternion operator*(const MQuaternion& a, double b)
// {
// 	return MQuaternion(a.x * b, a.y * b, a.z * b, a.w * b);
// }


// Base node type definition used for all math nodes in this library
// template<typename TClass, const char* TTypeName>
// class BaseNode : public MPxNode
// {
// public:
// 	static void registerNode(class MFnPlugin& pluginFn, int typeId)
// 	{
// 		kTypeId = typeId;
// 		pluginFn.registerNode(
// 			(std::string(NODE_NAME_PREFIX) + TTypeName).c_str(),
// 			typeId,
// 			[]() -> void* { return new TClass(); },
// 			TClass::initialize
// 		);
// 	}
	
// 	static void deregisterNode(class MFnPlugin& pluginFn)
// 	{
// 		pluginFn.deregisterNode(kTypeId);
// 	}
	
// 	SchedulingType schedulingType() const override
// 	{
// 		return SchedulingType::kParallel;
// 	}

// protected:
// 	static int kTypeId;
// };

// template<typename TClass, const char* TTypeName>
// int BaseNode<TClass, TTypeName>::kTypeId = -1;

template <typename T>
MVector Lerp(MVector a, MVector b, T w)
{
	return a + w * (b - a);
}




// MStatus LockAndHideAttribute(MPlug& plug)
// {
// 	/* Locks and hides the given plug from the channelbox.

// 	Returns:
// 		status code (MStatus): kSuccess if the command was successful, kFailure if an error occured
// 			during the command.

// 	*/
// 	// MStatus status;

// 	plug.setLocked(true);
// 	plug.setKeyable(false);
// 	plug.setChannelBox(false);

// 	return MS::kSuccess;
// }
