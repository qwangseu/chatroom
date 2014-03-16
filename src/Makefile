all:app utest

CC = gcc

INSTDIR = ./out

# Options for development
# CFLAGS = -g -Wall -std=c99 -pedantic -rdynamic
  CFLAGS = -g -Wall -std=gnu99 -rdynamic

# Options for release
# CFLAGS = -O -Wall -std=c99 -pedantic 

vpath % rapp
vpath % unit-test

LIB  = devlib.a

# app vars
_APP_C = $(wildcard ./rapp/*.c)
APP_C  = $(notdir $(_APP_C))
APP_O  = $(patsubst %.c,%.o,$(APP_C))
APP    = $(patsubst %.c,%,$(APP_C))

app: app.o $(LIB)
	$(CC) $(CFLAGS) -o app app.o $(LIB)

$(LIB): $(LIB)(debug.o $(APP_O))
app.o: app.c
debug.o: debug.c
$(APP_O): $(APP_C)

# Test command
_TEST_C = $(wildcard ./unit-test/*_test.c)
TEST_C  = $(notdir $(_TEST_C))
TEST_O  = $(patsubst %.c,%.o,$(TEST_C))
TEST    = $(patsubst %.c,%,$(TEST_C))

utest: $(TEST)

%_test: %_test.o $(LIB)
	$(CC) $(CFLAGS) -o $@ $<  $(LIB)
%_test.o: %_test.c


#Tool command
clean:
	-rm app $(TEST) *.o $(LIB)

uninstall:clean
	-rm -rf $(INSTDIR)

install:app
ifeq ($(INSTDIR),$(wildcard $(INSTDIR)))
	@echo "Sorry , $(INSTDIR) does exist . Please uninstall.";
else
	@mkdir $(INSTDIR); \
	cp app $(INSTDIR);
endif
