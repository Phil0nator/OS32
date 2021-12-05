#ifndef OS32_LIST
#define OS32_LIST
#include <stddef.h>
#include <stdint.h>
#include "stdlib/error.h"
typedef struct list list_t;
typedef struct list_it 
{ 
    struct list_it* next; 
    struct list_it* prev; 
} __attribute__((packed)) list_it_t;
#define decl_list( type, name ) struct __listof_##type { void* next; void* prev; type member; } __attribute__((packed)) name;
#define list_push( lsptr, itemptr ) __list_push(  (list_t*)lsptr, itemptr, sizeof((lsptr)->member) )
#define list_pop( lsptr, itemptr ) __list_pop(  (list_t*)lsptr, itemptr, sizeof((lsptr)->member) )
#define list_begin( lsptr ) (list_it_t*)(lsptr)
#define list_deref( lsptr, itptr, dest ) __list_deref( itptr, dest, sizeof((lsptr)->member) )
#define list_reftp( itptr ) (void*)((uint32_t)(itptr)+sizeof(list_it_t))
#define list_not_end( it ) (it->next)
#define list_end(lsptr) (list_it_t*)((lsptr)->prev) 
#define list_advance( it, amt ) for ( size_t __i = 0; __i < amt; __i++) it = it->next;
#define list_insert( lsptr, it, itemptr ) __list_insert( it, itemptr, sizeof((lsptr)->member) )
#define list_free( lsptr ) __list_free( (list_t*)lsptr )

void list_create( void* list );
void __list_push( list_t* ls, void* data, size_t size );
err_t __list_pop( list_t* ls, void* dest, size_t size );
void __list_deref( list_it_t* it, void* dest, size_t size );
void __list_insert( list_it_t* it, void* data, size_t size );
void __list_free( list_t* list );

#endif