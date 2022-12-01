# ethercat_src ethercat connection for OpenPLC

![example workflow](https://github.com/r2k-in-the-vortex/ethercat_src/actions/workflows/c-cpp.yml/badge.svg)

ethercat_src ties Etherlabs IgH EtherCAT master to OpenPLC

links
 - etherlabs EtherCAT master https://gitlab.com/etherlab.org/ethercat.git
 - OpenPLC https://openplcproject.com/ https://github.com/thiagoralves/OpenPLC_v3

# setup instructions

ethercat_src can be built and installed independently of OpenPLC but is meant to be installed and work with it.

Reference hardware is Raspberry pi 4

Some dependancies should be checked to exist
````bash
sudo apt-get install udev
sudo apt-get install libxml2-dev
```

to install ethercat capable branch of OpenPLC 
```bash
git clone https://github.com/r2k-in-the-vortex/OpenPLC_v3
cd OpenPLC_v3
```

ethercat_src is arranged as submodules so these need to be fetched

```bash
git submodule init
git submodule update
cd utils/ethercat_src

git submodule init
git submodule update
cd external/ethercat
```

now Etherlabs IgH EtherCAT master needs to be built and installed
```bash
./bootstrap
./configure --sysconfdir=/etc --enable-8139too=no
make
make modules

sudo make install
sudo make modules_install install
sudo depmod
```

configured to correct ethernet adapter, first figure out mac address of interface you wish to use (ifconfig)

and then configure ethercat

```bash
sudo nano /etc/ethercat.conf
```

set correct mac address to `MASTER0_DEVICE="xx:xx:xx:xx:xx:xx"` and set `DEVICE_MODULES="generic"` unless you have a native driver available
save the file and exit

a device reboot at this point is recommended

After startup etherlabs master should be running, this can be verified by `ls dev/EtherCAT0` 
and by infomation from `sudo ethercat master` which shows state of master interface. `ethercat --help` for more information

`sudo ethercat rescan` and `sudo ethercat xml` obtains ethercat slave configuration which needs to be given to ethercat_src

At this point, OpenPLC may be built and installed

cd to OpenPLC_v3 source folder and `./install.sh linux` for regular OpenPLC runtime install.

ethercat_src is controlled by conf files, in the OpenPLC folder `OpenPLC_v3/utils/ethercat_src/build/ethercat.cfg` are the main options

xml file is the previously mentioned slave configuration.

Now OpenPLC can be started, on startup OpenPLC will print out (also in web ide) the pdos - input / output variables it has on ethercat

e.g.
```bash
Slave0_Channel_1 AT %IX0.0 : BOOL; (* EK1814 EtherCAT-EA-Koppler (1A E-Bus, 4 K. Dig. Ein, 3ms, 4 K.  *)
Slave0_Channel_2 AT %IX1.0 : BOOL; (* EK1814 EtherCAT-EA-Koppler (1A E-Bus, 4 K. Dig. Ein, 3ms, 4 K.  *)
Slave0_Channel_3 AT %IX2.0 : BOOL; (* EK1814 EtherCAT-EA-Koppler (1A E-Bus, 4 K. Dig. Ein, 3ms, 4 K.  *)
....
```

These variables must be copied into PLC program and used to access the IO

