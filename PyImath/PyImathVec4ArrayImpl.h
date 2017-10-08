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

#ifndef _PyImathVec4ArrayImpl_h_
#define _PyImathVec4ArrayImpl_h_

//
// This .C file was turned into a header file so that instantiations
// of the various V4* types can be spread across multiple files in
// order to work around MSVC limitations.
//

#include "python_include.h"
#include <PyImathVec.h>
#include "PyImathDecorators.h"
#include <boost/format.hpp>
#include <PyImath.h>
#include <ImathVec.h>
#include <ImathVecAlgo.h>
#include <Iex.h>
#include <PyImathMathExc.h>
#include <PyImathOperators.h>
#include <PyImathVecOperators.h>

namespace PyImath {

using namespace IMATH_NAMESPACE;

// XXX fixme - template this
// really this should get generated automatically...

template <class T,int index>
static FixedArray<T>
Vec4Array_get(FixedArray<IMATH_NAMESPACE::Vec4<T> > &va)
{
    return FixedArray<T>(&va[0][index],va.len(),4*va.stride(),va.handle());
}

template <class T>
static void
setItemTuple(FixedArray<IMATH_NAMESPACE::Vec4<T> > &va, Py_ssize_t index, const py::tuple &t)
{
    if(py::cast<int>(t.attr("__len__")()) == 4)
    {
        Vec4<T> v;
        v.x = py::cast<T>(t[0]);
        v.y = py::cast<T>(t[1]);
        v.z = py::cast<T>(t[2]);
        v.w = py::cast<T>(t[3]);

        va[va.canonical_index(index)] = v;
    }
    else
        THROW(IEX_NAMESPACE::LogicExc, "tuple of length 4 expected");
}

template <class T>
static IMATH_NAMESPACE::Vec4<T>
Vec4Array_min(const FixedArray<IMATH_NAMESPACE::Vec4<T> > &a) {
    Vec4<T> tmp(Vec4<T>(0));
    size_t len = a.len();
    if (len > 0)
        tmp = a[0];
    for (size_t i=1; i < len; ++i)
    {
        if (a[i].x < tmp.x)
            tmp.x = a[i].x;
        if (a[i].y < tmp.y)
            tmp.y = a[i].y;
        if (a[i].z < tmp.z)
            tmp.z = a[i].z;
        if (a[i].w < tmp.w)
            tmp.w = a[i].w;
    }
    return tmp;
}

template <class T>
static IMATH_NAMESPACE::Vec4<T>
Vec4Array_max(const FixedArray<IMATH_NAMESPACE::Vec4<T> > &a)
{
    Vec4<T> tmp(Vec4<T>(0));
    size_t len = a.len();
    if (len > 0)
        tmp = a[0];
    for (size_t i=1; i < len; ++i)
    {
        if (a[i].x > tmp.x)
            tmp.x = a[i].x;
        if (a[i].y > tmp.y)
            tmp.y = a[i].y;
        if (a[i].z > tmp.z)
            tmp.z = a[i].z;
        if (a[i].w > tmp.w)
            tmp.w = a[i].w;
    }
    return tmp;
}

template <class T>
py::class_<FixedArray<IMATH_NAMESPACE::Vec4<T> > >
register_Vec4Array(py::module &m)
{
    using boost::mpl::true_;

    py::class_<FixedArray<IMATH_NAMESPACE::Vec4<T> > > vec4Array_class = FixedArray<IMATH_NAMESPACE::Vec4<T> >::register_(m, "Fixed length array of IMATH_NAMESPACE::Vec4");
    vec4Array_class
        .def_property_readonly("x",&Vec4Array_get<T,0>)
        .def_property_readonly("y",&Vec4Array_get<T,1>)
        .def_property_readonly("z",&Vec4Array_get<T,2>)
        .def_property_readonly("w",&Vec4Array_get<T,3>)
        .def("__setitem__", &setItemTuple<T>)
        .def("min", &Vec4Array_min<T>)
        .def("max", &Vec4Array_max<T>)
        ;

    add_arithmetic_math_functions(vec4Array_class);
    add_comparison_functions(vec4Array_class);

    generate_member_bindings(vec4Array_class, &op_vecLength<IMATH_NAMESPACE::Vec4<T> >::apply,"length","");
    generate_member_bindings(vec4Array_class, &op_vecLength2<IMATH_NAMESPACE::Vec4<T> >::apply,"length2","");
    generate_member_bindings(vec4Array_class, &op_vecNormalize<IMATH_NAMESPACE::Vec4<T> >::apply,"normalize","");
    generate_member_bindings(vec4Array_class, &op_vecNormalized<IMATH_NAMESPACE::Vec4<T> >::apply,"normalized","");

    generate_member_bindings(vec4Array_class, &op_vecDot<IMATH_NAMESPACE::Vec4<T> >::apply,"dot","return the inner product of (self,x)");
    generate_member_bindings(vec4Array_class, &op_mul<IMATH_NAMESPACE::Vec4<T>,T>::apply,"__mul__" ,"self*x");
    generate_member_bindings(vec4Array_class, &op_mul<IMATH_NAMESPACE::Vec4<T>,T>::apply,"__rmul__","x*self");
    generate_member_bindings(vec4Array_class, &op_imul<IMATH_NAMESPACE::Vec4<T>,T>::apply,"__imul__","self*=x");
    generate_member_bindings(vec4Array_class, &op_div<IMATH_NAMESPACE::Vec4<T>,T>::apply,"__div__" ,"self/x");
    generate_member_bindings(vec4Array_class, &op_div<IMATH_NAMESPACE::Vec4<T>,T>::apply,"__truediv__" ,"self/x");
    generate_member_bindings(vec4Array_class, &op_idiv<IMATH_NAMESPACE::Vec4<T>,T>::apply,"__idiv__","self/=x");
    generate_member_bindings(vec4Array_class, &op_idiv<IMATH_NAMESPACE::Vec4<T>,T>::apply,"__itruediv__","self/=x");

    decoratecopy(vec4Array_class);

    return vec4Array_class;
}


}  // namespace PyImath

#endif   // _PyImathVec4ArrayImpl_h_
