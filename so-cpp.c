#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "functions.h"

/*Initializarea HashMap-ului si alocarea de memorie*/
HashMap *initHashMap(int size)
{
	HashMap *h = (HashMap *)calloc(1, sizeof(HashMap));

	DIE(h == NULL, "12");

	h->size = 0;
	h->list = (node **)calloc(size, sizeof(node *));
	DIE(h->list == NULL, "12");

	return h;
}

/*Initializarea listei*/
void initList(list **a)
{
	*a = NULL;
}

/*Inserarea unui nod in HashMap*/
void insert(HashMap *h, char *symbol, char *mapping)
{
	node *new = (node *)calloc(1, sizeof(node));

	DIE(new == NULL, "12");

	new->symbol = (char *)calloc(strlen(symbol) + 1, sizeof(char));
	DIE(new->symbol == NULL, "12");
	strcpy(new->symbol, symbol);

	new->mapping = (char *)calloc(strlen(mapping) + 1, sizeof(char));
	DIE(new->mapping == NULL, "12");
	strcpy(new->mapping, mapping);

	h->list[h->size] = new;
	h->size++;
}

/*Stergea unui nod din HashMap*/
void delete(HashMap *h, char *symbol)
{
	int i;

	for (i = 0; i < h->size; i++)
		if (strcmp(h->list[i]->symbol, symbol) == 0)  {
			if (i+1 < h->size) {
				h->list[i]->symbol = h->list[i + 1]->symbol;
				h->list[i]->mapping = h->list[i + 1]->mapping;
				free(h->list[h->size]->symbol);
				free(h->list[h->size]->mapping);
				free(h->list[h->size]);
			} else {
				free(h->list[i]->symbol);
				free(h->list[i]->mapping);
				free(h->list[i]);
			}
		}
	h->size--;
}

/*Adaugarea intr-o lista la sfarsit*/
void addListFirst(list **l, char *new_value)
{
	list *new = (list *)calloc(1, sizeof(list));

	DIE(new == NULL, "12");

	new->value = (char *)calloc(strlen(new_value) + 1, sizeof(char));
	DIE(new->value == NULL, "12");

	strcpy(new->value, new_value);
	new->next = (*l);
	new->prev = NULL;

	if ((*l) != NULL)
		(*l)->prev = new;

	(*l) = new;
}

/*Adaugarea intr-o lista la inceput*/
void addList(list **a, char *new_value)
{
	list *new;
	list *aux;

	new = (list *)calloc(1, sizeof(list));
	DIE(new == NULL, "12");

	new->value = (char *)calloc(strlen(new_value) + 1, sizeof(char));
	DIE(new->value == NULL, "12");

	strcpy(new->value, new_value);
	new->next = NULL;
	new->prev = NULL;

	if (*a == NULL) {
		*a = new;
		return;
	}

	aux = *a;

	while (aux->next != NULL)
		aux = aux->next;

	aux->next = new;
	new->prev = aux;
}

/*Dezalocarea de memorie a HashMap-ului*/
void free_HashMap(HashMap *h)
{
	int i;

	for (i = 0; i < h->size; i++) {
		free(h->list[i]->symbol);
		free(h->list[i]->mapping);
		free(h->list[i]);
	}
	free(h->list);
	free(h);
}

/*Dezalocarea de memorie a listei*/
void free_list(list *l)
{
	list *aux;

	while (l != NULL) {
		aux = l;
		l = l->next;
		free(aux->value);
		free(aux);
	}
	free(l);
}

/*Functie de inlocuirea a tuturor aparitiilor
 *unui cuvant cu altul intr-un string.
 *Se verifica daca newWord il contine pe oldWord, iar in acest caz
 *se iese fortat din while la finalul inlocuirii; in caz contrar se va
 *executa while-ul la infinit.
 */
void replaceWord(char *str, char *oldWord, char *newWord)
{
	char *pos, temp[257];
	int index = 0;
	int owlen;
	int contains = 0;

	if (strstr(newWord, oldWord) != NULL)
		contains = 1;

	owlen = strlen(oldWord);
	while ((pos = strstr(str, oldWord)) != NULL) {
		strcpy(temp, str);
		index = pos - str;
		str[index] = '\0';
		//AICI ERORI
		strcat(str, newWord);
		strcat(str, temp + index + owlen);

		if (contains == 1 && index == 1)
			break;
	}
}

