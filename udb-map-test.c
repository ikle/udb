/*
 * Universal Database Map: unit test
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include <udb/udb-map.h>

static void udb_map_show_head (struct udb_map *o, int indent)
{
	unsigned i;

	for (i = 0; i < indent; ++i)
		putchar (' ');

	for (i = 0; i < o->tail; ++i)
		printf (" %2d", i);

	printf ("\n");

	for (i = 0; i < indent; ++i)
		putchar (' ');

	for (i = 0; i < o->tail; ++i)
		printf (" --");

	printf ("\n");
}

static void udb_map_show (struct udb_map *o, int indent)
{
	unsigned i;

	for (i = 0; i < indent; ++i)
		putchar (' ');

	for (i = 0; i < o->head; ++i)
		printf (" ..");

	for (i = o->head; i < o->tail; ++i)
		printf (" %02x", o->page[i].avail);

	printf ("\n");
}

int main (int argc, char *argv[])
{
	char block[64];
	struct udb_map *o = (void *) block;
	int i, j, avail, req;

	udb_map_init (o, sizeof (block));

	for (i = 0; i < o->tail - 3; ++i)
		udb_map_add (o, i+1, rand () % 256);

	udb_map_resort (o);
	udb_map_show_head (o, 15);
	udb_map_show (o, 15);

	for (i = 0; i < 32; ++i) {
		if ((j = rand () % o->tail) < o->head)
			continue;

		avail = rand () % 256;
		printf ("%2d : %02x -> %02x |", j, o->page[j].avail, avail);

		o->page[j].avail = avail;

		udb_map_update (o, j);
		udb_map_show (o, 0);
	}

	printf ("\n");
	udb_map_show_head (o, 15);

	for (i = 0; i < 32; ++i) {
		req = rand () % 256;
		j = udb_map_search (o, req);

		printf ("%2d : %02x -> %02x |",
			j, req, j < 0 ? 0 : o->page[j].avail);
		udb_map_show (o, 0);
	}

	return 0;
}
