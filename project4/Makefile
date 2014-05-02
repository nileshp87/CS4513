#
# Makefile for saucer shoot game using Dragonfly
#
# 'make depend' to generate new dependency list
# 'make clean' to remove all constructed files
# 'make' to build executable
#
# Variables of interest:
#   GAMESRC is the source code files for the game
#   GAME is the game main() source
#   DEBUG can be set to -DDEBUG to enable various debug flags
#

CC= g++ 
#PLATFORM= -DCYGWIN			# uncomment for cygwin
#PLATFORM= -DMACOS  		# uncomment for macos
LDFLAGS= -lncurses -lrt		# uncomment linux/cygwin
#LDFLAGS= -lncurses 		# uncomment for macos
INCPATH= include		# path to engine includes
ENG= lib/libdragonfly.a	# dragonfly engine
GAMESRC= Saucer.cpp \
         Bullet.cpp \
         Explosion.cpp \
         EventNuke.cpp \
         Points.cpp \
         Star.cpp \
         GameStart.cpp \
         GameOver.cpp \
         Hero.cpp \
         ServerHero.cpp \
         ClientHero.cpp \
         DummyHero.cpp \
         ServerSaucer.cpp \
         ClientSaucer.cpp \
         ClientPoints.cpp \
         ServerPoints.cpp
GAME= game.cpp
EXECUTABLE= game		
OBJECTS= $(GAMESRC:.cpp=.o)

all: $(EXECUTABLE) Makefile

$(EXECUTABLE): $(ENG) $(OBJECTS) $(GAME) $(GAMESRC) 
	$(CC) $(GAME) $(OBJECTS) $(ENG) $(PLATFORM) -o $@ -I$(INCPATH) $(LDFLAGS) 

.cpp.o: 
	$(CC) -c $(DEBUG) -I$(INCPATH) $(PLATFORM) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE) core dragonfly.log Makefile.bak *~

depend: 
	makedepend *.cpp 2> /dev/null

# DO NOT DELETE
