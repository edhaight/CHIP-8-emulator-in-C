CFLAGS = -Wall -Wextra -pedantic

HDRS=$(wildcard include/*.h)
SRC=$(wildcard src/*.c)

ODIR=obj
_OBJ=main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

TARGET = chip8
CLEANUP= chip8 make.out *.core

$(TARGET): $(OBJ) 
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIBS)

$(ODIR)/%.o: src/%.c $(HDRS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -c -o $@ $<

clean: 
	@rm -f $(CLEANUP) 
	@rm -rf $(ODIR)

-include $(ODIR)/*.d