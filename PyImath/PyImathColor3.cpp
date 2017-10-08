///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 1998-2011, Industrial Light & Magic, a division of Lucas
// Digital Ltd. LLC
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission. 
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

//
// This .C file was turned into a header file so that instantiations
// of the various V3* types can be spread across multiple files in
// order to work around MSVC limitations.
//

#include "python_include.h"
#include <PyImathColor.h>
#include <PyImathVec.h>
#include "PyImathDecorators.h"
#include "PyImathExport.h"
#include <boost/format.hpp>
#include <PyImath.h>
#include <PyImathMathExc.h>
#include <ImathVec.h>
#include <ImathColor.h>
#include <ImathColorAlgo.h>
#include <Iex.h>
#include "PyImathColor3ArrayImpl.h"

namespace PyImath {
template <> const char *PyImath::C3cArray::name() { return "C3cArray"; }
template <> const char *PyImath::C3fArray::name() { return "C3fArray"; }

 
using namespace IMATH_NAMESPACE;

template <class T> struct Color3Name { static const char *value; };
template<> const char *Color3Name<unsigned char>::value  = "Color3c";
template<> const char *Color3Name<float>::value  = "Color3f";

// create a new default constructor that initializes Color3<T> to zero.
template <class T>
static Color3<T> * Color3_construct_default()
{
    return new Color3<T>(T(0),T(0),T(0));
}

template <class T, class S>
static Color3<T> * Color3_component_construct1(S x, S y, S z)
{
    // Assigning a floating point value to an integer type can cause a
    // float-point error, which we want to translate into an exception. 
    
    MATH_EXC_ON;

    if(strcmp(Color3Name<T>::value, "Color3c") == 0)
    {
        unsigned char r = (unsigned char) x;
        unsigned char g = (unsigned char) y;
        unsigned char b = (unsigned char) z;
        
        return new Color3<T>(r,g,b);
    }
    else    
        return new Color3<T>(T (x), T(y), T(z));
}

template <class T, class S>
static Color3<T> * Color3_component_construct2(S x)
{
    MATH_EXC_ON;

    if(strcmp(Color3Name<T>::value, "Color3c") == 0)
    {
        unsigned char u = (unsigned char) x;

        return new Color3<T>(u,u,u);
    }
    else    
        return new Color3<T>(T(x), T(x), T(x));
}

template <class T, class S>
static Color3<T> * Color3_color_construct(const Color3<S> &c)
{
    MATH_EXC_ON;

    if(strcmp(Color3Name<T>::value, "Color3c") == 0)
    {
        unsigned char r = (unsigned char) c.x;
        unsigned char g = (unsigned char) c.y;
        unsigned char b = (unsigned char) c.z;
        
        return new Color3<T>(r,g,b);
    }
    else
     return new Color3<T>(T (c.x), T(c.y), T(c.z));
}

template <class T, class S>
static Color3<T> * Color3_vector_construct(const Vec3<S> &c)
{
    MATH_EXC_ON;

    if(strcmp(Color3Name<T>::value, "Color3c") == 0)
    {
        unsigned char r = (unsigned char) c.x;
        unsigned char g = (unsigned char) c.y;
        unsigned char b = (unsigned char) c.z;
        
        return new Color3<T>(r,g,b);
    }
    else
     return new Color3<T>(T (c.x), T(c.y), T(c.z));
}


template <class T>
static std::string 
color3_str(const Color3<T> &v)
{
    std::stringstream stream;
    if(strcmp(Color3Name<T>::value, "Color3c") == 0)
    {
        int r = int(v.x);
        int g = int(v.y);
        int b = int(v.z);

        stream << Color3Name<T>::value << "(" << r << ", " << g << ", " << b << ")";
        return stream.str();
    }
    else
    {    
        stream << Color3Name<T>::value << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return stream.str();
    }
}

// Non-specialized repr is same as str
template <class T>
static std::string 
color3_repr(const Color3<T> &v)
{
    return color3_str(v);
}

// Specialization for float to full precision
template <>
std::string 
color3_repr(const Color3<float> &v)
{
    return (boost::format("%s(%.9g, %.9g, %.9g)")
                        % Color3Name<float>::value % v.x % v.y % v.z).str();
}

// No specialization for double, since we don't instantiate Color3d


template <class T>
static Color3<T> * Color3_construct_tuple(const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 3)
    {
        return new Color3<T>(py::cast<T>(t[0]), py::cast<T>(t[1]), py::cast<T>(t[2]));
    }
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color3 expects tuple of length 3");
}

