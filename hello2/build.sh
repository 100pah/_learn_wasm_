mkdir -p out
emcc -o out/hello2.html hello2.c \
    --js-library my_js_library.js \
    --shell-file html_template/my_shell.html \
    -s"EXPORTED_RUNTIME_METHODS=['ccall']" \
    -s"EXPORTED_FUNCTIONS=['_main', '_free', '_malloc']"\
    -s"ALLOW_MEMORY_GROWTH=1"
