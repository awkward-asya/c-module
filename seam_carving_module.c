#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <Python.h>

#define MAX_VALUE 2147483647 // 2^31 - 1

int mymax(int a, int b) {
    return (a > b ? a : b);
}

int mymin(int a, int b) {
    return (a < b ? a : b);
}

int indexInArrayByColor(int width, int height, int x, int y, int color) {
    if (x < 0 || y < 0 || x >= width || y >= height) {
        x = mymax(0, mymin(x, width - 1));
        y = mymax(0, mymin(y, height - 1));
    }
    return (y * width + x) * 3 + color;
}

int getSobel(int* array, int w, int h, int x, int y, int color) {
    double z1 = array[indexInArrayByColor(w, h, x - 1, y - 1, color)];
    double z2 = array[indexInArrayByColor(w, h, x, y - 1, color)];
    double z3 = array[indexInArrayByColor(w, h, x + 1, y - 1, color)];
    double z4 = array[indexInArrayByColor(w, h, x - 1, y, color)];
    double z6 = array[indexInArrayByColor(w, h, x + 1, y, color)];
    double z7 = array[indexInArrayByColor(w, h, x - 1, y + 1, color)];
    double z8 = array[indexInArrayByColor(w, h, x, y + 1, color)];
    double z9 = array[indexInArrayByColor(w, h, x + 1, y + 1, color)];
    double gx = z7 + 2 * z8 + z9 - (z1 + 2 * z2 + z3);
    double gy = z3 + 2 * z6 + z9 - (z1 + 2 * z4 + z7);
    return (int) sqrt(gx * gx + gy * gy);
    }

int min3(int width, int height, int i, int j, int* sobelArray) {
    int mi = j * width + i;
    int ri = 1 + mi;
    int li = -1 + mi;
    if (i == 0) {
        return (sobelArray[mi] > sobelArray[ri] ? i + 1 : i);
    } else if (i == width - 1) {
        return (sobelArray[mi] > sobelArray[li] ? i - 1 : i);
    } else {
        return (sobelArray[li] > sobelArray[ri] ?
                (sobelArray[mi] > sobelArray[ri] ? i + 1 : i) :
                (sobelArray[mi] > sobelArray[li] ? i - 1 : i));
    }
}

void changeSide(int width, int height, int* array) {
    int mod, index;
    int* newArray = malloc(sizeof(int) * height * width * 3);
    if (newArray == NULL) {
        return;
    }
    for (int i = 0; i < height * width * 3; i++) {
        mod = i % 3;
        index = (((i / 3) % width) * height + i / 3 / width) * 3;
        newArray[index + mod] = array[i];
    }
    for (int i = 0; i < height * width * 3; i++) {
        array[i] = newArray[i];
    }
    free(newArray);
}

void changeSideBack(int width, int height, int* array){
    width;
    int i, mod, index;
    int* newArray = (int*) malloc(sizeof(int) * height * width * 3);
    for (i = 0; i < height * width * 3; i++) {
        mod = i % 3;
        index = (((i / 3) % height) * width + i / 3 / height) * 3;
        newArray[index + mod] = array[i];
    }
    for (i = 0; i < height * width * 3; i++) {
        array[i] = newArray[i];
    }
    free(newArray);
}

void scriptMinusW(int width, int height, int* array) {
    int size = height * width;
    int* sobelArray = (int*)malloc(size * sizeof(int));
    for(int i = 0; i < size; i++){
       sobelArray[i] = 0;
    }
    int index, pathIndex, l = 0, minC = MAX_VALUE, minI = 0;
    int* path = (int*)malloc(height * sizeof(int));
    int* newArray = (int*)malloc((size - height) * 3 * sizeof(int));
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            index = j * width + i;

            for (int k = 0; k < 3; k++) {
                sobelArray[index] += getSobel(array, width, height, i, j, k) / 3;
            }
            sobelArray[index] = mymin(255, sobelArray[index]);
        }
    }

    for (int j = height - 2; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            index = j * width + i;
            sobelArray[index] += mymin(sobelArray[index + width],
                    mymin((i == 0 ? MAX_VALUE : sobelArray[index + width - 1]),
                            (i == width - 1 ? MAX_VALUE : sobelArray[index + width + 1])));
        }
    }

    for (int i = 0; i < width; i++) {
        if (sobelArray[i] < minC) {
            minC = sobelArray[i];
            minI = i;
        }
    }

    path[0] = minI;
    for (int j = 0; j < height - 1; j++) {
        pathIndex = j + 1;
        path[pathIndex] = min3(width,height,path[j], pathIndex, sobelArray);
    }

    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            index = (j * width + i) * 3;
            if (path[j] == i) {
                for (int k = 0; k < 3; k++) {
                    array[index + k] = -1;
                }
            }
        }
    }
    for (int i = 0; i < size * 3; i++) {
        if (array[i] == -1) {
            l++;
            continue;
        }
        newArray[i-l] = array[i];
    }

    for (int i = 0; i < (size - height)*3; i++) {
        array[i] = newArray[i];
    }
    free(newArray);
    free(sobelArray);
    free(path);
}

int* runResizing(int w, int h, int dw, int dh, int* arr) {

    if (dw >= 0 && dw < w && dh < h && dh >= 0) {
        for (int i = 0; i < dw; i++) {
            scriptMinusW(w, h, arr);
            w--;
        }
        changeSide(w, h, arr);

        for (int i = 0; i < dh; i++) {
            scriptMinusW(h, w, arr);
            h--;
        }
        changeSideBack(w, h, arr);
        return arr;
    }
    return NULL;
}
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

    array = runResizing(w, h, dw, dh, array);

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