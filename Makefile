.phony all:
all: ACS

ACS: diskinfo.c
	gcc diskinfo.c -pthread  -lreadline -lhistory -o diskinfo
	gcc disklist.c -o disklist

.PHONY clean:
clean:
	-rm -rf *.o *.exe

