#include "EquationPlan.h"

void resolutionEquation(EquationPlan* eq, Vertex* pt1, Vertex* pt2, Vertex* pt3)
{
	Vertex AB;
	Vertex AC;

	AB.coords[0] = pt2->coords[0] - pt1->coords[0];
	AB.coords[1] = pt2->coords[1] - pt1->coords[1];
	AB.coords[2] = pt2->coords[2] - pt1->coords[2];

	AC.coords[0] = pt3->coords[0] - pt1->coords[0];
	AC.coords[1] = pt3->coords[1] - pt1->coords[1];
	AC.coords[2] = pt3->coords[2] - pt1->coords[2];

	eq->m_a =  AB.coords[1] * AC.coords[2] - AB.coords[2] * AC.coords[1];
	eq->m_b = -AB.coords[0] * AC.coords[2] + AB.coords[2] * AC.coords[0];/*attention au signe*/
	eq->m_c =  AB.coords[0] * AC.coords[1] - AB.coords[1] * AC.coords[0];
	eq->m_d = -eq->m_a*pt1->coords[0] - eq->m_b*pt1->coords[1] - eq->m_c*pt1->coords[2];/*on résoud*/

}
