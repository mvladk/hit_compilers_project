#define HASH_BINS 223
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "symbol.h"

typedef struct table {
    struct entry *bins[HASH_BINS];
    struct table *next;
} table;

typedef struct entry {
    char *name;
    elm_type type;
    elm_role role;
    struct entry *next;
} entry;

unsigned int hash(char *str)
{
    unsigned int result = 0;
    unsigned int i;
    unsigned int len = strlen(str);
    for (i = 0; i < len; i++)
        result = 37 * result + (unsigned char)str[i];
    return result % HASH_BINS;
}

table_ptr make_table(table_ptr current_table)
{
    table *t = calloc(1, sizeof(table));
    t->next = current_table;
    return t;
}

table_ptr pop_table(table_ptr current_table) {
    table *t = current_table->next;
    int i;
    for (i = 0; i < HASH_BINS; i++)
    {
        entry *e = current_table->bins[i];
        entry *e2;
        while (e != NULL) {
            e2 = e->next;
            free(e->name);
            free(e);
            e = e2;
        }
    }
    free(current_table);
    return t;
}

table_entry insert(table_ptr current_table, char *id_name)
{
	int bin_number;
	entry *e;
    if (lookup(current_table, id_name)) return NULL;
    bin_number = hash(id_name);
    e = calloc(1, sizeof(entry));
    e->name = strdup(id_name);
    e->next = current_table->bins[bin_number];
    current_table->bins[bin_number] = e;
    return e;
}

table_entry lookup(table_ptr current_table, char *id_name)
{
    int bin_number = hash(id_name);
    entry *e = current_table->bins[bin_number];
    while (e != NULL)
    {
        if (strcmp(e->name, id_name) == 0)
            return e;
        e = e->next;
    }
    return NULL;
}

table_entry find(table_ptr current_table, char *id_name)
{
    while (current_table != NULL)
    {
        table_entry e = lookup(current_table, id_name);
        if (e != NULL) return e;
        current_table = current_table->next;
    }
    return NULL;
}

void set_id_type(table_entry id_entry, elm_type id_type)
{
    if (id_type < 800 || id_type >= 900) {
        printf("WARNING: set_id_type with invalid type\n");
    }
    id_entry->type = id_type;
}

elm_type get_id_type(table_entry id_entry)
{
    return id_entry->type;
}

void set_id_role(table_entry id_entry, elm_role id_role)
{
    if (id_role < 900 || id_role >= 1000) {
        printf("WARNING: set_id_role with invalid role\n");
    }
    id_entry->role = id_role;
}

elm_role get_id_role(table_entry id_entry)
{
    return id_entry->role;
}

void print_all(table_ptr current_table)
{
    int i;
    entry *e;
    if (current_table == NULL) {
        printf("-------------------------\nNULL\n------------------------\n");
        return;
    }
    print_all(current_table->next);
    printf("--------------------------\n");
    for (i = 0; i < HASH_BINS; i++)
    {
        for (e = current_table->bins[i]; e != NULL; e = e->next)
        {
            char *type = e->type == INT_TYPE ? "INT" : e->type == REAL_TYPE ? "REAL" : "OTHER";
            char *role = e->role == VARIABLE_ROLE  ? "VARIABLE"  :
                         e->role == CONSTANT_ROLE  ? "COSNTANT"  :
                         e->role == EXCEPTION_ROLE ? "EXCEPTION" :
                                                     "OTHER";
            printf("Name: %s\nType: %s\nRole: %s\n", e->name, type, role);
        }
    }
}

/*int main()
{
    table_ptr head = NULL;
    table_entry e;
    printf("DEBUG\n"); print_all(head);
    head = make_table(head);
    e = insert(head, "a");
    set_id_type(e, INT_TYPE);
    set_id_role(e, VARIABLE_ROLE);
    e = insert(head, "b");
    set_id_type(e, REAL_TYPE);
    set_id_role(e, CONSTANT_ROLE);
    if (insert(head, "a") != NULL) {
        printf("should not allow inserting a twice\n");
        abort();
    }
    printf("DEBUG\n"); print_all(head);
    head = make_table(head);
    e = insert(head, "a");
    set_id_type(e, REAL_TYPE);
    set_id_role(e, VARIABLE_ROLE);
    e = insert(head, "c");
    set_id_type(e, REAL_TYPE);
    set_id_role(e, CONSTANT_ROLE);
    if (insert(head, "c") != NULL) {
        printf("should not allow inserting c twice\n");
    }
    printf("DEBUG\n"); print_all(head);
    head = make_table(head);
    e = insert(head, "c");
    set_id_type(e, ERROR_TYPE);
    set_id_role(e, EXCEPTION_ROLE);
    if (get_id_type(find(head, "b")) != REAL_TYPE ||
        get_id_role(find(head, "b")) != CONSTANT_ROLE ||
        get_id_type(find(head, "a")) != REAL_TYPE ||
        get_id_role(find(head, "a")) != VARIABLE_ROLE ||
        get_id_type(find(head, "c")) != ERROR_TYPE ||
        get_id_role(find(head, "c")) != EXCEPTION_ROLE)
    {
        printf("consistency lookup failure\n");
    }
    print_all(head);
    head = pop_table(head);
    printf("DEBUG\n"); print_all(head);
    head = pop_table(head);
    printf("DEBUG\n"); print_all(head);
    head = pop_table(head);
    printf("DEBUG\n"); print_all(head);
}*/