/*Functie de decodare a directivelor pentru a putea
 *fi folosite intr-un switch
 */
int decode(char *c)
{
	if (strcmp(c, "#define") == 0)
		return 1;
	if (strcmp(c, "#undef") == 0)
		return 2;
	if (strcmp(c, "#if") == 0)
		return 3;
	if (strcmp(c, "#ifdef") == 0)
		return 4;
	if (strcmp(c, "#ifndef") == 0)
		return 5;
	if (strcmp(c, "#include") == 0)
		return 6;
	return 7;
}

/*Functie care adauga in HashMap symbol-urile si mapping-ul
 *din argumentele functiei
 */
void define_args(char *argv[], int *i, HashMap **h)
{
	//[-D <SYMBOL>[=<MAPPING>]] sau [-D<SYMBOL>[=<MAPPING>]]
	char *symbol;
	char *mapping;
	char *token;

	if (strcmp(argv[*i], "-D") == 0) {
		token = strtok(argv[*i + 1], "=");
		symbol = (char *)calloc(strlen(token) + 1,
			sizeof(char));
		DIE(symbol == NULL, "12");
		strcpy(symbol, token);
		token = strtok(NULL, " ");
		if (token != NULL) {
			mapping = (char *)calloc(strlen(token) + 1,
				sizeof(char));
			DIE(mapping == NULL, "12");
			strcpy(mapping, token);
			insert(*h, symbol, mapping);
			free(symbol);
			free(mapping);
		} else {
			insert(*h, symbol, "");
			free(symbol);
		}
		(*i)++;
	} else {
		token = strtok(argv[*i] + 2, "=");
		symbol = (char *)calloc(strlen(token) + 1,
			sizeof(char));
		DIE(symbol == NULL, "12");
		strcpy(symbol, token);

		if (token != NULL) {
			mapping = (char *)calloc(strlen(token) + 1,
				sizeof(char));
			DIE(mapping == NULL, "12");
			strcpy(mapping, token);
			insert(*h, symbol, mapping);
			free(symbol);
			free(mapping);
		} else {
			free(symbol);
			insert(*h, symbol, "");
		}
	}
}

/*Functie care adauga in lista path-urile pentru directiva
 *include din argumentele functiei
 */
void include_args(char *argv[], int *i, list **l)
{
	char *token;
	char *dir;

	if (strcmp(argv[*i], "-I") == 0) {
		token = strtok(argv[*i + 1], "\n");
		dir = (char *)calloc(strlen(token) + 1, sizeof(char));
		DIE(dir == NULL, "12");
		strcpy(dir, token);

		addList(l, dir);
		free(dir);
		(*i)++;
	}
}

/*Functie care adauga in HashMap symbol-urile si mapping-ul
 *corespunzatoare directivei #define, daca mapping-urile
 *sunt definite anterior, se foloseste valoarea din HashMap
 */
void define(HashMap **h, char *line[])
{
	char *symbol;
	char *mapping;
	int i;
	char *token;
	char *new_token;
	char *aux;

	token = strtok(*line, " \r\n");
	token = strtok(NULL, " \r\n");

	symbol = (char *)calloc(strlen(token) + 1, sizeof(char));
	DIE(symbol == NULL, "12");
	strcpy(symbol, token);

	token = strtok(NULL, "\n");

	if (token != NULL) {
		mapping = (char *)calloc(257, sizeof(char));
		DIE(mapping == NULL, "12");
		strcpy(mapping, token);
	} else {
		mapping = (char *)calloc(257, sizeof(char));
		DIE(mapping == NULL, "12");
		strcpy(mapping, "");
	}

	for (i = 0; i < (*h)->size; i++) {
		aux = (char *)calloc(strlen(mapping) + 1, sizeof(char));
		DIE(aux == NULL, "12");
		strcpy(aux, mapping);
		token = strtok(aux, "\n\"()- ");

		while (token != NULL) {
			if (strcmp(token, (*h)->list[i]->symbol) == 0)
				replaceWord(mapping,
				(*h)->list[i]->symbol, (*h)->list[i]->mapping);
			token = strtok(NULL, "\n\"()- ");
		}
		free(aux);
	}
	insert(*h, symbol, mapping);
	free(symbol);
	free(mapping);
}

