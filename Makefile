
main:recoContact.o main.c equipment.h recoContact.h
	gcc -o main.c recoContact.o
recoContact.o:recoContact.c equipment.h recoInfo.h
	gcc -c recoContact.c
clean:
	rm *.o main