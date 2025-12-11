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

/*----------------------------------------------------------------------------------*/
/* Decode one byte from lsb.
 * Inputs: 8 raw bytes from stego file.
 * Output: Decoded single character.
 * Description: Get 8 raw bytes from stego file and decode it's lsb to get encoded one charcter of secret data.
 */
Status decode_byte_from_lsb(unsigned char data, unsigned char *image_buffer)
{
    /* Check if buffer is valid or not */
    if (image_buffer == NULL)
    {
        printf("ERROR: Invalid buffer data in byte_from_lsb\n");
        return e_failure;
    }
    for (int i = 0; i < 8; i++)
    {
        int bit = image_buffer[i] & 1;
        data = bit << (7 - i);
    }
    return e_success;
}

