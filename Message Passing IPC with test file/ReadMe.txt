Developer: Kirubha Shankar Murugesan 
Paste all the files in the path provided

callnr.h -------	/usr/src/include/minix/
table.c -------	/usr/src/servers/pm/
proto.h -------	/usr/src/servers/pm/
mess.c -------	/usr/src/servers/pm/
messfun.c -------	/usr/src/servers/pm/
messagequeue.h -------	/usr/src/servers/pm/
Makefile -------	/usr/src/servers/pm/

in the path  /usr/src/servers/pm/

to compile the definitions, type: make


=========================================================================

After compile :

copy the proj.h to the paths given below.

cp proj.h /usr/include/
cp proj.h /usr/src/include/

 /usr/src/releasetools

Type: make includes
Type: make hdboot
Type: reboot

After the reboot:


=========================================================================
-> copy the project.c and receiver.c file to the /root/ 

project.c /root/
receiver.c /root/

-> make both the c programs
make project 
make receiver
make notify2

and run these 
./notify2
./project
./receiver




