.phony all:
all: discinfo

discinfo: discinfo.c
	gcc discinfo.c -pthread  -lreadline -lhistory -o discinfo

.PHONY clean:
clean:
	-rm -rf *.o *.exe
