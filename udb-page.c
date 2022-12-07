/*
 * Universal Database Page
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <string.h>

#include <udb/udb-page.h>

unsigned udb_page_alloc (struct udb_page *o, unsigned size, uint16_t *avail)
{
	struct udb_page_slot *s = o->slot;
	unsigned i = s[0].size;

	int lo = sizeof (s[0]) * (i + 1);
	int hi = (s[i-1].offset - (size + 7)) & ~7;  /* align to 8 */

	if (size > s[0].offset || lo > hi)
		return 0;

	s[i].size   = size;
	s[i].offset = hi;

	++s[0].size;
	*avail = (hi - lo - sizeof (s[0])) & ~7;  /* max size of next item */
	return i;
}

void udb_page_vacuum (struct udb_page *o, uint16_t *avail)
{
	struct udb_page_slot *s = o->slot;
	const unsigned count = s[0].size;
	unsigned char *p = (void *) o;
	unsigned i, j, lo, hi, size;

	for (i = 1; i < count; ++i)
		if (s[i].size == 0) {
			lo   = s[count-1].offset;
			hi   = s[i].offset;
			size = s[i-1].offset - hi;

			memmove (p + lo + size, p + lo, hi - lo);

			for (j = i; j < count; ++j)
				s[j].offset += size;
		}

	lo = sizeof (s[0]) * count;
	hi = s[count-1].offset;

	*avail = (hi - lo - sizeof (s[0])) & ~7;
}
