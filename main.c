#include "encode.h"
#include "decode.h"
int main(int argc, char *argv[])
{
    EncodeInfo encInfo;
    DecodeInfo decInfo;

    int argc_count = argc;

    /* Check operation */
    int result = check_operation_type(argv);
    if (result == e_encode)
    {
        
        // Encoding
        /* Read and validate Encode args from argv */
        if (read_and_validate_encode_args(argv, &encInfo) == e_failure)
        {
            printf("The encode args validation failed.\n");
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
        /* Read and validate Decode args from agrv */
        if (read_and_validate_decode_args(argv, &decInfo) == e_failure)
        {
            printf("The decode args validation failed.\n");
            return 1;
        }
        if (do_decoding(&decInfo) == e_failure)
        {
            printf("Do decoding failed.\n");
            return 1;
        }

    }
    else
    {
        // Unsupported
    }
   

    

    return 0;
}
