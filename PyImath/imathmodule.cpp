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
#include <ImathVec.h>
#include <ImathQuat.h>
#include <ImathEuler.h>
#include <ImathFun.h>
#include <ImathMatrixAlgo.h>
#include <PyImathFixedArray.h>
#include <PyImath.h>
#include <PyImathExport.h>
#include <PyImathBasicTypes.h>
#include <PyImathVec.h>
#include <PyImathMatrix.h>
#include <PyImathBox.h>
#include <PyImathFun.h>
#include <PyImathQuat.h>
#include <PyImathEuler.h>
#include <PyImathColor.h>
#include <PyImathFrustum.h>
#include <PyImathPlane.h>
#include <PyImathLine.h>
#include <PyImathRandom.h>
#include <PyImathShear.h>
#include <PyImathMathExc.h>
//#include <PyImathAutovectorize.h>
#include <PyImathStringArrayRegister.h>


using namespace PyImath;

namespace {

template <typename T>
IMATH_NAMESPACE::Box<IMATH_NAMESPACE::Vec3<T> >
computeBoundingBox(const PyImath::FixedArray<IMATH_NAMESPACE::Vec3<T> >& position)
{
    IMATH_NAMESPACE::Box<IMATH_NAMESPACE::Vec3<T> > bounds;
    int len = position.len();
    for (int i = 0; i < len; ++i)
        bounds.extendBy(position[i]);
    return bounds;
}

IMATH_NAMESPACE::M44d
procrustes1(PyObject* from_input,
    PyObject* to_input,
    PyObject* weights_input = 0,
    bool doScale = false)
{
    // Verify the sequences:
    if (!PySequence_Check(from_input))
    {
        PyErr_SetString(PyExc_TypeError, "Expected a sequence type for 'from'");
        throw py::error_already_set();
    }

    if (!PySequence_Check(to_input))
    {
        PyErr_SetString(PyExc_TypeError, "Expected a sequence type for 'to'");
        throw py::error_already_set();
    }

    bool useWeights = PySequence_Check(weights_input);

    // Now verify the lengths:
    const size_t n = PySequence_Length(from_input);
    if (n != PySequence_Length(to_input) ||
        (useWeights && n != PySequence_Length(weights_input)))
    {
        PyErr_SetString(PyExc_TypeError, "'from, 'to', and 'weights' should all have the same lengths.");
        throw py::error_already_set();
    }

    std::vector<IMATH_NAMESPACE::V3d> from;  from.reserve(n);
    std::vector<IMATH_NAMESPACE::V3d> to;    to.reserve(n);
    std::vector<double> weights;   weights.reserve(n);

    for (size_t i = 0; i < n; ++i)
    {
        PyObject* f = PySequence_GetItem(from_input, i);
        PyObject* t = PySequence_GetItem(to_input, i);
        PyObject* w = 0;
        if (useWeights)
            w = PySequence_GetItem(weights_input, i);

        if (f == 0 || t == 0 || (useWeights && w == 0))
        {
            PyErr_SetString(PyExc_TypeError,
                "Missing element in array");
            throw py::error_already_set();
        }

        from.push_back(py::cast<IMATH_NAMESPACE::V3d>(f));
        to.push_back(py::cast<IMATH_NAMESPACE::V3d>(t));
        if (useWeights)
            weights.push_back(py::cast<double>(w));
    }

    if (useWeights)
        return IMATH_NAMESPACE::procrustesRotationAndTranslation(&from[0], &to[0], &weights[0], n, doScale);
    else
        return IMATH_NAMESPACE::procrustesRotationAndTranslation(&from[0], &to[0], n, doScale);
}

FixedArray2D<int> rangeX(int sizeX, int sizeY)
{
    FixedArray2D<int> f(sizeX, sizeY);
    for (int j=0; j<sizeY; j++)
        for (int i=0; i<sizeX; i++)
            f(i,j) = i;
    return f;
}

FixedArray2D<int> rangeY(int sizeX, int sizeY)
{
    FixedArray2D<int> f(sizeX, sizeY);
    for (int j=0; j<sizeY; j++)
        for (int i=0; i<sizeX; i++)
            f(i,j) = j;
    return f;
}

}

