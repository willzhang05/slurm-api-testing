#include <slurm/slurm.h>
#include <slurm/slurm_errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    if (jobs_suspended == 0) {
        printf("No jobs suspended.\n");
    }
    return jobs_suspended;
}

int resume_all_jobs(job_info_msg_t* job_buffer_ptr) {
    int jobs_resumed = 0;
    job_info_t* job_ptr;
    for (size_t i = 0; i < job_buffer_ptr->record_count; i++) {
        job_ptr = &job_buffer_ptr->job_array[i];
        if (job_ptr->job_state == JOB_SUSPENDED) {
            int job_resume = slurm_resume(job_ptr->job_id);
            if (job_resume == 0) {
                printf("Job %d resumed.\n", job_ptr->job_id);
                jobs_resumed++;
            } else {
                printf("Job %d failed to resume. Error code %d.\n", job_ptr->job_id, job_resume);
            }
        }
    }
    if (jobs_resumed == 0) {
        printf("No jobs to resumed.\n");
    }
    return jobs_resumed;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid arguments.\n\tUsage: ./slurmutil <option>\n");
        return 1;
    }

    job_info_msg_t* job_buffer_ptr = NULL;

    if (slurm_load_jobs((time_t)NULL, &job_buffer_ptr, SHOW_ALL)) {
        slurm_perror("slurm_load_jobs error");
        return 1;
    }

    if (strcmp(argv[1], "suspend") == 0) {
        suspend_all_jobs(job_buffer_ptr);
    } else if (strcmp(argv[1], "resume") == 0) {
        resume_all_jobs(job_buffer_ptr);
    } else {
        fprintf(stderr, "Invalid arguments.\n\tUsage: ./slurmutil <option>\n");
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

    slurm_free_job_info_msg(job_buffer_ptr);
    return 0;
}
