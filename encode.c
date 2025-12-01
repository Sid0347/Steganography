#include "encode.h"

/* Function Definitions */

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo)
{
    /* Open files for encoding */
    if (open_files(encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "open_files");
        return 1;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "open_files");
    }

    /* check capacity */
    if (check_capacity(encInfo) == e_failure)
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
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "copy_bmp_header");
        return 1;
    }
    else
    {
        printf("SUCCESS: %s function completed\n", "copy_bmp_header");
    }
}

/*----------------------------------------------------------------------------------*/
/* Encode magic string.
 * Inputs: MAGIC_STRING and Structure
 * Output: Encoded magic string
 * Description: Get defined magic string, encode it and write to stego file
*/
Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo)
{
    unsigned char buffer[8];
    for (int i = 0; magic_string != '\0'; i++)
    {
        fread(buffer, 1, 8, encInfo->fptr_src_image);
        encode_byte_to_lsb(magic_string[i], buffer);
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
}

/*----------------------------------------------------------------------------------*/
/* Encode one byte from lsb.
 * Inputs: Single character to encode and 8 raw bytes from source file
 * Output: Encoded 8 bytes with single byte
 * Description: Get bit from encode byte and replace in 8 bytes lsb of raw RGB data
*/
Status encode_byte_to_lsb(unsigned char data, unsigned char *image_buffer)
{
    for (int i = 7, k = 0; i >= 0; i--, k++)
    {
        int bit = (data >> i) & 1;
        image_buffer[k] &= 0xFE;
        image_buffer[k] |= bit;
    }
}