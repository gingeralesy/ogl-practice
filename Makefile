NAME = foo

CC = gcc
LD = gcc

SDIR = src
ODIR = obj
IDIR = include
LDIR = lib
BDIR = bin

CFLAGS = -Wall -I$(IDIR) $(shell pkg-config --cflags glfw3 glm)
LDFLAGS = -L$(LDIR) $(shell pkg-config --libs glfw3 glm) -lopengl32 -lgdi32

MKDIR_P = mkdir -p
RM = rm -f

SOURCES  := $(wildcard $(SDIR)/*.c)
INCLUDES := $(wildcard $(IDIR)/*.h)
OBJECTS  := $(SOURCES:$(SDIR)/%.c=$(ODIR)/%.o)
OUTFILE   = $(BDIR)/$(NAME)

$(OUTFILE): $(BDIR) objects
	@$(LD) $(OBJECTS) $(LDFLAGS) -o $@
	@echo " *** Linked "$@" successfully!"

objects: $(ODIR) $(OBJECTS)
	@echo " *** Compilation complete!"

$(OBJECTS): $(ODIR)/%.o : $(SDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo " *** Compiled "$<" successfully!"

$(ODIR):
	@$(MKDIR_P) $(ODIR)

$(BDIR):
	@$(MKDIR_P) $(BDIR)

.PHONY: clean
clean:
	@$(RM) $(OBJECTS)
	@echo " *** Cleanup complete!"

.PHONY: remove
remove: clean
	@$(RM) $(OUTFILE)
	@echo " *** Executable removed!"
