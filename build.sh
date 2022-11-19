mkdir -p build
cp -n conf/ethercat.cfg build/ethercat.cfg
cp -n conf/master_config.xml build/master_config.xml
cmake -S . -B build/
cmake --build build/