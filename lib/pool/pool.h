/*
 * pool.h - Pool Allocator
 *
 * Author: frostzt
 * Date: 2026-01-09
 */

#include <stddef.h>
#include <stdlib.h>

#define CHUNK_SZ 64

typedef union Chunk Chunk;
union Chunk {
  Chunk *next;
  char arr[CHUNK_SZ];
};

typedef struct ArrayStart ArrayStart;
struct ArrayStart {
  Chunk *arr;
  ArrayStart *next;
};

typedef struct Pool Pool;
struct Pool {
  Chunk *free_chunk;
  ArrayStart *array_starts;
};

/* creates a new pool */
Pool *pool_new(size_t pool_sz);

/* allocates a chunk from the pool and returns it back */
void *pool_alloc(Pool *pool);

/* frees the pointer and returns it back to the pool */
void pool_free(Pool *pool, void *ptr);

/* frees everything and destroys the pool */
void pool_destroy(Pool *pool);

/* expands the pool */
bool pool_expand(Pool *pool, size_t extra_sz);
