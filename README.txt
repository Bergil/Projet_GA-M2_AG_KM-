
Triangle
	vertex tabPoints[3] //trié par ordre trigo
	vertex tabPointsVoisins[3] //trié selon le tab voisins, voisin opposé au point
	List pointAppartenant // Liste de point appartenant au triangle

Vertex
	double coords[3] //tableau de coordonnée
	int hauteur; //hauteur par rapport au triangle auquel il appartient
	triangle //
	bool bool

//Creation des deux triangles de départ
	- division du carré en deux
	- ajout des points appartenant aux triangles

//Ajout d'un point
	- recupérer le premier point de la file de prio qui est le plus haut par rapport au triangle auquel il appartient

//Subdivision d'un triangle
	- tester si l'on peut inverser deux triangles à l'aide du cercle
