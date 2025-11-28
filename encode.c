#include "encode.h"

/* Function Definitions */

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo)
{
    /* Open files for encoding */
    if (open_files(&encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "open_files");
        return 1;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "open_files");
    }

    /* check capacity */
    if (check_capacity(&encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "check_capacity");
        return 1;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "check_capacity");
    }

    /* Copy bmp image header */
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: %s function failed\n", "copy_bmp_header");
        return 1;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "copy_bmp_header");
    }

    /* Store Magic String */
    if (encode_magic_string(MAGIC_STRING, &encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "copy_bmp_header");
        return 1;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "copy_bmp_header");
    }
}

Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    
}