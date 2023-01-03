#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <string.h>
#include <emscripten/emscripten.h>

extern "C" void EMSCRIPTEN_KEEPALIVE my_write_long_in_c(long* long_ptr) {
    /// 传入的 long* ptr_alloc_in_js1 是 JS 里 malloc 得到的 ptr 。
    /// 这里用这个 ptr 直接向 linear memory 写入结果。
    *long_ptr = 12345l;
}
extern "C" void EMSCRIPTEN_KEEPALIVE my_return_str_list_from_c(
        uint64_t* str_ptr_list_ptr_ptr, uint64_t* str_ptr_list_len_ptr) {

    std::string str_list[] = {
        "first sentence in C.",
        "中文中文 third sentence in C.",
        "third sentence in C.",
    };
    *str_ptr_list_len_ptr = sizeof(str_list) / sizeof(str_list[0]);
    uint64_t* str_ptr_list_ptr = static_cast<uint64_t*>(::malloc(*str_ptr_list_len_ptr * sizeof(uint64_t)));
    *str_ptr_list_ptr_ptr = reinterpret_cast<uint64_t>(str_ptr_list_ptr);

    for (size_t i = 0; i < *str_ptr_list_len_ptr; ++i) {
        auto& str = str_list[i];
        /// 在 linear memory 里申请空间。
        char* str_ptr = static_cast<char*>(::malloc(str.length() + 1));
        strcpy(str_ptr, str.c_str());
        str_ptr_list_ptr[i] = reinterpret_cast<uint64_t>(str_ptr);
    }
}

extern "C" void EMSCRIPTEN_KEEPALIVE my_send_str_to_c(char* str_ptr) {
    std::cout << str_ptr << std::endl;
}

int main(int argc, char ** argv) {
    std::cout << "[cpp] main Hello World\n" << std::endl;
    return 0;
}

