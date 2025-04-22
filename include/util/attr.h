#ifndef ATTR_H
#define ATTR_H

#if defined(__clang__)
// #define MALLOC_ATTR(fn, index) __attribute__((malloc))
#define MALLOC_ATTR(fn, index)

#else
// #define MALLOC_ATTR(fn, index) __attribute__((malloc, malloc(fn, index)))
#define MALLOC_ATTR(fn, index)

#endif

#endif // ATTR_H
