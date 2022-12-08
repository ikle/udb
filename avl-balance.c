/*
 * AVL Tree Balance
 *
 * Copyright (c) 2010-2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <data/avl.h>

static inline int avl_height (struct avl *o)
{
	return o != NULL ? o->h : 0;
}

static int avl_rotate (struct avl **root, int deep)
{
	struct avl *x = *root, *y = x->child[deep], *z = y->child[!deep];
	int hx = x->h, hz = avl_height (z);

	if (hz > avl_height (y->child[deep])) {
		/*
		 *      x  deep = 0               x  deep = 1
		 *     / \           z           / \              z
		 *    y   D         / \         A   y            / \
		 *   / \     ->    y   x           / \    ->    x   y
		 *  A   z         / \ / \         z   D        / \ / \
		 *     / \       A  B C  D       / \          A  B C  D
		 *    B   C                     B   C
		 */
		x->child[ deep] = z->child[!deep];
		y->child[!deep] = z->child[ deep];
		z->child[!deep] = x;
		z->child[ deep] = y;
		x->h = hz;
		y->h = hz;
		z->h = hz + 1;

		*root = z;
		return z->h - hx;
	}

	/*
	 *      x  deep = 0    y          x  deep = 1    y
	 *     / \            / \        / \            / \
	 *    y   B    ->    A   x      A   y    ->    x   B
	 *   / \                / \        / \        / \
	 *  A   z              z   B      z   B      A   z
	 */
	x->child[ deep] = z;
	y->child[!deep] = x;
	x->h = hz + 1;
	y->h = hz + 2;

	*root = y;
	return y->h - hx;
}

/*
 * Returns the difference between the new and old heights
 */
int avl_balance (struct avl **root)
{
	struct avl *o = *root;
	int h, l = avl_height (o->child[0]), r = avl_height (o->child[1]);

	if (l - r + 1u < 3u) {  /* |h0 - h1| < 2 */
		h = o->h;
		o->h = l < r ? r + 1 : l + 1;
		return o->h - h;
	}

	return avl_rotate (root, l < r);
}
