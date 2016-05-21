#include "fs.h"
#include "file.h"
#include "path.h"
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <minix/callnr.h>
#include <minix/safecopies.h>
#include <minix/endpoint.h>
#include <minix/com.h>
#include <minix/sysinfo.h>
#include <minix/u64.h>
#include <minix/vfsif.h>
#include <sys/ptrace.h>
#include <sys/svrctl.h>
#include <sys/resource.h>
#include <sys/dirent.h>
#include <sys/stat.h>
#include "file.h"
#include "vnode.h"
#include "vmnt.h"
#include "path.h"
#include "lock.h"


/*===========================================================================*
 *				do_inodewalker			     *
 *===========================================================================*/

int do_inodewalker(){
  struct vnode *vp;
  struct vmnt *vmp;
  struct lookup resolve;
  char *devPath;
  devPath="/dev/c0d0p0s2";

  lookup_init(&resolve, devPath, PATH_NOFLAGS, &vmp, &vp);
  resolve.l_vmnt_lock = VMNT_READ;
  resolve.l_vnode_lock = VNODE_READ;

  if ((vp = eat_path(&resolve, fp)) == NULL) {
				printf("unknown MFS API error\n");
				return -1;
   }
	unlock_vnode(vp);
	unlock_vmnt(vmp);
	put_vnode(vp);
   	req_inodewalker(vmp->m_fs_e);
  return 0;
}

/*===========================================================================*
 *				do_zonewalker			     *
 *===========================================================================*/
int do_zonewalker()
{
	struct vnode *vp;
	struct vmnt *vmp;
	char *devPath;
	struct lookup resolve;
    devPath="/dev/c0d0p0s2";
	lookup_init(&resolve, devPath, PATH_NOFLAGS, &vmp, &vp);
	resolve.l_vmnt_lock = VMNT_READ;
	resolve.l_vnode_lock = VNODE_READ;
	if ((vp = eat_path(&resolve, fp)) == NULL) {
		printf("MFS error\n");
		return -1;
		}
	unlock_vnode(vp);
	unlock_vmnt(vmp);
	put_vnode(vp);
	  req_zonewalker(vmp->m_fs_e);
	  return 0;
}

int do_damageinode()
{
	int inode_nr;
	inode_nr = job_m_in.m_fs_vfs_lookup.inode;
	printf("%d",inode_nr);
	printf("HI From the new file in vfs \n");
	struct vnode *vp;
	struct vmnt *vmp;
	char *devPath;
	struct lookup resolve;
	devPath="/dev/c0d0p0s2";
	lookup_init(&resolve, devPath, PATH_NOFLAGS, &vmp, &vp);
	resolve.l_vmnt_lock = VMNT_READ;
	resolve.l_vnode_lock = VNODE_READ;
	if ((vp = eat_path(&resolve, fp)) == NULL) {
		printf("MFS error\n");
		return -1;
    }
	req_damageinode(vmp->m_fs_e,inode_nr);
	unlock_vnode(vp);
	unlock_vmnt(vmp);
	put_vnode(vp);
  printf("HI From the new file in vfs \n");
  return 0;

}

int do_restoreinode()
{
	int inode_nr;
	inode_nr = job_m_in.m_fs_vfs_lookup.inode;
	printf("%d",inode_nr);
	printf("HI From the new file in vfs \n");
	struct vnode *vp;
	struct vmnt *vmp;
	char *devPath;
	struct lookup resolve;
	devPath="/dev/c0d0p0s2";
	lookup_init(&resolve, devPath, PATH_NOFLAGS, &vmp, &vp);
	resolve.l_vmnt_lock = VMNT_READ;
	resolve.l_vnode_lock = VNODE_READ;
	if ((vp = eat_path(&resolve, fp)) == NULL) {
		printf("MFS error\n");
		return -1;
    }
	req_restoreinode(vmp->m_fs_e,inode_nr);
	unlock_vnode(vp);
	unlock_vmnt(vmp);
	put_vnode(vp);
  //printf("HI From the new file in vfs \n");
  return 0;

}