/*Functie care scoate din HashMap symbol-urile si mapping-ul
 *in functie de symbol-ul dat
 */
void undefine(HashMap **h, char **token)
{
	char *symbol;

	*token = strtok(NULL, "\n");
	symbol = (char *)calloc(strlen(*token) + 1, sizeof(char));
	DIE(symbol == NULL, "12");
	strcpy(symbol, *token);
	delete(*h, symbol);
	free(symbol);
}


void check_if_else(char *copy_line, char *line[], int cond_check,
	int write_file, FILE *fwrite, HashMap *h)
{
	char *token;

	token = strtok(copy_line, " \r\n");
	if (strcmp(token, "#define") == 0)
		define(&h, line);
	else {
		if (cond_check != 0)
			if (write_file == 1)
				fprintf(fwrite, "%s", *line);
			else
				printf("%s", *line);
	}
}

/*Functie care verifica directivele #if, #elif, #else, #else
 *In variabila cond se stocheaza conditia, iar in cond_check se
 *stocheaza valoarea ei de adevar. In cazul in care conditia este
 *un string, atunci se cauta in HashMap si este inlocuita cu mapping-ul
 *corespunzator.
 */
void if_else(FILE *fread, FILE *fwrite, int write_file, HashMap *h,
	char *line[])
{
	int i;
	char *cond;
	char *copy_line;
	int len = 256;
	int cond_check;
	char *token;

	token = strtok(*line, " \r\n");
	token = strtok(NULL, " \r\n");
	cond = (char *)calloc(strlen(token) + 1, sizeof(char));
	DIE(cond == NULL, "12");
	strcpy(cond, token);

	for (i = 0; i < h->size; i++)
		if (strcmp(h->list[i]->symbol, cond) == 0) {
			free(cond);
			cond = (char *)calloc(strlen(h->list[i]->mapping)
				+ 1, sizeof(char));
			DIE(cond == NULL, "12");
			strcpy(cond, h->list[i]->mapping);
		}

	cond_check = atoi(cond);
	free(cond);
	fgets(*line, len, fread);

	copy_line = (char *)calloc(strlen(*line) + 1, sizeof(char));
	DIE(copy_line == NULL, "12");
	strcpy(copy_line, *line);

	while (strncmp(*line, "#endif", 5) != 0) {
		token = strtok(copy_line, " \r\n");
		if (strcmp(token, "#else") == 0) {
			fgets(*line, len, fread);
			if (write_file == 1 && cond_check == 0)
				fprintf(fwrite, "%s", *line);
			if (write_file == 0 && cond_check == 0)
				printf("%s", *line);
		} else {
			if (strcmp(token, "#elif") == 0) {
				token = strtok(*line, " \r\n");
				token = strtok(NULL, " \r\n");

				cond = (char *)calloc(strlen(token)
					+ 1, sizeof(char));
				DIE(cond == NULL, "12");
				strcpy(cond, token);

				for (i = 0; i < h->size; i++)
					if (strcmp(h->list[i]->symbol, cond)
						== 0) {
						free(cond);
						cond = (char *)calloc(strlen(
							h->list[i]->mapping)
							+ 1, sizeof(char));
						DIE(cond == NULL, "12");
						strcpy(cond,
							h->list[i]->mapping);
					}

				cond_check = atoi(cond);
				free(cond);
				fgets(*line, len, fread);

				if (write_file == 1 && cond_check != 0)
					fprintf(fwrite, "%s", *line);
				if (write_file == 0 && cond_check != 0)
					printf("%s", *line);
			} else
				check_if_else(copy_line, line, cond_check,
					write_file, fwrite, h);
		}
		free(copy_line);

		fgets(*line, len, fread);
		copy_line = (char *)calloc(strlen(*line) + 1, sizeof(char));
		DIE(copy_line == NULL, "12");
		strcpy(copy_line, *line);
	}
	free(copy_line);
}

