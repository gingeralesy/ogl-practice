NAME = foo

CC = gcc
LD = gcc

SDIR = src
ODIR = obj
IDIR = include
LDIR = lib
BDIR = bin
DDIR = data
ODDIR = $(BDIR)/$(DDIR)

CFLAGS = -std=c11 -Wall -I$(IDIR) $(shell pkg-config --cflags glfw3 glm)
LDFLAGS = -L$(LDIR) $(shell pkg-config --libs glfw3 glm) -lopengl32 -lgdi32

MKDIR_P = mkdir -p
RM = rm -f
RMDIR = rmdir
CP = rsync -rup --delete

SOURCES  := $(wildcard $(SDIR)/*.c)
INCLUDES := $(wildcard $(IDIR)/*.h)
OBJECTS  := $(SOURCES:$(SDIR)/%.c=$(ODIR)/%.o)
OUTFILE   = $(BDIR)/$(NAME)

$(OUTFILE): $(BDIR) $(ODDIR) objects
	@$(LD) $(OBJECTS) $(LDFLAGS) -o $@
	@echo " *** Linked "$@" successfully!"

.PHONY: objects
objects: $(ODIR) $(OBJECTS)
	@echo " *** Compilation complete!"

$(OBJECTS): $(ODIR)/%.o : $(SDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo " *** Compiled "$<" successfully!"

$(ODIR):
	@$(MKDIR_P) $(ODIR)

$(BDIR):
	@$(MKDIR_P) $(BDIR)

$(ODDIR): $(BDIR)
	@$(CP) $(DDIR) $(BDIR)

.PHONY: clean
clean:
	@$(RM) $(OBJECTS)
	@$(RM) $(ODDIR)/*.*
ifneq ($(wildcard $(ODDIR)/.),)
	@$(RMDIR) $(ODDIR)
endif
	@echo " *** Cleanup complete!"

.PHONY: remove
remove: clean
	@$(RM) $(OUTFILE)
ifneq ($(wildcard $(ODIR)/.),)
	@$(RMDIR) $(ODIR)
endif
ifneq ($(wildcard $(BDIR)/.),)
	@$(RMDIR) $(BDIR)
endif
	@echo " *** Executable removed!"

.PHONY: all
all: remove $(OUTFILE)
