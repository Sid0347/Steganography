#include "encode.h"

/* Function Definitions */
/*----------------------------------------------------------------------------------*/
/* Check operation typy */
OperationType check_operation_type(char *argv[])
{
    
    /* Validate operation type. */
    if (strcmp(argv[1], "-e") == 0)
        return e_encode;
    else if (strcmp(argv[1], "-d") == 0)
        return e_decode;
    else
        e_unsupported;
}
/*----------------------------------------------------------------------------------*/
/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(int argc, char *argv[], EncodeInfo *encInfo)
{
    /* Check source file extention */
    if (strstr(argv[2], ".bmp") != NULL)
    {
        encInfo->src_image_fname = argv[2];
    }
    else
        return e_failure;

    /* Check secret file extention */
    if (strstr(argv[3], ".txt") != NULL)
    {
        encInfo->secret_fname = argv[3];
        strncpy(encInfo->extn_secret_file, argv[3] + (strlen(argv[3]) - 4), 4);
    }
    else
        return e_failure;

    /* Check Output file is given or not, If not create one default file */
    if (argc == 5)
    {
        if (strstr(argv[4], ".bmp") != NULL)
            encInfo->stego_image_fname = argv[4];
        else
            e_failure;
    }
    else
        encInfo->stego_image_fname = "stego.bmp";

    return e_success;
}
/*----------------------------------------------------------------------------------*/
/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR1: Unable to open file %s\n", encInfo->src_image_fname);

    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR2: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR3: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/*----------------------------------------------------------------------------------*/
/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/*----------------------------------------------------------------------------------*/
/* Get file size
 * Inputs: Secret file ptr
 * Output: Secret_file_size
 * Return values: size of secret file
 * Description: Get the secret file size using random access file functions
*/
uint get_file_size(FILE *fptr)
{
    fseek(fptr, 0, SEEK_END);
    uint size = ftell(fptr);
    rewind(fptr);
    return size;
}

/*----------------------------------------------------------------------------------*/
/* Check capacity
 * Inputs: File pointers
 * Output: Image_capacity and secret_file_size
 * Return values: e_success or e_failure
 * Description: Check whether image file is sufficient to encode secret file data
*/
Status check_capacity(EncodeInfo *encInfo)
{
    /* Get size of source bmp file */
    encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
    printf("INFO: Image size = %u\n", encInfo->image_capacity);

    /* Get size of secret file */
    encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);
    printf("INFO: File size = %lu\n", encInfo->size_secret_file);

    if (encInfo->image_capacity > (2 * 8 + 32 + 32 + 32 + encInfo->size_secret_file * 8))   /* Image capacity should be greater than all encoding data */
    {
        return e_success;
    }
    else
    {
        return e_failure;
    }
}

/*----------------------------------------------------------------------------------*/
/* Copy bmp image header
 * Inputs: source bmp file ptr and Stego image file ptr
 * Output: Copy header to stego image file
 * Return values: e_success or e_failure
 * Description: Header contains all data of bmp file, so header should be copy as it is in stego image file
*/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    unsigned char header[54];
    rewind(fptr_src_image);
    if (fread(header, 1, 54, fptr_src_image) != 54)
    {
        return e_failure;
    }
    fwrite(header, 1, 54, fptr_dest_image);
    return e_success;
}