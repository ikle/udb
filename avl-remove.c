/*
 * AVL Tree Delete
 *
 * Copyright (c) 2010-2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include "avl-int.h"

struct avl *avl_remove (struct avl **root, const void *key, avl_cmp_cb *cmp)
{
	struct avl **path[AVL_MAXH], *o, *target, *child;
	int i = 0, c, hole;

	for (
		path[i++] = root, o = *root;
		o != NULL;
		path[i++] = &o->child[c > 0], o = o->child[c > 0]
	)
		if ((c = cmp (key, o)) == 0)
			break;

	if (o == NULL)
		return NULL;

	target = o;  /* node to remove */

	if (o->child[0] != NULL) {
		hole = i-1;  /* the place to insert replacement */

		/* find preceding node of target */

		for (
			path[i++] = &o->child[0], o = o->child[0];
			o->child[1] != NULL;
			path[i++] = &o->child[1], o = o->child[1]
		) {}

		child = o->child[0];

		/* move o node to hole, save hole links intact */

		*o = *target;
		*path[hole] = o;
		path[hole+1] = &o->child[0];
	}
	else
		child = o->child[1];

	/* move child up and rebalance ancestors */

	for (*path[--i] = child; i > 0 && avl_balance (path[--i]) != 0; ) {}

	return target;
}
