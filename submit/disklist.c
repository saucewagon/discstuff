/*

Alexander Steel
V00855144
CSC 360: Assignment 3

disklist.c

REFERENCES:

I used "sample_date_time_print.c" by Huan Wang on the course resources page as a reference
on bit masking/shifting the date and time info

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

	int beginRootDirectory = 19*512; // Jump to root directory
	data = data + beginRootDirectory; 

	for(data=data; data[0] >  0; data+=32){ // Begin extracting file info from root dir, loop ends when remaining entries are free (data[0] = 0x00)
		if ((data[11] & 0x08) != 0x08){ // Mask with 0x08
			int i = 0;
			char* name = malloc(1);
			if (data[11]  == 0x10){printf("D\t");} // Check attribute entry to check if file/directory
			else{printf("F\t");}	
			while (i < 8){name[i]=data[i];i++;} // Filename stored in first 8 bytes of root directory
			printf("%s\t",name);
	
			int date = data[16] + (data[17] << 8); // Access byte 16 and 17 for creation date
			printf("%d-",((date & 0xFE00) >> 9) + 1980); // Mask and shift to access individual month/days/years 
			printf("%d-",((date & 0x01E0) >> 5));
			printf("%d\t",((date & 0x1F)));	

			int time = data[14] + (data[15] << 8); // Access byte 14 and 15 for creation time 
			printf("%d:",(time & 0x7E0) >> 5);	
			printf("%d",(time & 0xF800) >> 11);

			printf("\n");
		}	
	}
	munmap(data,fileSize);
	close(fp);
	
	return 0;
}

