#include "decode.h"

/* Decoding Operations Defination */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    /* Check for the stego bmp file */
    if (strstr(argv[2], ".bmp") != NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else   
        return e_failure;

    /* Check for the output file */
    if (argc_count == 4)
    {
        decInfo->secret_fname = strtok(argv[3], ".");
        if (decInfo->secret_fname == NULL)
            return e_failure;
    }
    else
    {
        decInfo->secret_fname = "secret";
    }
}

/*----------------------------------------------------------------------------------*/
/* 
 * Get File pointer for i/p file. 
 * Inputs: Stego Image file.
 * Output: FILE pointer for above file
 * Return Value: e_success or e_failure, on file errors
 */
Status open_stego_file(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");
    /* ERROR handling condition */
    if (decInfo->stego_image_fname == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open %s file\n", decInfo->stego_image_fname);
        return e_failure;
    }
    /* No error occur return success */
    return e_success;
}

