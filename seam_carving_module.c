#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <Python.h>

#define MAX_VALUE 2147483647 // 2^31 - 1

/* функции, доступные для вызова из питона */
static PyObject* resizeVariableRes(PyObject* self, PyObject* args) {
    int dw, dh, w, h;
    PyObject* pyList;
    if (!PyArg_ParseTuple(args, "Oiiii", &pyList, &w, &h, &dw, &dh)) {
        return NULL;
    }

    if (!PyList_Check(pyList)) {
        PyErr_SetString(PyExc_Exception,"Ошибка - объект пришедший из Питона не является списком.");
        return NULL;
    }

    int* array = malloc(sizeof(int) * h * w * 3);

    if (!array) {
        PyErr_SetString(PyExc_Exception, "Ошибка - не удалось выделить память.");
        return NULL;
    }

    for(int i = 0; i < w*h*3; i++){
        PyObject * pyItem = PyList_GetItem(pyList, i);
        long item = PyLong_AsLong(pyItem);
        
        if (item == -1 && PyErr_Occurred()) {
            PyErr_SetString(PyExc_Exception, "Ошибка - элемент списка считан некорректно.");
            return NULL;
        }

        array[i] = (int)item;
    }

//    array = runResizing(w, h, dw, dh, array);

    int size = (h-dh)*(w-dw)*3;
    PyObject* resultImage = PyList_New(size);

    if (resultImage == NULL) {
        PyErr_SetString(PyExc_Exception, "Ошибка - не удалось создать результат в виде списка значений пикселей.");
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        if(array[i] == -1){
            array[i] = 0;
        }
        PyObject* item = PyLong_FromLong(array[i]);

        if (item == NULL) {
            Py_DECREF(resultImage);
            PyErr_SetString(PyExc_Exception, "Ошибка - не удалось создать объект PyLong из значения array.");
            return NULL;
        }
    
        PyList_SET_ITEM(resultImage, i, item);
    }

    return resultImage;
}


/* функции для сборки модуля */
static char annotationVarFunction[] = "resize_as_variable(filename, dw, dh): returns a list of lists, containing 3-element tuples of resized image, shrinking it by provided dw amount of pixels by width and dh amount of pixels by height";

static PyMethodDef module_methods[] = {
    {"resize_as_variable", (PyCFunction)resizeVariableRes, METH_VARARGS, annotationVarFunction},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef seam_carving = {
    PyModuleDef_HEAD_INIT,
    "seam_carving",
    "Module for content aware image resizing using seam carving algorithm.",
    -1,
    module_methods
};

PyMODINIT_FUNC PyInit_seam_carving(void) {
    PyObject* module;
    module = PyModule_Create(&seam_carving);
    if (module == NULL)
        return NULL;
    return module;
}