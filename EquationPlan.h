#ifndef _EQUATION_PLAN_H_
#define _EQUATION_PLAN_H_

#include "Vertex.h"


/**
 * \file EquationPlan.h
 * \brief Header de la struct EquationPlan
 * \author GRIVOLLAT-MALVEZIN
 * \version 0.1
 * \date 7 novembre 2014
 *
 * Déclaration de la structure EquationPlan et ces méthodes associées
 *
 */

 /** @defgroup s_equation_plan EquationPlan
 *
 * Equation d'un plan
 * @{
**/


 /**
  * \struct s_equation_plan
  *
  * \brief Objet EquationPlan
  * 
  * EquationPlan représente l'équation d'un plan
  */
typedef struct s_equation_plan
{
	double m_a; /*!< valeur de a dans la formule ax + by + cz + d*/
	double m_b; /*!< valeur de a dans la formule ax + by + cz + d*/
	double m_c; /*!< valeur de a dans la formule ax + by + cz + d*/
	double m_d; /*!< valeur de a dans la formule ax + by + cz + d*/
} EquationPlan;

/**
 * \fn void resolutionEquation(EquationPlan* eq, Vertex* pt1, Vertex* pt2, Vertex* pt3); 
 *
 * \brief resou l'equation d'un plan defini par 3 points
 *
 * \param eq pointeur l'equation de plan
 * \param pt1 pointeur sur le premier poin du plan
 * \param pt2 pointeur sur le second poin du plan
 * \param pt3 pointeur sur le troisième poin du plan
 */
void resolutionEquation(EquationPlan* eq, Vertex* pt1, Vertex* pt2, Vertex* pt3); 

#endif