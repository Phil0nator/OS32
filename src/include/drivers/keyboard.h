#ifndef OS32_KEYBOARDDRIVER
#define OS32_KEYBOARDDRIVER

#include <stdint.h>
#include <stddef.h>
#include "stdlib/error.h"

// Keyboard driver definitions
#define KB_PRESS 0
#define KB_RELEASE 1

#define KB_EN_US 0



typedef int os32_kb_action_t;
typedef int os32_kb_layout_t;

// Keyboard routine to be called on interrupt
typedef void(*os32_kb_routine)( uint32_t scancode, char ASCII, os32_kb_action_t action );

/**
 * Perform keyboard installation
 */
err_t __install_keyboard();

// Set the routine to be called on interrupt
void os32_keyboard_set_routine( os32_kb_routine routine );
// Set the active keyboard layout
void os32_keyboard_set_layout( os32_kb_layout_t layout );

#endif