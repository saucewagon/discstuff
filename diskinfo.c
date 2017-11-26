/*
http://beej.us/guide/bgipc/output/html/multipage/mmap.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void osname(char* name, char* data);
void disklabel(char* label, char* data);

int getNumberOfRootFiles(char* p) {
	p += 512 * 19;
	int count = 0;

	while (p[0] != 0x00) {
		if ((p[11] & 0b00000010) == 0 && (p[11] & 0b00001000) == 0 && (p[11] & 0b00010000) == 0) {
			count++;
		}
		p += 32;
	}

	return count;
}

int main(int argc, char* argv[]) {

	int fp;
	char* data;
	struct stat sbuf;

	if (argc != 2){return;}
	char* diskName = argv[1];
	if ((fp = open(diskName,O_RDONLY)) == -1){return;}
	stat(diskName, &sbuf);
	int fileSize = sbuf.st_size;
	data = mmap(0,fileSize,PROT_READ,MAP_SHARED,fp,0);

	char * name = malloc(1);
	osname(name,data);

	char * label = malloc(1);
	disklabel(label,data);
		
	printf("Num of FAT copies = %d\n",data[16]);
	int sectorsperfat = data[22];
	int shiftnextbyte = data[23] << 8;
	printf("Num of sectors per FAT = %d\n",sectorsperfat + shiftnextbyte);
	

//	int diskSize = getTotalDiskSize(p);
//	int freeSize = getFreeDiskSize(diskSize, p);
//	int numberOfRootFiles = getNumberOfRootFiles(p);


//	printInfo(osName, diskLabel, diskSize, freeSize, numberOfRootFiles, numberOfFatCopies, sectorsPerFat);

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

void disklabel(char* label, char* data){
	int i = 43;
	
	while (i < 51){
		label[i-43]=data[i];i++;	
	}

	if (label[0] == ' ') {
		data += 512 * 19;
		while (data[0] != 0) {
			if (data[11] == 8) {
				for (i = 0; i < 8; i++) {
					label[i] = data[i];
				}
			}
		data += 32;
		}
	}

	printf("Disk Label  = %s\n",label);


}

