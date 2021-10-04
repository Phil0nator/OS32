#ifndef OS32_PAGE_H
#define OS32_PAGE_H
#include <stdint.h>
#include <stddef.h>

#pragma pack(1)
typedef struct pagent
{
    uint32_t present: 1;
    uint32_t rw: 1;
    uint32_t user: 1;
    uint32_t accessed: 1;
    uint32_t dirty: 1;
    uint32_t reserved_bits: 7;
    uint32_t frame: 20;
} pagent_t;

typedef struct page_table
{
    pagent_t pages[1024];
} page_table_t;

typedef struct page_dir
{
    
    page_table* tables[1024];
} page_dir_t;

#pragma pack(0)



#endif