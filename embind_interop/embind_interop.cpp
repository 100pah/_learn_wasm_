#include <iostream>
#include <emscripten/bind.h>
#include <emscripten/val.h>

struct Point2f {
    float x;
    float y;
};

struct PersonRecord {
    std::string name;
    int age;
};

PersonRecord sample_fn(Point2f pt) {
    std::cout << "[cpp] pt.x: " << pt.x << ", pt.y: " << pt.y << std::endl;
    return {
        .name = "MyName",
        .age = 100
    };
}

thread_local static size_t my_buffer_size = 100;
thread_local static unsigned char* my_buffer = new unsigned char[my_buffer_size];
emscripten::val get_my_buffer() {
    /// 得到 Uint8Array （因为 my_buffer 是 unsigned char, 如果是 char 则得到 Int8Array ）
    emscripten::val buffer_val = emscripten::val(emscripten::typed_memory_view(my_buffer_size, my_buffer));
    return buffer_val;
}
void print_my_buffer() {
    std::cout << "[cpp] print_my_buffer: ";
    for (size_t i = 0; i < my_buffer_size; ++i) {
        std::cout << static_cast<unsigned int>(*(my_buffer + i)) << " ";
    }
    std::cout << std::endl;
}
void print_given_typed_array_view(emscripten::val view) {
}

std::vector<PersonRecord> return_my_vector() {
    std::vector<PersonRecord> vec(10, {"haha", 123});
    return vec;
}

std::map<int, PersonRecord> return_my_map() {
    std::map<int, PersonRecord> my_map;
    my_map.insert(std::pair<int, PersonRecord>(10, {"haha", 123}));
    my_map.insert(std::pair<int, PersonRecord>(20, {"haha2", 987}));
    return my_map;
}


/// 如果要转成 JS string 没法用 char* 或者 char[] ，只能用 std::string
static const std::string SOME_CONSTANT_STR = "This is some constant string";
static const int SOME_CONSTANT_INT = 12345;

enum class NewStyle {
    ONE,
    TWO
};

class MyClass {
  public:
    MyClass(int x, std::string y) : x(x), y(y) {}
    ~MyClass() {
        std::cout << "MyClass destructor called" << std::endl;
    }
    void incrementX() {
        ++x;
    }
    int getX() const { return x; }
    void setX(int x_) { x = x_; }
    /// std::string 自动映射为 JS string
    /// 必须用引用，除非 allow_raw_pointers()
    static std::string get_str_from_instance(const MyClass& instance) {
        return instance.y;
    }
    /// 要用 allow_raw_pointers()
    static std::string pass_raw_pointer(MyClass* instance) {
        return instance->y;
    }
  private:
    int x;
    std::string y;
};

// Binding code
EMSCRIPTEN_BINDINGS(my_class_example) {
    /// 把 JS 的 array (tuple) 转成 struct （用起来应该都是拷贝赋值，不 heap ）
    emscripten::value_array<Point2f>("Point2f")
        .element(&Point2f::x)
        .element(&Point2f::y)
        ;

    /// 把 JS 的 object 转成 strcut （用起来应该都是拷贝赋值，不 heap ）
    emscripten::value_object<PersonRecord>("PersonRecord")
        .field("name", &PersonRecord::name)
        .field("age", &PersonRecord::age)
        ;

    emscripten::function("sampleFn", &sample_fn);
    emscripten::function("getMyBuffer", &get_my_buffer);
    emscripten::function("printMyBuffer", &print_my_buffer);
    emscripten::function("printGivenTypeArrayView", &print_given_typed_array_view);
    emscripten::function("returnMyVector", &return_my_vector);
    emscripten::function("returnMyMap", &return_my_map);

    /// vector 和 map 要主动注册，参数是 class name
    emscripten::register_vector<PersonRecord>("my_vector<PersonRecord>");
    emscripten::register_map<int, PersonRecord>("my_map<int, PersonRecord>");
    emscripten::register_vector<int>("my_vector<int>"); // map key type must be registered

    emscripten::constant("SOME_CONSTANT_STR", SOME_CONSTANT_STR);
    emscripten::constant("SOME_CONSTANT_INT", SOME_CONSTANT_INT);

    emscripten::enum_<NewStyle>("NewStyle")
        .value("ONE", NewStyle::ONE)
        .value("TWO", NewStyle::TWO)
        ;

    emscripten::class_<MyClass>("MyClass")
        /// 另外， 工厂模式的 constructor 使用方式见：
        /// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#external-constructors
        /// 另外， smart pointer 式的 constructor 方式见：
        /// https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#smart-pointers
        .constructor<int, std::string>()
        .function("incrementX", &MyClass::incrementX)
        .property("x", &MyClass::getX, &MyClass::setX)
        .class_function("getStrFromInstance", &MyClass::get_str_from_instance)
        .class_function("passRawPointer", &MyClass::pass_raw_pointer, emscripten::allow_raw_pointers())
        ;
}