template <class T>
static Color3<T> * Color3_construct_list(const py::list &l)
{
    MATH_EXC_ON;
    if(py::cast<int>(l.attr("__len__")()) == 3)
    {
        return new Color3<T>(py::cast<T>(l[0]), py::cast<T>(l[1]), py::cast<T>(l[2]));
    }
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color3 expects list of length 3");
}

template <class T>
static const Color3<T> &
iadd(Color3<T> &color, const Color3<T> &color2)
{
    MATH_EXC_ON;
    return color += color2;
}

template <class T>
static Color3<T>
add(Color3<T> &color, const Color3<T> &color2)
{
    MATH_EXC_ON;
    return color + color2;
}

template <class T>
static Color3<T>
addTuple(Color3<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 3)
        return Color3<T>(color.x + py::cast<T>(t[0]), 
                         color.y + py::cast<T>(t[1]), 
                         color.z + py::cast<T>(t[2]));
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color3 expects tuple of length 3");
}

template <class T>
static Color3<T>
addT(Color3<T> &v, T a)
{
    MATH_EXC_ON;
    Color3<T> w(v.x + a, v.y + a, v.z + a);
    return w;
}

template <class T>
static const Color3<T> &
isub(Color3<T> &color, const Color3<T> &color2)
{
    MATH_EXC_ON;
    return color -= color2;
}

template <class T>
static Color3<T>
sub(Color3<T> &color, const Color3<T> &color2)
{
    MATH_EXC_ON;
    return color - color2;
}

template <class T>
static Color3<T>
subtractL(Color3<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 3)
        return Color3<T>(color.x - py::cast<T>(t[0]), 
                         color.y - py::cast<T>(t[1]), 
                         color.z - py::cast<T>(t[2]));
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color3 expects tuple of length 3");
}

template <class T>
static Color3<T>
subtractLT(const Color3<T> &color, T a)
{
    MATH_EXC_ON;
    return Color3<T>(color.x - a, 
                     color.y - a, 
                     color.z - a);
}

template <class T>
static Color3<T>
subtractRT(const Color3<T> &color, T a)
{
    MATH_EXC_ON;
    return Color3<T>(a - color.x, 
                     a - color.y, 
                     a - color.z);
}

template <class T>
static Color3<T>
subtractR(Color3<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 3)
        return Color3<T>(py::cast<T>(t[0]) - color.x, 
                         py::cast<T>(t[1]) - color.y, 
                         py::cast<T>(t[2]) - color.z);
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color3 expects tuple of length 3");
}

template <class T>
static Color3<T>
neg(Color3<T> &color)
{
    MATH_EXC_ON;
    return -color;
}

template <class T>
static const Color3<T> &
negate(Color3<T> &color)
{
    MATH_EXC_ON;
    return color.negate();
}

template <class T>
static const Color3<T> &
imul(Color3<T> &color, const Color3<T> &color2)
{
    MATH_EXC_ON;
    return color *= color2;
}

template <class T>
static const Color3<T> &
imulT(Color3<T> &color, const T &t)
{
    MATH_EXC_ON;
    return color *= t;
}

template <class T>
static Color3<T>
mul(Color3<T> &color, const Color3<T> &color2)
{
    MATH_EXC_ON;
    return color * color2;
}

template <class T>
static Color3<T>
mulT(Color3<T> &color, const T &t)
{
    MATH_EXC_ON;
    return color * t;
}

template <class T>
static Color3<T>
rmulT(Color3<T> &color, const T &t)
{
    MATH_EXC_ON;
    return t * color;
}

template <class T>
static Color3<T>
mulTuple(Color3<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 3)
        return Color3<T>(color.x * py::cast<T>(t[0]), 
                         color.y * py::cast<T>(t[1]), 
                         color.z * py::cast<T>(t[2]));
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color3 expects tuple of length 3");
}

template <class T>
static const Color3<T> &
idiv(Color3<T> &color, const Color3<T> &color2)
{
    MATH_EXC_ON;
    return color /= color2;
}

template <class T>
static const Color3<T> &
idivT(Color3<T> &color, const T &t)
{
    MATH_EXC_ON;
    return color /= t;
}

template <class T>
static Color3<T>
div(Color3<T> &color, const Color3<T> &color2)
{
    MATH_EXC_ON;
    return color / color2;
}

template <class T>
static Color3<T>
divT(Color3<T> &color, const T &t)
{
    MATH_EXC_ON;
    return color / t;
}

template <class T>
static Color3<T>
divTupleL(Color3<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 3)
        return Color3<T>(color.x / py::cast<T>(t[0]), 
                         color.y / py::cast<T>(t[1]), 
                         color.z / py::cast<T>(t[2]));
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color3 expects tuple of length 3");    
}

