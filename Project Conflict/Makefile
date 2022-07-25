ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=D:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: ProjectConflict.exe

ProjectConflict.exe: conflict.o
	gcc -o ProjectConflict.exe conflict.o $(PATH_ALLEGRO)$(LIB_ALLEGRO) my.res

conflict.o: conflict.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c conflict.c
	
	
clean:
	del conflict.o 
	del ProjectConflict.exe

