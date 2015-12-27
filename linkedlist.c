/***************************************************************************
                          linkedlist.c  -  description
                             -------------------
    begin                : Mon Sep 2 2002
    copyright            : (C) 2002 by Tim-Philipp Müller
    email                : t.i.m@orange.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h>
#include <string.h>

#include "global.h"

#include "linkedlist.h"


/* slist_append
 *
 * appends an item to an SList
 *
 */

SList *
slist_append (SList *list, void *data)
{
	SList	*newitem = malloc (sizeof(SList));
	SList	*last;

	if (!newitem)		/* best thing to do in this case */
		return list;

	newitem->next = NULL;
	newitem->data = data;
	newitem->last = newitem;	/* in case this is going to be the first item */

	if (!list)
		return newitem;

	last = list->last;

	if (!last)					/* Houston, we have a problem */
		return list;

	last->next = newitem;
	list->last = newitem;
	
	return list;
}


/* slist_length
 *
 * returns the number of items in a SList (and 0 if list==NULL)
 *
 */

unsigned int
slist_length (SList *list)
{
	SList			*item = list;
	unsigned int	 num = 0;

	while (item)
	{
		num++;
		item = item->next;
	}
	return num;
}




