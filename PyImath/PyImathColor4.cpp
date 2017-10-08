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

#include "python_include.h"
#include <PyImathColor.h>
#include "PyImathDecorators.h"
#include "PyImathExport.h"
#include <boost/format.hpp>
#include <PyImath.h>
#include <PyImathMathExc.h>
#include <ImathColor.h>
#include <ImathColorAlgo.h>
#include <Iex.h>
#include "PyImathColor4Array2DImpl.h"
#include "PyImathColor4ArrayImpl.h"

namespace PyImath {
template <> const char *PyImath::C4cArray::name() { return "C4cArray"; }
template <> const char *PyImath::C4fArray::name() { return "C4fArray"; }


using namespace IMATH_NAMESPACE;

template <class T> struct Color4Name { static const char *value; };
template<> const char *Color4Name<unsigned char>::value  = "Color4c";
template<> const char *Color4Name<float>::value  = "Color4f";
// template<> const char *Color4ArrayName<float>::value() { return "Color4fArray"; }
// template<> const char *Color4ArrayName<unsigned char>::value() { return "Color4cArray"; }
template<> const char *Color4Array2DName<float>::value() { return "Color4fArray2D"; }
template<> const char *Color4Array2DName<unsigned char>::value() { return "Color4cArray2D"; }

// create a new default constructor that initializes Color4<T> to zero.
template <class T>
static Color4<T> * Color4_construct_default()
{
    return new Color4<T>(T(0),T(0),T(0),T(0));
}

template <class T, class S>
static Color4<T> * Color4_component_construct1(S x, S y, S z, S w)
{
    // Assigning a floating point value to an integer type can cause a
    // float-point error, which we want to translate into an exception. 
    
    MATH_EXC_ON;

    if(strcmp(Color4Name<T>::value, "Color4c") == 0)
    {
        unsigned char r = (unsigned char) x;
        unsigned char g = (unsigned char) y;
        unsigned char b = (unsigned char) z;
        unsigned char a = (unsigned char) w;
        return new Color4<T>(r,g,b,a);
    }
    else
        return new Color4<T>(T(x) , T(y), T(z), T(w));
}

template <class T, class S>
static Color4<T> * Color4_component_construct2(S x)
{
    MATH_EXC_ON;
    if(strcmp(Color4Name<T>::value, "Color4c") == 0)
    {
        unsigned char u = (unsigned char) x;

        return new Color4<T>(u,u,u,u);
    }
    else    
        return new Color4<T>(T(x),T(x),T(x),T(x));
}

template <class T, class S>
static Color4<T> * Color4_color_construct(const Color4<S> &c)
{
    MATH_EXC_ON;
    if(strcmp(Color4Name<T>::value, "Color4c") == 0)
    {
        unsigned char r = (unsigned char) c.r;
        unsigned char g = (unsigned char) c.g;
        unsigned char b = (unsigned char) c.b;
        unsigned char a = (unsigned char) c.a;
        
        return new Color4<T>(r,g,b,a);
    }
    else
     return new Color4<T>(T (c.r), T(c.g), T(c.b), T(c.a));
}

template <class T>
static Color4<T> * Color4_construct_tuple(const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 4)
    {
        return new Color4<T>(py::cast<T>(t[0]), 
                             py::cast<T>(t[1]), 
                             py::cast<T>(t[2]),
                             py::cast<T>(t[3]));
    }
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color4 expects tuple of length 4");
}

template <class T>
static Color4<T> * Color4_construct_list(const py::list &l)
{
    MATH_EXC_ON;
    if(py::cast<int>(l.attr("__len__")()) == 4)
    {
        return new Color4<T>(py::cast<T>(l[0]), 
                             py::cast<T>(l[1]), 
                             py::cast<T>(l[2]),
                             py::cast<T>(l[3]));
    }
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color4 expects list of length 4");
}

