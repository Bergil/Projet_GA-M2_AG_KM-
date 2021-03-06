﻿#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>

#include <GL/glut.h> 
#include "Node.h"

/**
 * \file List.h
 * \brief Header de la struct List
 * \version 0.1
 * \date 7 novembre 2014
 *
 * Déclaration de la structure List et ces méthodes associées
 *
 */

/** @defgroup s_list List
 *
 * Liste doublement chaînée de void*
 * Préfixe des fonctions : lst
 * @{
**/

/**
  * \struct s_list
  *
  * \brief Objet List
  * 
  * List est une liste chainée
  */
typedef struct s_list
{
	Node* First; /*!< Premier élément de la liste */
	Node* Last; /*!< Deuxième élément de la liste */
	unsigned int Count; /*!< nombre d'élément de la liste */
} List;

/** @brief Réserve une nouvelle liste en mémoire et l'initialise
 *
 * @return Pointeur vers la liste nouvellement crée
**/
List* newList();

/** @brief Initialise la liste à vide
 *
 * @param L Liste à initialiser
**/
void lstInit(List* L);

/** @brief Libère les Nodes de la liste
 *
 **/
void lstFree(List* L);

/** @brief Libère les Nodes de la liste puis libère la liste
 *
**/
void delList(List* L);

/** @brief Ajoute un élément en fin (enfile)
**/
void lstAdd(List* L, Elem Data);

/** @brief Ajoute un élément en début (enpile)
 **/
void lstAddAtBeginning(List* L, Elem Data);

/** @brief Supprime l'élément passé en paramètre s'il se trouve dans la liste
 *
 * Complexité en O(n)
 * @param L Liste où chercher l'élément
 * @param E Elément à supprimer
**/
void lstDel(List* L, Elem E);

/** @brief Supprime le premier élément dans la liste
 *
 * @param L Liste
 **/
void lstPopFront(List* L);

/** @brief Chercher l'existence d'un élément dans la liste
 *
 * Complexité en O(n)
 * @param L Liste où chercher l'élément
 * @param E Elément à chercher
 **/
int lstHaveElem(List* L, Elem E);

/** @brief Supprime le Node passé en paramètre
 *
 * Complexité en O(1)
 * ATTENTION ! Comportement aléatoire si L ne contient pas N !
 * @param L Liste contenant N
 * @param N Node à supprimer
**/
void lstRem(List* L, Node* N);

/** @brief Retourne un pointeur vers le premier Node de la liste
**/
Node* lstFirst(List* L);

/** @brief Retourne un pointeur vers le dernier Node de la liste
**/
Node* lstEnd(List* L);

/** @brief Accesseur de Count
**/
unsigned int lstCount(List* L);

/** @brief Test de Regression
**/
void lstRegressionTest();

/** @}
**/
void affichage2DList(List* l);
#endif
