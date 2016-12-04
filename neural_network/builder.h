#ifndef NEURAL_BUILDER_H
#define NEURAL_BUILDER_H

#include <stdbool.h>

bool load_network(const t_network*, bool allocate_io);
void free_network(const t_network*);
t_network *dup_network(const t_network *net, const char *name);

#endif
