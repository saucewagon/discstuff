/*

Alexander Steel
V00855144
CSC 360: Assignment 3

diskinfo.c

REFERENCES:

I used the following website as a reference on how to use mmap to read in the disk image:
	http://beej.us/guide/bgipc/output/html/multipage/mmap.html

*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>

void osname(char* name, char* data);
void disklabel(char* label, char* data);

int main(int argc, char* argv[]) {

	int fp;
	char* data;
	struct stat sbuf;

	if (argc != 2){return;}
	char* diskName = argv[1];
	if ((fp = open(diskName,O_RDONLY)) == -1){return;}
	stat(diskName, &sbuf);
	int fileSize = sbuf.st_size;
	data = mmap(0,fileSize,PROT_READ,MAP_SHARED,fp,0); // Map disk image to main memory

	char * name = malloc(1);
	osname(name,data); // Get the OS name

	char * label = malloc(1);
	disklabel(label,data); // Get the disk label
	
	printf("--------\n");		
	printf("Num of FAT copies = %d\n",data[16]); // # fat copies in boot sector
	int sectorsperfat = data[22]; // Get two bytes starting at location 22
	int shiftnextbyte = data[23] << 8;
	printf("Num of sectors per FAT = %d\n",sectorsperfat + shiftnextbyte); // # sectors in boot sector

	munmap(data,fileSize);
	close(fp);
	
	return 0;
}
/*

osname: read 8 bytes from boot sector starting at byte 3

*/
void osname(char* name,char* data){

	int i = 3;
	
	while (i < 11){
		name[i-3]=data[i];i++;	
	}
	printf("OS Name = %s\n",name);
}
/*

disklabel: copy disk label from boot sector

*/
void disklabel(char* label, char* data){
	int i = 43;
	
	if (data[43] != ' '){ // If the label is in boot sector, copy it
		while (i < 51){
			label[i-43]=data[i];i++;	
		}
		return;
	}
	// TODO Grab data from root dir

	printf("Disk Label  = %s\n",label);
}
