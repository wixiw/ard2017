/**
 * This file contains python binding for the ardHdlc library
 * https://docs.python.org/3/extending/extending.html
 */

#include <Python.h>

#include "hdlc.h"

static PyObject *ardHdlc_NoMessage;
static PyObject *ardHdlc_FCSError;
static PyObject *ardHdlc_TooShort;

/* ---------- ardHdlc function ---------- */


/*
 * Resets values used in ardHdlc_get_data function
 * to keep track of received buffers.
 */
static PyObject *resetState(PyObject *self, PyObject *args)
{
    ardHdlc_state_t state;
    ardHdlc_getState(&state);
    ardHdlc_resetState(&state);
	Py_RETURN_NONE;
}


/*Retrieves data from specified buffer containing the HDLC frame.*/
static PyObject *decodeFrame(PyObject *self, PyObject *args)
{
    int ret;
    const char *frame_data;
    char recv_data[HDLC_FRAME_LENGTH];
    unsigned int buf_length = 0, recv_length = 0;
    ardHdlc_control_t control;
    ardHdlc_state_t state;
    ardHdlc_getState(&state);

    if (!PyArg_ParseTuple(args, "s#", &frame_data, &buf_length))
        return NULL;

    if (buf_length > HDLC_FRAME_LENGTH)
    {
        PyErr_SetString(PyExc_ValueError, "buffer too long");
        return NULL;
    }

    ret = ardHdlc_decodeFrame(&state, &control, frame_data, buf_length, recv_data, &recv_length);

    if (ret >= 0)
    {
        PyObject *t;

        t = PyTuple_New(4);
        PyTuple_SetItem(t, 0, PyBytes_FromStringAndSize(recv_data, recv_length));
        PyTuple_SetItem(t, 1, PyBytes_FromStringAndSize(frame_data+ret+1, buf_length-ret-1));
        PyTuple_SetItem(t, 2, PyLong_FromUnsignedLong(control.frame));
        PyTuple_SetItem(t, 3, PyLong_FromUnsignedLong(control.seq_no));

        return t;
    }
    else if (ret == -EINVAL)
    {
        PyErr_SetString(PyExc_ValueError, "invalid parameter");
        return NULL;
    }
    else if (ret == -ENOMSG)
    {
        PyErr_SetString(ardHdlc_NoMessage, "no message found");
        return NULL;
    }
    else if (ret == -ETOOSHORT)
    {
        PyObject* cleanedBuff = PyBytes_FromStringAndSize(frame_data+recv_length, buf_length-recv_length);
        PyObject_SetAttrString(ardHdlc_TooShort, "buffer", cleanedBuff);
        PyErr_SetString(ardHdlc_TooShort, "message too short");
        return NULL;
    }
    else if (ret == -EFCS)
    {
        PyObject* cleanedBuff = PyBytes_FromStringAndSize(frame_data+recv_length, buf_length-recv_length);
        PyObject_SetAttrString(ardHdlc_FCSError, "buffer", cleanedBuff);
        PyErr_SetString(ardHdlc_FCSError, "invalid FCS");
        return NULL;
    }
    else
    {
        PyErr_SetString(PyExc_RuntimeError, "unknown error");
        return NULL;
    }
}



/* Creates HDLC frame with specified data buffer.*/
static PyObject *createDataFrame(PyObject *self, PyObject *args)
{
	int ret;
	const char *send_data;
	char frame_data[HDLC_FRAME_LENGTH];
	unsigned int data_length = 0, frame_length = 0, frame_type = ARDHDLC_FRAME_DATA, seq_no = 0;
	ardHdlc_control_t control;
    ardHdlc_state_t state;
    ardHdlc_getState(&state);

	if (!PyArg_ParseTuple(args, "s#|II", &send_data, &data_length, &frame_type, &seq_no))
		return NULL;

	if (data_length > HDLC_FRAME_LENGTH)
	{
		PyErr_SetString(PyExc_ValueError, "data too long");
		return NULL;
	}
	else if (frame_type != ARDHDLC_FRAME_DATA && frame_type != ARDHDLC_FRAME_ACK && frame_type != ARDHDLC_FRAME_NACK)
	{
		PyErr_SetString(PyExc_ValueError, "bad frame type");
		return NULL;
	}
	else if (seq_no < 0 || seq_no > 7)
	{
		PyErr_SetString(PyExc_ValueError, "invalid sequence number");
		return NULL;
	}

	control.frame = frame_type;
	control.seq_no = seq_no;
	ret = ardHdlc_createDataFrame(&control, send_data, data_length, frame_data, &frame_length);

	if (ret == 0)
		return PyBytes_FromStringAndSize(frame_data, frame_length);
	else
	{
		PyErr_SetString(PyExc_ValueError, "invalid parameter");
		return NULL;
	}
}

/* ---------- Settings ---------- */

/*
	Python module's methods.
*/
static PyMethodDef ardHdlcMethods[] = {
	{"resetState", resetState, METH_VARARGS, "Resets values used in get_data method to keep track of received buffers"},
    {"decodeFrame",decodeFrame, METH_VARARGS, "Retrieves data from specified buffer containing the HDLC frame"},
	{"createDataFrame", createDataFrame, METH_VARARGS, "Creates HDLC frame with specified data buffer"},
	{NULL, NULL, 0, NULL}
};

/*
 * Python module itself.
 */
static struct PyModuleDef ardHdlc_module =
{
	PyModuleDef_HEAD_INIT,
	"ardHdlc",
	"ARD HDLC implementation",
	-1,
	ardHdlcMethods
};

/*
	Python module initialization.
*/
PyMODINIT_FUNC PyInit_ardHdlc(void)
{
	PyObject *m;

	m = PyModule_Create(&ardHdlc_module);

	if (m == NULL)
		return NULL;


    ardHdlc_NoMessage = PyErr_NewException("ardHdlc.NoMessage", NULL, NULL);
    Py_INCREF(ardHdlc_NoMessage);
    PyModule_AddObject(m, "NoMessage", ardHdlc_NoMessage);

    ardHdlc_FCSError = PyErr_NewException("ardHdlc.FCSError", NULL, NULL);
    Py_INCREF(ardHdlc_FCSError);
    PyModule_AddObject(m, "FCSError", ardHdlc_FCSError);

    ardHdlc_TooShort = PyErr_NewException("ardHdlc.TooShort", NULL, NULL);
    Py_INCREF(ardHdlc_TooShort);
    PyModule_AddObject(m, "TooShort", ardHdlc_TooShort);

	PyModule_AddIntConstant(m, "FRAME_DATA", ARDHDLC_FRAME_DATA);
	PyModule_AddIntConstant(m, "FRAME_ACK", ARDHDLC_FRAME_ACK);
	PyModule_AddIntConstant(m, "FRAME_NACK", ARDHDLC_FRAME_NACK);

	return m;
}
