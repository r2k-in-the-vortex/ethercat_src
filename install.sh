mkdir -p build
cmake -S . -B build/
cmake --build build/
cmake --install build/
/sbin/ldconfig -v | grep ethercat