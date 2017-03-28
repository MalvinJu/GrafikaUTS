CPPFLAGS=-g -pthread -std=c++11
LDFLAGS=-g -pthread
ODIR=obj
SDIR=.
OUT=uts


_OBJS=Color.o LineDrawer.o Screen.o shape.o Point.o Shapeclip.o VectorOperation.o stars.o Folder.o mainUTS.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))

uts: directories $(OBJS)
	g++ -o uts $(OBJS) $(LDFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp
	g++ -c -o $@ $< $(CPPFLAGS)

.PHONY: clean directories

directories:
	mkdir -p obj; mkdir -p build;
  
clean:
	rm -f $(ODIR)/*.o $(OUT)
