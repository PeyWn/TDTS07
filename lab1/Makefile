# The final executable
PROGRAM = main.x

# The source files (*.cc)
SRCS = sensor.cc generator.cc controller.cc main.cc

##################################
## DO NOT CHANGE ANYTHING BELOW ##
##################################

SYSTEMC = /courses/TDTS07/sw/systemc
TARGET_ARCH = linux64

LIBDIR = -L. -L.. -L$(SYSTEMC)/lib-$(TARGET_ARCH)
LIBS = -Wl,-Bstatic -lsystemc -Wl,-Bdynamic -lm -lpthread
LDFLAGS = -g

CC = g++-4.8
OPT = -O3
CFLAGS = -Wno-deprecated
EXE = $(PROGRAM)

INCDIR= -I. -I.. -I$(SYSTEMC)/include

OBJS = $(SRCS:.cc=.o)

.SUFFIXES: .cc .o .x

$(EXE): $(OBJS)
	$(CC) $(LDFLAGS) $(INCDIR) $(LIBDIR) -o $(EXE) $(OBJS) $(LIBS)

# To build the whole project we must compile each .cc file into a .o file, and
# then link all .o-files into an executable. The rule above tells the make tool
# how to link the object files into an executable. To create the object files we
# need additional rules (one for each .cc-file). For each .cc-file we add two
# lines in the make file. The first line consists of a list of one .cc file
# (corresponding to the .o-file beeing created) and possibly a list of .h-files.
# Whenever any of the files in the list has changed, the make tool will
# recompile the object file. The list should be the .cc-file beeing compiled,
# and all the .h-files that are included in that .cc-file.

main.o: main.cc sensor.h controller.h generator.h
	$(CC) -c $(CFLAGS) $(INCDIR) main.cc -o main.o

sensor.o: sensor.cc sensor.h
	$(CC) -c $(CFLAGS) $(INCDIR) sensor.cc -o sensor.o

controller.o: controller.cc controller.h
	$(CC) -c $(CFLAGS) $(INCDIR) controller.cc -o controller.o

generator.o: generator.cc generator.h
	$(CC) -c $(CFLAGS) $(INCDIR) generator.cc -o generator.o

clean:
	rm -f $(OBJS) $(EXE) *~