template <class T>
static Color3<T>
divTupleR(Color3<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 3)
        return Color3<T>(py::cast<T>(t[0]) / color.x, 
                         py::cast<T>(t[1]) / color.y, 
                         py::cast<T>(t[2]) / color.z);
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color3 expects tuple of length 3");    
}

template <class T>
static Color3<T>
divTR(Color3<T> &color, T a)
{
    MATH_EXC_ON;
    return Color3<T>(a / color.x, 
                     a / color.y, 
                     a / color.z);
}

template <class T>
static Color3<T>
hsv2rgb(Color3<T> &color)
{    
    MATH_EXC_ON;
    return IMATH_NAMESPACE::hsv2rgb(color);
}

template <class T>
static Color3<T>
hsv2rgbTuple(const py::tuple &t)
{
    MATH_EXC_ON;
    Color3<T> color;
    if(t.attr("__len__")() == 3)
    {
        color.x = py::cast<T>(t[0]);
        color.y = py::cast<T>(t[1]);
        color.z = py::cast<T>(t[2]); 
        
        return IMATH_NAMESPACE::hsv2rgb(color);
    }
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color3 expects tuple of length 3");    
}

template <class T>
static Color3<T>
rgb2hsv(Color3<T> &color)
{    
    MATH_EXC_ON;
    return IMATH_NAMESPACE::rgb2hsv(color);
}

template <class T>
static Color3<T>
rgb2hsvTuple(const py::tuple &t)
{
    MATH_EXC_ON;
    Color3<T> color;
    if(py::cast<int>(t.attr("__len__")()) == 3)
    {
        color.x = py::cast<T>(t[0]);
        color.y = py::cast<T>(t[1]);
        color.z = py::cast<T>(t[2]); 
        
        return IMATH_NAMESPACE::rgb2hsv(color);
    }
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color3 expects tuple of length 3");    
}

template <class T>
static void
setValue1(Color3<T> &color, const T &a, const T &b, const T &c)
{
    MATH_EXC_ON;
    color.setValue(a, b, c);
}

template <class T>
static void
setValue2(Color3<T> &color, const Color3<T> &v)
{
    MATH_EXC_ON;
    color.setValue(v);
}

template <class T>
static void
setValueTuple(Color3<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    Color3<T> v;
    if(py::cast<int>(t.attr("__len__")()) == 3)
    {
        v.x = py::cast<T>(t[0]);
        v.y = py::cast<T>(t[1]);
        v.z = py::cast<T>(t[2]); 
        
        color.setValue(v);
    }
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color3 expects tuple of length 3");
}

template <class T>
static bool
lessThan(Color3<T> &v, const Color3<T> &w)
{
    bool isLessThan = (v.x <= w.x && v.y <= w.y && v.z <= w.z)
                    && v != w;
                   
   return isLessThan;
}

template <class T>
static bool
greaterThan(Color3<T> &v, const Color3<T> &w)
{
    bool isGreaterThan = (v.x >= w.x && v.y >= w.y && v.z >= w.z)
                       && v != w;
                   
   return isGreaterThan;
}

template <class T>
static bool
lessThanEqual(Color3<T> &v, const Color3<T> &w)
{
    bool isLessThanEqual = (v.x <= w.x && v.y <= w.y && v.z <= w.z);

    return isLessThanEqual;
}

template <class T>
static bool
greaterThanEqual(Color3<T> &v, const Color3<T> &w)
{
    bool isGreaterThanEqual = (v.x >= w.x && v.y >= w.y && v.z >= w.z);

    return isGreaterThanEqual;
}

