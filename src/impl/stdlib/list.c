#include "stdlib/list.h"
#include "stdlib/kmalloc.h"
#include "stdlib/string.h"

void list_create( void* list )
{
    list_it_t* lsptr = list;
    lsptr->next = NULL;
    lsptr->prev = NULL;
}
void __list_push( list_t* ls, void* data, size_t size )
{
    list_it_t* lsptr = (list_it_t*)ls;
    if (lsptr->next)
    {
        list_it_t* newptr = kcalloc(size + sizeof(list_it_t));
        newptr->next = NULL;
        newptr->prev = lsptr->prev;
        lsptr->prev->next = newptr;
        lsptr->prev = newptr;
        memcpy(newptr+sizeof(list_it_t), data, size);
    }
    else
    {
        lsptr->next = (lsptr->prev = kcalloc(size+sizeof(list_it_t)));
        memcpy(lsptr->prev+sizeof(list_it_t), data, size);
    }
}
err_t __list_pop( list_t* ls, void* dest, size_t size )
{
    list_it_t* lsptr = (list_it_t*)ls;
    if (lsptr->prev == 0)
    {
        return OS32_ERROR;
    }
    memcpy( dest, lsptr->prev+sizeof(list_it_t), size );
    list_it_t* oldptr = lsptr->prev;
    lsptr->prev = oldptr->prev;
    if (lsptr->next == oldptr)
    {   
        lsptr->next = NULL;
    }
    kfree(oldptr);
    return OS32_SUCCESS;
}
void __list_deref( list_it_t* it, void* dest, size_t size )
{
    memcpy(dest, it+sizeof(list_it_t ), size);
}
void __list_insert( list_it_t* it, void* data, size_t size )
{

}
void __list_free( list_t* list )
{
    while ( __list_pop( list, NULL, 0 ) == OS32_SUCCESS );
}