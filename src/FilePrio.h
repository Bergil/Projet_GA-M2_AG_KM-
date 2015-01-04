#ifndef __FP_H__
#define __FP_H__


#define TAILLE_MAX 10000

#include <stdio.h>
#include <stdlib.h>
#include "Simplex.h"

/**
 * \file FilePrio.h
 * \brief Header de la struct fdp
 * \author GRIVOLLAT-MALVEZIN
 * \version 0.1
 * \date 7 novembre 2014
 *
 * Déclaration de la structure fdp et ces méthodes associées
 *
 */

 /** @defgroup s_fdp fdp
 *
 * File de priorité contenant des simplexs
 * @{
**/


 /**
  * \struct s_fdp
  *
  * \brief Objet fdp
  * 
  * fdp est une structure permettant de stocker des simplex de façon ordonner
  */
typedef struct s_fdp
{
  Simplex** tableau_Simplex; /*!< tableau de pointeur sur simplex*/
  int nbSimplex; /*!< nombre de simplex contenu dans la file de priorité */
} fdp;

/**
 * \fn fdp * allouerFDP(int taille)
 *
 * \brief creation d'une instance de fdp intitialisée à une certaine taille
 *
 * \param taille de la file de prio
 *
 * \return une instance de fdp
 */
fdp * allouerFDP(int taille);

/**
 * \fn void initFDP(fdp * f, int nb_element)
 *
 * \brief initialise une fdp déjà allouée
 *
 * \param f pointeur sur file de priorité
 * \param taille taille max de la file de rpio
 */
void initFDP(fdp * f, int taille);

/**
 * \fn void upHeap(fdp *f, int position)
 *
 * \brief Fais remonter le simplex dans la file de prio si sa hauteur est superieure à son père
 *
 * \param f pointeur sur file de prio
 * \param position du simplex que l'on souhaite remonter
 */
void upHeap(fdp *f, int position);

/**
 * \fn void downHeap(fdp *f)
 *
 * \brief Fais redescendre le premier element de la fdp
 *
 * \param f pointeur dur la file de prio
 */
void downHeap(fdp *f);
void downHeapPos(fdp *f, int position);
/**
 * \fn void insertSimplex(fdp *f, Simplex *s)
 *
 * \brief insert un simplex dans la file de prio, il sera ordonné tout seul
 *
 * \param f pointeur sur file de prio
 * \param s pointeur sur le simplex que l'on souhaite ajouté
 */
void insertSimplex(fdp *f, Simplex *s);

/**
 * \fn Simplex* getTete(fdp *f) 
 *
 * \brief recupère la tete de la file de rpio
 *
 * \param f pointeur sur uen file de prio
 *
 * \return simplex en tête de la file de prio
 */
Simplex* getTete(fdp *f);

/**
 * \fn void affichageHauteurFDP(fdp * f)
 *
 * \brief affiche la hauteur des simplex de la file de prio
 *
 * \param f pointeur sur la file de prio
 */
void affichageHauteurFDP(fdp * f);

/**
 * \fn void cleanFDP(fdp *f)
 * 
 * \brief nettoyage de la file de prio
 *
 * \param f pointeur sur la file de prio
 */
void cleanFDP(fdp *f);

/**
 * \fn void destructionFDP(fdp * f)
 *
 * \brief destruction de la fdp
 *
 * \param f file de prio
 */
void destructionFDP(fdp * f);

/**
 * \fn void supprElement(fdp * f, Simplex * s)
 *
 * \brief supprime un élément dans la file de prio
 *
 * \param f pointeur sur la file de rpio
 * \param s simplex a supprimer
 */
void supprElement(fdp * f, Simplex * s);

/**
 * \fn void affichageFDP(fdp * f)
 *
 * \brief affiche la fdp 
 *
 * \param f pointeur sur la file de prio
 */ 
void affichageFDP(fdp * f);


int positionDansFDP(fdp * f, Simplex *s);
#endif

