IDIR =include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =lib

LIBS=-lcurl

_DEPS = sk_test.h sk_metric.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = sk_test.o sk_metric.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

skTest: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f skTest $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	
.PHONY: test
test: skTest
	./skTest -H "Accept: text/plain" -H "Access-Control-Request-Method: GET" -n5