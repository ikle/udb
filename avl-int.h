/*
 * AVL Tree Internals
 *
 * Copyright (c) 2010-2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef DATA_AVL_INT_H
#define DATA_AVL_INT_H  1

#include <limits.h>

#include <data/avl.h>

/*
 * int	pointer	min-node-size	max-nodes		avl-max	maxh
 *
 * 16	16	48		≈ 1365			14	24
 * 16	32	80		≈ 53687091		36	48
 * 16	64	144		≈ 128102389400760775	81	96
 * 32	32	96		≈ 44739242		36	48
 * 32	64	160		≈ 115292150460684697	81	96
 * 64	64	192		≈ 96076792050570581	80	96
 *
 * AVL tree height < avl-max = k log₂(node-count + 2) + b < maxh, where
 *
 *	k = log₂φ               ≈ 1.44
 *	φ = (1 + √5) / 2        ≈ 1.618 — golden ratio
 *	b = log₂5 / 2 log₂φ - 2 ≈ -0.3
 */

#define AVL_MAXH  (sizeof (void *) * CHAR_BIT * 3/2)

/*
 * Returns the difference between the new and old heights
 */
int avl_balance (struct avl **root);

#endif  /* DATA_AVL_INT_H */
