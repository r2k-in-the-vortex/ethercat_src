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

<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" version="1.1" width="1582px" viewBox="-0.5 -0.5 1582 549" content="&lt;mxfile&gt;&lt;diagram id=&quot;O1-QmVV9RvIppVxTdCmx&quot; name=&quot;Page-1&quot;&gt;&lt;mxGraphModel dx=&quot;1987&quot; dy=&quot;854&quot; grid=&quot;1&quot; gridSize=&quot;10&quot; guides=&quot;1&quot; tooltips=&quot;1&quot; connect=&quot;1&quot; arrows=&quot;1&quot; fold=&quot;1&quot; page=&quot;1&quot; pageScale=&quot;1&quot; pageWidth=&quot;850&quot; pageHeight=&quot;1100&quot; math=&quot;0&quot; shadow=&quot;0&quot;&gt;&lt;root&gt;&lt;mxCell id=&quot;0&quot;/&gt;&lt;mxCell id=&quot;1&quot; parent=&quot;0&quot;/&gt;&lt;mxCell id=&quot;2&quot; value=&quot;&quot; style=&quot;endArrow=none;dashed=1;dashPattern=1 3;strokeWidth=2;&quot; edge=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry width=&quot;50&quot; height=&quot;50&quot; relative=&quot;1&quot; as=&quot;geometry&quot;&gt;&lt;mxPoint x=&quot;239&quot; y=&quot;440&quot; as=&quot;sourcePoint&quot;/&gt;&lt;mxPoint x=&quot;239&quot; y=&quot;240&quot; as=&quot;targetPoint&quot;/&gt;&lt;/mxGeometry&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;6&quot; value=&quot;&quot; style=&quot;edgeStyle=none;exitX=1;exitY=0.5;exitDx=0;exitDy=0;entryX=0;entryY=0.5;entryDx=0;entryDy=0;&quot; edge=&quot;1&quot; parent=&quot;1&quot; source=&quot;3&quot; target=&quot;5&quot;&gt;&lt;mxGeometry relative=&quot;1&quot; as=&quot;geometry&quot;&gt;&lt;Array as=&quot;points&quot;&gt;&lt;mxPoint x=&quot;400&quot; y=&quot;410&quot;/&gt;&lt;mxPoint x=&quot;440&quot; y=&quot;350&quot;/&gt;&lt;/Array&gt;&lt;/mxGeometry&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;3&quot; value=&quot;Read Inputs&quot; style=&quot;rounded=0;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;240&quot; y=&quot;380&quot; width=&quot;120&quot; height=&quot;60&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;13&quot; value=&quot;&quot; style=&quot;edgeStyle=none;exitX=0.5;exitY=1;exitDx=0;exitDy=0;entryX=0;entryY=0.5;entryDx=0;entryDy=0;dashed=1;&quot; edge=&quot;1&quot; parent=&quot;1&quot; source=&quot;23&quot;&gt;&lt;mxGeometry relative=&quot;1&quot; as=&quot;geometry&quot;&gt;&lt;mxPoint x=&quot;-10&quot; y=&quot;320&quot; as=&quot;sourcePoint&quot;/&gt;&lt;mxPoint x=&quot;240&quot; y=&quot;410&quot; as=&quot;targetPoint&quot;/&gt;&lt;Array as=&quot;points&quot;&gt;&lt;mxPoint x=&quot;-130&quot; y=&quot;640&quot;/&gt;&lt;mxPoint x=&quot;250&quot; y=&quot;640&quot;/&gt;&lt;mxPoint x=&quot;250&quot; y=&quot;600&quot;/&gt;&lt;mxPoint x=&quot;30&quot; y=&quot;600&quot;/&gt;&lt;mxPoint x=&quot;30&quot; y=&quot;450&quot;/&gt;&lt;mxPoint x=&quot;70&quot; y=&quot;410&quot;/&gt;&lt;/Array&gt;&lt;/mxGeometry&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;4&quot; value=&quot;Write Outputs&quot; style=&quot;rounded=0;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;360&quot; y=&quot;260&quot; width=&quot;120&quot; height=&quot;60&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;5&quot; value=&quot;Calculate PLC&quot; style=&quot;rounded=0;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;480&quot; y=&quot;320&quot; width=&quot;120&quot; height=&quot;60&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;7&quot; value=&quot;&quot; style=&quot;endArrow=none;dashed=1;dashPattern=1 3;strokeWidth=2;&quot; edge=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry width=&quot;50&quot; height=&quot;50&quot; relative=&quot;1&quot; as=&quot;geometry&quot;&gt;&lt;mxPoint x=&quot;670&quot; y=&quot;440&quot; as=&quot;sourcePoint&quot;/&gt;&lt;mxPoint x=&quot;670&quot; y=&quot;240&quot; as=&quot;targetPoint&quot;/&gt;&lt;/mxGeometry&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;8&quot; value=&quot;&quot; style=&quot;edgeStyle=none;exitX=1;exitY=0.5;exitDx=0;exitDy=0;entryX=0;entryY=0.5;entryDx=0;entryDy=0;&quot; edge=&quot;1&quot; parent=&quot;1&quot; source=&quot;9&quot; target=&quot;11&quot;&gt;&lt;mxGeometry relative=&quot;1&quot; as=&quot;geometry&quot;&gt;&lt;Array as=&quot;points&quot;&gt;&lt;mxPoint x=&quot;830&quot; y=&quot;410&quot;/&gt;&lt;mxPoint x=&quot;870&quot; y=&quot;350&quot;/&gt;&lt;/Array&gt;&lt;/mxGeometry&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;9&quot; value=&quot;Read Inputs&quot; style=&quot;rounded=0;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;670&quot; y=&quot;380&quot; width=&quot;120&quot; height=&quot;60&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;10&quot; value=&quot;Write Outputs&quot; style=&quot;rounded=0;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;790&quot; y=&quot;260&quot; width=&quot;120&quot; height=&quot;60&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;11&quot; value=&quot;Calculate PLC&quot; style=&quot;rounded=0;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;910&quot; y=&quot;320&quot; width=&quot;120&quot; height=&quot;60&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;12&quot; value=&quot;&quot; style=&quot;edgeStyle=none;exitX=1;exitY=0.5;exitDx=0;exitDy=0;entryX=0;entryY=0.5;entryDx=0;entryDy=0;&quot; edge=&quot;1&quot; parent=&quot;1&quot; source=&quot;15&quot;&gt;&lt;mxGeometry relative=&quot;1&quot; as=&quot;geometry&quot;&gt;&lt;mxPoint x=&quot;670&quot; y=&quot;350&quot; as=&quot;sourcePoint&quot;/&gt;&lt;mxPoint x=&quot;790&quot; y=&quot;290&quot; as=&quot;targetPoint&quot;/&gt;&lt;Array as=&quot;points&quot;&gt;&lt;mxPoint x=&quot;710&quot; y=&quot;350&quot;/&gt;&lt;mxPoint x=&quot;750&quot; y=&quot;290&quot;/&gt;&lt;/Array&gt;&lt;/mxGeometry&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;14&quot; value=&quot;EtherCAT device chain&quot; style=&quot;text;strokeColor=none;fillColor=none;align=left;verticalAlign=middle;rounded=0;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry y=&quot;610&quot; width=&quot;220&quot; height=&quot;30&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;15&quot; value=&quot;&quot; style=&quot;rounded=0;dashed=1;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;600&quot; y=&quot;320&quot; width=&quot;40&quot; height=&quot;60&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;16&quot; value=&quot;Constant time&quot; style=&quot;text;strokeColor=none;fillColor=none;align=left;verticalAlign=top;rounded=0;dashed=1;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;240&quot; y=&quot;210&quot; width=&quot;120&quot; height=&quot;30&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;17&quot; value=&quot;Constant time&quot; style=&quot;text;strokeColor=none;fillColor=none;align=left;verticalAlign=top;rounded=0;dashed=1;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;360&quot; y=&quot;210&quot; width=&quot;120&quot; height=&quot;30&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;18&quot; value=&quot;Variable time&quot; style=&quot;text;strokeColor=none;fillColor=none;align=left;verticalAlign=top;rounded=0;dashed=1;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;480&quot; y=&quot;210&quot; width=&quot;120&quot; height=&quot;30&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;19&quot; value=&quot;Idle time&quot; style=&quot;text;strokeColor=none;fillColor=none;align=left;verticalAlign=top;rounded=0;dashed=1;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;600&quot; y=&quot;210&quot; width=&quot;70&quot; height=&quot;30&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;20&quot; value=&quot;&quot; style=&quot;endArrow=none;dashed=1;dashPattern=1 3;strokeWidth=2;&quot; edge=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry width=&quot;50&quot; height=&quot;50&quot; relative=&quot;1&quot; as=&quot;geometry&quot;&gt;&lt;mxPoint x=&quot;-190&quot; y=&quot;440&quot; as=&quot;sourcePoint&quot;/&gt;&lt;mxPoint x=&quot;-190&quot; y=&quot;240&quot; as=&quot;targetPoint&quot;/&gt;&lt;/mxGeometry&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;23&quot; value=&quot;Write Outputs&quot; style=&quot;rounded=0;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;-190&quot; y=&quot;260&quot; width=&quot;120&quot; height=&quot;60&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;24&quot; value=&quot;Calculate PLC&quot; style=&quot;rounded=0;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;-70&quot; y=&quot;320&quot; width=&quot;120&quot; height=&quot;60&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;25&quot; value=&quot;&quot; style=&quot;edgeStyle=none;exitX=1;exitY=0.5;exitDx=0;exitDy=0;entryX=0;entryY=0.5;entryDx=0;entryDy=0;&quot; edge=&quot;1&quot; parent=&quot;1&quot; source=&quot;24&quot;&gt;&lt;mxGeometry relative=&quot;1&quot; as=&quot;geometry&quot;&gt;&lt;mxPoint x=&quot;240&quot; y=&quot;350&quot; as=&quot;sourcePoint&quot;/&gt;&lt;mxPoint x=&quot;360&quot; y=&quot;290&quot; as=&quot;targetPoint&quot;/&gt;&lt;Array as=&quot;points&quot;&gt;&lt;mxPoint x=&quot;280&quot; y=&quot;350&quot;/&gt;&lt;mxPoint x=&quot;320&quot; y=&quot;290&quot;/&gt;&lt;/Array&gt;&lt;/mxGeometry&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;26&quot; value=&quot;&quot; style=&quot;endArrow=none;dashed=1;dashPattern=1 3;strokeWidth=2;&quot; edge=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry width=&quot;50&quot; height=&quot;50&quot; relative=&quot;1&quot; as=&quot;geometry&quot;&gt;&lt;mxPoint x=&quot;-550&quot; y=&quot;440&quot; as=&quot;sourcePoint&quot;/&gt;&lt;mxPoint x=&quot;-550&quot; y=&quot;240&quot; as=&quot;targetPoint&quot;/&gt;&lt;/mxGeometry&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;32&quot; value=&quot;&quot; style=&quot;edgeStyle=none;entryX=0;entryY=0.5;entryDx=0;entryDy=0;&quot; edge=&quot;1&quot; parent=&quot;1&quot; target=&quot;30&quot;&gt;&lt;mxGeometry relative=&quot;1&quot; as=&quot;geometry&quot;&gt;&lt;mxPoint x=&quot;-530&quot; y=&quot;350&quot; as=&quot;sourcePoint&quot;/&gt;&lt;/mxGeometry&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;30&quot; value=&quot;Calculate PLC&quot; style=&quot;rounded=0;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;-440&quot; y=&quot;320&quot; width=&quot;120&quot; height=&quot;60&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;31&quot; value=&quot;&quot; style=&quot;edgeStyle=none;exitX=1;exitY=0.5;exitDx=0;exitDy=0;&quot; edge=&quot;1&quot; parent=&quot;1&quot; source=&quot;30&quot;&gt;&lt;mxGeometry relative=&quot;1&quot; as=&quot;geometry&quot;&gt;&lt;mxPoint x=&quot;-190&quot; y=&quot;350&quot; as=&quot;sourcePoint&quot;/&gt;&lt;mxPoint x=&quot;-190&quot; y=&quot;290&quot; as=&quot;targetPoint&quot;/&gt;&lt;Array as=&quot;points&quot;&gt;&lt;mxPoint x=&quot;-280&quot; y=&quot;350&quot;/&gt;&lt;mxPoint x=&quot;-240&quot; y=&quot;290&quot;/&gt;&lt;/Array&gt;&lt;/mxGeometry&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;33&quot; value=&quot;Zero inputs&quot; style=&quot;text;strokeColor=none;fillColor=none;align=left;verticalAlign=middle;rounded=0;dashed=1;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;-530&quot; y=&quot;350&quot; width=&quot;60&quot; height=&quot;30&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;35&quot; value=&quot;Cycle 0&amp;#10;Need data for writing outputs&quot; style=&quot;text;strokeColor=none;fillColor=none;align=left;verticalAlign=bottom;rounded=0;dashed=1;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;-550&quot; y=&quot;100&quot; width=&quot;240&quot; height=&quot;100&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;36&quot; value=&quot;Cycle 1&amp;#10;Need one EtherCAT frame sent before there is any inputs to read&quot; style=&quot;text;strokeColor=none;fillColor=none;align=left;verticalAlign=bottom;rounded=0;dashed=1;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;-190&quot; y=&quot;100&quot; width=&quot;150&quot; height=&quot;100&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;37&quot; value=&quot;&quot; style=&quot;edgeStyle=none;entryX=0;entryY=0.5;entryDx=0;entryDy=0;exitX=0;exitY=0;exitDx=0;exitDy=0;&quot; edge=&quot;1&quot; parent=&quot;1&quot; source=&quot;38&quot; target=&quot;24&quot;&gt;&lt;mxGeometry relative=&quot;1&quot; as=&quot;geometry&quot;&gt;&lt;mxPoint x=&quot;-40&quot; y=&quot;350&quot; as=&quot;sourcePoint&quot;/&gt;&lt;mxPoint x=&quot;50&quot; y=&quot;350&quot; as=&quot;targetPoint&quot;/&gt;&lt;/mxGeometry&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;38&quot; value=&quot;Zero inputs&quot; style=&quot;text;strokeColor=none;fillColor=none;align=left;verticalAlign=middle;rounded=0;dashed=1;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;-170&quot; y=&quot;350&quot; width=&quot;60&quot; height=&quot;30&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;39&quot; value=&quot;Cycle 2&amp;#10;Normal operation&quot; style=&quot;text;strokeColor=none;fillColor=none;align=left;verticalAlign=bottom;rounded=0;dashed=1;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;240&quot; y=&quot;100&quot; width=&quot;240&quot; height=&quot;100&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;mxCell id=&quot;40&quot; value=&quot;Cycle N&amp;#10;Normal operation&quot; style=&quot;text;strokeColor=none;fillColor=none;align=left;verticalAlign=top;rounded=0;dashed=1;&quot; vertex=&quot;1&quot; parent=&quot;1&quot;&gt;&lt;mxGeometry x=&quot;670&quot; y=&quot;440&quot; width=&quot;240&quot; height=&quot;100&quot; as=&quot;geometry&quot;/&gt;&lt;/mxCell&gt;&lt;/root&gt;&lt;/mxGraphModel&gt;&lt;/diagram&gt;&lt;/mxfile&gt;" onclick="(function(svg){var src=window.event.target||window.event.srcElement;while (src!=null&amp;&amp;src.nodeName.toLowerCase()!='a'){src=src.parentNode;}if(src==null){if(svg.wnd!=null&amp;&amp;!svg.wnd.closed){svg.wnd.focus();}else{var r=function(evt){if(evt.data=='ready'&amp;&amp;evt.source==svg.wnd){svg.wnd.postMessage(decodeURIComponent(svg.getAttribute('content')),'*');window.removeEventListener('message',r);}};window.addEventListener('message',r);svg.wnd=window.open('https://viewer.diagrams.net/?client=1&amp;page=0&amp;edit=_blank');}}})(this);" style="cursor:pointer;max-width:100%;max-height:549px;"><defs/><g><path d="M 790 340 L 790 140" fill="none" stroke="rgb(0, 0, 0)" stroke-width="2" stroke-miterlimit="10" stroke-dasharray="2 6" pointer-events="stroke"/><path d="M 911 310 L 941 310 Q 951 310 956.55 301.68 L 985.45 258.32 Q 991 250 1001 250 L 1024.63 250" fill="none" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="stroke"/><path d="M 1029.88 250 L 1022.88 253.5 L 1024.63 250 L 1022.88 246.5 Z" fill="rgb(0, 0, 0)" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="all"/><rect x="791" y="280" width="120" height="60" fill="rgb(255, 255, 255)" stroke="rgb(0, 0, 0)" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" text-anchor="middle" font-size="12px"><text x="850.5" y="314.5">Read Inputs</text></g><path d="M 421 220 L 421 530 Q 421 540 431 540 L 791 540 Q 801 540 801 530 L 801 510 Q 801 500 791 500 L 591 500 Q 581 500 581 490 L 581 360 Q 581 350 588.07 342.93 L 613.93 317.07 Q 621 310 631 310 L 784.63 310" fill="none" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" stroke-dasharray="3 3" pointer-events="stroke"/><path d="M 789.88 310 L 782.88 313.5 L 784.63 310 L 782.88 306.5 Z" fill="rgb(0, 0, 0)" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="all"/><rect x="911" y="160" width="120" height="60" fill="rgb(255, 255, 255)" stroke="rgb(0, 0, 0)" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" text-anchor="middle" font-size="12px"><text x="970.5" y="194.5">Write Outputs</text></g><rect x="1031" y="220" width="120" height="60" fill="rgb(255, 255, 255)" stroke="rgb(0, 0, 0)" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" text-anchor="middle" font-size="12px"><text x="1090.5" y="254.5">Calculate PLC</text></g><path d="M 1221 340 L 1221 140" fill="none" stroke="rgb(0, 0, 0)" stroke-width="2" stroke-miterlimit="10" stroke-dasharray="2 6" pointer-events="stroke"/><path d="M 1341 310 L 1371 310 Q 1381 310 1386.55 301.68 L 1415.45 258.32 Q 1421 250 1431 250 L 1454.63 250" fill="none" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="stroke"/><path d="M 1459.88 250 L 1452.88 253.5 L 1454.63 250 L 1452.88 246.5 Z" fill="rgb(0, 0, 0)" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="all"/><rect x="1221" y="280" width="120" height="60" fill="rgb(255, 255, 255)" stroke="rgb(0, 0, 0)" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" text-anchor="middle" font-size="12px"><text x="1280.5" y="314.5">Read Inputs</text></g><rect x="1341" y="160" width="120" height="60" fill="rgb(255, 255, 255)" stroke="rgb(0, 0, 0)" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" text-anchor="middle" font-size="12px"><text x="1400.5" y="194.5">Write Outputs</text></g><rect x="1461" y="220" width="120" height="60" fill="rgb(255, 255, 255)" stroke="rgb(0, 0, 0)" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" text-anchor="middle" font-size="12px"><text x="1520.5" y="254.5">Calculate PLC</text></g><path d="M 1191 250 L 1251 250 Q 1261 250 1266.55 241.68 L 1295.45 198.32 Q 1301 190 1311 190 L 1334.63 190" fill="none" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="stroke"/><path d="M 1339.88 190 L 1332.88 193.5 L 1334.63 190 L 1332.88 186.5 Z" fill="rgb(0, 0, 0)" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="all"/><rect x="551" y="510" width="220" height="30" fill="none" stroke="none" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" font-size="12px"><text x="552.5" y="529.5">EtherCAT device chain</text></g><rect x="1151" y="220" width="40" height="60" fill="rgb(255, 255, 255)" stroke="rgb(0, 0, 0)" stroke-dasharray="3 3" pointer-events="all"/><rect x="791" y="110" width="120" height="30" fill="none" stroke="none" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" font-size="12px"><text x="792.5" y="127.5">Constant time</text></g><rect x="911" y="110" width="120" height="30" fill="none" stroke="none" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" font-size="12px"><text x="912.5" y="127.5">Constant time</text></g><rect x="1031" y="110" width="120" height="30" fill="none" stroke="none" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" font-size="12px"><text x="1032.5" y="127.5">Variable time</text></g><rect x="1151" y="110" width="70" height="30" fill="none" stroke="none" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" font-size="12px"><text x="1152.5" y="127.5">Idle time</text></g><path d="M 361 340 L 361 140" fill="none" stroke="rgb(0, 0, 0)" stroke-width="2" stroke-miterlimit="10" stroke-dasharray="2 6" pointer-events="stroke"/><rect x="361" y="160" width="120" height="60" fill="rgb(255, 255, 255)" stroke="rgb(0, 0, 0)" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" text-anchor="middle" font-size="12px"><text x="420.5" y="194.5">Write Outputs</text></g><rect x="481" y="220" width="120" height="60" fill="rgb(255, 255, 255)" stroke="rgb(0, 0, 0)" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" text-anchor="middle" font-size="12px"><text x="540.5" y="254.5">Calculate PLC</text></g><path d="M 601 250 L 821 250 Q 831 250 836.55 241.68 L 865.45 198.32 Q 871 190 881 190 L 904.63 190" fill="none" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="stroke"/><path d="M 909.88 190 L 902.88 193.5 L 904.63 190 L 902.88 186.5 Z" fill="rgb(0, 0, 0)" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="all"/><path d="M 1 340 L 1 140" fill="none" stroke="rgb(0, 0, 0)" stroke-width="2" stroke-miterlimit="10" stroke-dasharray="2 6" pointer-events="stroke"/><path d="M 21 250 L 104.63 250" fill="none" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="stroke"/><path d="M 109.88 250 L 102.88 253.5 L 104.63 250 L 102.88 246.5 Z" fill="rgb(0, 0, 0)" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="all"/><rect x="111" y="220" width="120" height="60" fill="rgb(255, 255, 255)" stroke="rgb(0, 0, 0)" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" text-anchor="middle" font-size="12px"><text x="170.5" y="254.5">Calculate PLC</text></g><path d="M 231 250 L 261 250 Q 271 250 276.55 241.68 L 305.45 198.32 Q 311 190 321 190 L 354.63 190" fill="none" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="stroke"/><path d="M 359.88 190 L 352.88 193.5 L 354.63 190 L 352.88 186.5 Z" fill="rgb(0, 0, 0)" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="all"/><rect x="21" y="250" width="60" height="30" fill="none" stroke="none" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" font-size="12px"><text x="22.5" y="269.5">Zero inputs</text></g><rect x="1" y="0" width="240" height="100" fill="none" stroke="none" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" font-size="12px"><text x="2.5" y="80.5">Cycle 0</text><text x="2.5" y="94.5">Need data for writing outputs</text></g><rect x="361" y="0" width="150" height="100" fill="none" stroke="none" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" font-size="12px"><text x="362.5" y="80.5">Cycle 1</text><text x="362.5" y="94.5">Need one EtherCAT frame sent before there is any inputs to read</text></g><path d="M 381 250 L 474.63 250" fill="none" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="stroke"/><path d="M 479.88 250 L 472.88 253.5 L 474.63 250 L 472.88 246.5 Z" fill="rgb(0, 0, 0)" stroke="rgb(0, 0, 0)" stroke-miterlimit="10" pointer-events="all"/><rect x="381" y="250" width="60" height="30" fill="none" stroke="none" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" font-size="12px"><text x="382.5" y="269.5">Zero inputs</text></g><rect x="791" y="0" width="240" height="100" fill="none" stroke="none" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" font-size="12px"><text x="792.5" y="80.5">Cycle 2</text><text x="792.5" y="94.5">Normal operation</text></g><rect x="1221" y="340" width="240" height="100" fill="none" stroke="none" pointer-events="all"/><g fill="rgb(0, 0, 0)" font-family="Helvetica" font-size="12px"><text x="1222.5" y="357.5">Cycle N</text><text x="1222.5" y="371.5">Normal operation</text></g></g></svg>

   - PLC default output data is needed to send a frame
   - master is the only one that can send a frame, it loops back and only after that can master read data
   - writing frame and reading frame are constant time operations
   - PLC task is bounded in realtime, but not constant time, some code paths take longer than others
   - logically, PLC task must be last in cycle
   - read task reads frame sent in last cycle, this task is constant time
   - write task writes frame calculated in last cycle, this is constant time and timing of that frame must be as consistent as possible
   - this necessitates the illustrated initialization logic


