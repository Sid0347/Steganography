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

    if (decode_magic_string(decInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "decode_magic_string");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "decode_magic_string");
}

/*----------------------------------------------------------------------------------*/
/* Decode one byte from lsb.
 * Inputs: 8 raw bytes from stego file.
 * Output: Decoded single character.
 * Description: Get 8 raw bytes from stego file and decode it's lsb to get encoded one charcter of secret data.
 */
Status decode_byte_from_lsb(unsigned char *data, unsigned char *image_buffer)
{
    /* Check if buffer is valid or not */
    if (image_buffer == NULL)
    {
        printf("ERROR: Invalid buffer data in byte_from_lsb\n");
        return e_failure;
    }
    *data = 0;
    for (int i = 0; i < 8; i++)
    {
        int bit = image_buffer[i] & 1;            
        *data |= bit << (7 - i);
    }
    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* Decode size to lsb.
 * Inputs: Size of decode data
 * Output: Decoded 32 bit from 32 bytes of encoded data.
 * Description: Get 32 bytes from decoded file and and extract lsb and get secret data.
 */
Status decode_size_from_lsb(unsigned char *data, unsigned char *image_buffer)
{
    /* Check if buffer is valid or not */
    if (image_buffer == NULL)
    {
        printf("ERROR: Invalid buffer data in size_from_lsb");
        return e_failure;
    }
    *data = 0;
    for (int i = 0; i < 32; i++)
    {
        int bit = image_buffer[i] & 1;
        *data |= bit << (31 - i);
    }
    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* Decode magic string.
 * Inputs: MAGIC_STRING bytes file and Structure
 * Output: Decoded magic string
 * Description: Get decoded bytes from stego file and and extract magic string.
 */
Status decode_magic_string(DecodeInfo *decInfo)
{
    char str[3];
    printf("Enter magic string to validation : ");  /* To know wether stego file contain encoded data or not. */
    scanf("%[^\n]", str);
    unsigned char magic_string[3];

    for (int i = 0; i < 2; i++)
    {
        fread(decInfo->image_data, 1, 8, decInfo->fptr_stego_image);
        if (decode_byte_from_lsb(decInfo->secret_data, decInfo->image_data) == e_failure)
            return e_failure;
        magic_string[i] = decInfo->secret_data;
    }
    magic_string[2] = '\0';
    if (strcmp(str, magic_string) != 0)
    {
        printf("Magic string not matched decoding not possible\n");
        return e_failure;
    }
    return e_success;
}