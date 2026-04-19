#ifndef FILEDEDUP_H
#define FILEDEDUP_H

#define FILEDEDUP_OK (1)
#define FILEDEDUP_ERROR (0)

typedef struct Filededup_t *FILEDEDUP;

FILEDEDUP FDInit(void);

/* Frees the deduplicator and all stored path copies */
void FDFree(FILEDEDUP fd);

/* Copies filepath internally. Returns FILEDEDUP_OK or FILEDEDUP_ERROR */
int FDCheck(FILEDEDUP fd, const char *filepath);

/* Returns NULL-separated groups of paths. Caller must free() the array */
char **FDDump(FILEDEDUP fd, int *length);

#endif // FILEDEDUP_H
