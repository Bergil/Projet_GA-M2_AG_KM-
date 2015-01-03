#include "Vertex.h"

#define RIGHT -1
#define STRAIGHT 0
#define LEFT 1

int Angle (Vertex *A, Vertex *B, Vertex *C);
int InCircle (Vertex *A, Vertex *B, Vertex *C, Vertex *Z);