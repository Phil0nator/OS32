#ifndef OS32_STACK
#define OS32_STACK
#include <stddef.h>
#include <stdbool.h>

#define statset_decl(type, name, size) struct __statset_##name { bool present; type member; } name[size] = {{0}};
#define statset_add( name, item ) for ( struct __statset_##name* i = &name[0]; i < (((char*)&name[0])+sizeof(name)); i++ ) if ( !i->present ) { i->member = item; i->present = true; break; }
#define statset_pull( name, dest ) for ( struct __statset_##name* i = &name[0]; i < (((char*)&name[0])+sizeof(name)); i++ ) if ( i->present ) { dest = i->member; i->present = false; break; }

#endif