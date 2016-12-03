#include<Python.h>
// from sl.c
typedef struct {
	int y,x;
	char c;
}store;
extern int N;
extern store** store_all ;
extern int* store_nums ;
extern int store_now;
extern char* output_map;
void windowInit(int c, int l,char *arg);
void windowDestroy(void);
void mapModify(store* s,int num);

int sl_step=0;

static PyObject * slpy_init(PyObject *self, PyObject *args)
{
	int c,l;
	const char *arg;
	if (!PyArg_ParseTuple(args, "iis", &c,&l,&arg))
		return NULL;
	printf("%s\n",arg);
	windowInit(c,l,arg);
	sl_step=0;
	Py_RETURN_NONE;
}
static PyObject * slpy_len(PyObject *self, PyObject *args)
{
	return Py_BuildValue("i",N);
}

static PyObject * slpy_step(PyObject *self, PyObject *args)
{
	if(sl_step < N)
	{
		mapModify(store_all[sl_step],store_nums[sl_step]);
		++sl_step;
		return Py_BuildValue("s",output_map);
	}
	else
	{
		windowDestroy();
		Py_RETURN_NONE;
	}
}

static PyMethodDef methods[] = {
    {"init",  slpy_init, METH_VARARGS, "init"},
    {"len" ,  slpy_len , METH_VARARGS, "get len of the list"},
    {"step",  slpy_step, METH_VARARGS, "sl generator"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

static struct PyModuleDef module = {
   PyModuleDef_HEAD_INIT,
   "slpy",   /* name of module */
   "sl work on python", /* module documentation, may be NULL */
   -1,       /* size of per-interpreter state of the module,
                or -1 if the module keeps state in global variables. */
   methods
};

PyMODINIT_FUNC PyInit_slpy(void)
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
    PyImport_AppendInittab("slpy", PyInit_slpy);

    /* Pass argv[0] to the Python interpreter */
    Py_SetProgramName(program);

    /* Initialize the Python interpreter.  Required. */
    Py_Initialize();

    /* Optionally import the module; alternatively,
       import can be deferred until the embedded script
       imports it. */
    PyImport_ImportModule("slpy");

    PyMem_RawFree(program);
    return 0;
}
