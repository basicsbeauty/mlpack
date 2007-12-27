/* src/Ifpack_config.h.  Generated from Ifpack_config.h.in by configure.  */
/* src/Ifpack_config.h.in.  Generated from configure.ac by autoheader.  */

/* Define to dummy `main' function (if any) required to link to the Fortran
   libraries. */
/* #undef F77_DUMMY_MAIN */

/* Define to a macro mangling the given C identifier (in lower and upper
   case), which must not contain underscores, for linking with Fortran. */
#define F77_FUNC(name,NAME) name ## _

/* As F77_FUNC, but for C identifiers containing underscores. */
#define F77_FUNC_(name,NAME) name ## __

/* Define if F77 and FC dummy `main' functions are identical. */
/* #undef FC_DUMMY_MAIN_EQ_F77 */

/* Define to 1 if you have the <algorithm> header file. */
#define HAVE_ALGORITHM 1

/* Define to 1 if you have the <algorithm.h> header file. */
/* #undef HAVE_ALGORITHM_H */

/* Define to 1 if you have the <algo.h> header file. */
/* #undef HAVE_ALGO_H */

/* Define to 1 if you have the <assert.h> header file. */
/* #undef HAVE_ASSERT_H */

/* Define if you have a BLAS library. */
#define HAVE_BLAS 1

/* define if bool is a built-in type */
#define HAVE_BOOL 

/* Define to 1 if you have the <cassert> header file. */
#define HAVE_CASSERT 1

/* Define to 1 if you have the <cmath> header file. */
#define HAVE_CMATH 1

/* Define to 1 if you have the <cstdio> header file. */
#define HAVE_CSTDIO 1

/* Define to 1 if you have the <cstdlib> header file. */
#define HAVE_CSTDLIB 1

/* Define if want to build examples */
#define HAVE_EXAMPLES 

/* Define if you want to build export makefiles. */
#define HAVE_EXPORT_MAKEFILES 

/* Define if you are using gnumake - this will shorten your link lines. */
/* #undef HAVE_GNUMAKE */

/* Define if want to build with ifpack enabled */
/* #undef HAVE_IFPACK_AMESOS */

/* Define if want to build with ifpack enabled */
#define HAVE_IFPACK_AZTECOO 1

/* Define if want to build with ifpack enabled */
/* #undef HAVE_IFPACK_EPETRAEXT */

/* Define if want to build ifpack-examples */
#define HAVE_IFPACK_EXAMPLES 

/* Define if want to build with ifpack enabled */
/* #undef HAVE_IFPACK_GALERI */

/* Define if you want to build Ifpack's ParameterList interface */
/* #undef HAVE_IFPACK_METIS */

/* Define if want to build ifpack-tests */
#define HAVE_IFPACK_TESTS 

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the <iostream> header file. */
#define HAVE_IOSTREAM 1

/* Define to 1 if you have the <iostream.h> header file. */
/* #undef HAVE_IOSTREAM_H */

/* Define if you have LAPACK library. */
#define HAVE_LAPACK 1

/* Define if want to build libcheck */
#define HAVE_LIBCHECK 

/* Define to 1 if you have the <math.h> header file. */
/* #undef HAVE_MATH_H */

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* define if we want to use MPI */
/* #undef HAVE_MPI */

/* define if the compiler supports the mutable keyword */
#define HAVE_MUTABLE 

/* define if the compiler implements namespaces */
#define HAVE_NAMESPACES 

/* define if the compiler accepts the new for scoping rules */
#define HAVE_NEW_FOR_SCOPING 

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
/* #undef HAVE_STDIO_H */

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* define if std::sprintf is supported */
#define HAVE_STD_SPRINTF 

/* define if the compiler supports Standard Template Library */
#define HAVE_STL 

/* Define to 1 if you have the <string> header file. */
#define HAVE_STRING 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define if want to build tests */
#define HAVE_TESTS 

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <vector> header file. */
#define HAVE_VECTOR 1

/* Define to 1 if you have the <vector.h> header file. */
/* #undef HAVE_VECTOR_H */

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "maherou@sandia.gov"

/* Define to the full name of this package. */
#define PACKAGE_NAME "ifpack"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "ifpack 3.2"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "ifpack"

/* Define to the version of this package. */
#define PACKAGE_VERSION "3.2"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* define if compiling for ASCI Red, aka TFLOP */
/* #undef TFLOP */

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
/* #undef inline */
#endif
