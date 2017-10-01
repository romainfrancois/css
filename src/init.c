#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* .Call calls */
extern SEXP tokenize_css_impl(SEXP);

static const R_CallMethodDef CallEntries[] = {
  {"tokenize_css_impl", (DL_FUNC) &tokenize_css_impl, 1},
  {NULL, NULL, 0}
};

void R_init_css(DllInfo *dll)
{
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
