
Module.onRuntimeInitialized = function () {
    my_case_bind_fn();
    my_case_bind_constant();
    my_case_bind_enum();
    my_case_bind_class();
};

function my_case_bind_fn() {
    var person = Module.sampleFn([10.2, 156.5]);
    console.log('[JS] Their name is ' + person.name + ' and they are ' + person.age + ' years old');

    var buffer = Module.getMyBuffer();
    console.log('[JS] myBuffer: ', buffer); /// buffer: Uint8Array
    for (var i = 0; i < buffer.length; i++) {
        buffer[i] = i;
    }
    Module.printMyBuffer();
    Module.printGivenTypeArrayView();

    var vec = window.myVec = Module.returnMyVector();
    for (var i = 0; i < vec.size(); i++) {
        console.log('vec: ' + i + ' ' + JSON.stringify(vec.get(i)));
    }

    var myMap = window.myMap = Module.returnMyMap();
    var keys = myMap.keys();
    for (var i = 0; i < keys.size(); i++) {
        var key = keys.get(i);
        var value = myMap.get(key);
        console.log('map item key: s' + key + ' value: ' + JSON.stringify(value));
    }
}

function my_case_bind_constant() {
    console.log('Module.SOME_CONSTANT_STR', Module.SOME_CONSTANT_STR);
    console.log('Module.SOME_CONSTANT_INT', Module.SOME_CONSTANT_INT);
}

function my_case_bind_enum() {
    console.log('Module.NewStyle.ONE', Module.NewStyle.ONE);
    console.log('Module.NewStyle.TWO', Module.NewStyle.TWO);
}

function my_case_bind_class() {
    var jsIns = new Module.MyClass(10, 'hello');
    jsIns.incrementX();
    console.log('jsIns.x: ' + jsIns.x); // 11
    jsIns.x = 20;
    console.log('jsIns.x: ' + jsIns.x); // 20
    var str = Module.MyClass.getStrFromInstance(jsIns);
    console.log('str: ' + str); // 'hello'
    var str2 = Module.MyClass.passRawPointer(jsIns);
    console.log('str2: ' + str2); // 'hello'

    /// clone 得到的东西共享 cpp instance ，只是 clone js instance
    /// （这一点 clone 这个命名有迷惑性）
    /// clone 得到的东西也要 delete （内部用 reference counting 来维护）
    var jsIns2WithTheSameCppIns = jsIns.clone(); // ref counting: 2
    jsIns2WithTheSameCppIns.x = 99;
    console.log('jsIns.x: ' + jsIns.x); // 99
    jsIns2WithTheSameCppIns.delete(); // ref counting: 1

    // 注意：需要手动 delete ！
    jsIns.delete(); // ref counting: 0, and delete cpp instance
}
