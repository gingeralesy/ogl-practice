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
LDFLAGS = -L$(LDIR) $(shell pkg-config --libs glfw3 glm)

# Check for Windows
ifdef SYSTEMROOT
  LDFLAGS := $(LDFLAGS) -lopengl32 -lgdi32
else
  ifeq ($(shell uname), Linux)
    LDFLAGS := $(LDFLAGS) -lm -ldl
  else
    LDFLAGS := $(LDFLAGS) -lopengl32 -lgdi32
  endif
endif

MKDIR_P = mkdir -p
RM = rm -f
RMDIR = rmdir
CP = rsync -rup --delete

SOURCES  := $(wildcard $(SDIR)/*.c)
HEADERS := $(wildcard $(IDIR)/*.h)
OBJECTS  := $(SOURCES:$(SDIR)/%.c=$(ODIR)/%.o)
OUTFILE   = $(BDIR)/$(NAME)

$(OUTFILE): $(BDIR) $(ODDIR) objects
	@$(LD) $(OBJECTS) $(LDFLAGS) -o $@
	@echo " *** Linked "$@" successfully!"

tags: $(HEADERS) $(SOURCES)
	@ctags -e $^

$(OBJECTS): $(ODIR)/%.o : $(SDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo " *** Compiled "$<" successfully!"

$(ODIR):
	@$(MKDIR_P) $(ODIR)

objects: $(ODIR) $(OBJECTS)
	@echo " *** Compilation complete!"

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
ifneq ($(wildcard $(ODIR)/.),)
	@$(RM) $(OBJECTS)
	@$(RMDIR) $(ODIR)
endif
ifneq ($(wildcard $(BDIR)/.),)
	@$(RM) $(OUTFILE)
	@$(RMDIR) $(BDIR)
endif
	@echo " *** Executable removed!"

.PHONY: all
all: remove $(OUTFILE)
