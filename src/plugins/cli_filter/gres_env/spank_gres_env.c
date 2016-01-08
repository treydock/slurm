/*
 * Original case: https://bugs.schedmd.com/show_bug.cgi?id=9389
 * Ideas from another case: https://bugs.schedmd.com/show_bug.cgi?id=6609#c3
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <slurm/spank.h>

/* Required for the library function strstr() */
#define _GNU_SOURCE

/* All SPANK plugins must define this macro for the Slurm plugin loader. */
SPANK_PLUGIN(gres_env, 1);

#define GRES_ENV_VAR "SLURM_JOB_GRES"

int slurm_spank_init_post_opt(spank_t spank, int ac, char *argv[])
{
	char *gres;
	if (getenv(GRES_ENV_VAR)) {
		gres = getenv(GRES_ENV_VAR);
		spank_job_control_setenv(spank, "GRES", gres, 1);
		return 0;
	}
	return 0;
}
