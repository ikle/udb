/*
 * Universal Database Map: slice free space info
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdlib.h>

#include <udb/udb-map.h>

int udb_map_search (struct udb_map *o, unsigned size)
{
	struct udb_map_page *p = o->page;
	unsigned i, lo, hi;

	if (o->head >= o->tail || p[o->head].avail < size)
		return -1;

	for (lo = o->head, hi = o->tail - 1; lo < hi; ) {
		i = (lo + hi + 1) / 2;

		if (p[i].avail >= size)
			lo = i;
		else
			hi = (lo + hi) / 2;
	}

	return lo;
}

void udb_map_update (struct udb_map *o, unsigned page)
{
	struct udb_map_page *p = o->page, target;
	unsigned i, lo, hi;

	if (o->head < page && p[page-1].avail < p[page].avail) {
		for (lo = o->head, hi = page-1; lo < hi; ) {
			i = (lo + hi) / 2;

			if (p[i].avail < p[page].avail)
				hi = i;
			else
				lo = (lo + hi + 1) / 2;
		}

		for (target = p[page], i = page; i > hi; --i)
			p[i] = p[i-1];

		p[i] = target;
	}
	else if (page < (o->tail-1) && p[page].avail < p[page+1].avail) {
		for (lo = page+1, hi = o->tail-1; lo < hi; ) {
			i = (lo + hi + 1) / 2;

			if (p[page].avail < p[i].avail)
				lo = i;
			else
				hi = (lo + hi) / 2;
		}

		for (target = p[page], i = page; i < lo; ++i)
			p[i] = p[i+1];

		p[i] = target;
	}
}

static int page_resort_cmp (const void *A, const void *B)
{
	const struct udb_map_page *a = A, *b = B;

	return (int) b->avail - (int) a->avail;  /* descending order */
}

void udb_map_resort (struct udb_map *o)
{
	qsort (o->page + o->head, o->tail - o->head, sizeof (o->page[0]),
	       page_resort_cmp);
}
