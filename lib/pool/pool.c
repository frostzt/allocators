/*
 * pool.c - Pool Allocator
 *
 * Author: frostzt
 * Date: 2026-01-09
 */

#include <stdbool.h>

#include "pool.h"

Pool *pool_new(size_t pool_sz) {
  Pool *pool = (Pool *)malloc(sizeof(Pool));
  if (pool == NULL) {
    return NULL;
  }

  /* allocates the first free chunk array */
  Chunk *arr = pool->free_chunk = malloc(sizeof(Chunk) * pool_sz);
  if (arr == NULL) {
    free(pool);
    return NULL;
  }

  /* initialize the arr and set the next ptrs */
  for (size_t i = 0; i < pool_sz - 1; i++) {
    arr[i].next = &arr[i + 1];
  }
  arr[pool_sz - 1].next = NULL;

  /* allocate a new array start ptr and track it */
  pool->array_starts = malloc(sizeof(ArrayStart));
  if (pool->array_starts == NULL) {
    free(arr);
    free(pool);
    return NULL;
  }

  pool->array_starts->next = NULL;
  pool->array_starts->arr = arr;
  return pool;
}

void *pool_alloc(Pool *pool) {
  if (pool == NULL || pool->free_chunk == NULL) {
    return NULL;
  }

  Chunk *ptr = pool->free_chunk;
  pool->free_chunk = pool->free_chunk->next;
  return ptr;
}

void pool_free(Pool *pool, void *ptr) {
  if (pool == NULL || ptr == NULL) {
    return;
  }

  Chunk *new_free_chunk = (Chunk *)ptr;
  new_free_chunk->next = pool->free_chunk;
  pool->free_chunk = new_free_chunk;
}

void pool_destroy(Pool *pool) {
  if (pool == NULL) {
    return;
  }

  ArrayStart *arr_start = pool->array_starts;
  while (arr_start != NULL) {
    ArrayStart *next = arr_start->next;
    free(arr_start->arr);
    free(arr_start);
    arr_start = next;
  }
}

bool pool_expand(Pool *pool, size_t extra_sz) {
  if (pool == NULL) {
    return NULL;
  }

  /* allocate this new requested block size */
  Chunk *extra_arr = (Chunk *)malloc(sizeof(Chunk) * extra_sz);
  if (extra_arr == NULL) {
    return NULL;
  }

  /* link the blocks */
  for (size_t i = 0; i < extra_sz - 1; i++) {
    extra_arr[i].next = &extra_arr[i + 1];
  }
  extra_arr[extra_sz - 1].next = pool->free_chunk;
  pool->free_chunk = extra_arr;

  //                               f
  // [x]->[x]->[x]->[x] -> NULL | []->[]->[]->[] --| (NULL)
  //                       ^                       |
  //                       |-----------------------|

  /* allocate a new array block */
  ArrayStart *new_arr_start = (ArrayStart *)malloc(sizeof(ArrayStart));
  if (new_arr_start == NULL) {
    free(extra_arr);
    return NULL;
  }

  new_arr_start->arr = extra_arr;
  new_arr_start->next = pool->array_starts;
  pool->array_starts = new_arr_start;
  return true;
}
