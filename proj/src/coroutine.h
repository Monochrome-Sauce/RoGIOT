#ifndef COROUTINE_H
#define COROUTINE_H

/*
The function's algorithm must start with CO__START() and end with CO__END().

CO__YIELD()  - return a value and save the current exit point.
CO__FINISH() - used to finish the execution of the coroutine and jump to CO_END().
              Must be called at least once in a function.

CO__INIT_CTX()    - must be used by the caller to initialize a context object.
CO__IS_FINISHED() - used by the caller to check if the function returned without yielding.

Context object - any struct which contains a signed integer member named '_co__state'.
Every function can only use CO__START() and CO__END() once.
*/

#define CO__START(ctx) switch ((ctx)._co__state) { case 0:
#define CO__END(ctx) } do { CO__FINISH_LABEL: (ctx)._co__state = -1; } while (0)

#define CO__YIELD(ctx, ...) do { (ctx)._co__state = __LINE__; return __VA_ARGS__; case __LINE__:; } while (0)
#define CO__FINISH() goto CO__FINISH_LABEL

#define CO__INIT_CTX(ctx) ((ctx)._co__state = 0)
#define CO__IS_FINISHED(ctx) ((ctx)._co__state < 0)

#endif /* COROUTINE_H */