char *search_path(char *c)
{
	int i = 0;
	int count = 0;
	char *new = (char *)calloc(256, sizeof(char));

	DIE(new == NULL, "12");

	for (i = strlen(c); i > 0; i--) {
		count++;
		if (c[i] == '/')
			break;
	}
	strncpy(new, c, strlen(c) - count + 2);
	new[strlen(c) - count + 3] = '\0';

	return new;
}

/*Functie care verifica daca intr-un string, substringul dat se afla
 *intr-un comentariu (intre ghilimele)
 */
int check_message(char *line, char *symbol)
{
	char *token;
	char *aux;
	int len1, len2;

	if (strstr(line, "\"") != NULL) {
		token = strtok(line, "\"");
		len1 = strlen(token);

		token = strtok(NULL, "\"");
		len2 = strlen(token);

		aux = (char  *)calloc(strlen(token) + 1, sizeof(char));
		DIE(aux == NULL, "12");
		strcpy(aux, token);

		if (strstr(aux, symbol) != NULL) {
			free(aux);
			return len1 + len2 + 2;
		}
		free(aux);
	}
	return 0;
}

void text(char **token, char *line, HashMap *h, int write_file, FILE *fwrite)
{
	char *result;
	int i;
	int value;
	char *copy_line;

	if (strcmp(*token, "\n") != 0) {

		result = (char *)calloc(257, sizeof(char));
		DIE(result == NULL, "12");
		strcpy(result, line);

		for (i = 0; i < h->size; i++)
			if (strstr(line, h->list[i]->symbol) != NULL) {
				copy_line = (char *)calloc(strlen(line)
					+ 1, sizeof(char));
				DIE(copy_line == NULL, "12");
				strcpy(copy_line, line);
				value = check_message(copy_line,
					h->list[i]->symbol);
				replaceWord(result + value,
					h->list[i]->symbol,
					h->list[i]->mapping);
				free(copy_line);
			}
		if (write_file == 1)
			fprintf(fwrite, "%s", result);
		else
			printf("%s", result);

		free(result);
	}
}

/*Functie care urmareste parcurgerea HashMap-ului si verificarea daca
 *simbolul dat exista; daca exista inseamna ca a fost definit. Am folosit
 *o variabila "find" care are valoarea 1 daca a fost gasit simbolul si
 *valoarea 0 daca nu. Se iese din functie la intalnirea terminatorului #endif
 */
void if_def(HashMap *h, FILE *fread, FILE *fwrite, int write_file, char *line[])
{
	int i;
	char *symbol;
	int find = 0;
	int len = 256;
	char *token;
	char *copy_line;
	int value;

	token = strtok(*line, " \r\n");
	token = strtok(NULL, " \r\n");
	symbol = (char *)calloc(strlen(token) + 1, sizeof(char));
	DIE(symbol == NULL, "12");
	strcpy(symbol, token);
	fgets(*line, len, fread);

	copy_line = (char *)calloc(strlen(*line) + 1, sizeof(char));
	DIE(copy_line == NULL, "12");
	strcpy(copy_line, *line);

	for (i = 0; i < h->size; i++)
		if (strcmp(h->list[i]->symbol, symbol) == 0)
			find = 1;

	while (strncmp(*line, "#endif", 5) != 0) {
		if (strcmp(token, "#else") == 0) {
			fgets(*line, len, fread);
			if (find == 0) {
				token = strtok(copy_line, " \r\n");
				value = decode(token);

				switch (value) {
				case 1:
					define(&h, line);
					break;
				case 2:
					undefine(&h, line);
					break;
				case 3:
					if_else(fread, fwrite, write_file,
						h, line);
					break;
				case 4:
					if_def(h, fread, fwrite, write_file,
						line);
					break;
				default:
					if (write_file == 1)
						fprintf(fwrite, "%s", *line);
					if (write_file == 0)
						printf("%s", *line);
					break;
				}
			}
		} else {
			if (find == 1) {
				token = strtok(copy_line, " \r\n");
				value = decode(token);

				switch (value) {
				case 1:
					define(&h, line);
					break;
				case 2:
					undefine(&h, line);
					break;
				case 3:
					if_else(fread, fwrite, write_file,
						h, line);
					break;
				case 4:
					if_def(h, fread, fwrite, write_file,
						line);
					break;
				default:
					if (write_file == 1)
						fprintf(fwrite, "%s", *line);
					if (write_file == 0)
						printf("%s", *line);
					break;
				}
			}
		}
		free(copy_line);
		fgets(*line, len, fread);
		copy_line = (char *)calloc(strlen(*line) + 1, sizeof(char));
		DIE(copy_line == NULL, "12");
		strcpy(copy_line, *line);
		token = strtok(copy_line, " \r\n");
	}
	free(symbol);
	free(copy_line);
}

