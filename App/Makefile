# Makefile pour un unique ex�cutable

# liste des variables � renseigner
#   CIBLE : nom du programme ( $(CIBLE).c doit contenir main() )
#   SRCS : ensemble des fichiers sources 
#   LIBS : liste des biblioth�ques utiles � l'�dition des liens 
#          (format : -lnom1 -lnom2 ...) 
#   PREFIX : chemin de la hi�rarchie 
#
# NE PAS OUBLIER D'AJOUTER LA LISTE DES DEPENDANCES A LA FIN DU FICHIER

CIBLE = main

SRC_DIR := ./src
OBJ_DIR := ./target

SRCS := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp) #profondeur 2 max
LIBS = -lm -lpthread  -lstdc++
#########################################################"

INCDIR = .
LIBDIR = .
BINDIR = .

# nom du compilateur
CC = g++
CPP = g++

# options du compilateur          
CFLAGS = -Wall # -g pour gdb https://web.eecs.umich.edu/~sugih/pointers/summary.html
CXXFLAGS = -Wall -g
# option du preprocesseur
CPPFLAGS =  -I$(INCDIR) 

# options du linker et liste des biblioth�ques � charger
LDFLAGS =           
LDLIBS = -L$(LIBDIR) $(LIBS)  


OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS)) 

# cible par d�faut
$(CIBLE): $(OBJS)

install:  $(CIBLE)
	cp $(CIBLE) $(BINDIR)/

installdirs:
	test -d $(INCDIR) || mkdir $(INCDIR)
	test -d $(LIBDIR) || mkdir $(LIBDIR)
	test -d $(BINDIR) || mkdir $(BINDIR)

clean:
	rm -f  *~  $(CIBLE) $(OBJS)

veryclean: clean
	rm -f $(BINDIR)/$(CIBLE)

dep:
	$(CPP) $(CPPFLAGS) -MM $(SRCS)

# Bien penser à créer les sous dossiers dans target si jamais!
main: $(OBJS)
	$(CPP)  -o $@ $^ $(LDFLAGS)  $(CPPFLAGS) $(CXXFLAGS) $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP)  -o $@ $< $(LDFLAGS)  $(CPPFLAGS) $(CXXFLAGS) -c $(LDLIBS)
