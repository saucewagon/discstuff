#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

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

	int beginRootDirectory = 19*512;
	data = data + beginRootDirectory;

	while (data[0] > 0){
		if ((data[11] & 0x08) == 0){
			int i = 0;
			char* name = malloc(1);
			if ((data[11] & 0x10) == 0x10){printf("D\t");}
			else{printf("F\t");}	
			while (i < 8){name[i]=data[i];i++;}
			printf("%s\t",name);
	
		int time, date;
		int hours, minutes, day, month, year;
	
//		time = *(unsigned short *)(directory_entry_startPos + 14);
//	date = *(unsigned short *)(directory_entry_startPos + 16);
	
	//the year is stored as a value since 1980
	//the year is stored in the high seven bits
	year = ((data[16] & 0xFE00) >> 9) + 1980;
	//the month is stored in the middle four bits
	month = (data[16] & 0x1E0) >> 5;
	//the day is stored in the low five bits
//	day = (date & 0x1F);
	printf("%d\t%d",year,month);	
//	printf("%d-%02d-%02d ", year, month, day);
	//the hours are stored in the high five bits
//	hours = (time & 0xF800) >> 11;
	//the minutes are stored in the middle 6 bits
//	minutes = (time & 0x7E0) >> 5;
	
//	printf("%02d:%02d\n", hours, minutes);
		printf("\n");
		}
		data += 32;
		
	}


	munmap(data,fileSize);
	close(fp);
	

	return 0;
}

