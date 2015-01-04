#ifndef _PILE_H_
#define _PILE_H_

#include <stdlib.h>
#include "Simplex.h"
#include "Tool.h"

/**
 * \file Pile.h
 * \brief Header de la struct Pile
 * \author GRIVOLLAT-MALVEZIN
 * \version 0.1
 * \date 7 novembre 2014
 *
 * Déclaration de la structure Pile et ces méthodes associées
 *
 */

/** @defgroup s_pile Pile
 *
 * Une pile de Simplex
 * @{
**/

/**
 * \struct s_pile
 *
 * \brief Objet Pile
 *
 * Pile est une structure pour stocker des simplex
 */
typedef struct s_pile
{
	Simplex ** m_tab_simplex; /*!< tableau de pointeur sur simplex */
	int m_nb_elements; /*!< nombre d'elements dans la pile */	
} Pile;

/**
 * \fn Pile * creationPile(int nb_elements)
 *
 * \brief Creation d'une instance de pile initialisé à une certaine taille
 *
 * \param taille taille max de la pile
 *
 * \return retourne la pile créée
 */
Pile * creationPile(int taille);

/**
 * \fn int estDans(Pile * p, Simplex * s)
 *
 * \brief Test si un simplex est dans la pile
 *
 * \param p pointeur sur pile
 * \param s pointeur sur simplex que l'on souhaite chercher dans la pile
 *
 * \return 1 si le simplex est dans la pile, 0 sinon
 */
int estDans(Pile * p, Simplex * s);

/**
 * \fn void empiler(Pile * p, Simplex * s)
 *
 * \brief ajoute un simplex à la suite des simplexs deja dans la pile
 *
 * \param p pointeur sur pile
 * \param s pointeur sur simplex que l'on souhaite empiler
 */
void empiler(Pile * p, Simplex * s);

/**
 * \fn Simplex * depiler(Pile * p)
 *
 * \brief depile le premier simplex de la pile
 *
 * \param p pointeur sur la pile
 *
 * \return retourne le simplex depiler
 */
Simplex * depiler(Pile * p);

/**
 * \fn void affichagePile(Pile * p)
 *
 * \brief affiche les coordonnées des points des simplexs contenus dans la pile
 *
 * \param p pointeur sur pile
 */
void affichagePile(Pile * p);

#endif