/*
 * pool.c - Pool Allocator
 *
 * Author: frostzt
 * Date: 2026-01-09
 */

#include "pool.h"

Pool *pool_new(size_t pool_sz) {
  Pool *pool = (Pool *)malloc(sizeof(Pool));
  if (pool == NULL) {
    return NULL;
  }

  pool->chunk_arr = pool->free_chunk = malloc(sizeof(Chunk) * pool_sz);
  if (pool->chunk_arr == NULL) {
    free(pool);
    return NULL;
  }

  for (size_t i = 0; i < pool_sz - 1; i++) {
    pool->chunk_arr[i].next = &pool->chunk_arr[i + 1];
  }

  pool->chunk_arr[pool_sz].next = NULL;
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

  free(pool->chunk_arr);
  free(pool);
}