template <class T>
static std::string 
color4_str(const Color4<T> &c)
{
    std::stringstream stream;
    if(strcmp(Color4Name<T>::value, "Color4c") == 0)
    {
        int r = int(c.r);
        int g = int(c.g);
        int b = int(c.b);
        int a = int(c.a);

        stream << Color4Name<T>::value << "(" << r << ", " << g << ", " << b << ", " << a << ")";
        return stream.str();
    }
    else
    {    
        stream << Color4Name<T>::value << "(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
        return stream.str();
    }
}

// Non-specialized repr is same as str
template <class T>
static std::string 
color4_repr(const Color4<T> &c)
{
    return color4_str(c);
}

// Specialization for float to full precision
template <>
std::string 
color4_repr(const Color4<float> &c)
{
    return (boost::format("%s(%.9g, %.9g, %.9g, %.9g)")
                        % Color4Name<float>::value % c.r % c.g % c.b % c.a).str();
}

// No specialization for double, since we don't instantiate Color4d


template <class T>
static Color4<T>
hsv2rgb(Color4<T> &color)
{    
    MATH_EXC_ON;
    return IMATH_NAMESPACE::hsv2rgb(color);
}

template <class T>
static Color4<T>
hsv2rgbTuple(const py::tuple &t)
{
    MATH_EXC_ON;
    Color4<T> color;
    if(t.attr("__len__")() == 4)
    {
        color.r = py::cast<T>(t[0]);
        color.g = py::cast<T>(t[1]);
        color.b = py::cast<T>(t[2]);
        color.a = py::cast<T>(t[3]);
        
        return IMATH_NAMESPACE::hsv2rgb(color);
    }
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color4 expects tuple of length 4");    
}

template <class T>
static Color4<T>
rgb2hsv(Color4<T> &color)
{    
    MATH_EXC_ON;
    return IMATH_NAMESPACE::rgb2hsv(color);
}

template <class T>
static Color4<T>
rgb2hsvTuple(const py::tuple &t)
{
    MATH_EXC_ON;
    Color4<T> color;
    if(py::cast<int>(t.attr("__len__")()) == 4)
    {
        color.r = py::cast<T>(t[0]);
        color.g = py::cast<T>(t[1]);
        color.b = py::cast<T>(t[2]);
        color.a = py::cast<T>(t[3]);
        
        return IMATH_NAMESPACE::rgb2hsv(color);
    }
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color4 expects tuple of length 4");    
}


template <class T>
static void
setValue1(Color4<T> &color, const T &a, const T &b, const T &c, const T &d)
{
    MATH_EXC_ON;
    color.setValue(a, b, c, d);
}

template <class T>
static void
setValue2(Color4<T> &color, const Color4<T> &v)
{
    MATH_EXC_ON;
    color.setValue(v);
}

template <class T>
static void
setValueTuple(Color4<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    Color4<T> v;
    if(py::cast<int>(t.attr("__len__")()) == 4)
    {
        v.r = py::cast<T>(t[0]);
        v.g = py::cast<T>(t[1]);
        v.b = py::cast<T>(t[2]);
        v.a = py::cast<T>(t[3]);
        
        color.setValue(v);
    }
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color4 expects tuple of length 4");
}

template <class T>
static const Color4<T> &
iadd(Color4<T> &color, const Color4<T> &color2)
{
    MATH_EXC_ON;
    return color += color2;
}

template <class T>
static Color4<T>
add(Color4<T> &color, const Color4<T> &color2)
{
    MATH_EXC_ON;
    return color + color2;
}

template <class T>
static Color4<T>
addTuple(Color4<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 4)
        return Color4<T>(color.r + py::cast<T>(t[0]), 
                         color.g + py::cast<T>(t[1]), 
                         color.b + py::cast<T>(t[2]),
                         color.a + py::cast<T>(t[3]));
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color4 expects tuple of length 4");
}

