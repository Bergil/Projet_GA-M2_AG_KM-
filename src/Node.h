﻿#ifndef _NODE_H_
#define _NODE_H_

#include <stdlib.h>

/**
 * \file Node.h
 * \brief Header de la struct Node
 * \version 0.1
 * \date 7 novembre 2014
 *
 * Déclaration de la structure Node et ces méthodes associées
 *
 */

/** @defgroup s_Node Node
 *
 * Elément d'une List (Liste doublement chaînée)
 * Sert également l'itérateur sur ces listes.
 * Préfixe des fonctions : node
 * @{
**/

typedef void* Elem;

/**
  * \struct s_Node
  *
  * \brief Objet Node
  * 
  * Node est un noeud doublement chainé
  */
typedef struct s_Node
{
	Elem Data; /*!< Valeur du noeud */
	struct s_Node* Prev; /*!< pointeur sur l'élement précédent */
	struct s_Node* Next; /*!< pointeur sur l'élément suivant */
} Node;

/** @brief Alloue un nouveau Node en mémoire et l'initialise
 *
 * @param Data Information portée par le Node
 * @param Prev Node précédent dans le conteneur
 * @param Next Node suivant dans le conteneur
 * @return Pointeur vers le Node nouvellement créé
**/
Node* newNode(Elem Data, Node* Prev, Node* Next);

/** @brief Initialise un node avec la valeur Data
 *
 * @param ptrNode Node à initialiser
 * @param Data Information portée par le Node
 * @param Prev Node précédent dans le conteneur
 * @param Next Node suivant dans le conteneur
**/
void nodeInit(Node* ptrNode, Elem Data, Node* Prev, Node* Next);

/** @brief Libère le node
 *
 * @param ptrNode Node à détruire
**/
void delNode(Node* ptrNode);

/* Accesseurs */
/** @brief Retourne le noeud précédent
**/
Node* nodeGetPrev(Node* ptrNode);
/** @brief Retourne l'information portée par le noeud
**/
Elem nodeGetData(Node* ptrNode);
/** @brief Retourne le noeud suivant
**/
Node* nodeGetNext(Node* ptrNode);
/** @brief Teste la fin d'une liste
 *
 * @return 1 si la fin de la liste est atteinte
**/
int nodeEnd(Node* ptrNode);

/* Mutateurs */
/** @brief Mutateur du précédent
**/
void nodeSetPrev(Node* ptrNode, Node* prevNode);
/** @brief Mutateur du suivant
**/
void nodeSetNext(Node* ptrNode, Node* nextNode);

/** @}
**/

#endif
