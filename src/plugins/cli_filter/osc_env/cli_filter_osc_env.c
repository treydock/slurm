// REF - https://bugs.schedmd.com/show_bug.cgi?id=9389#c7
#define _GNU_SOURCE

#include <inttypes.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "src/common/slurm_xlator.h"

#include "slurm/slurm_errno.h"
#include "src/common/cli_filter.h"
#include "src/common/slurm_opt.h"

const char plugin_name[]    = "cli filter osc_env plugin";
const char plugin_type[]    = "cli_filter/osc_env";
const uint32_t plugin_version    = SLURM_VERSION_NUMBER;

extern int setup_defaults(slurm_opt_t *opt, bool early)
{
    return SLURM_SUCCESS;
}

extern int pre_submit(slurm_opt_t *opt, int offset)
{
    if (opt->time_limit != NO_VAL) {
        char *str;
        int size;
        int time_limit_secs;

        time_limit_secs = opt->time_limit * 60;
        /* Use snprintf to get length of string. */
        size = snprintf(NULL, 0, "SLURM_TIME_LIMIT=%d",
                time_limit_secs);
        str = malloc(size + 1); /* Add 1 for NULL-terminating byte. */
        snprintf(str, size + 1, "SLURM_TIME_LIMIT=%d",
                time_limit_secs);
        putenv(str);
    }
    return SLURM_SUCCESS;
}

extern void post_submit(int offset, uint32_t jobid, uint32_t stepid)
{
    return;
}
