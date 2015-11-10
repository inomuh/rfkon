# RFKON

**Türkçe doküman için 'Qt-DDS Tutorial.pdf' dosyasına bakınız.**

## Installation and Compiling

### Prerequisite:

* Qt Creator
* Opensplice DDS (Community Edition Version 6.x for Linux kernel 3 and up (64-bit) Host and Target, gcc 4.6 compiler, x86 chipset (Ubuntu 12))
* Source Codes of the RFKON

### Packages to install before installing on your Linux machine

$ sudo apt-get install gcc g++ <br />
$ sudo apt-get install autoconf <br />
$ sudo apt-get install automake <br />
$ sudo apt-get install build-essential <br />
$ sudo apt-get install libboost-all-dev <br />

### Editing 'release.com' file to source the environmental variable for DDS. 'release.com' file must be in <the path you download the DDS source files>/HDE/x86_64.linux

$ cd ~/ <br />
$ mkdir opt <br />
$ mv ~/Downloads/HDE ~/opt <br />
$ cd ~/opt/HDE/x86_64.linux <br />
$ sudo chmod +x release.com <br />
$ gedit release.com <br />

**Replace your content with this one (Just remember to fill <xxx> with your hostname):**

> echo "<<< OpenSplice HDE Release V6.4.140407OSS For x86_64.linux, Date 2014-04-15 >>>" <br />
 if [ "${SPLICE_ORB:=}" = "" ] <br />
 then <br />
    SPLICE_ORB=DDS_OpenFusion_2 <br />
    export SPLICE_ORB <br />
fi <br />
if [ "${SPLICE_JDK:=}" = "" ] <br />
then <br />
    SPLICE_JDK=jdk <br />
    export SPLICE_JDK <br />
fi <br />
BOOST_ROOT="/usr/include/boost" <br />
OSPL_HOME="/home/**xxx**/opt/HDE/x86_64.linux" <br />
PATH=$OSPL_HOME/bin:$PATH <br />
LD_LIBRARY_PATH=$OSPL_HOME/lib${LD_LIBRARY_PATH:+:}$LD_LIBRARY_PATH <br />
CPATH=$OSPL_HOME/include:$OSPL_HOME/include/sys:${CPATH:=} <br />
OSPL_URI=file://$OSPL_HOME/etc/config/ospl_unicast.xml <br />
OSPL_TMPL_PATH=$OSPL_HOME/etc/idlpp <br />
<br />
export OSPL_HOME PATH LD_LIBRARY_PATH CPATH OSPL_TMPL_PATH OSPL_URI BOOST_ROOT <br />

$ source ~/opt/HDE/x86_64.linux/release.com <br />

### Editing 'environment' file for Qt Creator

$ sudo gedit /etc/environment <br />

**Replace your content with this one (Just remember to fill <xxx> with your hostname):**

> PATH=/home/xxx/opt/HDE/x86_64.linux/etc/idlpp:/home/xxx/opt/HDE/x86_64.linux/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games <br />
OSPL_HOME=/home/xxx/opt/HDE/x86_64.linux <br />
OSPL_TARGET=x86_64.linux <br />
LD_LIBRARY_PATH=/home/xxx/opt/HDE/x86_64.linux/lib <br />
CPATH=/home/xxx/opt/HDE/x86_64.linux/include://home/xxx/opt/HDE/x86_64.linux/include/sys:/home/xxx/opt/HDE/x86_64.linux/include/dcps/C++/SACPP:/home/xxx/opt/HDE/x86_64.linux/include/dcps/C++/CCPP:/home/xxx/opt/HDE/x86_64.linux/include/dcps/C++/isocpp <br />
OSPL_URI=file://home/xxx/opt/HDE/x86_64.linux/etc/config/ospl.xml <br />
OSPL_TMPL_PATH=/home/xxx/opt/HDE/x86_64.linux/etc/idlpp <br />
QTDIR=/home/xxx/opt/Qt5.5/5.5/gcc_64/ <br />
CLASSPATH=/home/xxx/opt/HDE <br />

Reboot your system! <br />
$ sudo reboot <br />

### Cloning the RFKON source codes and producing necessary DDS source codes using 'idlpp' command

$ cd Downloads <br />
$ git clone https://github.com/inomuh/rfkon <br />

And you can open any Qt projects 

**LAST BUT CRUCIAL NOTE: Do not forget to change Qt Project file (.pro) according to your hostname and DDS library which has just been sourced.**

For ex: Server_Requester.pro file content has this part which has to be changed:

LIBS +=     -L/home/bolatu/opt/HDE/x86_64.linux/lib \ <br />
            -lddsserialization \ <br />
            -lddskernel \ <br />
            -lddsutil \ <br />
            -lddsdatabase \ <br />
            -lddsuser \ <br />
            -lddsconf \ <br />
            -lddsconfparser \ <br />
            -ldcpsgapi \ <br />
            -ldcpssacpp \ <br />
            -lddsos \ <br />
            -L/usr/lib \ <br />
            -lboost_filesystem \ <br />
            -lboost_system \ <br />
            -lboost_regex \ <br />



INCLUDEPATH +=  /home/bolatu/opt/HDE/x86_64.linux/include \ <br />
                /home/bolatu/opt/HDE/x86_64.linux/include/dcps/C++/SACPP/ \ <br />
                /home/bolatu/opt/HDE/x86_64.linux/examples/include \ <br />

DEPENDPATH +=   /home/bolatu/opt/HDE/x86_64.linux/include \ <br />
                /home/bolatu/opt/HDE/x86_64.linux/include/dcps/C++/SACPP/ \ <br />
                /home/bolatu/opt/HDE/x86_64.linux/examples/include \ <br />

**Replace <bolatu> part with your hostname instead of 'bolatu'(if you move your DDS library into other path, you have to declare that path instead of /home/xxx/opt/HDE/x86_64.linux/)**


## Tutorial Video

<a href="http://www.youtube.com/watch?feature=player_embedded&v=3y8ua0ahqho
" target="_blank"><img src="http://img.youtube.com/vi/3y8ua0ahqho/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

