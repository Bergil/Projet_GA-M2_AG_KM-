
triangle* newTriangle()
{
	pt1 = NULL;
	pt2 = NULL;
	pt3 = NULL;
}

triangle* newTriangle(vertex* p1, vertex* p2, vertex* p3)
{
	triangle* t = (triangle*) malloc(sizeof(triangle));
	t->pt1 = p1;
	if(orientationPolaire(p1,p2,p3)){
		t->pt2 = p2;
		t->pt3 = p3; 
	}else{
		t->pt2 = p3;
		t->pt3 = p2; 
	}	
}

void deleteAllTriangle(triangle* t)
{
	free(t->pt1);
	free(t->pt2);
	free(t->pt3);
	free(t);
}

void deleteTriangle(triangle* t)
{
	free(t);
}

//Teste si le point est compris dans un triangle
int estDansTriangle(triangle* t, vertex *pt)
{
    int orientationTriangle;
    orientationTriangle = orientationPolaire(t->pt1,t->pt2,t->pt3);
    if(orientationPolaire(t->pt1,t->pt2,pt) == orientationTriangle || orientationPolaire(t->pt1,t->pt2,pt) == ALIGNE)
    {
		if(orientationPolaire(t->pt2,t->pt3,pt) == orientationTriangle || orientationPolaire(t->pt2,t->pt3,pt) == ALIGNE)
		{
			if(orientationPolaire(t->pt3,t->pt1,pt) == orientationTriangle || orientationPolaire(t->pt3,t->pt1,pt) == ALIGNE){
				if(orientationPolaire(t->pt1,t->pt2,pt) == ALIGNE || orientationPolaire(t->pt2,t->pt3,pt) == ALIGNE ||
				   orientationPolaire(t->pt3,t->pt1,pt) == ALIGNE)
					return -1;
				return 1;
			}
		}
    }	
    return 0;
}
