/*
 * AVL Tree Search
 *
 * Copyright (c) 2010-2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "avl-int.h"

struct avl *avl_search (struct avl **root, const void *key, avl_cmp_cb *cmp,
			avl_alloc_cb *alloc)
{
	struct avl **path[AVL_MAXH], *o;
	int i = 0, c;

	for (
		path[i++] = root, o = *root;
		o != NULL;
		path[i++] = &o->child[c > 0], o = o->child[c > 0]
	)
		if ((c = cmp (key, o)) == 0)
			return o;

	if ((o = alloc (key)) == NULL)
		return o;

	o->child[1] = o->child[0] = NULL;
	o->h = 1;

	/* insert the new node and rebalance ancestors */

	for (*path[--i] = o; i > 0 && avl_balance (path[--i]) != 0; ) {}

	return o;
}
