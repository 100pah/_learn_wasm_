mkdir -p out
emcc -o out/embind_interop.html embind_interop.cpp \
    -lembind \
    --post-js post_js.js \
    -s"EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap', 'getValue', 'setValue', 'stringToUTF8', 'UTF8ToString']" \
    -s"EXPORTED_FUNCTIONS=['_free', '_malloc']"\
    -s"ALLOW_MEMORY_GROWTH=1"

    # -g3 -gsource-map --source-map-base "http://localhost:8001/wasm-all/try/first_proj/embind_interop/" \