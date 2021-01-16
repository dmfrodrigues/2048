RMDIR=rm -rf
RM   =rm -f

MAIN   =main
PROG   =main
SDIR   =./src
IDIR   =./include
ODIR   =./obj

CC     =g++
CFLAGS =-Wall
IFLAGS =-I$(IDIR)
LFLAGS =-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -lsfml-network

#IFLAGS += -IC:\sfml\SFML-2.5.1\include
#LFLAGS += -LC:\sfml\SFML-2.5.1\lib

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
