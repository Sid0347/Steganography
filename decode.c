#include "decode.h"

/*----------------------------Decode Function Definitions---------------------------*/
/*----------------------------------------------------------------------------------*/
/* Perform the encoding */
Status do_decoding(DecodeInfo *decInfo)
{
    /* Open stego file */
    if (open_stego_file(decInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "open_stego_file");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "open_stego_file");

    if (skip_header(decInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: %s function failed\n", "skip_header");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "skip_header");

    
}
