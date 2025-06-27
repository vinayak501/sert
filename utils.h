#ifndef UTILS_H
#define UTILS_H

char *read_file(const char *path, size_t *out_len);
void compute_hash(const char *data, size_t len, char *out_hash_str);
void cat_object(const char *hash);

#endif