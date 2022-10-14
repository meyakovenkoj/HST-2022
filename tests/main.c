#include "input.h"
#include "plain.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>
#include <mpi.h>
#include "cuda_process.h"
#include "common.h"

int alloc_data(struct RunConfig *run_config)
{
    int error_code = EXIT_FAILURE;
    run_config->array1 = (float *)calloc(run_config->size, sizeof(float));
    if (!run_config->array1) {
        error_code = EXIT_FAILURE;
        CHECK_ERROR_CODE(error_code);
    }

    run_config->array2 = (float *)calloc(run_config->size, sizeof(float));
    if (!run_config->array2) {
        error_code = EXIT_FAILURE;
        CHECK_ERROR_CODE(error_code);
    }

    run_config->result = (float *)calloc(run_config->count, sizeof(float));
    if (!run_config->result) {
        error_code = EXIT_FAILURE;
        CHECK_ERROR_CODE(error_code);
    }

    error_code = EXIT_SUCCESS;
done:
    return error_code;
}

int get_best_config(struct RunConfig *run_config, int size, int length, int gpu_util)
{
    int error_code = EXIT_FAILURE;
    int vectors_count = size / length;
    run_config->length = length;
    int gpu_count = vectors_count * gpu_util / 10; 
    run_config->count = (vectors_count - gpu_count) / run_config->config->num_procs;
    int base_count = run_config->count;
    int base_size = base_count * run_config->length;
    int remainder = vectors_count % run_config->config->num_procs;
    if (run_config->config->proc_id < remainder) {
        ++run_config->count;
    }
    if (run_config->config->proc_id == 0) {
        run_config->count += gpu_count;
    }

    run_config->size = run_config->count * run_config->length;

    run_config->scatter_sizes = (int *)calloc(run_config->config->num_procs, sizeof(int));
    if (!run_config->scatter_sizes) {
        error_code = EXIT_FAILURE;
        CHECK_ERROR_CODE(error_code);
    }
    run_config->scatter_offsets = (int *)calloc(run_config->config->num_procs, sizeof(int));
    if (!run_config->scatter_offsets) {
        error_code = EXIT_FAILURE;
        CHECK_ERROR_CODE(error_code);
    }
    run_config->gather_counts = (int *)calloc(run_config->config->num_procs, sizeof(int));
    if (!run_config->gather_counts) {
        error_code = EXIT_FAILURE;
        CHECK_ERROR_CODE(error_code);
    }
    run_config->gather_offsets = (int *)calloc(run_config->config->num_procs, sizeof(int));
    if (!run_config->gather_offsets) {
        error_code = EXIT_FAILURE;
        CHECK_ERROR_CODE(error_code);
    }

    int gather_offset = 0;
    int scatter_offset = 0;
    if (run_config->config->proc_id != 0) {
        gpu_count = 0;
    }
    int gpu_size = gpu_count * run_config->length;
    for (int i = 0; i < remainder; ++i) {
        run_config->scatter_sizes[i] = base_size + run_config->length + gpu_size;
        run_config->scatter_offsets[i] = scatter_offset;
        scatter_offset += base_size + run_config->length + gpu_size;

        run_config->gather_counts[i] = base_count + 1 + gpu_count;
        run_config->gather_offsets[i] = gather_offset;
        gather_offset += base_count + 1 + gpu_count;
    }
    for (int i = remainder; i < run_config->config->num_procs; ++i) {
        run_config->scatter_sizes[i] = base_size + gpu_size;
        run_config->scatter_offsets[i] = scatter_offset;
        scatter_offset += base_size + gpu_size;

        run_config->gather_counts[i] = base_count + gpu_count;
        run_config->gather_offsets[i] = gather_offset;
        gather_offset += base_count + gpu_count;
    }

    error_code = EXIT_SUCCESS;
done:
    return error_code;
}

