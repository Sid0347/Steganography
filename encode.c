#include "encode.h"

/* Function Definitions */

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo)
{
    /* Open files for encoding */
    if (open_files(encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "open_files");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "open_files");

    /* check capacity */
    if (check_capacity(encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "check_capacity");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "check_capacity");

    /* Copy bmp image header */
    if (copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
    {
        printf("ERROR: %s function failed\n", "copy_bmp_header");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "copy_bmp_header");

    /* Store Magic String */
    if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "encode_magic_string");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "encode_magic_string");

    /* Encode secret file extenstion size */
    if (encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "encode_secret_file_extn_size");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "encode_secret_file_extn_size");

    /* Encode secret file extenstion */
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "encode_secret_file_extn");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "encode_secret_file_extn");

     /* Encode secret file size */
    if (encode_secret_file_size(encInfo->size_secret_file, encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "encode_secret_file_size");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "encode_secret_file_size");

     /* Encode secret file data */
    if (encode_secret_file_data(encInfo) == e_failure)
    {
        printf("ERROR: %s function failed\n", "encode_secret_file_data");
        return e_failure;
    }
    else
        printf("SUCCESS: %s function completed\n", "encode_secret_file_data");

    
}

/*----------------------------------------------------------------------------------*/
/* Encode one byte from lsb.
 * Inputs: Single character to encode and 8 raw bytes from source file
 * Output: Encoded 8 bytes with single byte
 * Description: Get bit from encode byte and replace in 8 bytes lsb of raw RGB data
 */
Status encode_byte_to_lsb(unsigned char data, unsigned char *image_buffer)
{
    /* Check if buffer is valid or not. */
    if (image_buffer == NULL)
    {
        printf("Error: Invalid buffer data in byte_to_lsb\n");
        return e_failure;
    }
    for (int i = 0; i < 8; i++)
    {
        int bit = (data >> (7 - i)) & 1;
        image_buffer[i] &= 0xFE;
        image_buffer[i] |= bit;
    }
    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* Encode one byte from lsb.
 * Inputs: Size of encode data
 * Output: Encoded 32 bytes with 32 bit data
 * Description: Get bit from encode byte and replace in 32 bytes lsb of raw RGB data
 */
Status encode_size_to_lsb(long size, unsigned char *image_buffer)
{
    /* Check if buffer is valid or not. */
    if (image_buffer == NULL)
    {
        printf("Error: Invalid buffer data in size_to_lsb\n");
        return e_failure;
    }
    for (int i = 0; i < 32; i++)
    {
        int bit = (size >> (31 - i)) & 1;
        image_buffer[i] &= 0xFE;
        image_buffer[i] |= bit;
    }
    return e_success;
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
        if (encode_byte_to_lsb(magic_string[i], buffer) == e_failure)
            return e_failure;
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* Encode secret file extenstion size.
 * Inputs: Size of stored secret file extention and structure
 * Output: Encoded size of secret file extenstion
 * Description: Get size of stored secrete file extenstion from the structure and return encoded data to store in stego file.
 */
Status encode_secret_file_extn_size(long file_extn_size, EncodeInfo *encInfo)
{
    unsigned char buffer[32];

        fread(buffer, 1, 32, encInfo->fptr_src_image);
        if (encode_size_to_lsb(file_extn_size, buffer) == e_failure)
            return e_failure;
        fwrite(buffer, 1, 32, encInfo->fptr_stego_image);
    
    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* Encode secret file extenstion.
 * Inputs: Stored file secret file extention and structure
 * Output: Encoded secret file extenstion
 * Description: Get stored secrete file extenstion from the structure and return encoded data to store in stego file.
 */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    unsigned char buffer[8];
    for (int i = 0; file_extn != '\0'; i++)
    {
        fread(buffer, 1, 8, encInfo->fptr_src_image);
        if (encode_byte_to_lsb(file_extn[i], buffer) == e_failure)
            return e_failure;
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* Encode secret file size.
 * Inputs: Stored file secret file size and structure
 * Output: Encoded secret file size 
 * Description: Get stored secrete file size from the structure and return encoded data to store in stego file.
 */
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    unsigned char buffer[32];

        fread(buffer, 1, 32, encInfo->fptr_src_image);
        if (encode_size_to_lsb(file_size, buffer) == e_failure)
            return e_failure;
        fwrite(buffer, 1, 32, encInfo->fptr_stego_image);
    
    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* Encode secret file data.
 * Inputs: Stored file secret file size and structure
 * Output: Encoded secret file data
 * Description: Get stored secrete file size from the structure and return encoded data to store in stego file.
 */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
    unsigned char buffer[8];
    
    while (fread(encInfo->secret_data, 1, 1, encInfo->fptr_secret) == 1)
    {
        fread(buffer, 1, 8, encInfo->fptr_src_image);
        if (encode_byte_to_lsb(encInfo->secret_data, buffer) == e_failure)
            return e_failure;
        fwrite(buffer, 1, 8, encInfo->fptr_stego_image);
    }
    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* Copy remaining image data
 * Inputs: Source and destination file pointer
 * Output: Copy remaining image data
 * Description: Copy all the remaining image data from source file to stego file.
 */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    uint current_pos = ftell(fptr_src);
    uint size = get_file_size(fptr_src);
    fseek(fptr_src, current_pos, SEEK_SET);
    uint remaining_data_bytes = size - current_pos;

    unsigned char *buffer;
    buffer = malloc(remaining_data_bytes);
    if  (!buffer)
    {
        perror("Memory allocation failed.\n");
        return e_failure;
    }
    fread(buffer, 1, remaining_data_bytes, fptr_src);
    fwrite(buffer, 1, remaining_data_bytes, fptr_dest);

    return e_success;
}