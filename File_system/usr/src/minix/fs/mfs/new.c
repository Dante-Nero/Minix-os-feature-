#include "fs.h"
#include <sys/types.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <minix/ipc.h>
#include "const.h"
#include "inode.h"
#include "type.h"
#include "mfsdir.h"
#include "super.h"
#include <unistd.h>
#include <time.h>
#include <limits.h>
#include <errno.h>
#include "buf.h"
#include <sys/stat.h>
#include <dirent.h>

struct inode *rip;
struct super_block *sp;
unsigned int NB_INODES;
/*===========================================================================*
 *				fs_inodewalker			     *
 *===========================================================================*/
int fs_inodewalker()
{
	block_t b;
	int i,countInodes=0,countusedInodes=0;
	printf("List the inodes\r\n");
	sp = get_super(fs_dev);	/* get pointer to super block */
	printf("Number of usable inodes =%d\r\n", sp->s_ninodes);
	printf("Number of zones available=%d\r\n", (sp->s_zones));
	printf("Number of Inodemap used =%d\r\n",sp->s_imap_blocks);
	printf("Number of Zonemap used =%d\r\n",sp->s_zmap_blocks);
	printf("Number of inodes =%d \r\n ", ((sp->s_ninodes+sp->s_imap_blocks -1)/sp->s_imap_blocks));
	printf("NR_INODES=%d\r\n", NR_INODES);
	for (rip = &inode[0]; rip < &inode[NR_INODES]; rip++) {
          if (rip->i_count > 0) {
			countInodes++;
			countusedInodes +=  rip->i_count;
			//printf("iNode count =%d",rip->i_count);
          }
    }
	printf("No of used iNodes are=%d",countusedInodes);
	//printf("\n Details of inode:  %llu\n",rip->i_num);
	//printf("NDZones of inode  %d\n",rip->i_ndzones);
	//printf("ZoneBlocks are \n");
	//for (i = 0; i < V2_NR_TZONES; i++){
	//b = (block_t)rip->i_zone[i];
	//printf("%d , ",b);
	//}
	printf("\n");
    return 0;
}
/*===========================================================================*
 *				fs_zonewalker			     *
 *===========================================================================*/
int fs_zonewalker()
{	sp = get_super(fs_dev);
	bit_t i, b;
	int j=0,blockCount=0,currentCount=0;
	register struct inode *rip;
	printf("Total number number of zones available=%d\r\n", (sp->s_zones));
	#if 1
	for (i=1; i < 500 ; i++) {
		//The inode is used hence it would have some zonebitmaps. Iterate and find non-zero blocks
		rip = get_inode(fs_dev,i);
	    if (rip->i_count > 0) {
	    	for (j = 0; j < V2_NR_TZONES; j++){
	    		b = (block_t)rip->i_zone[j];
	    		//Check if the block has some pointer or empty
	    		if(b !=0 )
	    			currentCount++;
	    		}
	    	}
			blockCount = blockCount + currentCount;
			currentCount  =0;
	    }
	#endif
	printf("Total number of Zonebitmaps used=%d \n",blockCount);
	return 0;
}
/*===========================================================================*
 *				Damage_INODE			     *
 *===========================================================================*/
int fs_damageinode(ino_t ino_nr){

int inode_nr,i=0;
block_t b;

printf("inode to be erased %llu\n",ino_nr);
rip=NULL;

printf("Printing details of iNode to be erased \n");

rip =  get_inode(fs_dev,ino_nr);
b = (block_t)rip->i_zone[8];
if( b==-1){
		printf("iNode already erased\n");
		return 0;
}
//printf("\n I_NDZones of inode  %x",rip->i_ndzones);
printf("\n Mode = %d",rip->i_mode);
printf("\n Size = %d",rip->i_size);
printf("\n aTime = %d",rip->i_atime);
printf("\n cTime = %d",rip->i_ctime);
printf("\n nLinks = %d",rip->i_mtime);
printf("\n nGid = %d",rip->i_gid);
printf("ZonesBlocks are \n");
for (i = 0; i < V2_NR_TZONES; i++){
b = (block_t)rip->i_zone[i];
	printf(" %d, ",b);
}
printf("\n");
  rip->i_size = 0;
  rip->i_update = ATIME | CTIME | MTIME;
  IN_MARKDIRTY(rip);

  for (i = 0; i < V2_NR_TZONES; i++)
	  rip->i_zone[i]--;

printf("\nInode Erased");

return 0;
}

/*===========================================================================*
 *				Restore_INODE			     *
 *===========================================================================*/
int fs_restoreinode(ino_t ino_nr){

	block_t b;
	int i;

	printf("iNode to recover= %llu\n",ino_nr);
	rip=NULL;
	sp = get_super(fs_dev);
	rip =  get_inode(fs_dev,ino_nr);
	rip->i_size = 0;
	rip->i_update = ATIME | CTIME | MTIME;
	IN_MARKDIRTY(rip);
	for (i = 0; i < V2_NR_TZONES; i++)
		  rip->i_zone[i]++;

	rip =  get_inode(fs_dev,ino_nr);
	b = (block_t)rip->i_zone[8];
	if( b ==-1 ){
			printf("iNode recovery failed\n");
			return -1;
	}
	printf("Inode %llu, recovered succesfully\r\n", ino_nr);
	//printf("\n I_NDZones of inode  %x",rip->i_ndzones);
	printf("\n Mode = %d",rip->i_mode);
	printf("\n Size = %d",rip->i_size);
	printf("\n aTime = %d",rip->i_atime);
	printf("\n cTime = %d",rip->i_ctime);
	printf("\n nLinks = %d",rip->i_mtime);
	printf("\n nGid = %d",rip->i_gid);
	printf("\nReturn to Menu\n");
	return 0;
}
