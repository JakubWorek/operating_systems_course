CXX=gcc
CFLAGS=-Wall -O0

bibl.o:
	$(CXX) $(CFLAGS) -c -fPIC bibl1.c -o bibl.o

bibl.so: bibl.o
	$(CXX) -shared bibl.o -o bibl.so 

dynamic: dynamic.o bibl.so
	$(CXX) $(CFLAGS) dynamic.o -Wl,-rpath,. -o dynamic

dynamic.o:
	$(CXX) $(CFLAGS) -c zad1.c -o dynamic.o

clean:
	rm -rf dynamic dynamic.o bibl.o bibl.so