RMDIR=rm -rf
RM   =rm -f

MAIN   =main
PROG   =2048
SDIR   =./src
IDIR   =./include
ODIR   =./obj

CC     =g++
CFLAGS =-Wall
IFLAGS =-I$(IDIR)
LFLAGS =-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -lsfml-network

ifeq ($(SFML_INCLUDE),)
else
IFLAGS += -I$(SFML_INCLUDE)
endif

ifeq ($(SFML_LIB),)
else
LFLAGS += -L$(SFML_LIB)
endif

ifeq ($(OS),Windows_NT)
LFLAGS += -mwindows
endif

all: $(PROG)

O_FILES=$(ODIR)/RoundedRectangleShape.o

$(PROG): $(SDIR)/$(MAIN).cpp $(O_FILES)
	$(CC) $(CFLAGS) $(IFLAGS) $(SDIR)/$(MAIN).cpp $(O_FILES) $(LFLAGS) -o $@

$(ODIR)/%.o: $(SDIR)/%.cpp | $(ODIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(ODIR):
	mkdir -p $@

clean:
	$(RMDIR) $(ODIR)
	$(RM) $(PROG)
