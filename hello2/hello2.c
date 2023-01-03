#include <stdio.h>
#include <emscripten/emscripten.h>

#ifdef __cplusplus
extern "C" {
#endif
///
/// EMSCRIPTEN_KEEPALIVE 的意思：
/// By default, Emscripten-generated code always just calls the main() function, and other functions are eliminated as dead code. Putting EMSCRIPTEN_KEEPALIVE before a function name stops this from happening. You also need to import the emscripten.h library to use EMSCRIPTEN_KEEPALIVE.
///
int EMSCRIPTEN_KEEPALIVE myFunction(int argc, char ** argv) {
    printf("我的函数已被调用\n");
    return 0;
}
///
#ifdef __cplusplus
}
#endif

EM_JS(int, hahaha, (int x, int y), { return 2 * x + y; })


#ifdef __cplusplus
extern "C" {
#endif
/// 只声明方法。
extern void my_js_impl_fn(void);
#ifdef __cplusplus
}
#endif


int main(int argc, char ** argv) {
    printf("main Hello World\n");
    int result = hahaha(15, 20);
    printf("call hahaha get result %d\n", result);

    my_js_impl_fn();

    printf("All done\n");
}

