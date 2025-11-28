#include "encode.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;

    encInfo.argc = argc;
    // Fill with sample filenames
    // encInfo.src_image_fname = "beautiful.bmp";
    // encInfo.secret_fname = "secret.txt";
    // encInfo.stego_image_fname = "stego_img.bmp";

    /* Check operation */
    int result = check_operation_type(argv);
    if (result == e_encode)
    {
        
        // Encoding
        /* Read and validate Encode args from argv */
        if (read_and_validate_encode_args(argv, &encInfo) == e_failure)
        {
            printf("The args validation failed.\n");
            return 1;
        }
        if (do_encoding(&encInfo) == e_failure)
        {
            printf("Do encoding failed.\n");
            return 1;
        }
    }
    else if (result == e_decode)
    {
        // Decode
    }
    else
    {
        // Unsupported
    }
   

    

    return 0;
}
