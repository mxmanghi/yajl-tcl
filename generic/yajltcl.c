/*
 *
 */

#include <tcl.h>
#include "yajltcl.h"


/*
 *--------------------------------------------------------------
 *
 * yajl --
 *
 *
 * Results:
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */
void
yajltcl_print_callback (void *context, const char *str, unsigned int len)
{
    // yajltcl_clientData *yajlData = (yajltcl_clientData *)context;

    printf("print callback '%s'\n", str);
}


/*
 *----------------------------------------------------------------------
 *
 * yajltcl_yajlObjectObjCmd --
 *
 *    dispatches the subcommands of a yajl object command
 *
 * Results:
 *    stuff
 *
 *----------------------------------------------------------------------
 */
int
yajltcl_yajlObjectObjCmd(ClientData cData, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[])
{
    int         optIndex;
    yajltcl_clientData *yajlClientData = (yajltcl_clientData *)cData;
    yajl_gen hand = yajlClientData->handle;
    yajl_gen_status status = yajl_gen_status_ok;
    char *errString = NULL;

    static CONST char *options[] = {
        "array_open",
        "array_close",
	"bool",
	"clear",
	"double",
	"integer",
	"map_close",
	"map_open",
	"null",
	"number",
	"string",
	"free",
	NULL
    };

    enum options {
        OPT_ARRAY_OPEN,
	OPT_ARRAY_CLOSE,
	OPT_BOOL,
	OPT_CLEAR,
	OPT_DOUBLE,
	OPT_INTEGER,
	OPT_MAP_CLOSE,
	OPT_MAP_OPEN,
	OPT_NULL,
	OPT_NUMBER,
	OPT_STRING,
	OPT_FREE
    };

    if ((objc < 2) || (objc > 3)) {
        Tcl_WrongNumArgs (interp, 2, objv, "option ?value?");
	return TCL_ERROR;
    }

    if (Tcl_GetIndexFromObj (interp, objv[1], options, "option",
        TCL_EXACT, &optIndex) != TCL_OK) {
	return TCL_ERROR;
    }

    switch ((enum options) optIndex) {
      case OPT_ARRAY_OPEN: {
	  if (objc != 2) {
	    Tcl_WrongNumArgs (interp, 1, objv, "array_open");
	    return TCL_ERROR;
	  }
          status = yajl_gen_array_open (hand);
	  break;
      }

      case OPT_ARRAY_CLOSE: {
	  if (objc != 2) {
	    Tcl_WrongNumArgs (interp, 1, objv, "array_close");
	    return TCL_ERROR;
	  }
          status = yajl_gen_array_close (hand);
	  break;
      }

      case OPT_BOOL: {
          int bool;

	  if (objc != 3) {
	    Tcl_WrongNumArgs (interp, 1, objv, "bool value");
	    return TCL_ERROR;
	  }

	  if (Tcl_GetBooleanFromObj (interp, objv[2], &bool) == TCL_ERROR) {
	      return TCL_ERROR;
	  }

          status = yajl_gen_bool (hand, bool);
	  break;
      }

      case OPT_CLEAR: {
	  if (objc != 2) {
	    Tcl_WrongNumArgs (interp, 1, objv, "clear");
	    return TCL_ERROR;
	  }
          yajl_gen_clear (hand);
	  status = yajl_gen_status_ok;
	  break;
      }

      case OPT_DOUBLE: {
          double doub;

	  if (objc != 3) {
	    Tcl_WrongNumArgs (interp, 1, objv, "double value");
	    return TCL_ERROR;
	  }

          if (Tcl_GetDoubleFromObj (interp, objv[2], &doub) == TCL_ERROR) {
	      return TCL_ERROR;
	  }

	  status = yajl_gen_double (hand, doub);
	  break;
      }

      case OPT_INTEGER: {
          long lon;

	  if (objc != 3) {
	    Tcl_WrongNumArgs (interp, 1, objv, "integer value");
	    return TCL_ERROR;
	  }

          if (Tcl_GetLongFromObj (interp, objv[2], &lon) == TCL_ERROR) {
	      return TCL_ERROR;
	  }

	  status = yajl_gen_integer (hand, lon);
	  break;
      }

      case OPT_MAP_OPEN: {
	  if (objc != 2) {
	    Tcl_WrongNumArgs (interp, 1, objv, "map_open");
	    return TCL_ERROR;
	  }
          status = yajl_gen_map_open (hand);
	  break;
      }

      case OPT_MAP_CLOSE: {
	  if (objc != 2) {
	    Tcl_WrongNumArgs (interp, 1, objv, "mapclose");
	    return TCL_ERROR;
	  }
          status = yajl_gen_map_close (hand);
	  break;
      }

      case OPT_NULL: {
	  if (objc != 2) {
	    Tcl_WrongNumArgs (interp, 1, objv, "null");
	    return TCL_ERROR;
	  }
          status = yajl_gen_null (hand);
	  break;
      }

      case OPT_NUMBER: {
          char *number;
	  int   len;

	  if (objc != 3) {
	    Tcl_WrongNumArgs (interp, 1, objv, "number value");
	    return TCL_ERROR;
	  }

          number = Tcl_GetStringFromObj (objv[2], &len);
	  status = yajl_gen_number (hand, number, len);
      }

      case OPT_STRING: {
          char *string;
	  int   len;

	  if (objc != 3) {
	    Tcl_WrongNumArgs (interp, 1, objv, "string value");
	    return TCL_ERROR;
	  }

          string = Tcl_GetStringFromObj (objv[2], &len);
	  status = yajl_gen_string (hand, (unsigned char *)string, len);
      }

      case OPT_FREE: {
      }
    }

    switch (status) {
      case yajl_gen_status_ok: {
	  break;
      }

      case yajl_gen_keys_must_be_strings: {
          errString = "map key needed but string not called";
	  break;
      }

      case yajl_max_depth_exceeded: {
          errString = "maximum generation depth exceeded";
	  break;
      }

      case yajl_gen_in_error_state: {
          errString = "generator option called while in error state";
	  break;
      }

      case yajl_gen_generation_complete: {
          printf("generation complete\n");
	  break;
      }

      case yajl_gen_invalid_number: {
          errString = "invalid floating point value";
	  break;
      }

      case yajl_gen_no_buf: {
          errString = "no internal buffer";
	  break;
      }
    }

    if (errString != NULL) {
        Tcl_SetObjResult (interp, Tcl_NewStringObj (errString, -1));
	return TCL_ERROR;
    }

    return TCL_OK;
}



/*
 *----------------------------------------------------------------------
 *
 * yajltcl_yajlObjCmd --
 *
 *	This procedure is invoked to process the "yajl" command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

    /* ARGSUSED */
int
yajltcl_yajlObjCmd(clientData, interp, objc, objv)
    ClientData clientData;		/* registered proc hashtable ptr. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int objc;				/* Number of arguments. */
    Tcl_Obj   *CONST objv[];
{
    yajl_gen_config yConfig;

    yajltcl_clientData *yajlData = (yajltcl_clientData *)ckalloc (sizeof (yajltcl_clientData));
    yajlData->interp = interp;

    yConfig.beautify = 0;
    yConfig.indentString = "\t";

    yajlData->handle = yajl_gen_alloc2 (yajltcl_print_callback, &yConfig, NULL, yajlData);


    if (objc != 3) {
        Tcl_WrongNumArgs (interp, 2, objv, "create name");
	return TCL_ERROR;
    }


    Tcl_CreateObjCommand (interp, Tcl_GetString (objv[2]), yajltcl_yajlObjectObjCmd, yajlData, NULL);

    return TCL_OK;
}

