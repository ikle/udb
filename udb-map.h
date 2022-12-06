/*
 * Universal Database Map: slice free space info
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef UDB_MAP_H
#define UDB_MAP_H  1

#include <stdint.h>

struct udb_map_page {
	uint16_t avail;			/* size of the available area	*/
	uint16_t link;			/* data page number		*/
};

/*
 * The slots in the array are sorted in descending order.
 */
struct udb_map {
	uint16_t head;			/* index of first page link	*/
	uint16_t tail;			/* number of pages in map	*/
	struct udb_map_page page[];	/* data page links		*/
};

static inline void udb_map_init (struct udb_map *o, unsigned size)
{
	o->head = o->tail = (size - sizeof (*o)) / sizeof (o->page[0]);
}

static inline int udb_map_add (struct udb_map *o, unsigned link, unsigned avail)
{
	if (o->head == 0)
		return -1;

	unsigned i = --o->head;

	o->page[i].avail = avail;
	o->page[i].link  = link;
	return i;
}

int  udb_map_search (struct udb_map *o, unsigned size);
void udb_map_update (struct udb_map *o, unsigned page);
void udb_map_resort (struct udb_map *o);

#endif  /* UDB_MAP_H */
