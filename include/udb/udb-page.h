/*
 * Universal Database Page
 *
 * Copyright (c) 2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#ifndef UDB_PAGE_H
#define UDB_PAGE_H  1

#include <stddef.h>
#include <stdint.h>

struct udb_page_slot {
	uint16_t size;		/* size of slot data			*/
	uint16_t offset;	/* offset of data from start of page	*/
};

/*
 * Slot 0 special:
 *
 * slot[0].size   = number of slots in page including slot 0
 * slot[0].offset = size of page
 *
 * All other slots are data slots.
 */
struct udb_page {
	struct udb_page_slot slot[1];
};

static inline
void udb_page_init (struct udb_page *o, unsigned size, uint16_t *avail)
{
	struct udb_page_slot *s = o->slot;

	s[0].size   = 1;
	s[0].offset = size;

	*avail = (size - sizeof (s[0]) * 2) & ~7;  /* max size of next item */
}

unsigned udb_page_alloc (struct udb_page *o, unsigned size, uint16_t *avail);

static inline void udb_page_free (struct udb_page *o, unsigned slot)
{
	struct udb_page_slot *s = o->slot;

	if (slot < s[0].size)
		s[slot].size = 0;
}

void udb_page_vacuum (struct udb_page *o, uint16_t *avail);

static inline void *udb_page_data (struct udb_page *o, unsigned slot)
{
	struct udb_page_slot *s = o->slot;
	unsigned char *p = (void *) o;

	return slot < s[0].size ? p + s[slot].offset : NULL;
}

static inline unsigned udb_page_data_size (struct udb_page *o, unsigned slot)
{
	struct udb_page_slot *s = o->slot;

	return slot < s[0].size ? s[slot].size : 0;
}

#endif  /* UDB_PAGE_H */
