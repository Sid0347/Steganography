#include "decode.h"

/* Decoding Operations Defination */

/* Read and validate Decode args from argv */
Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo)
{
    /* Check for the stego bmp file */
    if (strstr(argv[2], ".bmp") != NULL)
    {
        decInfo->stego_image_fname = argv[2];
    }
    else
        return e_failure;

    /* Check for the output file */
    if (argc == 4)
    {
        decInfo->secret_fname = strtok(argv[3], ".");
        if (decInfo->secret_fname == NULL)
            return e_failure;
    }
    else
    {
        decInfo->secret_fname = "Decoded_secret";
    }
}

/*----------------------------------------------------------------------------------*/
/*
 * Open stego file in read mode.
 * Inputs: Stego Image file.
 * Output: FILE pointer for above file
 * Return Value: e_success or e_failure, on file errors
 */
Status open_stego_file(DecodeInfo *decInfo)
{
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    /* ERROR handling condition */
    if (decInfo->fptr_stego_image == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open %s file\n", decInfo->stego_image_fname);
        return e_failure;
    }
    /* No error occur return success */
    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* Skip stego bmp header
 * Inputs: Stego image file ptr
 * Output: Skip header to stego image file
 * Return values: e_success or e_failure
 * Description: Skip the header part of stego file, because secret data was encoded after bmp header.
 */
Status skip_header(FILE *fptr_stego_fname)
{
    if (fseek(fptr_stego_fname, 54, SEEK_SET) != 0)
        return e_failure;

    return e_success;
}

/*----------------------------------------------------------------------------------*/
/*
 *  Open secret file in write mode.
 * Inputs: Secret file name from structure.
 * Output: FILE pointer for above file
 * Return Value: e_success or e_failure, on file errors
 */
Status open_secret_file(DecodeInfo *decInfo)
{
    decInfo->fptr_secret = fopen(decInfo->secret_fname, "w");
    /* ERROR handling condition */
    if (decInfo->fptr_secret == NULL)
    {
        perror("fopen");
        fprintf(stderr, "ERROR: Unable to open %s file\n", decInfo->secret_fname);
        return e_failure;
    }
    /* No error occur return success */
    return e_success;
}