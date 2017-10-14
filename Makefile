CC = gcc

SDIR = src
IDIR = include
LDIR = lib

CFLAGS = -Wall -I$(IDIR) $(shell pkg-config --cflags glfw3 glm)
LDFLAGS = -L$(LDIR) $(shell pkg-config --libs glfw3 glm) -lopengl32 -lgdi32

MKDIR_P = mkdir -p
RM = rm -f

NAME = foo
BINDIR = bin

OUTFILE = $(BINDIR)/$(NAME)

all: $(SDIR)/*.c
	$(MKDIR_P) $(BINDIR)
	$(RM) $(OUTFILE)
	$(CC) $(CFLAGS) -o $(OUTFILE) $^ $(LDFLAGS)
