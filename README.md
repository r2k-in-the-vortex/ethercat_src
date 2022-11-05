# ecat_test

![example workflow](https://github.com/r2k-in-the-vortex/ecat_test/actions/workflows/c-cpp.yml/badge.svg)

For basic project structure refer to https://github.com/sun1211/cmake_with_add_subdirectory

Generic plan:

 - etherlabs EtherCAT master https://gitlab.com/etherlab.org/ethercat.git
 - to be ultimately mated with https://openplcproject.com/
 - sudo ethercat xml > master_config.xml
 - parse EtherCAT master xml, originating from master_config.xml, --filename
 - config slaves
 - config RxPDOs
 - config TxPDOs
 - separately getting link copnfiguration to and from plc
 - start cyclic
 - classic read inputs > calculate plc logic > write outputs

necessary
 
 sudo apt-get install libxml2-dev

 mkdir build

 cmake -S . -B build/

 cmake --build build/

 ./build/bin/ecat_test --filename conf/master_config.xml 

 outputs
```
ai.filename_arg: conf/master_config.xml
ai.verbose_flag: 0
ai.index_arg: 0
parsing file conf/master_config.xml
Root is <EtherCATInfoList> (1)
         parsing slave EtherCATInfo
         vendor id 2
         parsing descriptions
         EK1814 product code #x07162c52, rev #x00120000
         parsing slave EtherCATInfo
         vendor id 2
         parsing descriptions
         EL1018 product code #x03fa3052, rev #x00120000
         parsing slave EtherCATInfo
         vendor id 2
         parsing descriptions
         EL1018 product code #x03fa3052, rev #x00120000
         parsing slave EtherCATInfo
         vendor id 2
         parsing descriptions
         EL2008 product code #x07d83052, rev #x00120000
```