template <class T>
py::class_<Color3<T>, Vec3<T> >
register_Color3(py::module &m)
{
    py::class_<Color3<T>, Vec3<T> > color3_class(m, Color3Name<T>::value, Color3Name<T>::value);
    color3_class
        .def(py::init<Color3<T> >(/*"copy construction"*/))
        .def(py::init(&Color3_construct_default<T>),"initialize to (0,0,0)")
        .def(py::init(&Color3_construct_tuple<T>), "initialize to (r,g,b) with a python tuple")
        .def(py::init(&Color3_construct_list<T>), "initialize to (r,g,b) with a python list")
        .def(py::init(&Color3_component_construct1<T,float>))
        .def(py::init(&Color3_component_construct1<T,int>))
        .def(py::init(&Color3_component_construct2<T,float>))
        .def(py::init(&Color3_component_construct2<T,int>))
        .def(py::init(&Color3_color_construct<T,float>))
        .def(py::init(&Color3_color_construct<T,int>))
        .def(py::init(&Color3_color_construct<T,unsigned char>))
        .def(py::init(&Color3_vector_construct<T,float>))
        .def(py::init(&Color3_vector_construct<T,double>))
        .def(py::init(&Color3_vector_construct<T,int>))

        .def_readwrite("r", &Color3<T>::x)
        .def_readwrite("g", &Color3<T>::y)
        .def_readwrite("b", &Color3<T>::z)
        .def("__str__", &color3_str<T>)
        .def("__repr__", &color3_repr<T>)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("__iadd__", &iadd<T>, py::return_value_policy::reference_internal)
        .def("__add__", &add<T>)
        .def("__add__", &addTuple<T>)
        .def("__add__", &addT<T>)
        .def("__radd__", &addTuple<T>)
        .def("__radd__", &addT<T>)
        .def("__isub__", &isub<T>, py::return_value_policy::reference_internal)
        .def("__sub__", &sub<T>)
        .def("__sub__", &subtractL<T>)
        .def("__sub__", &subtractLT<T>)
        .def("__rsub__", &subtractR<T>)
        .def("__rsub__", &subtractRT<T>)
        .def("__neg__", &neg<T>)
        .def("negate",&negate<T>, py::return_value_policy::reference_internal, "component-wise multiplication by -1")
        .def("__imul__", &imul<T>, py::return_value_policy::reference_internal)
        .def("__imul__", &imulT<T>, py::return_value_policy::reference_internal)
        .def("__mul__", &mul<T>)
        .def("__mul__", &mulT<T>)
        .def("__rmul__", &rmulT<T>)
        .def("__mul__", &mulTuple<T>)
        .def("__rmul__", &mulTuple<T>)
        .def("__idiv__", &idiv<T>, py::return_value_policy::reference_internal)
        .def("__idiv__", &idivT<T>, py::return_value_policy::reference_internal)
        .def("__itruediv__", &idiv<T>, py::return_value_policy::reference_internal)
        .def("__itruediv__", &idivT<T>, py::return_value_policy::reference_internal)
        .def("__div__", &div<T>)
        .def("__div__", &divT<T>)
        .def("__div__", &divTupleL<T>)
        .def("__truediv__", &div<T>)
        .def("__truediv__", &divT<T>)
        .def("__truediv__", &divTupleL<T>)
        .def("__rdiv__", &divTupleR<T>)  
        .def("__rdiv__", &divTR<T>)
        .def("__lt__", &lessThan<T>)
        .def("__gt__", &greaterThan<T>)
        .def("__le__", &lessThanEqual<T>)
        .def("__ge__", &greaterThanEqual<T>)
        .def_static("dimensions", &Color3<T>::dimensions,"dimensions() number of dimensions in the color")
        .def_static("baseTypeEpsilon", &Color3<T>::baseTypeEpsilon,"baseTypeEpsilon() epsilon value of the base type of the color")
        .def_static("baseTypeMax", &Color3<T>::baseTypeMax,"baseTypeMax() max value of the base type of the color")
        .def_static("baseTypeMin", &Color3<T>::baseTypeMin,"baseTypeMin() min value of the base type of the color")
        .def_static("baseTypeSmallest", &Color3<T>::baseTypeSmallest,"baseTypeSmallest() smallest value of the base type of the color")
        .def("hsv2rgb", &hsv2rgb<T>, 
    	     "C.hsv2rgb() -- returns a new color which "
             "is C converted from RGB to HSV")
         .def("hsv2rgb", &rgb2hsvTuple<T>)
         
         .def("rgb2hsv", &rgb2hsv<T>, 	 			
              "C.rgb2hsv() -- returns a new color which "
	 	      "is C converted from HSV to RGB")
         .def("rgb2hsv", &rgb2hsvTuple<T>)
         
         .def("setValue", &setValue1<T>, 	 			
              "C1.setValue(C2)\nC1.setValue(a,b,c) -- "
	 	      "set C1's  elements")         
         .def("setValue", &setValue2<T>)
         .def("setValue", &setValueTuple<T>)
        ;

    decoratecopy(color3_class);

    return color3_class;
}

template PYIMATH_EXPORT py::class_<Color3<float>, Vec3<float > > register_Color3<float>(py::module &m);
template PYIMATH_EXPORT py::class_<Color3<unsigned char>, Vec3<unsigned char> > register_Color3<unsigned char>(py::module &m);
template PYIMATH_EXPORT py::class_<FixedArray<Color3<float> > > register_Color3Array<float>(py::module &m);
template PYIMATH_EXPORT py::class_<FixedArray<Color3<unsigned char> > > register_Color3Array<unsigned char>(py::module &m);

template<> PYIMATH_EXPORT IMATH_NAMESPACE::Color3<float> PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Color3<float> >::value()
{ return IMATH_NAMESPACE::Color3<float>(0,0,0); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Color3<unsigned char> PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Color3<unsigned char> >::value()
{ return IMATH_NAMESPACE::Color3<unsigned char>(0,0,0); }

}
