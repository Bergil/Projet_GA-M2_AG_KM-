#include "Vertex.h"

#define RIGHT -1
#define STRAIGHT 0
#define LEFT 1

/**
 * \file geom.h
 * \brief Header de la struct geom
 * \version 0.1
 * \date 7 novembre 2014
 */

int Angle (Vertex *A, Vertex *B, Vertex *C);
int InCircle (Vertex *A, Vertex *B, Vertex *C, Vertex *Z);