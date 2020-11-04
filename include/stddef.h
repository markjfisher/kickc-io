#ifndef _STDDEF_H
#define _STDDEF_H

/* Standard data types */
#ifndef _HAVE_ptrdiff_t
#define _HAVE_ptrdiff_t
typedef int ptrdiff_t;
#endif
#ifndef _HAVE_wchar_t
#define _HAVE_wchar_t
typedef char wchar_t;
#endif

//#ifndef _HAVE_size_t
//#define _HAVE_size_t
//typedef unsigned size_t;
//#endif

/* NULL pointer */
#ifndef _HAVE_NULL
#define NULL    0
#define _HAVE_NULL
#endif

/* offsetof macro */
#define offsetof(type, member)  (size_t) (&((type*) 0)->member)

/* End of stddef.h */
#endif
