typedef struct table *table_ptr;
typedef struct entry *table_entry;

typedef enum {
    INT_TYPE = 800, REAL_TYPE = 801, ERROR_TYPE = 802, EPSILON_TYPE = 803
} elm_type;

typedef enum {
    VARIABLE_ROLE = 900, CONSTANT_ROLE = 901, EXCEPTION_ROLE = 902
} elm_role;

table_ptr make_table(table_ptr current_table);
table_ptr pop_table(table_ptr current_table);
table_entry insert(table_ptr current_table, char *id_name);
table_entry lookup(table_ptr current_table, char *id_name);
table_entry find(table_ptr current_table, char *id_name);
void set_id_type(table_entry id_entry, elm_type id_type);
elm_type get_id_type(table_entry id_entry);
void set_id_role(table_entry id_entry, elm_role id_role);
elm_role get_id_role(table_entry id_entry);
