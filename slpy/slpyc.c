#include<Python.h>
// from sl.c
extern int N;
extern char* output_map;
void windowInit(int c, int l,char *arg);
void windowDestroy(void);
void mapModify(int n);

int sl_step=0;

static PyObject * slpyc_init(PyObject *self, PyObject *args)
{
	int c,l;
	const char *arg;
	if (!PyArg_ParseTuple(args, "iis", &c,&l,&arg))
		return NULL;
	windowInit(c,l,(char *)arg);
	sl_step=0;
	Py_RETURN_NONE;
}
static PyObject * slpyc_len(PyObject *self, PyObject *args)
{
	return Py_BuildValue("i",N);
}

static PyObject * slpyc_step(PyObject *self, PyObject *args)
{
//	printf("%d %d\n",sl_step,N);
	if(sl_step < N)
	{
		mapModify(sl_step);
		++sl_step;
		return Py_BuildValue("s",output_map);
	}
	else if(sl_step==N)
	{
		windowDestroy();
		++sl_step;
		Py_RETURN_NONE;
	}
	else
		Py_RETURN_NONE;
}

static PyMethodDef methods[] = {
    {"init",  slpyc_init, METH_VARARGS, "init"},
    {"len" ,  slpyc_len , METH_VARARGS, "get len of the list"},
    {"step",  slpyc_step, METH_VARARGS, "sl generator"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef module = {
   PyModuleDef_HEAD_INIT,
   "slpyc",   /* name of module */
   "sl work on python", /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   methods
};

PyMODINIT_FUNC PyInit_slpyc(void)
{
    return PyModule_Create(&module);
}

int main(int argc, char *argv[])
{
    wchar_t *program = Py_DecodeLocale(argv[0], NULL);
    if (program == NULL) {
        fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
        exit(1);
    }

    /* Add a built-in module, before Py_Initialize */
    PyImport_AppendInittab("slpyc", PyInit_slpyc);

    /* Pass argv[0] to the Python interpreter */
    Py_SetProgramName(program);

    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();

    /* Optionally import the module; alternatively,
       import can be deferred until the embedded script
       imports it. */
    PyImport_ImportModule("slpyc");

    PyMem_RawFree(program);
    return 0;
}
