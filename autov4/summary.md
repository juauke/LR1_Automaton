1)      Démarche

a.       Analyse de la structure représentant au mieux l’automate

b.       Compréhension du détail du contenu d’un fichier « .aut »

c.       Réflexion sur la pile évoquée dans l’ « algo » quel contenu pourquoi ? le plus efficace ?

d.       Analyse et mise en place de l’algorithme

e.       Analyse du besoin « IHM » => proposition de reprise d’un squelette readline cf. lien

 

2)      Découpage en module

a.       La gestion de l’objet automate et son peuplement depuis le fichier

b.       Mise en œuvre de l’algo de l’automate

c.       L’interface utilisateur reprise et adaptation de Fileman

d.       Analyse du format « DOT » et déclinaison à partir de la structure choisie pour l’automate

 

3)      Difficultés rencontrées

a.       Analyse du fichier « .aut » => partie de texte (première ligne) d’autres donnée au format « binaire/hexa »

Solution mixte de « fscanf » et de « fread »

b.       Fin de séquence « \255\255\255 » en octal différent de « \xFF\xFF\xFF » (octal \377\377\377)

Solution analyse hexa du contenu d’un fichier « .aut » (# od –t x1 <fichier>) confirmation de la valeur octal « \255 »

c.       Mise ne œuvre de l’algorithme => dans le cas du « decale » bien pensé à conserver l’info de « s’ alpha » avant de faire le décalage…

d.       Affichage de la restitution de l’analyse visuellement parlante

Solution affichage coloré sur les terminaux « modernes » => fonctions avancées des terminaux…

e.       Construction du « .dot » : difficulté à constituer les chaînes de caractères contigüe pour une même transition (éviter de citer a,b,,c,d,e,f,g,…z, et privilégier a-Z, A-Z)

Solution nécessitant de ne pas réafficher les caractéres déjà traités dans les cas de transition précédentes => mise en place d’un tableau état pour chaque caractère (et chaque état)

f.        A l’affichage problème des caractères parasites (hors de l’ascii) entrés par l’utilisateur

Solution préparant l’évolution vers un alphabet autre que ascii (stockage des caractère sur des « unsigned int » plutôt que des « char » => ajout des fonctions de conversion de ‘chaîne de char’ ó ‘chaîne de ‘unsigned int’

g.       Adaptation du chargement du fichier « .aut » à un alphabet multibyte (unicode)

                                               Solution prise d’hypothèses sur un format étendu (par exemple le premier ‘a’ du fichier donnant le nombre byte de l’alphabet (ici un seul)… Mais arrêt à la limite des états stockés sur un byte dans le fichier… à suivre.

h.       Affichage X11 pas si simple à gérer… teste avec « vcxSrv »… puis vncServer

i.         Passer en tâche de « fond » du lancement de graphViz ‘dot’ pour l’affichage X11

 

4)      Limitations

a.       Etats de l’automate sur 1 octet dans le fichier « .aut »

b.       Alphabet multibytes à « peaufiner »

c.       Affichage « color » à durcir pour un fonctionnement plus robuste (COLOR_RESET en cas de crash ou d’interruption utilisateur).