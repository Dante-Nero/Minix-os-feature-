/* ======================================INCLUDES===================================*/
#include <sys/cdefs.h>
#include <lib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

/* ======================================Definitions===================================*/

/* ======================================DECLARATIONS===================================*/
int inodeWalker();
int zoneWalker();
int directoryWalker();
void damage();
void restore();

int damageinode()
{
	message m;
	memset(&m, 0, sizeof(m));
	int inodeNum = 0;
	printf("\nPlease input the Inode Number to Destroy : ");
	scanf("%d",&inodeNum);
  m.m_fs_vfs_lookup.inode = inodeNum;
  (_syscall(VFS_PROC_NR, VFS_DAMAGEINODE, &m));
   return 0;
}

int restoreinode()
{
	message m;
	memset(&m, 0, sizeof(m));
	int inodeNum = 0;
	printf("\nPlease input the Inode Number to Restore: ");
	scanf("%d",&inodeNum);
  m.m_fs_vfs_lookup.inode = inodeNum;
  (_syscall(VFS_PROC_NR, VFS_RESTOREINODE, &m));

  return 0;
}

void restore_directory(const char *path2)
{
	DIR *directory_name;
	struct dirent *start;
	if (!(directory_name = opendir(path2)))
		return;
	start = readdir(directory_name);
	if(strcmp(start->d_name,".")!= 0 || strcmp(start->d_name,"..") != 0)
	{
		printf("Inode number %llu \n",start->d_ino);
		message m;
		memset(&m, 0, sizeof(m));
		int inodeNum = start->d_ino;
		m.m_fs_vfs_lookup.inode = inodeNum;
		(_syscall(VFS_PROC_NR, VFS_RESTOREINODE, &m));
	}
}



void damage_directory(const char *path2)
{

	DIR *directory_name;
	struct dirent *start;
	if (!(directory_name = opendir(path2)))
		return;
	start = readdir(directory_name);
	if(strcmp(start->d_name,".")!= 0 || strcmp(start->d_name,"..") != 0)
	{
		printf("Inode number %llu \n",start->d_ino);
		message m;
		memset(&m, 0, sizeof(m));
		int inodeNum = start->d_ino;
		m.m_fs_vfs_lookup.inode = inodeNum;
		(_syscall(VFS_PROC_NR, VFS_DAMAGEINODE, &m));

	}

}

void recur_func (const char *path)
{
	DIR *directory_name;
	struct dirent *start;

	//directory_name = opendir(path);
	if (!(directory_name = opendir(path)))
		return;
	//struct inode *rip;
	while (start = readdir(directory_name)) {

			if(start->d_type == DT_DIR){
				char new[1000];
				int total_length = snprintf(new, sizeof(new)-1,"%s/%s",path,start->d_name);
				new[total_length] = 0;
				if(strcmp(start->d_name,".")== 0 || strcmp(start->d_name,"..") == 0)
					continue;
				printf("%s ----Inode number %llu \n",start->d_name,start->d_ino);
				recur_func(new);
			}
			else
				{
				printf("	-%s----Inode number %llu\n",start->d_name,start->d_ino);
				}
		}closedir(directory_name);

}

int main(){


	int input=0;

    printf("\n*********************File system tools*********************\n");
	do{
		printf("\nPlease enter \n 1 for Inode walker,\n 2 for Zonemap walker,\n 3 for Directory Walker,\n 4 for Erasing an inode,\n 5 for restoring an inode and\n 0 for exit\n");
		printf(">");
		scanf("%d",&input);

		  switch(input){

              case 1:
			  inodeWalker();
              break;

              case 2:
			  zoneWalker();
              break;

              case 3:
              directoryWalker();
              break;

              case 4:
			  printf("Damaging program\n ");
			  damage();
			  printf("\n");
              break;

			  case 5:
			  printf("Restoring program \n");
			  restore();
			  printf("\n");
              break;

             case 0:
			 exit(0);
             break;

			 default:
			 break;

		}
	}while(input !=0);
}

int inodeWalker()
{
  message m;
  memset(&m, 0, sizeof(m));
 (_syscall(VFS_PROC_NR, VFS_INODEWALKER, &m));
 return 0;

}

int zoneWalker()
{
  message m;
  memset(&m, 0, sizeof(m));
  (_syscall(VFS_PROC_NR, VFS_ZONEWALKER, &m));
  return 0;
}

int directoryWalker()
{
	char path[50];
	bzero(path,50);
	printf("Enter the file name along with the path you wish to open \n");
	scanf(" %[^\n]%*c",path);
	recur_func(path);
	return 0;
}

void damage()
{
	int input = 0;
	int inodeNum =0;
	char path2[50];
	bzero(path2,50);
	printf("Enter the path of the directory you need damage \n");
	scanf(" %[^\n]%*c",path2);
	printf("\nPlease enter \n 1 To delete inode of the file,\n 2 Directory file,\n 3 inodebit map ,\n 4 Delete Zone bitmap\n ");
	scanf("%d",&input);
	switch(input)
	{
	case 1:
		printf("\n");
		damageinode();
		break;
	case 2:
		damage_directory(path2);
		break;
	case 3:
		printf("Enter the inodebitmap of the file to be deleted\n");
		break;
	case 4:
		printf("Enter the zonebitmap of the file to be deleted\n");
		break;
	default:
		printf("Invalid key press \n");
		break;
	}

}

void restore()
{
	int input = 0;
	int inodeNum =0;
	char path2[50];
	bzero(path2,50);
	printf("Enter the path of the directory you need to restore \n");
	scanf(" %[^\n]%*c",path2);
	printf("\nPlease enter \n 1 To restore inode of the file,\n 2 Directory file,\n 3 inodebit map ,\n 4 restore Zone bitmap\n ");
	scanf("%d",&input);
	switch(input)
	{
	case 1:
		printf("\n");
		restoreinode();
		break;
	case 2:
		restore_directory(path2);
		break;
	case 3:
		printf("Enter the inodebitmap of the file to be restored\n");
		break;
	case 4:
		printf("Enter the zonebitmap of the file to be restored\n");
		break;
	default:
		printf("Invalid key press \n");
		break;
	}

}
