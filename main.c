#include <slurm/slurm.h>
#include <slurm/slurm_errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int main() {
    job_info_msg_t* job_buffer_ptr = NULL;

    if (slurm_load_jobs((time_t)NULL, &job_buffer_ptr, SHOW_ALL)) {
        slurm_perror("slurm_load_jobs error");
        return 1;
    }

    slurm_print_job_info_msg(stdout, job_buffer_ptr, 0);
    return 0;
}
