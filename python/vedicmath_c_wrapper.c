/**
 * python/vedicmath_c_wrapper.c - Python C API wrapper for VedicMath-AI
 * 
 * This file provides Python C extension interface for direct integration
 * with the VedicMath library, offering better performance than ctypes.
 */

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "vedicmath.h"
#include "vedicmath_dynamic.h"
#include "vedicmath_types.h"

// Forward declarations
static PyObject* py_vedic_multiply(PyObject* self, PyObject* args);
static PyObject* py_vedic_square(PyObject* self, PyObject* args);
static PyObject* py_vedic_divide(PyObject* self, PyObject* args);
static PyObject* py_ekadhikena_purvena(PyObject* self, PyObject* args);
static PyObject* py_nikhilam_mul(PyObject* self, PyObject* args);
static PyObject* py_evaluate_expression(PyObject* self, PyObject* args);

/**
 * Python wrapper for vedic_multiply
 */
static PyObject* py_vedic_multiply(PyObject* self, PyObject* args) {
    long a, b;
    
    if (!PyArg_ParseTuple(args, "ll", &a, &b)) {
        return NULL;
    }
    
    long result = vedic_multiply(a, b);
    return PyLong_FromLong(result);
}

/**
 * Python wrapper for vedic_square
 */
static PyObject* py_vedic_square(PyObject* self, PyObject* args) {
    long n;
    
    if (!PyArg_ParseTuple(args, "l", &n)) {
        return NULL;
    }
    
    long result = vedic_square(n);
    return PyLong_FromLong(result);
}

/**
 * Python wrapper for vedic_divide
 */
static PyObject* py_vedic_divide(PyObject* self, PyObject* args) {
    long dividend, divisor;
    
    if (!PyArg_ParseTuple(args, "ll", &dividend, &divisor)) {
        return NULL;
    }
    
    if (divisor == 0) {
        PyErr_SetString(PyExc_ZeroDivisionError, "Division by zero");
        return NULL;
    }
    
    long remainder;
    long quotient = vedic_divide(dividend, divisor, &remainder);
    
    // Return tuple (quotient, remainder)
    return Py_BuildValue("(ll)", quotient, remainder);
}

/**
 * Python wrapper for ekadhikena_purvena
 */
static PyObject* py_ekadhikena_purvena(PyObject* self, PyObject* args) {
    long n;
    
    if (!PyArg_ParseTuple(args, "l", &n)) {
        return NULL;
    }
    
    if (n % 10 != 5) {
        PyErr_SetString(PyExc_ValueError, "Number must end in 5 for Ekadhikena Purvena");
        return NULL;
    }
    
    long result = ekadhikena_purvena(n);
    return PyLong_FromLong(result);
}

/**
 * Python wrapper for nikhilam_mul
 */
static PyObject* py_nikhilam_mul(PyObject* self, PyObject* args) {
    long a, b;
    
    if (!PyArg_ParseTuple(args, "ll", &a, &b)) {
        return NULL;
    }
    
    long result = nikhilam_mul(a, b);
    return PyLong_FromLong(result);
}

/**
 * Python wrapper for expression evaluation
 */
static PyObject* py_evaluate_expression(PyObject* self, PyObject* args) {
    const char* expression;
    
    if (!PyArg_ParseTuple(args, "s", &expression)) {
        return NULL;
    }
    
    VedicValue result = vedic_dynamic_evaluate(expression);
    
    // Convert VedicValue to Python object based on type
    switch (result.type) {
        case VEDIC_INT32:
            return PyLong_FromLong(result.value.i32);
        case VEDIC_INT64:
            return PyLong_FromLongLong(result.value.i64);
        case VEDIC_FLOAT:
            return PyFloat_FromDouble(result.value.f32);
        case VEDIC_DOUBLE:
            return PyFloat_FromDouble(result.value.f64);
        default:
            PyErr_SetString(PyExc_RuntimeError, "Invalid result type");
            return NULL;
    }
}

/**
 * Get library version
 */
static PyObject* py_get_version(PyObject* self, PyObject* args) {
    return PyUnicode_FromString(vedicmath_version());
}

/**
 * Method definitions for the module
 */
