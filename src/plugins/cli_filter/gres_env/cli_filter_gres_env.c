#define _GNU_SOURCE

#include <inttypes.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "slurm/slurm_errno.h"
#include "src/common/slurm_xlator.h"
#include "src/common/slurm_opt.h"
#include "src/common/cli_filter.h"
#include "src/plugins/cli_filter/common/cli_filter_common.h"

const char plugin_name[]       	= "cli filter gres env plugin";
const char plugin_type[]       	= "cli_filter/gres_env";
const uint32_t plugin_version   = SLURM_VERSION_NUMBER;

extern int setup_defaults(slurm_opt_t *opt, bool early)
{
    return SLURM_SUCCESS;
}

extern int pre_submit(slurm_opt_t *opt, int offset)
{
	if (opt->gres != NULL) {
		char *str;
		int size;
		size = snprintf(NULL, 0, "SLURM_JOB_GRES=%s", opt->gres);
		str = malloc(size + 1);
		snprintf(str, size + 1, "SLURM_JOB_GRES=%s", opt->gres);
		putenv(str);
	}
	return SLURM_SUCCESS;
}

extern int post_submit(int offset, uint32_t jobid, uint32_t stepid)
{
	return SLURM_SUCCESS;
}
