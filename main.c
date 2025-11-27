// #include <stdio.h>
#include "encode.h"
// #include "types.h"

int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    uint img_size;

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
        read_and_validate_encode_args(argv, &encInfo);
    }
    else if (result == e_decode)
    {
        //Decode
    }
    else
    {
        //Unsupported
    }

    // Test open_files
    if (open_files(&encInfo) == e_failure)
    {
    	printf("ERROR: %s function failed\n", "open_files" );
    	return 1;
    }
    else
    {
    	printf("SUCCESS: %s function completed\n", "open_files" );
    }

    // Test get_image_size_for_bmp
    img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    printf("INFO: Image size = %u\n", img_size);

    
    return 0;
}
