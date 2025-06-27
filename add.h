#ifndef ADD_H
#define ADD_H
#include <stddef.h>

void store_blob(const char *hash, const char *data, size_t len);
void addfile(const char *filename);

#endif