PYBIND11_MODULE(imath, m)
{
    py::handle iex(PyImport_ImportModule("iex"));
    if (PyErr_Occurred()) throw py::error_already_set();

    m["iex"] = iex;
    m.doc() = "Imath module";

    register_basicTypes(m);

    auto iclass2D = IntArray2D::register_(m, "IntArray2D", "Fixed length array of ints");
    add_arithmetic_math_functions(iclass2D);
    add_mod_math_functions(iclass2D);
    add_comparison_functions(iclass2D);
    add_ordered_comparison_functions(iclass2D);
    add_explicit_construction_from_type<float>(iclass2D);
    add_explicit_construction_from_type<double>(iclass2D);

    py::class_<IntMatrix> imclass = IntMatrix::register_(m, "IntMatrix", "Fixed size matrix of ints");
    add_arithmetic_math_functions(imclass);

    py::class_<FloatArray2D> fclass2D = FloatArray2D::register_(m, "FloatArray2D", "Fixed length 2D array of floats");
    add_arithmetic_math_functions(fclass2D);
    add_pow_math_functions(fclass2D);
    add_comparison_functions(fclass2D);
    add_ordered_comparison_functions(fclass2D);
    add_explicit_construction_from_type<int>(fclass2D);
    add_explicit_construction_from_type<double>(fclass2D);

    py::class_<FloatMatrix> fmclass = FloatMatrix::register_(m, "FloatMatrix", "Fixed size matrix of floats");
    add_arithmetic_math_functions(fmclass);
    add_pow_math_functions(fmclass);

    py::class_<DoubleArray2D> dclass2D = DoubleArray2D::register_(m, "DoubleArray2D", "Fixed length array of doubles");
    add_arithmetic_math_functions(dclass2D);
    add_pow_math_functions(dclass2D);
    add_comparison_functions(dclass2D);
    add_ordered_comparison_functions(dclass2D);
    add_explicit_construction_from_type<int>(dclass2D);
    add_explicit_construction_from_type<float>(dclass2D);

    py::class_<DoubleMatrix> dmclass = DoubleMatrix::register_(m, "DoubleMatrix", "Fixed size matrix of doubles");
    add_arithmetic_math_functions(dmclass);
    add_pow_math_functions(dmclass);

    m.def("rangeX", &rangeX);
    m.def("rangeY", &rangeY);

    //
    //  Vec2
    //
    register_Vec2<short>(m);
    register_Vec2<int>(m);
    register_Vec2<float>(m);
    register_Vec2<double>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::V2s> > v2s_class = register_Vec2Array<short>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::V2i> > v2i_class = register_Vec2Array<int>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::V2f> > v2f_class = register_Vec2Array<float>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::V2d> > v2d_class = register_Vec2Array<double>(m);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2f>(v2i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2d>(v2i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2i>(v2f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2d>(v2f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2i>(v2d_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V2f>(v2d_class);

    //
    //  Vec3
    //
    register_Vec3<unsigned char>(m);
    register_Vec3<short>(m);
    register_Vec3<int>(m);
    register_Vec3<float>(m);
    register_Vec3<double>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::V3s> > v3s_class = register_Vec3Array<short>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::V3i> > v3i_class = register_Vec3Array<int>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::V3f> > v3f_class = register_Vec3Array<float>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::V3d> > v3d_class = register_Vec3Array<double>(m);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3f>(v3i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3d>(v3i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3i>(v3f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3d>(v3f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3i>(v3d_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3f>(v3d_class);

    //
    //  Vec4
    //
    register_Vec4<unsigned char>(m);
    register_Vec4<short>(m);
    register_Vec4<int>(m);
    register_Vec4<float>(m);
    register_Vec4<double>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::V4s> > v4s_class = register_Vec4Array<short>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::V4i> > v4i_class = register_Vec4Array<int>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::V4f> > v4f_class = register_Vec4Array<float>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::V4d> > v4d_class = register_Vec4Array<double>(m);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4f>(v4i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4d>(v4i_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4i>(v4f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4d>(v4f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V4i>(v4d_class);

    //
    //  Quat
    //
    register_Quat<float>(m);
    register_Quat<double>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Quatf> > quatf_class = register_QuatArray<float>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Quatd> > quatd_class = register_QuatArray<double>(m);
    add_explicit_construction_from_type<IMATH_NAMESPACE::Quatd>(quatf_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::Quatf>(quatd_class);

    //
    // Euler
    //
    register_Euler<float>(m);
    register_Euler<double>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Eulerf> > eulerf_class = register_EulerArray<float>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Eulerd> > eulerd_class = register_EulerArray<double>(m);
    add_explicit_construction_from_type<IMATH_NAMESPACE::Eulerd>(eulerf_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::Eulerf>(eulerd_class);

    //
    // Box2
    //
    register_Box2<IMATH_NAMESPACE::V2s>(m);
    register_Box2<IMATH_NAMESPACE::V2i>(m);
    register_Box2<IMATH_NAMESPACE::V2f>(m);
    register_Box2<IMATH_NAMESPACE::V2d>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Box2s> > b2s_class = register_BoxArray<IMATH_NAMESPACE::V2s>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Box2i> > b2i_class = register_BoxArray<IMATH_NAMESPACE::V2i>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Box2f> > b2f_class = register_BoxArray<IMATH_NAMESPACE::V2f>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Box2d> > b2d_class = register_BoxArray<IMATH_NAMESPACE::V2d>(m);

    //
    // Box3
    //
    register_Box3<IMATH_NAMESPACE::V3s>(m);
    register_Box3<IMATH_NAMESPACE::V3i>(m);
    register_Box3<IMATH_NAMESPACE::V3f>(m);
    register_Box3<IMATH_NAMESPACE::V3d>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Box3s> > b3s_class = register_BoxArray<IMATH_NAMESPACE::V3s>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Box3i> > b3i_class = register_BoxArray<IMATH_NAMESPACE::V3i>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Box3f> > b3f_class = register_BoxArray<IMATH_NAMESPACE::V3f>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Box3d> > b3d_class = register_BoxArray<IMATH_NAMESPACE::V3d>(m);

    //
    // Matrix33/44
    //
    register_Matrix33<float>(m);
    register_Matrix33<double>(m);
    register_Matrix44<float>(m);
    register_Matrix44<double>(m);

    //
    // M33/44Array
    //
    py::class_<FixedArray<IMATH_NAMESPACE::M44d> > m44d_class = register_M44Array<double>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::M44f> > m44f_class = register_M44Array<float>(m);
    add_explicit_construction_from_type< IMATH_NAMESPACE::Matrix44<double> >(m44d_class);
    add_explicit_construction_from_type< IMATH_NAMESPACE::Matrix44<float> >(m44f_class);

    py::class_<FixedArray<IMATH_NAMESPACE::M33d> > m33d_class = register_M33Array<double>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::M33f> > m33f_class = register_M33Array<float>(m);
    add_explicit_construction_from_type< IMATH_NAMESPACE::Matrix33<double> >(m33d_class);
    add_explicit_construction_from_type< IMATH_NAMESPACE::Matrix33<float> >(m33f_class);

    //
    // String Array
    //
    register_StringArrays(m);

    //
    // Color3/4
    //
    register_Color3<unsigned char>(m);
    register_Color3<float>(m);
    register_Color4<unsigned char>(m);
    register_Color4<float>(m);

    //
    // C3/4Array
    //
    py::class_<FixedArray<IMATH_NAMESPACE::Color3f> > c3f_class = register_Color3Array<float>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Color3c> > c3c_class = register_Color3Array<unsigned char>(m);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3f>(c3f_class);
    add_explicit_construction_from_type<IMATH_NAMESPACE::V3d>(c3f_class);

    py::class_<FixedArray<IMATH_NAMESPACE::Color4f> > c4f_class = register_Color4Array<float>(m);
    py::class_<FixedArray<IMATH_NAMESPACE::Color4c> > c4c_class = register_Color4Array<unsigned char>(m);

    //
    // Color4Array
    //
    register_Color4Array2D<float>(m);
    register_Color4Array2D<unsigned char>(m);

    //
    // Frustum
    //
    register_Frustum<float>(m);
    register_Frustum<double>(m);
    register_FrustumTest<float>(m);
    register_FrustumTest<double>(m);

    //
    // Plane
    //
    register_Plane<float>(m);
    register_Plane<double>(m);

    //
    // Line
    //
    register_Line<float>(m);
    register_Line<double>(m);

    //
    // Shear
    //
    register_Shear<float>(m);
    register_Shear<double>(m);

    //
    // Utility Functions
    //
    register_functions(m);

    m.def("procrustesRotationAndTranslation", procrustes1,
        //py::args("fromPts", "toPts", "weights", "doScale"),  // Can't use 'from' and 'to' because 'from' is a reserved keywork in Python
        "Computes the orthogonal transform (consisting only of rotation and translation) mapping the "
        "'fromPts' points as close as possible to the 'toPts' points in the least squares norm.  The 'fromPts' and "
        "'toPts' lists must be the same length or the function will error out.  If weights "
        "are provided, then the points are weighted (that is, some points are considered more important "
        "than others while computing the transform).  If the 'doScale' parameter is True, then "
        "the resulting matrix is also allowed to have a uniform scale.");

    //
    // Rand
    //
    register_Rand32(m);
    register_Rand48(m);

    //
    // Initialize constants
    //
    m.attr("EULER_XYZ") = IMATH_NAMESPACE::Eulerf::XYZ;
    m.attr("EULER_XZY") = IMATH_NAMESPACE::Eulerf::XZY;
    m.attr("EULER_YZX") = IMATH_NAMESPACE::Eulerf::YZX;
    m.attr("EULER_YXZ") = IMATH_NAMESPACE::Eulerf::YXZ;
    m.attr("EULER_ZXY") = IMATH_NAMESPACE::Eulerf::ZXY;
    m.attr("EULER_ZYX") = IMATH_NAMESPACE::Eulerf::ZYX;
    m.attr("EULER_XZX") = IMATH_NAMESPACE::Eulerf::XZX;
    m.attr("EULER_XYX") = IMATH_NAMESPACE::Eulerf::XYX;
    m.attr("EULER_YXY") = IMATH_NAMESPACE::Eulerf::YXY;
    m.attr("EULER_YZY") = IMATH_NAMESPACE::Eulerf::YZY;
    m.attr("EULER_ZYZ") = IMATH_NAMESPACE::Eulerf::ZYZ;
    m.attr("EULER_ZXZ") = IMATH_NAMESPACE::Eulerf::ZXZ;
    m.attr("EULER_XYZr") = IMATH_NAMESPACE::Eulerf::XYZr;
    m.attr("EULER_XZYr") = IMATH_NAMESPACE::Eulerf::XZYr;
    m.attr("EULER_YZXr") = IMATH_NAMESPACE::Eulerf::YZXr;
    m.attr("EULER_YXZr") = IMATH_NAMESPACE::Eulerf::YXZr;
    m.attr("EULER_ZXYr") = IMATH_NAMESPACE::Eulerf::ZXYr;
    m.attr("EULER_ZYXr") = IMATH_NAMESPACE::Eulerf::ZYXr;
    m.attr("EULER_XZXr") = IMATH_NAMESPACE::Eulerf::XZXr;
    m.attr("EULER_XYXr") = IMATH_NAMESPACE::Eulerf::XYXr;
    m.attr("EULER_YXYr") = IMATH_NAMESPACE::Eulerf::YXYr;
    m.attr("EULER_YZYr") = IMATH_NAMESPACE::Eulerf::YZYr;
    m.attr("EULER_ZYZr") = IMATH_NAMESPACE::Eulerf::ZYZr;
    m.attr("EULER_ZXZr") = IMATH_NAMESPACE::Eulerf::ZXZr;
    m.attr("EULER_X_AXIS") = IMATH_NAMESPACE::Eulerf::X;
    m.attr("EULER_Y_AXIS") = IMATH_NAMESPACE::Eulerf::Y;
    m.attr("EULER_Z_AXIS") = IMATH_NAMESPACE::Eulerf::Z;

    m.attr("INT_MIN") = IMATH_NAMESPACE::limits<int>::min();
    m.attr("INT_MAX") = IMATH_NAMESPACE::limits<int>::max();
    m.attr("INT_SMALLEST") = IMATH_NAMESPACE::limits<int>::smallest();
    m.attr("INT_EPS") = IMATH_NAMESPACE::limits<int>::epsilon();

    m.attr("FLT_MIN") = IMATH_NAMESPACE::limits<float>::min();
    m.attr("FLT_MAX") = IMATH_NAMESPACE::limits<float>::max();
    m.attr("FLT_SMALLEST") = IMATH_NAMESPACE::limits<float>::smallest();
    m.attr("FLT_EPS") = IMATH_NAMESPACE::limits<float>::epsilon();

    m.attr("DBL_MIN") = IMATH_NAMESPACE::limits<double>::min();
    m.attr("DBL_MAX") = IMATH_NAMESPACE::limits<double>::max();
    m.attr("DBL_SMALLEST") = IMATH_NAMESPACE::limits<double>::smallest();
    m.attr("DBL_EPS") = IMATH_NAMESPACE::limits<double>::epsilon();

    //
    // Register Exceptions
    //
    static py::exception<IMATH_NAMESPACE::NullVecExc> PyNullVecExc(m, "NullVecExc");
    static py::exception<IMATH_NAMESPACE::NullQuatExc> PyNullQuatExc(m, "NullQuatExc");
    static py::exception<IMATH_NAMESPACE::SingMatrixExc> PySingMatrixExc(m, "SingMatrixExc");
    static py::exception<IMATH_NAMESPACE::ZeroScaleExc> PyZeroScaleExc(m, "ZeroScaleExc");
    static py::exception<IMATH_NAMESPACE::IntVecNormalizeExc> PyIntVecNormalizeExc(m, "IntVecNormalizeExc");
    py::register_exception_translator([](std::exception_ptr p) {
        // translate c++ exception to python exception
        try {
            if (p) std::rethrow_exception(p);
        }
        //
        //
        catch (const IMATH_NAMESPACE::NullVecExc &e) {
            PyNullVecExc(e.what());
        }
        catch (const IMATH_NAMESPACE::NullQuatExc &e) {
            PyNullQuatExc(e.what());
        }
        catch (const IMATH_NAMESPACE::SingMatrixExc &e) {
            PySingMatrixExc(e.what());
        }
        catch (const IMATH_NAMESPACE::ZeroScaleExc &e) {
            PyZeroScaleExc(e.what());
        }
        catch (const IMATH_NAMESPACE::IntVecNormalizeExc &e) {
            PyIntVecNormalizeExc(e.what());
        }
    });

    m.def("computeBoundingBox", &computeBoundingBox<float>,
        "computeBoundingBox(position) -- computes the bounding box from the position array.");

    m.def("computeBoundingBox", &computeBoundingBox<double>,
        "computeBoundingBox(position) -- computes the bounding box from the position array.");

}
