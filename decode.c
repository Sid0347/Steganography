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

    /* Skip header of stego.bmp */
    if (skip_header(decInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: %s function failed\n", "skip_header");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "skip_header");

    /* Decode Magic String */
    if (decode_magic_string(decInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "decode_magic_string");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "decode_magic_string");

    /* Decode secret file extenstion size */
    if (decode_secret_file_extn_size(decInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "decode_secret_file_extn_size");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "decode_secret_file_extn_size");

    /* Decode secret file extenstion */
    if (decode_secret_file_extn(decInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "decode_secret_file_extn");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "decode_secret_file_extn");

    /* Decode secret file size */
    if (decode_secret_file_size(decInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "decode_secret_file_size");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "decode_secret_file_size");

    /* Decode secret file data */
    if (decode_secret_file_data(decInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "decode_secret_file_data");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "decode_secret_file_data");
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
Status decode_size_from_lsb(long *size, unsigned char *image_buffer)
{
    /* Check if buffer is valid or not */
    if (image_buffer == NULL)
    {
        printf("ERROR: Invalid buffer data in size_from_lsb");
        return e_failure;
    }
    *size = 0;
    for (int i = 0; i < 32; i++)
    {
        int bit = image_buffer[i] & 1;
        *size |= bit << (31 - i);
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
    printf("Enter magic string to validation : "); /* To know wether stego file contain encoded data or not. */
    scanf("%2s", str);
    unsigned char magic_string[3];
    for (int i = 0; i < 2; i++)
    {
        if (fread(decInfo->image_data, 1, 8, decInfo->fptr_stego_image) != 8)
        {
            printf("Failed to read magic string bytes\n");
            return e_failure;
        }
        if (decode_byte_from_lsb(&decInfo->secret_data, decInfo->image_data) == e_failure)
        {
            printf("Failed to decode magic string\n");
            return e_failure;
        }
        magic_string[i] = decInfo->secret_data;
    }
    magic_string[2] = '\0';
    printf("Decoded magic string : %s\n", magic_string);
    if (strcmp(str, magic_string) != 0)
    {
        printf("Magic string not matched decoding not possible\n");
        return e_failure;
    }
    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* coDede secret file extenstion size.
 * Inputs: structure
 * Output: Decoded size of secret file extenstion
 * Description: Get 32 bytes of decoded data from stego file and store actual size of secret file extenstion size.
 */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    unsigned char buffer[32];

    if (fread(buffer, 1, 32, decInfo->fptr_stego_image) != 32)
        return e_failure;

    if (decode_size_from_lsb(&decInfo->size_secret_extn, buffer) == e_failure)
        return e_failure;
    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* Decode secret file extenstion.
 * Inputs: Structure
 * Output: Decoded secret file extenstion
 * Description: Get no of extn size encoded bytes from stego file and decode it and give file extenstion.
 */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    for (int i = 0; i < decInfo->size_secret_extn; i++)
    {
        if (fread(decInfo->image_data, 1, 8, decInfo->fptr_stego_image) != 8)
            return e_failure;

        if (decode_byte_from_lsb(&decInfo->secret_data, decInfo->image_data) == e_failure)
            return e_failure;

        decInfo->extn_secret_file[i] = decInfo->secret_data;
    }
    decInfo->extn_secret_file[decInfo->size_secret_extn] = '\0';
    /* Concatenate secret file name with it's extenstion. */
    printf("secret file name : %s\n", decInfo->secret_fname);

    strcat(decInfo->secret_fname, decInfo->extn_secret_file);

    printf("secret file name : %s\n", decInfo->secret_fname);
    /* Open secret file in 'w' mode. */
    if (open_secret_file(decInfo) == e_failure)
        return e_failure;

    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* Decode secret file size.
 * Inputs: Structure members.
 * Output: Encoded secret file size.
 * Description: Get 32 byte of encoded data from stego file and and extract actual size of secret file.
 */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
    unsigned char buffer[32];

    if (fread(buffer, 1, 32, decInfo->fptr_stego_image) != 32)
        return e_failure;

    if (decode_size_from_lsb(&decInfo->size_secret_file, buffer) == e_failure)
        return e_failure;

    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* Decode secret file data.
 * Inputs: Encoded bytes.
 * Output: Decoded data.
 * Description: Get no of secret file size encoded bytes from stego file and write it in secret file by extracting it.
 */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
    for (int i = 0; i < decInfo->size_secret_file; i++)
    {
        if (fread(decInfo->image_data, 1, 8, decInfo->fptr_stego_image) != 8)
            return e_failure;

        if (decode_byte_from_lsb(&decInfo->secret_data, decInfo->image_data) == e_failure)
            return e_failure;
        /* fputc() because we are writing only one character in secret file. */
        fputc(decInfo->secret_data, decInfo->fptr_secret);
    }
}