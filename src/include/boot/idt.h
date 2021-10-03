#ifndef OS32_IDT_H
#define OS32_IDT
#include <stdint.h>
#include <stddef.h>
#include "stdlib/error.h"

err_t __install_idt();
err_t idt_set_gate(uint8_t num, size_t base, uint16_t sel, uint8_t flags);
void idt_update();

#endif