******************************************************************************************
**         Project 3 : Filesystem tools implementation			                **                       
**         Authors   : Kirubha shankar Murugesan, Jay Joshi, Naveena Chowdary Edpuganti **             
**         Course    : CS551                                                            **
**         Date      : 05/25/2015                                                       **
******************************************************************************************

CS551 Project 3 involves the implementation of file system tools which can restore data from the 
from damaged inode. It also involves the implementation of three new process - inodewalker 
and zonewalker and directory walker. The inodewalker does a complete inode traversal
and gets the inode information of all the files in the file system. The zonewalker gets
the complete zone map information from the inode traversal. The diretory walker provides the 
complete walk from the path provided and list the directories along with the d_inode of the file.
For this implementation new system calls were added to the system kernel. The inode damage and inode recovery tools were added. 
The inode damage tool helps to damage an inode of any directory file. This tool is added to test the
recovery tool.

The Project does the following:
1. Inodewalker it provides the total number of inodes and the total number of inodes that are being used.
2. Zonewalker, it gives the total number of zone blocks and the total number of zones that are being used.
3. Directorywalker, it gets from the user a path and extracts the directory and its corresponding inodes for the user.
4. Another function has been added which can damage an inode of a directory file.This tool is made to verify the
  recovery tool.
5. The repair tool has been implemented which will restore the inode that has been deleted by restoring the zonemap information

		************************       Project 3  Contents         ************************
This package contains the following files:
1. Read me file          - ReadMe.txt
2. Design Document       - Design_Document.docx
3. Modified System Files - request.c, proto.h, callnr.h, table.c, vfsif.h,fsdriver.h,
4. Newly Added Files	 - new.c---vfs new.c---mfs, test_program-- to test the tools, fsck.c which does the integirity check of the files

	*********************       Project 3 Installation Instructions       *********************
1. Extract all the contents in the Source Code folder to /. All the files will be extracted
   to their respective folders. Click yes to overwrite during extraction.
2. Go to /usr/src/releasetools and execute 
   make hdboot
3. Reboot Minix.
4. Go to /home and do make test_program
   ./test_program 
5. To check integrity that is inode bitmap and zone mitmap check go to /home/kirubha and type make
./fsck_mfs and give the device name to do both bitmap chek

	*********************              Project 3 Execution                *********************

A menu driven prompt will be observed. The Menu is as shown below

*********************File system tools*********************
Please enter
1 for Inode walker,
2 for Zonemap walker,
3 for Directory Walker,
4 for Erasing an inode,
5 for restoreing an inode
0 Exit
>

The options are as shown above.

1. for Inode walker

Once this option is pressed it list the information of the inodes in the filesystem.

2. for Zonemap walker

Once this option is pressed the program list the total number of zonemaps used and total
number of zone maps available. 

3. for Directory walker 

Once this option is pressed the program asks the user to enter the path of the directory to 
which the function has to be executed. Once the path is given it traverses throuth 
the path and lists the directory name and corresponding inode of the directory. 

4. for Erasing an inode
Once this option is prompt the user is asked to enter a path of the filesystem just 
in case if he wants to delete a particular file. and another menu option is provided
1. To delete an inode
2. To delete a file using name.

To delete the inode: when this option is chosen the user has to enter the inode to be deleted. 
The program deletes the inode entered when the same inode is tried to be deleted again 
it just notifies the user that the inode is already deleted. 

To delete the directory: Enter the directory path and the program will automatically delete the directory using its inode number
5. for restoring an inode
Once this option is selected the program prompts the user to choose a path of the 
filesytem just in case the user wants to restore the file using the name of the file. When this is done
another menu option is provided 
1. To retore an inode
2. TO restore an inode using filename

To restore inode: when this option is selected the program gets the inode number and restores it. 
If it cannot restore it throws an exception. Using filename is not implemented but it is done by getting 
the inode of direcotry name entered and passing it to the same function as above. 
To Restore the directory: Enter the directory path and the program will automatically restore the directory using its inode number

0 Exit

By choosing this program is exited. 

In order to do the inode bitmap and zone bitmap do the following 

Change the directory to /home/kirubha
and enter make
./fsck_mfs
This ask the device path for which the inode bitmap verification and zone bitmap verification has to be done 
provide any of the following /dev/c0d0p0s0 or /dev/c0d0p0s1 or /dev/c0d0p0s2
 
This checks the zone bit map and inode bitmap for the filesystem.



