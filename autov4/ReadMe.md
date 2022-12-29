### ***Manuel de construction (fichier README.build à déposer à côté du « .tar.gz », mais aussi à l’intérieur)***

 

#### Procédure de build/construction depuis le « tar.gz »

0.      Prérequis sur le système

Disposer du compilateur « gcc », et de l’outil « make »

Disposer des packages « libreadline » et « libreadline-dev »

à titre d’exemple, pour vérifier sous debian (ou ubuntu) :

> dpkg -l gzip tar gcc make libreadline8 libreadline-dev

Dans le cas contraire, installer les packages requis :
> apt-get install gzip tar gcc make libreadline8 libreadline-dev

(…)

||/ Nom                   Version      Architecture Description

+++-=====================-============-============-======================================================

ii  gcc                   4:10.2.1-1   amd64        GNU C compiler

ii  gzip                  1.10-4       amd64        GNU compression utilities

ii  libreadline-dev:amd64 8.1-1        amd64        GNU readline and history libraries, development files

ii  libreadline8:amd64    8.1-1        amd64        GNU readline and history libraries, run-time libraries

ii  make                  4.3-4.1      amd64        utility for directing compilation

ii  tar                   1.34+dfsg-1  amd64        GNU version of the tar archiving utility

 

1.      Extraire le contenu du « .tar.gz »

> gunzip –c « fichier.tar.gz » | tar xvf –

 

2.      Se positionner dans le répertoire des sources (là où se trouve le « Makefile »)

 

3.      Lancer la construction « make »

$ make

`gcc -Wall -Wextra -I include -c src/automaton_uichar.c -o objs/automaton_uichar.o`

`gcc -Wall -Wextra -I include -c src/automaton_check.c -o objs/automaton_check.o`

`gcc -Wall -Wextra -I include -c src/automaton_DOT.c -o objs/automaton_DOT.o`

`gcc -Wall -Wextra -I include -c src/automaton_file.c -o objs/automaton_file.o`

`gcc -Wall -Wextra -I include -c src/automaton_ui.c -o objs/automaton_ui.o`

`gcc -Wall -Wextra -I include -o bin/automaton_ui  objs/automaton_uichar.o objs/automaton_check.o objs/automaton_DOT.o objs/automaton_file.o objs/automaton_ui.o  -lreadline`

`gcc -Wall -Wextra -I include -c src/get_DOT.c -o objs/get_DOT.o`

`gcc -Wall -Wextra -I include -o bin/get_DOT objs/automaton_uichar.o objs/automaton_file.o objs/automaton_DOT.o objs/get_DOT.o  -lreadline`

`./bin/get_DOT aut/arith.aut > dot/arith.dot`

`./bin/get_DOT aut/dyck.aut > dot/dyck.dot`

`./bin/get_DOT aut/word.aut > dot/word.dot`

`./bin/get_DOT aut/word_bis.aut > dot/word_bis.dot`

 

=>  Par défaut, la construction crée

o   Le binaire interactif « automate_ui » dans bin

o   Le binaire « get_DOT » de construction  d’un graphe au format « .dot »  depuis un fichier décrivant un automate

-> Les fichiers ‘.o’ sont construits et déposés dans le sous-répertoire « objs »

-> Les 2 binaires sont construits et déposés dans le sous-répertoire « bin »

o   Les fichier graphes ‘.dot’ correspondant aux fichiers exemples présents dans le répertoire « aut »

-> Les fichiers ‘.dot’ sont construits et déposée dans le sous-répertoire « dot »