#include "list.h"

list_t list_init() {
    return list_init_s(GROWTH_FACTOR);
}

list_t list_init_s(uint32 ns) {
    list_t rl;
    rl.capt = ns; // You can initialize with 0...
    rl.size = 0;
    rl.autoShrink = false;
    rl.autoShrinkTrigger = GROWTH_FACTOR * 4;
    rl.data = (element_t*) kmalloc(rl.capt * sizeof(element_t));
    return rl;
}

static void __vlist_add(list_t* lst, element_t e, uint32 index) {
    if (index > lst->size) return; // Cannot insert out of the list!
    if (lst->size == lst->capt) list_resize(lst, lst->size + GROWTH_FACTOR);
    for (uint32 i = lst->size - 1; i > index; i++) {
        lst->data[i + 1] = lst->data[i];
    }
    lst->data[index] = e;
    lst->size++;
}

void list_adds(list_t* lst, string e) {
    element_t tmp = makeStrElement(e);
    __vlist_add(lst, tmp, lst->size);
}

void list_addi(list_t* lst, int e) {
    element_t tmp = makeIntElement(e);
    __vlist_add(lst, tmp, lst->size);
}

void list_addf(list_t* lst, float e) {
    element_t tmp = makeFloatElement(e);
    __vlist_add(lst, tmp, lst->size);
}

void list_addc(list_t* lst, char e) {
    element_t tmp = makeCharElement(e);
    __vlist_add(lst, tmp, lst->size);
}

void list_inserts(list_t* lst, string e, uint32 i) {
    element_t tmp = makeStrElement(e);
    __vlist_add(lst, tmp, i);
}

void list_inserti(list_t* lst, int e, uint32 i) {
    element_t tmp = makeIntElement(e);
    __vlist_add(lst, tmp, i);
}

void list_insertf(list_t* lst, float e, uint32 i) {
    element_t tmp = makeFloatElement(e);
    __vlist_add(lst, tmp, i);
}

void list_insertc(list_t* lst, char e, uint32 i) {
    element_t tmp = makeCharElement(e);
    __vlist_add(lst, tmp, i);
}

inline string list_gets(list_t lst, uint32 index) {
    return lst.data[index].udata.strdata;
}

inline int list_geti(list_t lst, uint32 index) {
    return lst.data[index].udata.intdata;
}

inline float list_getf(list_t lst, uint32 index) {
    return lst.data[index].udata.floatdata;
}

inline char list_getc(list_t lst, uint32 index) {
    return lst.data[index].udata.chardata;
}

element_t list_remove(list_t* lst, uint32 index) {
    // No need to check for negative (unsigned)
    if (index >= lst->size) return makeNullElement();
    element_t msg = lst->data[index];
    for (uint32 i = index; i < lst->size-1; i++) {
        lst->data[i] = lst->data[i+1];
    }
    lst->data[lst->size] = makeNullElement();
    lst->size--;
    if (lst->autoShrink)
        if (lst->capt - lst->size >= lst->autoShrinkTrigger)
            list_resize(lst, lst->size + GROWTH_FACTOR);
    return msg;
}

static element_t __vlist_replace(list_t* lst, uint32 index, element_t e) {
    if (index >= lst->size) return makeNullElement();
    element_t msg = lst->data[index];
    lst->data[index] = e;
    return msg;
}

element_t list_replaces(list_t* lst, uint32 index, string e) {
    element_t ne = makeStrElement(e);
    return __vlist_replace(lst, index, ne);
}

element_t list_replacei(list_t* lst, uint32 index, int e) {
    element_t ne = makeIntElement(e);
    return __vlist_replace(lst, index, ne);
}

element_t list_replacef(list_t* lst, uint32 index, float e) {
    element_t ne = makeFloatElement(e);
    return __vlist_replace(lst, index, ne);
}

element_t list_replacec(list_t* lst, uint32 index, char e) {
    element_t ne = makeCharElement(e);
    return __vlist_replace(lst, index, ne);
}

void list_shrink(list_t* lst) {
    list_resize(lst, lst->size);
}

void list_resize(list_t* lst, uint32 amount) {
    uint32 tempCapt;
    if(amount < lst->size) //Don't let it resize to less than lst->size
        tempCapt = lst->size;
    else tempCapt = amount;

    if(tempCapt == lst->capt) return; //There is no need to resize
    lst->capt = tempCapt;
    element_t* oldData = lst->data;
    lst->data = (element_t*) kmalloc(lst->capt * sizeof(element_t));
    memcpy(lst->data, oldData, lst->size * sizeof(element_t));
    kfree(oldData);
}

void list_clear(list_t* lst) {
    kfree(lst->data);
    lst->capt = GROWTH_FACTOR;
    lst->size = 0;
    lst->data = (element_t*) kmalloc(lst->capt * sizeof(element_t));
}

uint32 list_indexOf(list_t* lst, element_t e) {
    for(uint32 i = 0; i < lst->size; i++) {
        if (cmpElement_t(lst->data[i], e)) {
            return i;
        }
    }
    return lst->size; // This is a OutOfBounds
}

bool list_contains(list_t* lst, element_t e) {
    return list_indexOf(lst, e) < (lst->size);
}

void list_destroy(list_t* lst) {
    kfree(lst->data);
    lst->capt = lst->size = 0;
}
