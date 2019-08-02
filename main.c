#include <slurm/slurm.h>
#include <slurm/slurm_errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

int suspend_all_jobs(job_info_msg_t* job_buffer_ptr) {
    int jobs_suspended = 0;
    job_info_t* job_ptr;
    for (size_t i = 0; i < job_buffer_ptr->record_count; i++) {
        job_ptr = &job_buffer_ptr->job_array[i];
        if (job_ptr->job_state == JOB_RUNNING) {
            int job_suspend = slurm_suspend(job_ptr->job_id);
            if (job_suspend == 0) {
                printf("Job %d suspended.\n", job_ptr->job_id);
                jobs_suspended++;
            } else {
                printf("Job %d failed to suspend. Error code %d.\n", job_ptr->job_id, job_suspend);
            }
        }
        // slurm_print_job_info(stdout, job_ptr, 1);
    }
    return jobs_suspended;
}

int resume_all_jobs(job_info_msg_t* job_buffer_ptr) { return job_buffer_ptr->record_count; }

int main() {
    job_info_msg_t* job_buffer_ptr = NULL;

    if (slurm_load_jobs((time_t)NULL, &job_buffer_ptr, SHOW_ALL)) {
        slurm_perror("slurm_load_jobs error");
        return 1;
    }

    /*node_info_msg_t* node_buffer_ptr = NULL;

    if (slurm_load_node((time_t)NULL, &node_buffer_ptr, SHOW_ALL)) {
        slurm_perror("slurm_load_node error");
        return 1;
    }

    //slurm_print_node_table(stdout,
    slurm_print_job_info_msg(stdout, job_buffer_ptr, 0);

    slurm_print_node_info_msg(stdout, node_buffer_ptr, 0);
    */
    // slurm_free_node_info_msg(node_buffer_ptr);

    suspend_all_jobs(job_buffer_ptr);
    slurm_free_job_info_msg(job_buffer_ptr);
    return 0;
}
