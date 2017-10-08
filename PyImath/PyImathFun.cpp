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
#include <PyImathFun.h>
#include <PyImathDecorators.h>
#include <PyImathExport.h>
//#include <PyImathAutovectorize.h>
#include <boost/format.hpp>
#include <boost/mpl/bool.hpp>
#include <ImathVec.h>
#include <ImathMatrixAlgo.h>
#include <ImathFun.h>

namespace PyImath {


using namespace PyImath;

namespace {

template <class T>
struct rotationXYZWithUpDir_op
{
    static IMATH_NAMESPACE::Vec3<T>
    apply(const IMATH_NAMESPACE::Vec3<T> &from, const IMATH_NAMESPACE::Vec3<T> &to, 
          const IMATH_NAMESPACE::Vec3<T> &up)
    {
        IMATH_NAMESPACE::Vec3<T> retval;
        IMATH_NAMESPACE::extractEulerXYZ(IMATH_NAMESPACE::rotationMatrixWithUpDir(from,to,up),retval);
        return retval;
    }
};

template <class T>
struct abs_op
{
    static T
    apply(T value)
    {
        return IMATH_NAMESPACE::abs<T>(value);
    }
};

template <class T>
struct sign_op
{
    static T
    apply(T value)
    {
        return IMATH_NAMESPACE::sign<T>(value);
    }
};

template <class T>
struct log_op
{
    static T
    apply(T value)
    {
        return ::log(value);
    }
};

template <class T>
struct log10_op
{
    static T
    apply(T value)
    {
        return ::log10(value);
    }
};

template <class T>
struct lerp_op
{
    static T
    apply(T a, T b, T t)
    {
        return IMATH_NAMESPACE::lerp<T>(a,b,t);
    }
};

template <class T>
struct ulerp_op
{
    static T
    apply(T a, T b, T t)
    {
        return IMATH_NAMESPACE::ulerp<T>(a,b,t);
    }
};

template <class T>
struct lerpfactor_op
{
    static T
    apply(T a, T b, T t)
    {
        return IMATH_NAMESPACE::lerpfactor<T>(a,b,t);
    }
};

template <class T>
struct clamp_op
{
    static T
    apply(T value, T low, T high)
    {
        return IMATH_NAMESPACE::clamp<T>(value,low,high);
    }
};

template <class T>
struct cmp_op
{
    static T
    apply(T value)
    {
        return IMATH_NAMESPACE::cmp<T>(value);
    }
};

template <class T>
struct cmpt_op
{
    static T
    apply(T value)
    {
        return IMATH_NAMESPACE::cmpt<T>(value);
    }
};

template <class T>
struct iszero_op
{
    static T
    apply(T value)
    {
        return IMATH_NAMESPACE::iszero<T>(value);
    }
};

template <class T>
struct equal_op
{
    static T
    apply(T value)
    {
        return IMATH_NAMESPACE::equal<T>(value);
    }
};

template <class T>
struct floor_op
{
    static int
    apply(T value)
    {
        return IMATH_NAMESPACE::floor<T>(value);
    }
};

template <class T>
struct ceil_op
{
    static int
    apply(T value)
    {
        return IMATH_NAMESPACE::ceil<T>(value);
    }
};

template <class T>
struct trunc_op
{
    static int
    apply(T value)
    {
        return IMATH_NAMESPACE::trunc<T>(value);
    }
};

struct divs_op
{
    static int
    apply(int x, int y)
    {
        return IMATH_NAMESPACE::divs(x,y);
    }
};

struct mods_op
{
    static int
    apply(int x, int y)
    {
        return IMATH_NAMESPACE::mods(x,y);
    }
};

struct divp_op
{
    static int
    apply(int x, int y)
    {
        return IMATH_NAMESPACE::divp(x,y);
    }
};

struct modp_op
{
    static int
    apply(int x, int y)
    {
        return IMATH_NAMESPACE::modp(x,y);
    }
};

struct bias_op
{
    static inline float
    apply(float x, float b)
    {
        if (b != 0.5f)
        {
            static const float inverse_log_half = 1.0f / std::log(0.5f);
            const float biasPow = std::log(b)*inverse_log_half;
            return std::pow(x, biasPow);
        }
        return x;
    }
};

struct gain_op
{
    static inline float
    apply(float x, float g)
    {
        if (x < 0.5f)
            return 0.5f*bias_op::apply(2.0f*x, 1.0f - g);
        else
            return 1.0f - 0.5f*bias_op::apply(2.0f - 2.0f*x, 1.0f - g);
    }
};

} // namespace

void register_functions(py::module &m)
{
    //
    // Utility Functions
    //

    PyImath::generate_bindings(m, &abs_op<int>::apply,
        "abs",
        "return the absolute value of 'value'"
        );
    PyImath::generate_bindings(m, &abs_op<float>::apply,
        "abs",
        "return the absolute value of 'value'"
        );
    PyImath::generate_bindings(m, &abs_op<double>::apply,
        "abs",
        "return the absolute value of 'value'"
        );
    PyImath::generate_bindings(m, &sign_op<int>::apply,
        "sign",
        "return 1 or -1 based on the sign of 'value'"
        );
    PyImath::generate_bindings(m, &sign_op<float>::apply,
        "sign",
        "return 1 or -1 based on the sign of 'value'"
        );
    PyImath::generate_bindings(m, &sign_op<double>::apply,
        "sign",
        "return 1 or -1 based on the sign of 'value'"
        );

    PyImath::generate_bindings(m, &log_op<float>::apply,
        "log",
        "return the natural log of 'value'"
        );
    PyImath::generate_bindings(m, &log_op<double>::apply,
        "log",
        "return the natural log of 'value'"
        );

    PyImath::generate_bindings(m, &log10_op<float>::apply,
        "log10",
        "return the base 10 log of 'value'"
        );
    PyImath::generate_bindings(m, &log10_op<double>::apply,
        "log10",
        "return the base 10 log of 'value'"
        );

    PyImath::generate_bindings(m, &lerp_op<float>::apply,
        "lerp",
        "return the linear interpolation of 'a' to 'b' using parameter 't'"
        );
    PyImath::generate_bindings(m, &lerp_op<double>::apply,
        "lerp",
        "return the linear interpolation of 'a' to 'b' using parameter 't'"
        );

    PyImath::generate_bindings(m, &lerpfactor_op<float>::apply,
        "lerpfactor",
        R"(return how far m is between a and b, that is return t such that\
if:
    t = lerpfactor(m, a, b);
then:
    m = lerp(a, b, t);
if a==b, return 0.)"
        );
    PyImath::generate_bindings(m, &lerpfactor_op<double>::apply,
        "lerpfactor",
        R"(return how far m is between a and b, that is return t such that\n"
if:
    t = lerpfactor(m, a, b);
then:
    m = lerp(a, b, t);
if a==b, return 0.)"
        );

    PyImath::generate_bindings(m, &clamp_op<int>::apply,
        "clamp",
        "return the value clamped to the range [low,high]"
        );
    PyImath::generate_bindings(m, &clamp_op<float>::apply,
        "clamp",
        "return the value clamped to the range [low,high]"
        );
    PyImath::generate_bindings(m, &clamp_op<double>::apply,
        "clamp",
        "return the value clamped to the range [low,high]"
        );

    m.def("cmp", IMATH_NAMESPACE::cmp<float>);
    m.def("cmp", IMATH_NAMESPACE::cmp<double>);

    m.def("cmpt", IMATH_NAMESPACE::cmpt<float>);
    m.def("cmpt", IMATH_NAMESPACE::cmpt<double>);

    m.def("iszero", IMATH_NAMESPACE::iszero<float>);
    m.def("iszero", IMATH_NAMESPACE::iszero<double>);

    m.def("equal", IMATH_NAMESPACE::equal<float, float, float>);
    m.def("equal", IMATH_NAMESPACE::equal<double, double, double>);

    PyImath::generate_bindings(m, &floor_op<float>::apply,
        "floor",
        "return the closest integer less than or equal to 'value'"
        );
    PyImath::generate_bindings(m, &floor_op<double>::apply,
        "floor",
        "return the closest integer less than or equal to 'value'"
        );

    PyImath::generate_bindings(m, &ceil_op<float>::apply,
        "ceil",
        "return the closest integer greater than or equal to 'value'"
        );
    PyImath::generate_bindings(m, &ceil_op<double>::apply,
        "ceil",
        "return the closest integer greater than or equal to 'value'"
        );

    PyImath::generate_bindings(m, &trunc_op<float>::apply,
        "trunc",
        "return the closest integer with magnitude less than or equal to 'value'"
        );
    PyImath::generate_bindings(m, &trunc_op<double>::apply,
        "trunc",
        "return the closest integer with magnitude less than or equal to 'value'"
        );

    PyImath::generate_bindings(m, &divs_op::apply,
        "divs",
        R"(return x/y where the remainder has the same sign as x:
divs(x,y) == (abs(x) / abs(y)) * (sign(x) * sign(y)))"
        );

    PyImath::generate_bindings(m, &mods_op::apply,
        "mods",
        R"("return x%y where the remainder has the same sign as x:
mods(x,y) == x - y * divs(x,y))"
        );

    PyImath::generate_bindings(m, &divp_op::apply,
        "divp",
        R"(return x/y where the remainder is always positive:
divp(x,y) == floor (double(x) / double (y)))"
        );
    PyImath::generate_bindings(m, &modp_op::apply,
        "modp",
        R"(return x%y where the remainder is always positive:
modp(x,y) == x - y * divp(x,y))"
        );

    PyImath::generate_bindings(m, &bias_op::apply,
         "bias",
         "bias(x,b) is a gamma correction that remaps the unit interval such that bias(0.5, b) = b."
         );

    PyImath::generate_bindings(m, &gain_op::apply,
         "gain",
         R"(gain(x,g) is a gamma correction that remaps the unit interval with the property that gain(0.5, g) = 0.5.
 The gain function can be thought of as two scaled bias curves forming an 'S' shape in the unit interval.)"
         );

    //
    // Vectorized utility functions
    // 
    PyImath::generate_bindings(m, &rotationXYZWithUpDir_op<float>::apply,
        "rotationXYZWithUpDir",
        R"(return the XYZ rotation vector that rotates 'fromDir' to 'toDir'"
using the up vector 'upDir')"
        );
}

} // namespace PyImath
