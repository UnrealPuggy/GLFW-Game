#include <iostream>
#include <windows.h>
#include "idk.txt.h"
#include "pug.png.h"
int main()
{
    // int banana = 10;
    // char message[src_idk_txt_len + 1];
    // memcpy(message, src_idk_txt, src_idk_txt_len);
    // message[src_idk_txt_len] = '\0';
    MessageBoxA(0, (char *)src_pug_png, "idk.txt", 1);

    return 0;
}
