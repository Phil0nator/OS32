#ifndef OS32_CPUID
#define OS32_CPUID
#include <stdbool.h>
#include <stdint.h>

//https://en.wikipedia.org/wiki/CPUID

#pragma pack(1)
typedef struct cpuid_features
{
    unsigned fpu:    1;
    unsigned vme:    1;
    unsigned de:     1;
    unsigned pse:    1;
    unsigned tsc:    1;
    unsigned msr:    1;
    unsigned pae:    1;
    unsigned mce:    1;
    unsigned cx8:    1;
    unsigned apic:   1;

    unsigned __reserved: 1;
    
    unsigned sep:    1;
    unsigned mtrr:   1;
    unsigned pge:    1;
    unsigned mca:    1;
    unsigned cmov:   1;
    unsigned pat:    1;
    unsigned pse36:  1;
    unsigned psn:    1;
    unsigned clfsh:  1;

    unsigned _reserved: 1;

    unsigned ds:     1;
    unsigned acpi:   1;
    unsigned mmx:    1;
    unsigned fxsr:   1;
    unsigned sse:    1;
    unsigned sse2:   1;
    unsigned ss:     1;
    unsigned htt:    1;
    unsigned tm:     1;
    unsigned ia64:   1;
    unsigned pbe:    1;

    unsigned sse3:   1;
    unsigned pclmulqdq :1;
    unsigned dtes64: 1;
    unsigned monitor: 1;
    unsigned ds_cpl: 1;
    unsigned vmx:    1;
    unsigned smx:    1;
    unsigned est:    1;
    unsigned tm2:    1;
    unsigned ssse3:  1;
    unsigned cnxt_id:1;
    unsigned sdbg:   1;
    unsigned fma:    1;
    unsigned cx16:   1;
    unsigned xptr:   1;
    unsigned pdcm:   1;

    unsigned reserved: 1;

    unsigned pcid:   1;
    unsigned dca:    1;
    unsigned sse4_1: 1;
    unsigned sse4_2: 1;
    unsigned x2apic: 1;
    unsigned movbe:  1;
    unsigned popcnt: 1;
    unsigned tscd:   1;
    unsigned aes:    1;
    unsigned xsave:  1;
    unsigned osxsave:1;
    unsigned avx:    1;
    unsigned f16c:   1;
    unsigned rdrnd:  1;
    unsigned hyperv: 1; 
} cpuid_features_t;
#pragma pack(0)

/**
 * Call CPUID, and load the values into the given struct
 */
void cpuid_get_features( struct cpuid_features* dest );

#endif