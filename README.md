# RFKON

**Türkçe doküman için 'Qt-DDS Tutorial.pdf' dosyasına bakınız.**

## Installation and Compiling

### Prerequisite:

* Qt Creator
* Opensplice DDS (Community Edition Version 6.x for Linux kernel 3 and up (64-bit) Host and Target, gcc 4.6 compiler, x86 chipset (Ubuntu 12))
* Source Codes of the RFKON

### Packages to install before installing on your Linux machine

$ sudo apt-get install gcc g++
$ sudo apt-get install autoconf
$ sudo apt-get install automake

$ sudo apt-get install build-essential

$ sudo apt-get install libboost-all-dev

### Editing 'release.com' file to source the environmental variable for DDS. 'release.com' file must be in <the path you download the DDS source files>/HDE/x86_64.linux

$ cd ~/

$ mkdir opt

$ mv ~/Downloads/HDE ~/opt
 
$ cd ~/opt/HDE/x86_64.linux

$ sudo chmod +x release.com

$ gedit release.com

**Replace your content with this one (Just remember to fill <xxx> with your hostname):**

echo "<<< OpenSplice HDE Release V6.4.140407OSS For x86_64.linux, Date 2014-04-15 >>>"
if [ "${SPLICE_ORB:=}" = "" ]
then
    SPLICE_ORB=DDS_OpenFusion_2
    export SPLICE_ORB
fi
if [ "${SPLICE_JDK:=}" = "" ]
then
    SPLICE_JDK=jdk
    export SPLICE_JDK
fi
BOOST_ROOT="/usr/include/boost"
OSPL_HOME="/home/**xxx**/opt/HDE/x86_64.linux"
PATH=$OSPL_HOME/bin:$PATH
LD_LIBRARY_PATH=$OSPL_HOME/lib${LD_LIBRARY_PATH:+:}$LD_LIBRARY_PATH
CPATH=$OSPL_HOME/include:$OSPL_HOME/include/sys:${CPATH:=}
OSPL_URI=file://$OSPL_HOME/etc/config/ospl_unicast.xml
OSPL_TMPL_PATH=$OSPL_HOME/etc/idlpp

export OSPL_HOME PATH LD_LIBRARY_PATH CPATH OSPL_TMPL_PATH OSPL_URI BOOST_ROOT

$ source ~/opt/HDE/x86_64.linux/release.com

### Editing 'environment' file for Qt Creator

$ sudo gedit /etc/environment

**Replace your content with this one (Just remember to fill <xxx> with your hostname):**

PATH=/home/xxx/opt/HDE/x86_64.linux/etc/idlpp:/home/xxx/opt/HDE/x86_64.linux/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games
OSPL_HOME=/home/xxx/opt/HDE/x86_64.linux
OSPL_TARGET=x86_64.linux
LD_LIBRARY_PATH=/home/xxx/opt/HDE/x86_64.linux/lib
CPATH=/home/xxx/opt/HDE/x86_64.linux/include://home/xxx/opt/HDE/x86_64.linux/include/sys:/home/xxx/opt/HDE/x86_64.linux/include/dcps/C++/SACPP:/home/xxx/opt/HDE/x86_64.linux/include/dcps/C++/CCPP:/home/xxx/opt/HDE/x86_64.linux/include/dcps/C++/isocpp
OSPL_URI=file://home/xxx/opt/HDE/x86_64.linux/etc/config/ospl.xml
OSPL_TMPL_PATH=/home/xxx/opt/HDE/x86_64.linux/etc/idlpp
QTDIR=/home/xxx/opt/Qt5.5/5.5/gcc_64/
CLASSPATH=/home/xxx/opt/HDE

Reboot your system!
$ sudo reboot

### Cloning the RFKON source codes and producing necessary DDS source codes using 'idlpp' command

$ cd Downloads
$ git clone https://github.com/inomuh/rfkon

And you can open any Qt projects 

**LAST BUT CRUCIAL NOTE: Do not forget to change Qt Project file (.pro) according to your hostname and DDS library which has just been sourced.**

For ex: Server_Requester.pro file content has this part which has to be changed:

LIBS +=     -L/home/bolatu/opt/HDE/x86_64.linux/lib \
            -lddsserialization \
            -lddskernel \
            -lddsutil \
            -lddsdatabase \
            -lddsuser \
            -lddsconf \
            -lddsconfparser \
            -ldcpsgapi \
            -ldcpssacpp \
            -lddsos \
            -L/usr/lib \
            -lboost_filesystem \
            -lboost_system \
            -lboost_regex \



INCLUDEPATH +=  /home/bolatu/opt/HDE/x86_64.linux/include \
                /home/bolatu/opt/HDE/x86_64.linux/include/dcps/C++/SACPP/ \
                /home/bolatu/opt/HDE/x86_64.linux/examples/include \

DEPENDPATH +=   /home/bolatu/opt/HDE/x86_64.linux/include \
                /home/bolatu/opt/HDE/x86_64.linux/include/dcps/C++/SACPP/ \
                /home/bolatu/opt/HDE/x86_64.linux/examples/include \

**Replace <bolatu> part with your hostname (if you move your DDS library into other path, you have to declare that path instead of /home/<xxx>/opt/HDE/x86_64.linux/)**


## Tutorial Video

<a href="http://www.youtube.com/watch?feature=player_embedded&v=3y8ua0ahqho
" target="_blank"><img src="http://img.youtube.com/vi/3y8ua0ahqho/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

