MAIN   =main
PROG   =main
SDIR   =./src
IDIR   =./include
ODIR   =./obj

CC     =g++
CFLAGS =-Wall

all: main

O_FILES=RoundedRectangleShape.o

$(PROG): $(MAIN).cpp $(O_FILES)
	$(CC) $(CFLAGS) $(O_FILES) $(MAIN).cpp -o $@

$(ODIR)/%.o: $(SDIR)/%.cpp | $(ODIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR):
	mkdir -p $@
