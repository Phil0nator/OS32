#ifndef OS32_LIST
#define OS32_LIST
#include <stddef.h>
#include <stdint.h>
#include "stdlib/error.h"
// internal use
typedef struct list list_t;

// List iterator
typedef struct list_it 
{ 
    struct list_it* next; 
    struct list_it* prev; 
} __attribute__((packed)) list_it_t;

/**
 * @brief statically declare a macro'd linked list of type, with name
 */
#define decl_list( type, name ) struct __listof_##type { void* next; void* prev; type member; } __attribute__((packed)) name;
/**
 * @brief push to the end of a macro'd list
 * @param lsptr pointer to the list
 * @param itemptr pointer to item to add
 */
#define list_push( lsptr, itemptr ) __list_push(  (list_t*)lsptr, itemptr, sizeof((lsptr)->member) )

/**
 * @brief pop from end of a macro'd list
 * @param lsptr pointer to list
 * @param itemptr pointer to destination
 */
#define list_pop( lsptr, itemptr ) __list_pop(  (list_t*)lsptr, itemptr, sizeof((lsptr)->member) )

/**
 * @brief get first iterator of list
 * @param lsptr list pointer
 */
#define list_begin( lsptr ) (list_it_t*)(lsptr)

/**
 * @brief dereference an iterator
 * @param lsptr list pointer
 * @param itptr iterator pointer
 * @param dest destination for copy
 */
#define list_deref( lsptr, itptr, dest ) __list_deref( itptr, dest, sizeof((lsptr)->member) )

/**
 * @brief convert a list iterator into a pointer to the data
 * @param itptr iterator 
 */
#define list_reftp( itptr ) (void*)((uint32_t)(itptr)+sizeof(list_it_t))

/**
 * @brief check if an iterator is at the end of a list or not
 * @param it iterator
 */
#define list_not_end( it ) (it->next)

/**
 * @brief get the end iterator of a list
 * @param lsptr list pointer
 */
#define list_end(lsptr) (list_it_t*)((lsptr)->prev) 

/**
 * @brief advance an iterator by a certain amount
 * @param it iterator
 * @param amt amount to advance by
 */
#define list_advance( it, amt ) for ( size_t __i = 0; __i < amt; __i++) it = it->next;

/**
 * @brief insert into a list by an iterator
 * @param lsptr list pointer
 * @param it iterator
 * @param itemptr pointer to item to insert
 */
#define list_insert( lsptr, it, itemptr ) __list_insert( it, itemptr, sizeof((lsptr)->member) )

/**
 * @brief free a list
 * (pointers contained as members in the list will not be freed)
 * @param lsptr list pointer
 */
#define list_free( lsptr ) __list_free( (list_t*)lsptr )


//// Internal use

void list_create( void* list );
void __list_push( list_t* ls, void* data, size_t size );
err_t __list_pop( list_t* ls, void* dest, size_t size );
void __list_deref( list_it_t* it, void* dest, size_t size );
void __list_insert( list_it_t* it, void* data, size_t size );
void __list_free( list_t* list );

#endif