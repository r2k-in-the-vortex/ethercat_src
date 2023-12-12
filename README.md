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
```bash
sudo apt-get update
sudo apt-get install udev
sudo apt-get install libxml2-dev
sudo apt-get install autoconf
sudo apt-get install libtool
```

to install ethercat capable branch of OpenPLC 
```bash
git clone https://github.com/thiagoralves/OpenPLC_v3.git
cd OpenPLC_v3
```

ethercat_src is arranged as submodules so these need to be fetched

```bash
git submodule init
git submodule update
cd utils/ethercat_src
git checkout main

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

After startup etherlabs master should be running, this can be verified by `ls /dev/EtherCAT0` 
and by infomation from `sudo ethercat master` which shows state of master interface. `ethercat --help` for more information.

if EtherCAT0 doesn't run, try to start it with 
```
systemctl start ethercat   # For systemd based distro
/etc/init.d/ethercat start # For init.d based distro
```

`sudo ethercat rescan` and `sudo ethercat xml` obtains ethercat slave configuration which needs to be given to ethercat_src

At this point, OpenPLC may be built and installed

cd to OpenPLC_v3 source folder and `./install.sh linux ethercat` to install OpenPLC along with ethercat_src

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

This is how the runtime log should look like if EtherCAT is installed and configured properly

![image](https://github.com/r2k-in-the-vortex/ethercat_src/assets/30666740/b70c32f4-fa37-4bee-890b-bbf8ec4fc084)

If everything goes well then EtherCAT status will progress like so
```bash
4 slave(s).
AL states: PREOP.
Link is up.
Domain1: WC 3.
Domain1: State 1.
Domain1: WC 4.
Domain1: WC 5.
Domain1: WC 7.
Domain1: State 2.
AL states: OP.
```

WC or Working Counter is incremented each time a device successfully reads or writes from the datagram and as such is diagnostic indicator to show all the configured slaves recieved and sent their data.
Correct WC value depends on how many and which devices are configured.

Domain1: State 2. is good state

AL states: OP. means all slave devices are operational

# Advanced devices

Some devices like many servo drives will not start up unless they are correctly configured, which involves Service Data Objects

OpenPLC and ethercat_src provide no means to configure SDOs, but IgH EtherCAT master does

For example, running the following script before starting OpenPLC prepares EL7047 stepper driver in pos 4 for OP mode, it will not enter OP mode without this configuration

```bash
ethercat states preop
sleep 3
ethercat download -p 4 -t uint16 0x10f3 5 0x0000
ethercat download -p 4 -t uint32 0xf081 1 0x00100000
ethercat download -p 4 -t uint8 0x1c12 0 0x00
ethercat download -p 4 -t uint8 0x1c13 0 0x00
ethercat download -p 4 -t uint16 0x1c12 1 0x1600
ethercat download -p 4 -t uint16 0x1c12 2 0x1602
ethercat download -p 4 -t uint16 0x1c12 3 0x1604
ethercat download -p 4 -t uint16 0x1c13 1 0x1a00
ethercat download -p 4 -t uint16 0x1c13 1 0x1a03
ethercat download -p 4 -t uint8 0x1c12 0 0x03
ethercat download -p 4 -t uint8 0x1c13 0 0x02
```

The specific list of parameters and in which order they have to be written is device specific, one way to determine correct configuration is to set the device up in TwinCAT or other commercial EtherCAT capable PLC software and copy the configuration from there. For example [startup tab in TwinCAT](https://infosys.beckhoff.com/english.php?content=../content/1033/tc3_io_intro/1345265931.html&id=)
