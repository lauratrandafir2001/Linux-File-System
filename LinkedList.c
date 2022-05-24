#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "utils.h"

linked_list_t *
ll_create(unsigned int data_size)
{
    linked_list_t *list = malloc(sizeof(linked_list_t));
    DIE(list == NULL, "YOU HAVE NO LIST");
    list->head = NULL;
    list->data_size = data_size;
    list->size = 0;
    return list;
}

/*
 * Pe baza datelor trimise prin pointerul new_data, se creeaza un nou nod care e
 * adaugat pe pozitia n a listei reprezentata de pointerul list. Pozitiile din
 * lista sunt indexate incepand cu 0 (i.e. primul nod din lista se afla pe
 * pozitia n=0). Daca n >= nr_noduri, noul nod se adauga la finalul listei. Daca
 * n < 0, eroare.
 */
void ll_add_nth_node(linked_list_t *list, unsigned int n, char *new_data)
{
    ll_node_t *new;
    if (list == NULL)
    {
        return;
    }
    if (n < 0)
    {
        printf("eroare");
        return;
    }
    new = malloc(sizeof(ll_node_t));
    DIE(new == NULL, "malloc failed");
    new->data = malloc(list->data_size);
    DIE(new->data == NULL, "malloc failed");
    strcpy(new->data, new_data);
    if (n == 0 || list->size == 0)
    {
        new->next = list->head;
        list->head = new;
        list->size++;
        return;
    }
    if (n >= list->size)
    {
        ll_node_t *curr;
        new->next = NULL;
        curr = list->head;
        while (curr && curr->next)
        {
            curr = curr->next;
        }
        curr->next = new;
        list->size++;
        return;
    }
    if (n > 0 && n < list->size)
    {
        unsigned int i = 0;
        ll_node_t *curr;
        curr = list->head;
        while (i < n - 1)
        {
            curr = curr->next;
            i++;
        }
        new->next = curr->next;
        curr->next = new;
        list->size++;
        return;
    }
}

/*
 * Elimina nodul de pe pozitia n din lista al carei pointer este trimis ca
 * parametru. Pozitiile din lista se indexeaza de la 0 (i.e. primul nod din
 * lista se afla pe pozitia n=0). Daca n >= nr_noduri - 1, se elimina nodul de
 * la finalul listei. Daca n < 0, eroare. Functia intoarce un pointer spre acest
 * nod proaspat eliminat din lista. Este responsabilitatea apelantului sa
 * elibereze memoria acestui nod.
 */
ll_node_t *
ll_remove_nth_node(linked_list_t *list, unsigned int n)
{
    if (list == NULL)
    {
        exit(0);
    }
    if (n < 0)
    {
        printf("error404 not found");

        exit(0);
    }
    if (n >= list->size - 1)
    {
        n = list->size - 1;
    }
    ll_node_t *removed;
    if (n == 0)
    {
        removed = list->head;
        list->head = (list->head)->next;
        list->size--;
        return removed;
    }
    if (n > 0 && n <= list->size - 1)
    {
        unsigned int i = 0;
        ll_node_t *curr;
        curr = list->head;
        for (i = 0; i < n - 1; i++)
        {
            curr = curr->next;
        }
        removed = curr->next;
        curr->next = removed->next;
        list->size--;
    }
    return removed;
}

/*
 * Functia intoarce numarul de noduri din lista al carei pointer este trimis ca
 * parametru.
 */
unsigned int
ll_get_size(linked_list_t *list)
{
    DIE(list == NULL, "YOU HAVE NO LIST");
    return list->size;
}

/*
 * Procedura elibereaza memoria folosita de toate nodurile din lista, iar la
 * sfarsit, elibereaza memoria folosita de structura lista si actualizeaza la
 * NULL valoarea pointerului la care pointeaza argumentul (argumentul este un
 * pointer la un pointer).
 */
void ll_free(linked_list_t **pp_list)
{
    ll_node_t *curr;
    if (pp_list == NULL || *pp_list == NULL)
    {
        return;
    }
    curr = (*pp_list)->head;
    while ((*pp_list)->head)
    {
        (*pp_list)->head = ((*pp_list)->head)->next;
        free(curr->data);
        free(curr);
        curr = (*pp_list)->head;
    }
    free(*pp_list);
    *pp_list = NULL;
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza int-uri. Functia afiseaza toate valorile int stocate in nodurile
 * din lista inlantuita separate printr-un spatiu.
 */
void ll_print_int(linked_list_t *list)
{
    ll_node_t *curr;
    if (list == NULL)
    {
        return;
    }
    curr = list->head;
    while (curr)
    {
        printf("%d ", *(int *)curr->data);
        curr = curr->next;
    }
    printf("\n");
}

/*
 * Atentie! Aceasta functie poate fi apelata doar pe liste ale caror noduri STIM
 * ca stocheaza string-uri. Functia afiseaza toate string-urile stocate in
 * nodurile din lista inlantuita, separate printr-un spatiu.
 */
void ll_print_string(linked_list_t *list)
{
    ll_node_t *curr;
    if (list == NULL)
    {
        return;
    }
    curr = list->head;
    while (curr)
    {
        printf("%s ", (char *)curr->data);
        curr = curr->next;
    }
}
