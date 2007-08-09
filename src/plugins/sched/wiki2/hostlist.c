/*****************************************************************************\
 *  hostlist.c - Convert hostlist expressions between Slurm and Moab formats
 *****************************************************************************
 *  Copyright (C) 2007 The Regents of the University of California.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Morris Jette <jette1@llnl.gov>
 *  UCRL-CODE-226842.
 *  
 *  This file is part of SLURM, a resource management program.
 *  For details, see <http://www.llnl.gov/linux/slurm/>.
 *  
 *  SLURM is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission 
 *  to link the code of portions of this program with the OpenSSL library under 
 *  certain conditions as described in each individual source file, and 
 *  distribute linked combinations including the two. You must obey the GNU 
 *  General Public License in all respects for all of the code used other than 
 *  OpenSSL. If you modify file(s) with this exception, you may extend this 
 *  exception to your version of the file(s), but you are not obligated to do 
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in 
 *  the program, then also delete it here.
 *  
 *  SLURM is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *  
 *  You should have received a copy of the GNU General Public License along
 *  with SLURM; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#if HAVE_CONFIG_H
#  include "config.h"
#  if HAVE_INTTYPES_H
#    include <inttypes.h>
#  else
#    if HAVE_STDINT_H
#      include <stdint.h>
#    endif
#  endif  /* HAVE_INTTYPES_H */
#else   /* !HAVE_CONFIG_H */
#  include <inttypes.h>
#endif  /*  HAVE_CONFIG_H */

#include <stdlib.h>
#include <string.h>

#include "src/common/hostlist.h"
#include "src/common/node_select.h"
#include "src/common/xmalloc.h"
#include "src/common/xstring.h"

/*
 * Convert Moab supplied TASKLIST expression into a SLURM hostlist expression
 *
 * Moab format 1: tux0:tux0:tux1:tux1:tux2   (list host for each cpu)
 * Moab format 2: tux[0-1]*2:tux2            (list cpu count after host name)
 *
 * SLURM format:  tux0,tux0,tux1,tux1,tux2   (if consumable resources enabled)
 * SLURM format:  tux0,tux1,tux2             (if consumable resources disabled)
 *
 * NOTE: returned string must be released with xfree()
 */
extern char * moab2slurm_task_list(char *moab_tasklist, int *task_cnt)
{
	char *slurm_tasklist, *host, *tmp1, *tmp2, *tok, *tok_p;
	int i, reps;
	hostlist_t hl;
	static uint32_t cr_test = 0, cr_enabled = 0;

	if (cr_test == 0) {
		select_g_get_info_from_plugin(SELECT_CR_PLUGIN,
						&cr_enabled);
		cr_test = 1;
	}

	*task_cnt = 0;
	tmp1 = strchr(moab_tasklist, (int) '*');

	if (tmp1 == NULL) {	/* Moab format 1 */
		slurm_tasklist = xstrdup(moab_tasklist);
		if (moab_tasklist[0])
			*task_cnt = 1;
		for (i=0; slurm_tasklist[i]!='\0'; i++) {
			if (slurm_tasklist[i] == ':') {
				slurm_tasklist[i] = ',';
				(*task_cnt)++;
			}
		}		
		return slurm_tasklist;
	}

	/* Moab format 2 */
	slurm_tasklist = xstrdup("");
	tmp1 = xstrdup(moab_tasklist);
	tok = strtok_r(tmp1, ":", &tok_p);
	while (tok) {
		/* find task count, assume 1 if no "*" */
		tmp2 = strchr(tok, (int) '*');
		if (tmp2) {
			reps = atoi(tmp2 + 1);
			tmp2[0] = '\0';
		} else
			reps = 1;
		(*task_cnt) += reps;
		if (!cr_enabled)
			reps = 1;

		/* find host expression */
		hl = hostlist_create(tok);
		while ((host = hostlist_shift(hl))) {
			for (i=0; i<reps; i++) {
				if (*task_cnt)
					xstrcat(slurm_tasklist, ",");
				xstrcat(slurm_tasklist, host);
			}
			free(host);
		}
		hostlist_destroy(hl);

		/* get next token */
		tok = strtok_r(NULL, ":", &tok_p);
	}
	xfree(tmp1);
	return slurm_tasklist;
}
