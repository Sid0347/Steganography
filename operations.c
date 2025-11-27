#include "encode.h"
/* Check operation typy */
OperationType check_operation_type(char *argv[])
{
    /* Validate operation type. */
    if (argv[1] == "-e")
        return e_encode;
    else if(argv[1] == "-d")
        return e_decode;
    else
        e_unsupported;
}

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
    /* Check source file extention */
    if (strstr(argv[2], ".bmp") != NULL)
    {
        encInfo->src_image_fname = argv[2];
        printf("sorce file name %s", encInfo->src_image_fname);

    }
    else
        return e_failure;

    /* Check secret file extention */
    if (strstr(argv[3], ".txt") != NULL)
    {
        encInfo->secret_fname = argv[3];
        // strcpy(encInfo->extn_secret_file,h);
    }
}