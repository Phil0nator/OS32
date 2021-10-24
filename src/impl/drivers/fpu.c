#include "drivers/fpu.h"
#include "stdlib/cpuid.h"
#include "drivers/cr0.h"

err_t __install_fpu()
{
    cpuid_features_t feat;
    cpuid_get_features( &feat );

    if(feat.fpu)
    {
        // Disable emulation bit of cr0
        uint32_t cr0 = cr0_get();
        CR0_SET_BIT( cr0, CR0_EM, 0 ); 
        cr0_set( cr0 );
    }
    else {
        return OS32_ERROR;
    }
    fpu_reset();
    return OS32_SUCCESS;
}

bool fpu_installed()
{
    // determine if the emulation bit is set in cr0
    return !CR0_GET_BIT( cr0_get(), CR0_EM );
}
err_t fpu_reset()
{
    __asm__ __volatile__ ("FNINIT");
    return OS32_SUCCESS;
}
