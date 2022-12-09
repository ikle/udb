/*
 * AVL Tree Internals
 *
 * Copyright (c) 2010-2022 Alexei A. Smekalkine <ikle@ikle.ru>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <data/avl.h>

struct node {
	struct avl handle;
	const char *key;
};

static struct avl *node_alloc (const void *key)
{
	struct node *o;

	if ((o = malloc (sizeof (*o))) == NULL)
		return NULL;

	o->key = key;
	return &o->handle;
}

static void node_free (struct avl *node)
{
	struct node *o = (void *) node;  /* use container_of if you can */

	free (o);
}

static int node_cmp (const void *key, const struct avl *node)
{
	const struct node *o = (void *) node;

	return strcmp (key, o->key);
}

static void tree_show (struct avl *node)
{
	const struct node *o = (void *) node;

	if (node == NULL)
		return;

	tree_show (node->child[0]);

	printf ("%s\n", o->key);

	tree_show (node->child[1]);
}

static const char *key[] = {
	"test string #1",
	"Lorem ipsum dolor sit amet",
	"consectetur adipiscing elit",
	"sed do eiusmod tempor incididunt",
	"ut labore et dolore magna aliqua",
	"test string #2",
	NULL,
};

int main (int argc, char *argv[])
{
	struct avl *root = NULL;
	size_t i;

	for (i = 0; key[i] != NULL; ++i)
		if (avl_search (&root, key[i], node_cmp, node_alloc) == NULL) {
			fprintf (stderr, "E: Cannot insert key into tree\n");
			return 1;
		}

	printf ("Inserted keys:\n\n");
	tree_show (root);

	i = 2;
	printf ("\nRemove %s\n", key[i]);
	node_free (avl_remove (&root, key[i], node_cmp));

	printf ("\nKeys after removal:\n\n");
	tree_show (root);

	avl_free (root, node_free);
	return 0;
}
