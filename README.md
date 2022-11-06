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

 sudo ./build/bin/ecat_test --filename conf/master_config.xml -v

 outputs
```
r2k@DESKTOP-FT60JBF:~/ecat_test$ cmake --build build/ && ./build/bin/ecat_test --filename conf/master_config.xml -v
[ 20%] Built target log
[ 40%] Built target parser
[ 60%] Built target xml_conf_parser
Scanning dependencies of target ecat_handler
[ 70%] Building C object src/ecat_handler/CMakeFiles/ecat_handler.dir/ecat_handler.c.o
[ 80%] Linking C static library ../../lib/libecat_handler.a
[ 80%] Built target ecat_handler
[ 90%] Linking C executable ../bin/ecat_test
[100%] Built target ecat_test
22:13:41 TRACE output
22:13:41 ai.filename_arg: conf/master_config.xml
22:13:41 ai.verbose_flag: 1
22:13:41 ai.index_arg: 0
22:13:41 parsing file conf/master_config.xml
22:13:41 Root is <EtherCATInfoList> (1)
22:13:41 parsing slave EtherCATInfo
22:13:41 vendor id 2
22:13:41 parsing descriptions
22:13:41 EK1814 product code #x07162c52, rev #x00120000
22:13:41 EK1814 EtherCAT-EA-Koppler (1A E-Bus, 4 K. Dig. Ein, 3ms, 4 K.
22:13:41 RxPdos 4 | TxPdos 4
22:13:41 TxPdo 1 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
22:13:41 TxPdo 1 1 1 #x1a01 Channel 2 #x6010 1 1 Input BOOL
22:13:41 TxPdo 1 1 1 #x1a02 Channel 3 #x6020 1 1 Input BOOL
22:13:41 TxPdo 1 1 1 #x1a03 Channel 4 #x6030 1 1 Input BOOL
22:13:41 RxPdo 0 1 1 #x1608 Channel 5 #x7080 1 1 Output BOOL
22:13:41 RxPdo 0 1 1 #x1609 Channel 6 #x7090 1 1 Output BOOL
22:13:41 RxPdo 0 1 1 #x160a Channel 7 #x70a0 1 1 Output BOOL
22:13:41 RxPdo 0 1 1 #x160b Channel 8 #x70b0 1 1 Output BOOL
22:13:41 parsing slave EtherCATInfo
22:13:41 vendor id 2
22:13:41 parsing descriptions
22:13:41 EL1018 product code #x03fa3052, rev #x00120000
22:13:41 EL1018 8K. Dig. Eingang 24V, 10�s
22:13:41 RxPdos 0 | TxPdos 8
22:13:41 TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a01 Channel 2 #x6010 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a02 Channel 3 #x6020 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a03 Channel 4 #x6030 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a04 Channel 5 #x6040 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a05 Channel 6 #x6050 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a06 Channel 7 #x6060 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a07 Channel 8 #x6070 1 1 Input BOOL
22:13:41 parsing slave EtherCATInfo
22:13:41 vendor id 2
22:13:41 parsing descriptions
22:13:41 EL1018 product code #x03fa3052, rev #x00120000
22:13:41 EL1018 8K. Dig. Eingang 24V, 10�s
22:13:41 RxPdos 0 | TxPdos 8
22:13:41 TxPdo 0 1 1 #x1a00 Channel 1 #x6000 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a01 Channel 2 #x6010 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a02 Channel 3 #x6020 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a03 Channel 4 #x6030 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a04 Channel 5 #x6040 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a05 Channel 6 #x6050 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a06 Channel 7 #x6060 1 1 Input BOOL
22:13:41 TxPdo 0 1 1 #x1a07 Channel 8 #x6070 1 1 Input BOOL
22:13:41 parsing slave EtherCATInfo
22:13:41 vendor id 2
22:13:41 parsing descriptions
22:13:41 EL2008 product code #x07d83052, rev #x00120000
22:13:41 EL2008 8K. Dig. Ausgang 24V, 0.5A
22:13:41 RxPdos 8 | TxPdos 0
22:13:41 RxPdo 0 1 1 #x1600 Channel 1 #x7000 1 1 Output BOOL
22:13:41 RxPdo 0 1 1 #x1601 Channel 2 #x7010 1 1 Output BOOL
22:13:41 RxPdo 0 1 1 #x1602 Channel 3 #x7020 1 1 Output BOOL
22:13:41 RxPdo 0 1 1 #x1603 Channel 4 #x7030 1 1 Output BOOL
22:13:41 RxPdo 0 1 1 #x1604 Channel 5 #x7040 1 1 Output BOOL
22:13:41 RxPdo 0 1 1 #x1605 Channel 6 #x7050 1 1 Output BOOL
22:13:41 RxPdo 0 1 1 #x1606 Channel 7 #x7060 1 1 Output BOOL
22:13:41 RxPdo 0 1 1 #x1607 Channel 8 #x7070 1 1 Output BOOL
22:13:41 Initializing EtherCAT ecrt_version_magic 67072
```


The idea with cyclic task and initialization is as follows

![cycle logic](https://github.com/r2k-in-the-vortex/ecat_test/blob/main/documentation/timing.png?raw=true)

   - PLC default output data is needed to send a frame
   - master is the only one that can send a frame, it loops back and only after that can master read data
   - writing frame and reading frame are constant time operations
   - PLC task is bounded in realtime, but not constant time, some code paths take longer than others
   - logically, PLC task must be last in cycle
   - read task reads frame sent in last cycle, this task is constant time
   - write task writes frame calculated in last cycle, this is constant time and timing of that frame must be as consistent as possible
   - this necessitates the illustrated initialization logic


