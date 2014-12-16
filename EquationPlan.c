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

	// AB.x = B.x - A.x;
	// AB.y = B.y - A.y;
	// AB.z = B.z - A.z;

	// AC.x = C.x - A.x;
	// AC.y = C.y - A.y;
	// AC.z = C.z - A.z;

	// a = AB.y*AC.z - AB.z*AC.y;
	// b = -AB.x*AC.z + AB.z*AC.x;/*attention au signe*/
	// c = AB.x*AC.y - AB.y*AC.x;
	// d = -a*A.x - b*A.y - c*A.z;/*on résoud*/ 

    /*fprintf(stderr, "verif : %f\n", (xA*eq->m_a
    								+yA*mineurDeux
									+zA*mineurTrois
									+termeIndependant
									+xB*mineurUne
									+yB*mineurDeux
									+zB*mineurTrois
									+termeIndependant
									+xC*mineurUne
									+yC*mineurDeux
									+zC*mineurTrois
									+termeIndependant);*/
	fprintf(stderr, "--- Start Resolution equation --- \n");
	affichageVertex(pt1);
	affichageVertex(pt2);
	affichageVertex(pt3);
	fprintf(stderr, "equation: %fx + %fy + %fz + %f\n", eq->m_a ,eq->m_b ,eq->m_c ,eq->m_d ); 
}