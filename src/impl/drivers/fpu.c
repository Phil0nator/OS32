#include "drivers/fpu.h"
#include "stdlib/cpuid.h"

static bool fpu_is_installed = false;

err_t __install_fpu()
{
    cpuid_features_t feat;
    cpuid_get_features( &feat );

    if(feat.fpu)
    {
        
    }
}

bool fpu_installed()
{

}
err_t fpu_reset()
{
    __asm__ __volatile__ ("FNINIT");
}
