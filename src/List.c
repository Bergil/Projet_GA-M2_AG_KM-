﻿#include "List.h"
#include <stdio.h>

#include "Vertex.h"

List* newList()
{
	List* L = (List*) malloc(sizeof(List));
	lstInit(L);
	return L;
}

void lstInit(List* L)
{
	L->First = NULL;
	L->Last = NULL;
	L->Count = 0;
}

void lstFree(List* L)
{
	Node* tmp;
	while(L->Count != 0)
	{
		tmp = L->First;
		L->First = nodeGetNext(tmp);
		delNode(tmp);
		L->Count -= 1;
	}
}

void delList(List* L)
{
	lstFree(L);
	free(L);
}

void lstAdd(List* L, Elem Data)
{
	Node* node = newNode(Data, L->Last, NULL);
	if(L->Count == 0) L->First = node; else L->Last->Next = node;
	L->Last = node;
	L->Count += 1;
}

void lstAddAtBeginning(List* L, Elem Data)
{

	if(L->Count == 0)
	{
		Node* node = newNode(Data, L->Last, NULL);
		L->First = node;
		L->Last = node;
	}
	else
	{
		Node* node = newNode(Data, NULL, L->First);
		Node* First = L->First;
		L->First = node;
		First->Prev = node;
	}

	L->Count++;
}

void lstDel(List* L, Elem E)
{
	Node* tmpNode = L->First;
	while(!nodeEnd(tmpNode) && nodeGetData(tmpNode) != E)
	{
		tmpNode = nodeGetNext(tmpNode);
	}
	if(!nodeEnd(tmpNode))
	{
		lstRem(L, tmpNode);
	}
}

void lstPopFront(List* L)
{
	Node* it = L->First;
	if (it != NULL)
		lstRem(L, it);
}

int lstHaveElem(List* L, Elem E)
{
	Node* tmpNode = L->First;
	while(!nodeEnd(tmpNode) && nodeGetData(tmpNode) != E)
		tmpNode = nodeGetNext(tmpNode);

	if(!nodeEnd(tmpNode))
		return 1;
	return 0;
}

void lstRem(List* L, Node* N)
{
	if(N == L->First)
	{
		if(N == L->Last)
		{
			L->First = NULL;
			L->Last = NULL;
		} else {
			nodeSetPrev(nodeGetNext(N), NULL);
			L->First = nodeGetNext(N);
		}
	} else if(N == L->Last) {
		nodeSetNext(nodeGetPrev(N), NULL);
		L->Last = nodeGetPrev(N);
	} else {
		nodeSetNext(nodeGetPrev(N), nodeGetNext(N));
		nodeSetPrev(nodeGetNext(N), nodeGetPrev(N));
	}
	delNode(N);
	L->Count--;
}

Node* lstFirst(List* L)
{
	return L->First;
}

Node* lstEnd(List* L)
{
	return L->Last;
}

unsigned int lstCount(List* L)
{
	return L->Count;
}

#include <assert.h>
void lstRegressionTest()
{
	unsigned int i;
	printf("============== lstRegressionTest Begin ==============\n");
	List* L = newList();
	assert(L->Count == 0);
	int var[500];
	for(i = 0; i < 500; i++)
	{
		lstAdd(L, (void*) &var[i]);
		assert(lstHaveElem(L, (void*) &var[i]));
		assert(!lstHaveElem(L, (void*) &var[i+1]));
		assert(L->Count != 0);
		assert(lstCount(L) == i + 1);
	}
	for(i = 0; i < 500; i++)
	{
		assert(lstCount(L) == 500 - i);
		lstDel(L, (void*) &var[i]);
	}
	assert(L->Count == 0);
	delList(L);
	printf("============== lstRegressionTest End ================\n");
}


void affichage2DList(List* l)
{
	glBegin(GL_POINTS);

	Node * node_temp = l->First;
	Vertex * vertex_temp;
	while(node_temp != NULL)
	{
		vertex_temp = nodeGetData(node_temp);
		glVertex2f(vertex_temp->coords[0], vertex_temp->coords[1]);
		node_temp = nodeGetNext(node_temp);
	}

	glEnd();
}