template <class T>
static Color4<T>
addT(Color4<T> &v, T a)
{
    MATH_EXC_ON;
    Color4<T> w(v.r + a, v.g + a, v.b + a, v.a + a);
    return w;
}

template <class T>
static const Color4<T> &
isub(Color4<T> &color, const Color4<T> &color2)
{
    MATH_EXC_ON;
    return color -= color2;
}

template <class T>
static Color4<T>
sub(Color4<T> &color, const Color4<T> &color2)
{
    MATH_EXC_ON;
    return color - color2;
}

template <class T>
static Color4<T>
subtractL(Color4<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 4)
        return Color4<T>(color.r - py::cast<T>(t[0]), 
                         color.g - py::cast<T>(t[1]), 
                         color.b - py::cast<T>(t[2]),
                         color.a - py::cast<T>(t[3]));
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color4 expects tuple of length 4");
}

template <class T>
static Color4<T>
subtractR(Color4<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 4)
        return Color4<T>(py::cast<T>(t[0]) - color.r, 
                         py::cast<T>(t[1]) - color.g, 
                         py::cast<T>(t[2]) - color.b,
                         py::cast<T>(t[3]) - color.a);
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color4 expects tuple of length 4");
}

template <class T>
static Color4<T>
subtractLT(const Color4<T> &color, T a)
{
    MATH_EXC_ON;
    return Color4<T>(color.r - a, 
                     color.g - a, 
                     color.b - a,
                     color.a - a);
}

template <class T>
static Color4<T>
subtractRT(const Color4<T> &color, T a)
{
    MATH_EXC_ON;
    return Color4<T>(a - color.r, 
                     a - color.g, 
                     a - color.b,
                     a - color.a);
}

template <class T>
static Color4<T>
neg(Color4<T> &color)
{
    MATH_EXC_ON;
    return -color;
}

template <class T>
static const Color4<T> &
negate(Color4<T> &color)
{
    MATH_EXC_ON;
    return color.negate();
}

template <class T>
static const Color4<T> &
imul(Color4<T> &color, const Color4<T> &color2)
{
    MATH_EXC_ON;
    return color *= color2;
}

template <class T>
static const Color4<T> &
imulT(Color4<T> &color, const T &t)
{
    MATH_EXC_ON;
    return color *= t;
}

template <class T>
static Color4<T>
mul(Color4<T> &color, const Color4<T> &color2)
{
    MATH_EXC_ON;
    return color * color2;
}

template <class T>
static Color4<T>
mulT(Color4<T> &color, const T &t)
{
    MATH_EXC_ON;
    return color * t;
}

template <class T>
static Color4<T>
rmulT(Color4<T> &color, const T &t)
{
    MATH_EXC_ON;
    return t * color;
}

template <class T>
static Color4<T>
mulTuple(Color4<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 4)
        return Color4<T>(color.r * py::cast<T>(t[0]), 
                         color.g * py::cast<T>(t[1]), 
                         color.b * py::cast<T>(t[2]),
                         color.a * py::cast<T>(t[3]));
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color4 expects tuple of length 4");
}

template <class T>
static const Color4<T> &
idiv(Color4<T> &color, const Color4<T> &color2)
{
    MATH_EXC_ON;
    return color /= color2;
}

template <class T>
static const Color4<T> &
idivT(Color4<T> &color, const T &t)
{
    MATH_EXC_ON;
    return color /= t;
}

template <class T>
static Color4<T>
div(Color4<T> &color, const Color4<T> &color2)
{
    MATH_EXC_ON;
    return color / color2;
}

template <class T>
static Color4<T>
divT(Color4<T> &color, const T &t)
{
    MATH_EXC_ON;
    return color / t;
}

template <class T>
static Color4<T>
divTupleL(Color4<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 4)
        return Color4<T>(color.r / py::cast<T>(t[0]), 
                         color.g / py::cast<T>(t[1]), 
                         color.b / py::cast<T>(t[2]),
                         color.a / py::cast<T>(t[3]));
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color4 expects tuple of length 4");    
}

