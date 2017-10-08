#pragma once
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
namespace py = pybind11;


#include <type_traits>


template<typename T>
constexpr py::return_value_policy call_policy()
{
    return std::is_class<T>::value
        ? py::return_value_policy::reference_internal
        : py::return_value_policy::automatic
        ;
}

template<typename T>
constexpr py::return_value_policy const_call_policy()
{
    return std::is_class<T>::value
        ? py::return_value_policy::copy
        : py::return_value_policy::automatic
        ;
}

template<typename T>
class py_extract
{
    py::object m_p;
public:
    py_extract(py::object p)
        : m_p(p)
    {
    }

    bool check()const
    {
        try {
            py::cast<T>(m_p);
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    T operator()()
    {
        return py::cast<T>(m_p);
    }
};

/*
typedef typename boost::mpl::if_<
boost::is_class<T>,
    py::return_internal_reference<>,
    py::default_call_policies>::type call_policy;

typedef typename boost::mpl::if_<
boost::is_class<T>,
    py::return_value_policy<py::copy_const_reference>,
    py::default_call_policies>::type const_call_policy;
*/

#if 1
namespace PyImath {

    template<typename F>
    void generate_bindings(py::module &m, F f, const char *name, const char *doc)
    {
        m.def(name, f, doc);
    }

    template<typename C, typename F>
    void generate_member_bindings(py::class_<C> &c, F f, const char *name, const char *doc)
    {
    }
}
#else
#include "PyImathAutovectorize.h"
#endif
