#ifndef MACROS_H
#define MACROS_H


#define SIZEOF_ARRAY(arr) (sizeof (arr) / sizeof (*(arr)))

#define rgt_new(type) ((type*)malloc(sizeof (type)))
#define rgt_newArray(len, type) ((type(*)[])calloc(len, sizeof (type)))

#define likely(x)   __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)


#endif /* MACROS_H */
