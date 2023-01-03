
Module.onRuntimeInitialized = function () {
    my_case_get_long_from_c();
    my_case_get_str_list_from_c();
    my_case_send_str_to_c();
};

/// [case] JS malloc， C 写入 long，JS 读取。
function my_case_get_long_from_c() {
    var longPtr = Module._malloc(8); // 8 bytes 即 i64 即 long*
    Module.ccall('my_write_long_in_c', 'pointer', [], [longPtr]);
    var longValue = Module.getValue(longPtr, 'i64');
    console.log('[JS] longValue', longValue); /// 12345
}

/// [case] C malloc， C 写入 string，JS 读取。返回多个参数，返回 list 。
function my_case_get_str_list_from_c() {
    function getStrListFromC() {
        var strPtrListPtrPtr = Module._malloc(8);
        var strPtrListLenPtr = Module._malloc(8);
        Module.ccall(
            'my_return_str_list_from_c',
            null,
            ['pointer', 'pointer'],
            [strPtrListPtrPtr, strPtrListLenPtr]
        );
        var strPtrListPtr = Module.getValue(strPtrListPtrPtr, 'i64');
        var strPtrListLen = Module.getValue(strPtrListLenPtr, 'i64');
        var strList = [];
        for (var i = 0; i < strPtrListLen; i++) {
            var strPtr = Module.getValue(strPtrListPtr + i * 8, 'i64');
            var str = Module.UTF8ToString(strPtr);
            strList.push(str);
            Module._free(strPtr);
        }
        Module._free(strPtrListPtrPtr);
        Module._free(strPtrListLenPtr);

        return strList;
    }
    console.log(getStrListFromC());
}

/// [case] JS malloc，JS 写入 string，C 读取
function my_case_send_str_to_c() {
    var myStr1 = 'This is a literal string declared in JS';
    // at most 4 bytes per UTF-8 code point, +1 for the trailing '\0'
    var myStr1ByteLenMax = (myStr1.length << 2) + 1;
    var myStrPtr = Module._malloc(myStr1ByteLenMax);
    Module.stringToUTF8(myStr1, myStrPtr, myStr1ByteLenMax);
    Module.ccall('my_send_str_to_c', null, 'pointer', [myStrPtr]);
    Module._free(myStrPtr);
}