int main(int argc, char **argv)
{
    int error_code = MPI_ERR_UNKNOWN;
    struct MpiConfig mpi_config = { 0 };
    struct RunConfig run_config = { 0 };
    run_config.config = &mpi_config;
    float *array1 = NULL;
    float *array2 = NULL;
    float *result = NULL;
    error_code = MPI_Init(&argc, &argv);
    CHECK_ERROR_CODE(error_code);

    error_code = MPI_Comm_size(MPI_COMM_WORLD, &run_config.config->num_procs);
    CHECK_ERROR_CODE(error_code);

    error_code = MPI_Comm_rank(MPI_COMM_WORLD, &run_config.config->proc_id);
    CHECK_ERROR_CODE(error_code);

    int opt;
    int sfnd = 0;
    int lfnd = 0;
    int size = 0;
    int length = 0;
    int gpu_util = 5;
    int binary_mod = BINARY;
    while ((opt = getopt(argc, argv, "hg:s:l:")) != -1) {
        switch (opt) {
        case 'h':
            binary_mod = HUMAN;
            break;
        case 'g':
            gpu_util = atoi(optarg);
            if (gpu_util > 9 || gpu_util < 0) {
                fprintf(stderr, "GPU util must be in 0..9\n");
                error_code = EXIT_FAILURE;
                CHECK_ERROR_CODE(error_code);
            }
            break;
        case 's':
            size = atoi(optarg);
            sfnd = 1;
            break;
        case 'l':
            length = atoi(optarg);
            lfnd = 1;
            break;
        default: /* '?' */
            fprintf(stderr, "Usage: %s -s size -l length [-g gpu_util] [-h] filename1 filename2\n",
                    argv[0]);
            error_code = EXIT_FAILURE;
            CHECK_ERROR_CODE(error_code);
        }
    }

    if (!(sfnd && lfnd)) {
        fprintf(stderr, "Expected size and length options\n");
        error_code = EXIT_FAILURE;
        CHECK_ERROR_CODE(error_code);
    }

    if (optind != (argc - 2)) {
        fprintf(stderr, "Expected two filenames after options\n");
        error_code = EXIT_FAILURE;
        CHECK_ERROR_CODE(error_code);
    }

    int vectors_count = size / length;
    const char *name1 = argv[optind];
    const char *name2 = argv[optind + 1];

    error_code = get_best_config(&run_config, size, length, gpu_util);
    CHECK_ERROR_CODE(error_code);

    error_code = alloc_data(&run_config);
    CHECK_ERROR_CODE(error_code);

    if (run_config.config->proc_id == 0) {
        FILE *fp1, *fp2;
        if ((fp1 = fopen(name1, GetMode(binary_mod, READ))) == NULL) {
            fprintf(stderr, "Error opening file\n");
            error_code = EXIT_FAILURE;
            CHECK_ERROR_CODE(error_code);
        }

        if ((fp2 = fopen(name2, GetMode(binary_mod, READ))) == NULL) {
            fprintf(stderr, "Error opening file\n");
            error_code = EXIT_FAILURE;
            CHECK_ERROR_CODE(error_code);
        }
        result = (float *)calloc(vectors_count, sizeof(float));
        if (!result) {
            error_code = EXIT_FAILURE;
            CHECK_ERROR_CODE(error_code);
        }
        error_code = ReadData(binary_mod, size, &array1, fp1);
        CHECK_ERROR_CODE(error_code);
        error_code = ReadData(binary_mod, size, &array2, fp2);
        CHECK_ERROR_CODE(error_code);
    }

    error_code = MPI_Scatterv(array1, run_config.scatter_sizes, run_config.scatter_offsets, MPI_FLOAT, run_config.array1, run_config.size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    CHECK_ERROR_CODE(error_code);
    error_code = MPI_Scatterv(array2, run_config.scatter_sizes, run_config.scatter_offsets, MPI_FLOAT, run_config.array2, run_config.size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    CHECK_ERROR_CODE(error_code);

    double start_time = MPI_Wtime();
    if (run_config.config->proc_id == 0) {
        gpu_process(run_config.array1, run_config.array2, run_config.count, run_config.length, run_config.result);
    } else {
        clean_process(run_config.array1, run_config.array2, run_config.count, run_config.length, run_config.result);
    }
    double end_time = MPI_Wtime();

    error_code = MPI_Gatherv(run_config.result, run_config.count, MPI_FLOAT, result, run_config.gather_counts, run_config.gather_offsets, MPI_FLOAT, 0, MPI_COMM_WORLD);
    CHECK_ERROR_CODE(error_code);
    
    double proc_time = end_time - start_time;
    double elapsed_time;

    error_code = MPI_Reduce(&proc_time, &elapsed_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    CHECK_ERROR_CODE(error_code);

    if (run_config.config->proc_id == 0) {
        printf("elapsed time: %lf s\n", elapsed_time);
        const char *outname = "result.txt";
        FILE *outfp = NULL;
        if ((outfp = fopen(outname, GetMode(binary_mod, WRITE))) == NULL) {
            fprintf(stderr, "Error opening file\n");
            error_code = EXIT_FAILURE;
            CHECK_ERROR_CODE(error_code);
        }
        error_code = WriteData(binary_mod, vectors_count, result, outfp);
        CHECK_ERROR_CODE(error_code);
    }

done:
    free(run_config.array1);
    free(run_config.array2);
    free(run_config.result);
    free(run_config.gather_counts);
    free(run_config.gather_offsets);
    free(run_config.scatter_offsets);
    free(run_config.scatter_sizes);
    free(result);
    MPI_Finalize();
    return error_code;
}