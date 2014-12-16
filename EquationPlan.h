#ifndef _EQUATION_PLAN_H_
#define _EQUATION_PLAN_H_

#include "Vertex.h"

typedef struct s_equation_plan
{
	double m_a;
	double m_b;
	double m_c;
	double m_d;
} EquationPlan;

void resolutionEquation(EquationPlan* eq, Vertex* pt1, Vertex* pt2, Vertex* pt3); 

#endif