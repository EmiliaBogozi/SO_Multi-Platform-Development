#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#define MAX_ELEM 40

typedef struct list {
	char *value;
	struct list *next;
	struct list *prev;
}list;


void initList(list** a) {
	*a = NULL;
}

//functia de adaugare a unui nou nod in lista la final
void addList(list **a, char *new_value) {

	//pregatim nodul de inserat
	list *new = (list *)calloc(1, sizeof(list));

	new->value = (char *)calloc(strlen(new_value) + 1, sizeof(char));
	strcpy(new->value, new_value);
	new->next = NULL;
	new->prev = NULL;

	//adaugare a unui element intr-o lista vida
	if (*a == NULL) {
		*a = new;
		return;
	}

	//pointer auxiliar de parcurgere; parcurgere pana la ultima pozitie nevida
	list* aux = *a;

	while (aux->next != NULL)
		aux = aux->next;

	//adaugare efectiva
	aux->next = new;
	new->prev = aux;
}

//functia de afisare a listei
void show(list* a) {
	list* p = a;
	while (a != NULL) {
		printf("%s ", a->value);
		p = a;
		a = a->next;
	}
	a = p;
	printf("\n");
	/*while (a != NULL) {
		printf("%s ", a->value);
		p = a;
		a = a->prev;
	}
	printf("\n");*/
}

int main() {
	//Creati propriul vostru main
	//Valorile de introdus/sters se vor citi de la tastatura
	list *l;
	initList(&l);
	addList_Final(&l, "1");
	addList_Final(&l , "2");
	addList_Final(&l, "2");
	addList_Final(&l, "4");
	addList_Final(&l, "3");
	show(l);
	return 0;
}

