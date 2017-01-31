struct fl_struct_linked_list_head
{
    struct fl_struct_linked_list_head * next;
    struct fl_struct_linked_list_head * prev;
};

typedef struct fl_struct_linked_list_head fl_linked_list_t;

/**
 * Initialize a fl_linked_list_t within structure
 * @param
 */
#define fl_linked_list_init(structure, field) \
({ \
        fl_auto_type(structure) ___str = structure; \
        if (___str) {\
        	___str->field.next = (fl_linked_list_t *)0;\
        	___str->field.prev = (fl_linked_list_t *)0;\
        } \
})

/**
 * Get the number of nodes of the list.
 *
 * @param ptr_to_current
 * @return Pointer to the last node of the list, including ptr_to_current.
 */
#define fl_linked_list_count(ptr_to_current, field) \
({ \
        int ___ret = 0; \
        fl_auto_type(ptr_to_current) ___cur = fl_linked_list_first(ptr_to_current, field); fl_linked_list_t * ___head;\
        if (___cur) ___ret++; \
        if (___ret) \
        { \
            ___head = &___cur->field; \
            if (___head->next) { while (___head->next){ ___head = ___head->next; ___ret++;}} \
        } \
        ___ret; \
})


/**
 * Get the next node.
 * @param ptr_to_current
 * @return Pointer to the next node.
 */
#define fl_linked_list_next(ptr_to_current, field) \
({ \
        fl_auto_type(ptr_to_current) ___ret = ptr_to_current; fl_linked_list_t * ___head;\
        if (___ret) \
        { \
           ___head = ___ret->field.next; \
           if (___head) { ___ret  = fl_container_of(___head, fl_typeof(*(ptr_to_current)), field); }\
           else {___ret = 0; } \
        } \
        ___ret; \
})

/**
 * Get the previous node.
 * @param ptr_to_current
 * @return Pointer to the previous node.
 */
#define fl_linked_list_previous(ptr_to_current, field) \
({ \
        fl_auto_type(ptr_to_current) ___ret = ptr_to_current; fl_linked_list_t * ___head;\
        if (___ret) \
        { \
           ___head = ___ret->field.prev; \
           if (___head) { ___ret  = fl_container_of(___head, fl_typeof(*(ptr_to_current)), field); }\
           else {___ret = 0; } \
        } \
        ___ret; \
})

/**
 * Get the last node of the list, including ptr_to_current.
 *
 * @param ptr_to_current
 * @return Pointer to the last node of the list, including ptr_to_current.
 */
#define fl_linked_list_last(ptr_to_current, field) \
({ \
        fl_auto_type(ptr_to_current) ___ret = ptr_to_current; fl_linked_list_t * ___head;\
        if (___ret) \
        { \
            ___head = &___ret->field; \
            if (___head->next) { while (___head->next) ___head = ___head->next;} \
            if (___head) { ___ret  = fl_container_of(___head, fl_typeof(*(ptr_to_current)), field); } \
            else {___ret = 0; } \
        } \
        ___ret; \
})

/**
 * Get the first node of the list, including ptr_to_current.
 *
 * @param ptr_to_current
 * @return Pointer to the first node of the list, including ptr_to_current.
 */
#define fl_linked_list_first(ptr_to_current, field) \
({ \
        fl_auto_type(ptr_to_current) ___ret2 = ptr_to_current; fl_linked_list_t * ___head;\
        if (___ret2) \
        { \
            ___head = &___ret2->field; \
            if (___head->prev) { while (___head->prev) ___head = ___head->prev;} \
            if (___head) { ___ret2  = fl_container_of(___head, fl_typeof(*(ptr_to_current)), field); } \
            else {___ret2 = 0; } \
        } \
        ___ret2; \
})

/**
 * Insert new node between current and current's next.
 *
 * @param ptr_to_current
 * @param ptr_to_new
 */
#define fl_linked_list_insert_after(ptr_to_current, ptr_to_new, field) \
({ \
        fl_auto_type(ptr_to_current) ___curr = ptr_to_current; \
        fl_auto_type(ptr_to_new) ___new = ptr_to_new; \
        if (___curr && ___new) \
        { \
            ___new->field.next = ___curr->field.next; \
            ___new->field.prev = &___curr->field; \
            if (___curr->field.next) ___curr->field.next->prev = &___new->field; \
            ___curr->field.next = &___new->field; \
        } \
})

/**
 * Insert new node between current and current's previous.
 *
 * @param ptr_to_current
 * @param ptr_to_new
 */
#define fl_linked_list_insert_before(ptr_to_current, ptr_to_new, field) \
({ \
        fl_auto_type(ptr_to_current) ___curr = ptr_to_current; \
        fl_auto_type(ptr_to_new) ___new = ptr_to_new; \
        if (___curr && ___new) \
        { \
            ___new->field.prev = ___curr->field.prev; \
            ___new->field.next = &___curr->field; \
            if(___curr->field.prev) ___curr->field.prev->next = &___new->field; \
            ___curr->field.prev = &___new->field; \
        } \
})

/**
 * Remove the current node and return it.
 *
 * @param current
 * @return Return the new or old beginning of the list.
 */
#define fl_linked_list_remove(ptr_to_current, field) \
({ \
        fl_auto_type(ptr_to_current) ___cur = ptr_to_current; \
        fl_auto_type(ptr_to_current) ___ret = (fl_typeof(ptr_to_current))0; \
        fl_linked_list_t * ___head_next = (fl_linked_list_t*)0; \
        fl_linked_list_t * ___head_prev = (fl_linked_list_t*)0; \
        if (___cur) \
        { \
           ___head_next = ___cur->field.next; \
           ___head_prev = ___cur->field.prev; \
           if (___head_prev) ___head_prev->next = ___head_next; \
           if (___head_next) ___head_next->prev = ___head_prev; \
           fl_linked_list_init(___cur, field); \
           if (___head_prev) ___ret = fl_linked_list_first(fl_container_of(___head_prev, fl_typeof(*(ptr_to_current)), field), field); \
           else if(___head_next) ___ret = fl_linked_list_first(fl_container_of(___head_next, fl_typeof(*(ptr_to_current)), field), field); \
        } \
        ___ret; \
})

/**
 * Free all list members,
 * but instead calling free itself, this function calls
 * user free function passed on free_callback.
 *
 * @param ptr_to_current
 *
 * @warning
 *      Any pointer to the a list node will be invalid.
 */
#define fl_linked_list_free(ptr_to_current, field, free_callback) \
({ \
        fl_auto_type(ptr_to_current) ___item = fl_linked_list_first(ptr_to_current, field); \
        fl_typeof(___item) ___item_to_delete; \
        while (___item) \
        { \
            ___item_to_delete = ___item; \
            ___item = fl_linked_list_next(___item_to_delete, field); \
            free_callback(___item_to_delete); \
        } \
})

/**
 */
#define fl_linked_list_find(ptr_to_current, field, comparator_func, comparator_seed) \
({ \
        fl_auto_type(ptr_to_current) ___item = ptr_to_current; \
        while (___item) \
        { \
        	if (comparator_func(comparator_seed, ___item) == true) break; \
            ___item = fl_linked_list_next(___item, field); \
        } \
        ___item; \
})
