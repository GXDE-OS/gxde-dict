#include "speaker.h"


Speaker::Speaker(QString text)
{
    Py_Initialize();
    PyObject *sys = PyImport_ImportModule("sys");
    PyRun_SimpleString("import sys");
    PyRun_SimpleString(("sys.path.append('" + QCoreApplication::applicationDirPath() + "')").toStdString().c_str());
    PyRun_SimpleString(("sys.path.append('" + QCoreApplication::applicationDirPath() + "/../share/redict/')").toStdString().c_str());
    PyObject* pModule = PyImport_ImportModule("edge");
    PyObject* pFunc = PyObject_GetAttrString(pModule, "Play");
    PyObject* pArgs = PyTuple_New(1);
    PyTuple_SetItem(pArgs, 0, Py_BuildValue("s", text.toStdString().c_str()));
    PyObject* pReturn = PyObject_CallObject(pFunc, pArgs);
    Py_Finalize();
}