static PyMethodDef VedicMathMethods[] = {
    {"multiply", py_vedic_multiply, METH_VARARGS, 
     "Multiply two numbers using Vedic mathematics.\n\n"
     "Args:\n"
     "    a (int): First number\n"
     "    b (int): Second number\n\n"
     "Returns:\n"
     "    int: Product of a and b\n\n"
     "Example:\n"
     "    >>> multiply(102, 32)\n"
     "    3264"},
    
    {"square", py_vedic_square, METH_VARARGS,
     "Square a number using Vedic mathematics.\n\n"
     "Args:\n"
     "    n (int): Number to square\n\n"
     "Returns:\n"
     "    int: Square of n\n\n"
     "Example:\n"
     "    >>> square(25)\n"
     "    625"},
    
    {"divide", py_vedic_divide, METH_VARARGS,
     "Divide two numbers using Vedic mathematics.\n\n"
     "Args:\n"
     "    dividend (int): Number to be divided\n"
     "    divisor (int): Number to divide by\n\n"
     "Returns:\n"
     "    tuple: (quotient, remainder)\n\n"
     "Example:\n"
     "    >>> divide(1234, 56)\n"
     "    (22, 2)"},
    
    {"ekadhikena_purvena", py_ekadhikena_purvena, METH_VARARGS,
     "Square numbers ending in 5 using Ekadhikena Purvena.\n\n"
     "Args:\n"
     "    n (int): Number ending in 5\n\n"
     "Returns:\n"
     "    int: Square of n\n\n"
     "Example:\n"
     "    >>> ekadhikena_purvena(35)\n"
     "    1225"},
    
    {"nikhilam", py_nikhilam_mul, METH_VARARGS,
     "Multiply numbers near powers of 10 using Nikhilam.\n\n"
     "Args:\n"
     "    a (int): First number\n"
     "    b (int): Second number\n\n"
     "Returns:\n"
     "    int: Product of a and b\n\n"
     "Example:\n"
     "    >>> nikhilam(98, 97)\n"
     "    9506"},
    
    {"evaluate", py_evaluate_expression, METH_VARARGS,
     "Evaluate mathematical expression using Vedic methods.\n\n"
     "Args:\n"
     "    expression (str): Mathematical expression\n\n"
     "Returns:\n"
     "    int or float: Result of expression\n\n"
     "Example:\n"
     "    >>> evaluate('102 * 32')\n"
     "    3264"},
    
    {"version", py_get_version, METH_NOARGS,
     "Get VedicMath library version.\n\n"
     "Returns:\n"
     "    str: Library version string"},
    
    {NULL, NULL, 0, NULL}  // Sentinel
};

/**
 * Module definition
 */
static struct PyModuleDef vedicmath_module = {
    PyModuleDef_HEAD_INIT,
    "vedicmath",                                    // Module name
    "VedicMath-AI: Adaptive Arithmetic Engine",    // Module docstring
    -1,                                             // Size (-1 means module keeps state in global variables)
    VedicMathMethods                                // Method definitions
};

/**
 * Module initialization function
 * 
 * This function MUST be named PyInit_<modulename>
 * where <modulename> matches the module name defined above
 */
PyMODINIT_FUNC PyInit_vedicmath(void) {
    PyObject* module;
    
    // Create the module
    module = PyModule_Create(&vedicmath_module);
    if (module == NULL) {
        return NULL;
    }
    
    // Add module-level constants
    PyModule_AddStringConstant(module, "__version__", vedicmath_version());
    PyModule_AddStringConstant(module, "__author__", "VedicMath-AI Team");
    PyModule_AddIntConstant(module, "VEDIC_INT32", VEDIC_INT32);
    PyModule_AddIntConstant(module, "VEDIC_INT64", VEDIC_INT64);
    PyModule_AddIntConstant(module, "VEDIC_FLOAT", VEDIC_FLOAT);
    PyModule_AddIntConstant(module, "VEDIC_DOUBLE", VEDIC_DOUBLE);
    
    return module;
}

/**
 * Optional: Module cleanup function
 */
static void vedicmath_cleanup(void) {
    // Perform any necessary cleanup here
    // This is called when the module is being unloaded
}

// Register cleanup function
static void __attribute__((destructor))
vedicmath_module_destructor(void) {
    vedicmath_cleanup();
}