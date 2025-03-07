#ifndef LOADER_H
#define LOADER_H

#include "memory.h"
#include <stddef.h>


int load_program(Memory *mem, const char *filename);

#endif