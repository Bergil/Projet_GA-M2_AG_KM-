
Triangle
	vertex tabPoints[3] //trié par ordre trigo
	Triangle tabPointsVoisins[3] //trié selon le tab voisins, voisin opposé au point //Face
	List pointAppartenant // Liste de point appartenant au triangle // SEUL LE PREMIER A UNE IMPORTANCE
	int hauteur; //hauteur maximum des points par rapport à ce triangle
	booleen isEmpty;

Vertex
	double coords[3] //tableau de coordonnée

//Creation des deux triangles de départ
	- division du carré en deux
	- ajout des points appartenant aux triangles

//Ajout d'un point
	- recupérer le premier simplex de la file de prio (donc celui avec le point le plus haut)
	- subdivision du simplex
	- une fois ajouter au maillage, le supprimer de la liste

//Subdivision d'un triangle
	- tester si l'on peut inverser deux triangles à l'aide du cercle
	- si inversion, stocker les nouvaux triangles dans une pile et tester les inversions tant que le pile est non vide
	- si les triangles sont trouvé,
		- recalculer l'appartenance des points à un triangle
		- recalculer la hauteur max
		- remettre les voisins
	- mettre à jour la file de prio sur les triangles pour ejecter le triangle avec le point le plus haut

//Inversion de triangle
	- recreer les nouveau triangles
	- supprimer les anciens
	- mettre à jour la file

//Option du programme
	- -nX : X = nombre de points
	- -fX : X = nombre de facettes
	- -dX : X = nombre de dimensions (2 ou 3)
	- -pX : X = dessin direct ou pas-à-pas (0 ou 1)
	
//Manipulation de la caméra en mode 3D

	- Q et D : translation sur axe X
	- Z et S : translation sur axe Y
	- A et E : translation sur axe Z
	
	- I et K : rotation sur axe X
	- J et L : rotation sur axe Y
	- U et O : rotation sur axe Z