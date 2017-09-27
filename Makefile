IDIR =include
CC=gcc
CFLAGS=-I$(IDIR) -Werror -DSK_METRIC_LOG

ODIR=obj
LDIR =lib

LIBS=-lcurl

_DEPS = sk_test.h sk_metric.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = sk_test.o sk_metric.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	mkdir -p $(ODIR)
	$(CC) -c -o $@ $< $(CFLAGS)

skTest: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f skTest $(ODIR)/*.o *~ core $(INCDIR)/*~ 
	
.PHONY: test
test: skTest
	./skTest -H "Accept-Language: es" -H "Access-Control-Request-Method: GET" -H "Cookie: ID=2017" -n 5