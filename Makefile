
default:	BinEdit;

install:	BinEdit;
		cp -f Hex2Bin $(HOME)/bin/
		cp -f Bin2Hex $(HOME)/bin/

hex2bin.o:	hex2bin.c binedit.h;
		gcc -c hex2bin.c

bin2hex.o:	bin2hex.c binedit.h;
		gcc -c bin2hex.c

BinEdit: 	Bin2Hex Hex2Bin

Hex2Bin:	hex2bin.o
		gcc -o Hex2Bin hex2bin.o

Bin2Hex:	bin2hex.o
		gcc -o Bin2Hex bin2hex.o

clean:; rm -rf Hex2Bin Bin2Hex *.o
