#include <Windows.h>
#include <iostream>
#pragma comment(lib, "User32.lib")
#define VERSION_MAJOR 1
#define VERSION_MINOR 0

void send(INPUT input)
{
    UINT uSent = SendInput(1, &input, sizeof(INPUT));
    if (uSent != 1)
    {
        std::cout << "[ERROR] " << "SendInput failed: 0x" << HRESULT_FROM_WIN32(GetLastError()) << std::endl;
    }
}

int main()
{
    std::cout << "KBRemap " << VERSION_MAJOR << "." << VERSION_MINOR << std::endl;

    SHORT remap_k_i, remap_k_o, remap_k_e;
    INPUT input, input_up;
    bool k_down = false;
    bool end = false;

    /* Codes Virtual-Key
     * https://docs.microsoft.com/fr-fr/windows/win32/inputdev/virtual-key-codes
     * SendInput
     * https://docs.microsoft.com/fr-fr/windows/win32/api/winuser/nf-winuser-sendinput
     * SendMessage
     * https://docs.microsoft.com/fr-fr/windows/win32/api/winuser/nf-winuser-sendmessage
     */
    // change this to change the remap setup
    remap_k_i = VK_MENU;  // INPUT  (ALT)
    remap_k_o = VK_SHIFT; // OUTPUT (SHIFT)
    remap_k_e = VK_INSERT;// EXIT   (INSERT)
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = remap_k_o;
    input.ki.dwFlags = 0;
    input_up.type = INPUT_KEYBOARD;
    input_up.ki.wVk = remap_k_o;
    input_up.ki.dwFlags = 0;
    input_up.ki.dwFlags = KEYEVENTF_KEYUP;

    while(!end)
    {
        // remap
        if(GetKeyState(remap_k_i) & 0x8000)
        {
            if(!k_down)
                send(input);
            k_down = true;
        }
        else if(k_down)
        {
            k_down = false;
            send(input_up);
        }
        // exit
        if(GetKeyState(remap_k_e) & 0x8000)
        {
            end = true;
        }
        // avoid 100% cpu usage
        Sleep(10);
    }

    std::cout << "see you soon :)" << std::endl;

    return 0;
}
