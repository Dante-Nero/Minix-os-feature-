Project1-CS-551 IMPLEMENTATION OF THE SHELL

Developer :
KIRUBHA SHANKAR MURUGESAN


The folder contains the following files :
1. main.c : conatins code for the shell without history feature
2. .profile : contains initialization values for shell
3. Design Document : A design document.
4. p1 file contains the working code for the shell

Steps to build:
1. # make main

Steps to execute:
Run the following commands:
	# ./p1
Steps to configure using profile:
1. To set block timer off, add: 
	TIMER=0
2. To set timer for "n" seconds, where n is any positive integer, add:
	TIMER=n
3. To set PATH variable, add:
	PATH=PATH=/sbin:/usr/sbin:/bin:/usr/bin:/usr/pkg/sbin:/usr/pkg/bin:/usr/X11R7/bin:/usr/X11R6/bin:/usr/local/sbin:/usr/local/bin	
 this path is set to include most of the bin functions to the shell
5. =>q command can be used to quit the prompt.  