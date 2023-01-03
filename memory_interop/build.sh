mkdir -p out
emcc -o out/memory_interop.html memory_interop.cpp \
    --post-js post_js.js \
    -s"EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap', 'getValue', 'setValue', 'stringToUTF8', 'UTF8ToString']" \
    -s"EXPORTED_FUNCTIONS=['_main', '_free', '_malloc']"\
    -s"ALLOW_MEMORY_GROWTH=1"

    # -g3 -gsource-map --source-map-base "http://localhost:8001/wasm-all/first_proj/memory_interop/" \