template <class T>
static Color4<T>
divTupleR(Color4<T> &color, const py::tuple &t)
{
    MATH_EXC_ON;
    if(py::cast<int>(t.attr("__len__")()) == 4)
        return Color4<T>(py::cast<T>(t[0]) / color.r, 
                         py::cast<T>(t[1]) / color.g, 
                         py::cast<T>(t[2]) / color.b,
                         py::cast<T>(t[3]) / color.a);
    else
        THROW(IEX_NAMESPACE::LogicExc, "Color4 expects tuple of length 4");    
}

template <class T>
static Color4<T>
divTR(Color4<T> &color, T a)
{
    MATH_EXC_ON;
    return Color4<T>(a / color.r,
                     a / color.g,
                     a / color.b,
                     a / color.a);
}

template <class T>
static bool
lessThan(Color4<T> &v, const Color4<T> &w)
{
    bool isLessThan = (v.r <= w.r && v.g <= w.g && v.b <= w.b && v.a <= w.a)
                    && v != w;
                   
   return isLessThan;
}

template <class T>
static bool
greaterThan(Color4<T> &v, const Color4<T> &w)
{
    bool isGreaterThan = (v.r >= w.r && v.g >= w.g && v.b >= w.b && v.a >= w.a)
                       && v != w;
                   
   return isGreaterThan;
}

template <class T>
static bool
lessThanEqual(Color4<T> &v, const Color4<T> &w)
{
    bool isLessThanEqual = (v.r <= w.r && v.g <= w.g && v.b <= w.b && v.a <= w.a);

    return isLessThanEqual;
}

template <class T>
static bool
greaterThanEqual(Color4<T> &v, const Color4<T> &w)
{
    bool isGreaterThanEqual = (v.r >= w.r && v.g >= w.g && v.b >= w.b) && v.a >= w.a;

    return isGreaterThanEqual;
}

