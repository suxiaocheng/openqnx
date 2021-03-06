/*	$NetBSD: stringlist.c,v 1.10 2000/01/25 16:24:40 enami Exp $	*/

/*-
 * Copyright (c) 1994, 1999 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Christos Zoulas.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *        This product includes software developed by the NetBSD
 *        Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * $QNXLicenseC:
 * Copyright 2007, QNX Software Systems. All Rights Reserved.
 * 
 * You must obtain a written license from and pay applicable license fees to QNX 
 * Software Systems before you may reproduce, modify or distribute this software, 
 * or any work that includes all or part of this software.   Free development 
 * licenses are available for evaluation and non-commercial purposes.  For more 
 * information visit http://licensing.qnx.com or email licensing@qnx.com.
 *  
 * This file may contain contributions from others.  Please review this entire 
 * file for other proprietary rights or license notices, as well as the QNX 
 * Development Suite License Guide at http://licensing.qnx.com/license-guide/ 
 * for other information.
 * $
 */



#include <sys/cdefs.h>
#if defined(LIBC_SCCS) && !defined(lint)
__RCSID("$NetBSD: stringlist.c,v 1.10 2000/01/25 16:24:40 enami Exp $");
#endif /* LIBC_SCCS and not lint */

//#include "namespace.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stringlist.h>

#ifdef __weak_alias
__weak_alias(sl_add,_sl_add)
__weak_alias(sl_find,_sl_find)
__weak_alias(sl_free,_sl_free)
__weak_alias(sl_init,_sl_init)
#endif

#define _SL_CHUNKSIZE	20

#ifndef _DIAGASSERT
#define _DIAGASSERT(x) ((void)0)
#endif

/*
 * sl_init(): Initialize a string list
 */
StringList *
sl_init()
{
	StringList *sl;

	sl = malloc(sizeof(StringList));
	if (sl == NULL)
		return (NULL);

	sl->sl_cur = 0;
	sl->sl_max = _SL_CHUNKSIZE;
	sl->sl_str = malloc(sl->sl_max * sizeof(char *));
	if (sl->sl_str == NULL) {
		free(sl);
		sl = NULL;
	}
	return (sl);
}


/*
 * sl_add(): Add an item to the string list
 */
int
sl_add(sl, name)
	StringList *sl;
	char *name;
{

	_DIAGASSERT(sl != NULL);

	if (sl->sl_cur == sl->sl_max - 1) {
		char	**new;

		new = (char **)realloc(sl->sl_str,
		    (sl->sl_max + _SL_CHUNKSIZE) * sizeof(char *));
		if (new == NULL)
			return (-1);
		sl->sl_max += _SL_CHUNKSIZE;
		sl->sl_str = new;
	}
	sl->sl_str[sl->sl_cur++] = name;
	return (0);
}


/*
 * sl_free(): Free a stringlist
 */
void
sl_free(sl, all)
	StringList *sl;
	int all;
{
	size_t i;

	if (sl == NULL)
		return;
	if (sl->sl_str) {
		if (all)
			for (i = 0; i < sl->sl_cur; i++)
				free(sl->sl_str[i]);
		free(sl->sl_str);
	}
	free(sl);
}


/*
 * sl_find(): Find a name in the string list
 */
char *
sl_find(sl, name)
	StringList *sl;
	char *name;
{
	size_t i;

	_DIAGASSERT(sl != NULL);

	for (i = 0; i < sl->sl_cur; i++)
			/*
			 * XXX check sl->sl_str[i] != NULL?
			 */
		if (strcmp(sl->sl_str[i], name) == 0)
			return (sl->sl_str[i]);

	return (NULL);
}