/*Functie care urmareste parcurgerea HashMap-ului si verificarea daca
 *simbolul dat exista; daca exista inseamna ca a fost definit. Am folosit
 *o variabila "find" care are valoarea 1 daca a fost gasit simbolul si
 *valoarea 0 daca nu. Se iese din functie la intalnirea terminatorului #endif
 */
void if_ndef(HashMap *h, FILE *fread, FILE *fwrite, int write_file,
	char *line[], list *l)
{
	int i;
	char *symbol;
	int find = 0;
	int len = 256;
	int defined = 0;
	char *copy_line;
	char *token;
	int value;

	token = strtok(*line, " \r\n");
	token = strtok(NULL, " \r\n");
	symbol = (char *)calloc(strlen(token) + 1, sizeof(char));
	DIE(symbol == NULL, "12");
	strcpy(symbol, token);
	fgets(*line, len, fread);

	copy_line = (char *)calloc(strlen(*line) + 1, sizeof(char));
	DIE(copy_line == NULL, "12");
	strcpy(copy_line, *line);

	for (i = 0; i < h->size; i++)
		if (strcmp(h->list[i]->symbol, symbol) == 0)
			find = 1;

	while (strncmp(*line, "#endif", 5) != 0) {
		if (find == 0 && token != NULL) {
			token = strtok(copy_line, " \r\n");
			value = decode(token);

			switch (value) {
			case 1:
				define(&h, line);
				break;
			case 2:
				undefine(&h, line);
				break;
			case 3:
				if_else(fread, fwrite, write_file, h, line);
				break;
			case 4:
				if_def(h, fread, fwrite, write_file, line);
				break;
			case 5:
				if_ndef(h, fread, fwrite, write_file, line, l);
				break;
			case 6:
				include(l, write_file, fwrite, h, line);
				break;
			default:
				text(&token, *line, h, write_file, fwrite);
				break;
			}
		}
		free(copy_line);
		fgets(*line, len, fread);
		copy_line = (char *)calloc(strlen(*line) + 1, sizeof(char));
		DIE(copy_line == NULL, "12");
		strcpy(copy_line, *line);
		token = strtok(copy_line, " \r\n");
	}
	free(symbol);
	free(copy_line);
}

/*Am folosit o lista dublu inlantuita. In functie de prioritatea
 *elementului adaugat in lista, se putea facea adaugarea la inceput 
 *sau la final. Elementul adaugat mereu la inceput era path-ul input-ului. 
 *Urmatoarele path-uri date in parametrii cu care se executa programul se 
 *adaugau in ordinea citirii lor.
 */
void include(list *l, int write_file, FILE *fwrite, HashMap *h,
	char *line[])
{
	//[-I <DIR>] sau [-I<DIR>]
	char *new_token;
	char *dir, *dir_new;
	FILE *f;
	char *token;
	int len = 256;
	list *aux = l;
	int open = 0;
	int value;
	char *copy_line;

	token = strtok(*line, " \r\n");
	token = strtok(NULL, " \r\n");

	dir = (char *)calloc(257, sizeof(char));
	DIE(dir == NULL, "12");
	strcpy(dir, token);

	replaceWord(dir, "\"", "");

	while (aux != NULL) {
		dir_new = (char *)calloc(257, sizeof(char));
		DIE(dir_new == NULL, "12");
		strcpy(dir_new, aux->value);
		strcat(dir_new, "/");
		strcat(dir_new, dir);

		f = fopen(dir_new, "r");
		if (f == NULL)
			free(dir_new);
		else {
			open = 1;
			free(dir_new);
			break;
		}
		aux = aux->next;
	}

	DIE(open < 0, "12");

	while (fgets(*line, len, f)) {
		copy_line = (char *)calloc(strlen(*line) + 1, sizeof(char));
		DIE(copy_line == NULL, "12");
		strcpy(copy_line, *line);

		new_token = strtok(copy_line, " \r\n");
		value = decode(new_token);

		switch (value) {
		case 1:
			define(&h, line);
			break;
		case 2:
			undefine(&h, line);
			break;
		case 3:
			if_else(f, fwrite, write_file, h, line);
			break;
		case 4:
			if_def(h, f, fwrite, write_file, line);
			break;
		case 5:
			if_ndef(h, f, fwrite, write_file, line, l);
			break;
		default:
			text(&token, *line, h, write_file, fwrite);
			break;
		}
		free(copy_line);
	}

	free(dir);
	if (open == 1)
		fclose(f);
}

