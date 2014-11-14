
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
	t->pt2 = p2;
	t->pt3 = p3; 
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