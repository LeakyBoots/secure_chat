#include <stdio.h>
#include <string.h>

static void print_unsigned_char_array(unsigned char* input)
{
    size_t len = strlen(input);
    // printf("Length: %ld\n",len);
       for (int i = 0; i<len; i++)
    {
        printf("%02x",input[i]);
    }
    printf("\n");
}

// int main()
// {
//     // unsigned char *arr = "aab";

//     unsigned char arr[32];
//     for (size_t i = 0; i < 32; i++) 
//     {
//         // printf("i: %d", i);	
//         arr[i] = '1';
//     }
//     print_unsigned_char_array(arr);
//     return 0;
// }