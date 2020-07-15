/*****************************************************************************\
 *  job_submit_defaults.c - Set defaults in job submit request specifications.
 *****************************************************************************/

#include <slurm/slurm.h>
#include <slurm/slurm_errno.h>

#include "src/slurmctld/slurmctld.h"

#include <stdio.h>

const char plugin_name[]="Save script jobsubmit plugin";
const char plugin_type[]="job_submit/gres_env";
const uint32_t plugin_version   = SLURM_VERSION_NUMBER;

extern int job_submit(struct job_descriptor *job_desc, uint32_t submit_uid, char **err_msg) {

	if( job_desc->tres_per_node != NULL){
		debug("job_submit/gres_env: tres_per_node detected");
		if( env_array_append (&(job_desc->spank_job_env), "GRES", job_desc->tres_per_node) ){
			debug("job_submit/gres_env: set SPANK_GRES to %s", job_desc->tres_per_node);
			job_desc->spank_job_env_size++;
		}
		if (env_array_append (&(job_desc->environment), "SLURM_JOB_GRES", job_desc->tres_per_node)) {
			debug("job_submit/gres_env: set SLURM_JOB_GRES to %s", job_desc->tres_per_node);
			job_desc->env_size++;
		}
	}

	return SLURM_SUCCESS;
}

int job_modify(struct job_descriptor *job_desc, struct job_record *job_ptr,
	       uint32_t submit_uid)
{
	return SLURM_SUCCESS;
}
