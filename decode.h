#ifndef DECODE_H
#define DECODE_H

#include "types.h"  /* Contains user defined types and fnc return types */

/*
 * Structure to store information required for
 * decoding stego file to secret data file
 * Info about output and intermediate data is
 * also stored
*/

#define MAX_FILE_SUFFIX 4
typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Secret File Info */
    char *secret_fname;
    FILE *fptr_secret;
    char extn_secret_file[MAX_FILE_SUFFIX + 1];
    long size_secret_extn;
    long size_secret_file;

} DecodeInfo;

/* Decoding function prototype */

/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Open stego file for decoding */
Status open_stego_file(DecodeInfo *decInfo);

/* Open secret file */
Status open_secret_file(FILE *fptr_secret);

/* Skip header of stego.bmp */
Status skip_header(FILE *fptr_stego_image);

/* Decode Magic String */
Status decode_magic_string(DecodeInfo *decInfo);

/* Decode secret file extenstion size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo, long extn_size);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Decofe secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

#endif