void check_command(HashMap *h, int write_file, char line[256],
	FILE *fwrite, FILE *fread, list *l)
{
	char *copy_line;
	char *token;
	int value;

	copy_line = (char *)calloc(strlen(line) + 1, sizeof(char));
	DIE(copy_line == NULL, "12");
	strcpy(copy_line, line);
	token = strtok(copy_line, " ");
	value = decode(token);

	switch (value) {
	case 1:
		define(&h, &line);
		break;
	case 2:
		undefine(&h, &line);
		break;
	case 3:
		if_else(fread, fwrite, write_file, h, &line);
		break;
	case 4:
		if_def(h, fread, fwrite, write_file, &line);
		break;
	case 5:
		if_ndef(h, fread, fwrite, write_file, &line, l);
		break;
	case 6:
		include(l, write_file, fwrite, h, &line);
		break;
	default:
		text(&token, line, h, write_file, fwrite);
		break;
	}
	free(copy_line);
}

int decode_args(char *a)
{
	if (strncmp(a, "-D", 2) == 0)
		return 1;
	if (strncmp(a, "-I", 2) == 0)
		return 2;
	if (strncmp(a, "-o", 2) == 0)
		return 3;

	return 4;
}

char *delLastFile(char *name)
{
	int i = 0;
	int len = strlen(name);

	for (i = len; i > 0; i--) {
		if (name[i - 1] != '/')
			name[i - 1] = '\0';
		else {
			name[i - 1] = '\0';
			break;
		}
	}
	return name;
}

int main(int argc, char *argv[])
{
	FILE *fread;
	FILE *fwrite;
	int read_file = 0;
	int write_file = 0;
	HashMap *h;
	list *l;
	int i;
	char line[256];
	int len = 256;
	int value;
	char *path = (char *)calloc(256, sizeof(char));

	if (path == NULL)
		return 12;

	initList(&l);
	h = initHashMap(10);
	if (h == NULL)
		return 12;

	for (i = 1; i < argc; i++) {
		value = decode_args(argv[i]);

		switch (value) {
		case 1:
			define_args(argv, &i, &h);
			break;
		case 2:
			include_args(argv, &i, &l);
			break;
		case 3:
			if (write_file == 0) {
				if (strcmp(argv[i], "-o") == 0) {
					fwrite = fopen(argv[i + 1], "w");
					write_file = 1;
					i++;
				} else {
					fwrite = fopen(argv[i] + 2, "w");
					write_file = 1;
				}
			}
			break;
		case 4:
			if (read_file == 0) {
				strcpy(path, argv[i]);
				addListFirst(&l, delLastFile(path));
				fread = fopen(argv[i], "r");
				read_file = 1;
			} else {
				//[<OUTFILE>]
				if (write_file == 0) {
					fwrite = fopen(argv[i], "w");
					write_file = 1;
				} else
					return -1;
			}
			break;
		}
	}

	if (read_file == 1)
		while (fgets(line, len, fread))
			check_command(h, write_file, line, fwrite, fread, l);

	if (read_file == 0)
		while (fgets(line, len, stdin))
			check_command(h, write_file, line, fwrite, stdin, l);

	free(path);
	free_HashMap(h);
	free_list(l);
	if (write_file == 1)
		fclose(fwrite);
	if (read_file == 1)
		fclose(fread);

	return 0;
}



