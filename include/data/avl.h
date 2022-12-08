/*
 * AVL Tree
 *
 * Copyright (c) 2010-2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef DATA_AVL_H
#define DATA_AVL_H  1

#include <stddef.h>

struct avl {
	struct avl *child[2];
	int h;
};

typedef struct avl *avl_alloc_cb (const void *key);
typedef void avl_free_cb (struct avl *node);
typedef int avl_cmp_cb (const void *key, const struct avl *node);

void avl_free (struct avl *root, avl_free_cb *free);

struct avl *avl_find   (struct avl **root, const void *key, avl_cmp_cb *cmp);
struct avl *avl_search (struct avl **root, const void *key, avl_cmp_cb *cmp,
			avl_alloc_cb *alloc);
struct avl *avl_remove (struct avl **root, const void *key, avl_cmp_cb *cmp);

#endif  /* DATA_AVL_H */