template <class T>
py::class_<Color4<T> >
register_Color4(py::module &m)
{
    typedef PyImath::StaticFixedArray<Color4<T>,T,4> Color4_helper;
    void (IMATH_NAMESPACE::Color4<T>::*getValue1)(Color4<T> &) const = &IMATH_NAMESPACE::Color4<T>::getValue;
    void (IMATH_NAMESPACE::Color4<T>::*getValue2)(T &, T &, T &, T &) const = &IMATH_NAMESPACE::Color4<T>::getValue;

    py::class_<Color4<T> > color4_class(m, Color4Name<T>::value, Color4Name<T>::value);
    color4_class
        .def(py::init<Color4<T> >(/*"copy construction"*/))
        .def(py::init(&Color4_construct_default<T>),"initialize to (0,0,0,0)")
        .def(py::init(&Color4_construct_tuple<T>), "initialize to (r,g,b,a) with a python tuple")
        .def(py::init(&Color4_construct_list<T>), "initialize to (r,g,b,a) with a python list")
        .def(py::init(&Color4_component_construct1<T,float>))
        .def(py::init(&Color4_component_construct1<T,int>))
        .def(py::init(&Color4_component_construct2<T,float>))
        .def(py::init(&Color4_component_construct2<T,int>))
        .def(py::init(&Color4_color_construct<T,float>))
        .def(py::init(&Color4_color_construct<T,int>))
        .def(py::init(&Color4_color_construct<T,unsigned char>))
        .def_readwrite("r", &Color4<T>::r)
        .def_readwrite("g", &Color4<T>::g)
        .def_readwrite("b", &Color4<T>::b)
        .def_readwrite("a", &Color4<T>::a)
        .def("__str__", &color4_str<T>)
        .def("__repr__", &color4_repr<T>)
        /*
        .def(self == self)
        .def(self != self)
        */
        .def("__iadd__", &iadd<T>,py::return_value_policy::reference_internal)
        .def("__add__", &add<T>)
        .def("__add__", &addTuple<T>)
        .def("__add__", &addT<T>)
        .def("__radd__", &addTuple<T>)
        .def("__radd__", &addT<T>)
        .def("__isub__", &isub<T>,py::return_value_policy::reference_internal)
        .def("__sub__", &sub<T>)
        .def("__sub__", &subtractL<T>)
        .def("__sub__", &subtractLT<T>)
        .def("__rsub__", &subtractR<T>)
        .def("__rsub__", &subtractRT<T>)
        .def("__neg__", &neg<T>)
        .def("negate",&negate<T>,py::return_value_policy::reference_internal,"component-wise multiplication by -1")
        .def("__imul__", &imul<T>,py::return_value_policy::reference_internal)
        .def("__imul__", &imulT<T>,py::return_value_policy::reference_internal)
        .def("__mul__", &mul<T>)
        .def("__mul__", &mulT<T>)
        .def("__rmul__", &mulT<T>)
        .def("__mul__", &mulTuple<T>)
        .def("__rmul__", &mulTuple<T>)
        .def("__idiv__", &idiv<T>,py::return_value_policy::reference_internal)
        .def("__idiv__", &idivT<T>,py::return_value_policy::reference_internal)
        .def("__itruediv__", &idiv<T>,py::return_value_policy::reference_internal)
        .def("__itruediv__", &idivT<T>,py::return_value_policy::reference_internal)
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
        .def("__len__", Color4_helper::len)
        .def("__getitem__", Color4_helper::getitem, py::return_value_policy::copy)
        .def("__setitem__", Color4_helper::setitem)
	    .def_static("dimensions", &Color4<T>::dimensions,"dimensions() number of dimensions in the color")
        .def_static("baseTypeEpsilon", &Color4<T>::baseTypeEpsilon,"baseTypeEpsilon() epsilon value of the base type of the color")
        .def_static("baseTypeMax", &Color4<T>::baseTypeMax,"baseTypeMax() max value of the base type of the color")
        .def_static("baseTypeMin", &Color4<T>::baseTypeMin,"baseTypeMin() min value of the base type of the color")
        .def_static("baseTypeSmallest", &Color4<T>::baseTypeSmallest,"baseTypeSmallest() smallest value of the base type of the color")
        .def("__repr__",&color4_repr<T>)
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
    
        .def("getValue", getValue1, "getValue()")
        .def("getValue", getValue2)
        ;

    decoratecopy(color4_class);

    return color4_class;
}

template PYIMATH_EXPORT py::class_<Color4<float> > register_Color4<float>(py::module &m);
template PYIMATH_EXPORT py::class_<Color4<unsigned char> > register_Color4<unsigned char>(py::module &m);
template PYIMATH_EXPORT py::class_<FixedArray<Color4<float> > > register_Color4Array<float>(py::module &m);
template PYIMATH_EXPORT py::class_<FixedArray<Color4<unsigned char> > > register_Color4Array<unsigned char>(py::module &m);
template PYIMATH_EXPORT py::class_<FixedArray2D<Color4<float> > > register_Color4Array2D<float>(py::module &m);
template PYIMATH_EXPORT py::class_<FixedArray2D<Color4<unsigned char> > > register_Color4Array2D<unsigned char>(py::module &m);

template<> PYIMATH_EXPORT IMATH_NAMESPACE::Color4<float> PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Color4<float> >::value()
{ return IMATH_NAMESPACE::Color4<float>(0,0,0, 0); }
template<> PYIMATH_EXPORT IMATH_NAMESPACE::Color4<unsigned char> PyImath::FixedArrayDefaultValue<IMATH_NAMESPACE::Color4<unsigned char> >::value()
{ return IMATH_NAMESPACE::Color4<unsigned char>(0,0,0,0); }
}
