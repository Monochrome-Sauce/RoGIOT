#ifndef MACROS_H
#define MACROS_H


#define SIZEOF_ARRAY(arr) (sizeof (arr) / sizeof ((arr)[0]))

#define CAN_INDEX(index, arrLen) (0 <= (index) && (index) < (arrLen))

#define TOKSTR__(x) #x
#define TOKSTR(x) TOKSTR__(x)

#define rgt_new(type) ((type*)malloc(sizeof (type)))
#define rgt_newarr(len, type) ((type(*)[])calloc(len, sizeof (type)))
#define rgt_free(p) do { free(p); p = NULL; } while (0)

#define likely(x)   (__builtin_expect((x), 1))
#define unlikely(x) (__builtin_expect((x), 0))
/* example usage:
if unlikely (str == NULL) {
	...
}
else if likely (strcmp(str, "Bob") == 0) {
	...
}
*/


#endif /* MACROS_H */
