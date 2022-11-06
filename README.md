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
r2k@DESKTOP-FT60JBF:~/ecat_test$ cmake --build build/ && ./build/bin/ecat_test --filename conf/master_config.xml
Scanning dependencies of target xml_conf_parser
[ 16%] Building C object src/xml_conf_parser/CMakeFiles/xml_conf_parser.dir/xml_conf_parser.c.o
[ 33%] Linking C static library ../../lib/libxml_conf_parser.a
[ 33%] Built target xml_conf_parser
[ 66%] Built target parser
[ 83%] Linking C executable ../bin/ecat_test
[100%] Built target ecat_test
ai.filename_arg: conf/master_config.xml
ai.verbose_flag: 0
ai.index_arg: 0
parsing file conf/master_config.xml
Root is <EtherCATInfoList> (1)
         parsing slave EtherCATInfo
         vendor id 2
         parsing descriptions
         EK1814 product code #x07162c52, rev #x00120000
         EK1814 EtherCAT-EA-Koppler (1A E-Bus, 4 K. Dig. Ein, 3ms, 4 K.
         RxPdos 4 | TxPdos 4
         RxPdo 0 1 1 #x1608 Channel 5 #x7080 1 1 Output BOOL
         RxPdo 0 1 1 #x1608 Channel 5 #x7080 1 1 Output BOOL
         RxPdo 0 1 1 #x1608 Channel 5 #x7080 1 1 Output BOOL
         RxPdo 0 1 1 #x1608 Channel 5 #x7080 1 1 Output BOOL
         TxPdo 1 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 1 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 1 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 1 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         parsing slave EtherCATInfo
         vendor id 2
         parsing descriptions
         EL1018 product code #x03fa3052, rev #x00120000
         EL1018 8K. Dig. Eingang 24V, 10�s
         RxPdos 0 | TxPdos 8
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         parsing slave EtherCATInfo
         vendor id 2
         parsing descriptions
         EL1018 product code #x03fa3052, rev #x00120000
         EL1018 8K. Dig. Eingang 24V, 10�s
         RxPdos 0 | TxPdos 8
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
         parsing slave EtherCATInfo
         vendor id 2
         parsing descriptions
         EL2008 product code #x07d83052, rev #x00120000
         EL2008 8K. Dig. Ausgang 24V, 0.5A
         RxPdos 8 | TxPdos 0
         RxPdo 0 1 1 #x1600 Channel 1 #x7000 1 1 Output BOOL
         RxPdo 0 1 1 #x1600 Channel 1 #x7000 1 1 Output BOOL
         RxPdo 0 1 1 #x1600 Channel 1 #x7000 1 1 Output BOOL
         RxPdo 0 1 1 #x1600 Channel 1 #x7000 1 1 Output BOOL
         RxPdo 0 1 1 #x1600 Channel 1 #x7000 1 1 Output BOOL
         RxPdo 0 1 1 #x1600 Channel 1 #x7000 1 1 Output BOOL
         RxPdo 0 1 1 #x1600 Channel 1 #x7000 1 1 Output BOOL
         RxPdo 0 1 1 #x1600 Channel 1 #x7000 1 1 Output BOOL
...
```