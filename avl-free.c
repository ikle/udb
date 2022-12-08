/*
 * AVL Tree Free
 *
 * Copyright (c) 2010-2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <data/avl.h>

void avl_free (struct avl *o, avl_free_cb *free)
{
	if (o == NULL)
		return;

	avl_free (o->child[0], free);
	avl_free (o->child[1], free);

	free (o);
}
