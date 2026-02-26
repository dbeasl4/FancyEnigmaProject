#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <chrono>
#include "Enigma.cpp"
#include <windows.h>
#include <chrono>
#include <conio.h>


struct KeyData {
    char character;
    long long pressDown;
    long long holdDuration;
};

/**
 * Goal is to get a mapping of time to each character and allows user to edit any position within input.
 */
void setCursorPosition(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y }; // 0 is the row (y), x is the column
    SetConsoleCursorPosition(hConsole, pos);
}
std::vector<KeyData> handleUserInputWithTiming(int currentRow) {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(hInput, &mode);
    SetConsoleMode(hInput, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
    std::vector<KeyData> sessionData;
    int cursorBufferPos = 0;
    std::chrono::steady_clock::time_point lastKeyTime{};
    std::chrono::steady_clock::time_point pressStartTime{};
    bool firstKey = true;

    while (true) {

        INPUT_RECORD record;
        DWORD events;

        ReadConsoleInput(hInput, &record, 1, &events);

        if (record.EventType != KEY_EVENT)
            continue;

        KEY_EVENT_RECORD key = record.Event.KeyEvent;

        char c = key.uChar.AsciiChar;

        // --- HANDLE KEY DOWN ---
        if (key.bKeyDown) {

            auto currentTime = std::chrono::steady_clock::now();

            long long pressDelta = 0;

            if (!firstKey) {
                pressDelta = std::chrono::duration_cast<std::chrono::microseconds>(
                    currentTime - lastKeyTime
                ).count();
            } else {
                firstKey = false;
            }

            lastKeyTime = currentTime;
            pressStartTime = currentTime;
        // Handle special keys using VirtualKeyCode
        if (key.wVirtualKeyCode == VK_LEFT && cursorBufferPos > 0) {
            cursorBufferPos--;
            continue;
        }

        if (key.wVirtualKeyCode == VK_RIGHT && cursorBufferPos < sessionData.size()) {
            cursorBufferPos++;
            continue;
        }

        if (key.wVirtualKeyCode == VK_BACK) {
            if (cursorBufferPos > 0) {
                sessionData.erase(sessionData.begin() + cursorBufferPos - 1);
                cursorBufferPos--;
            }
            continue;
        }

        if (key.wVirtualKeyCode == VK_RETURN)
            break;

        if (c == 0)
            continue;

        KeyData newData{};
        newData.character = c;
        newData.pressDown = pressDelta;
        newData.holdDuration = 0;

        sessionData.insert(sessionData.begin() + cursorBufferPos, newData);
        cursorBufferPos++;
    }


    else {

        auto releaseTime = std::chrono::steady_clock::now();

        if (!sessionData.empty()) {
            long long hold = std::chrono::duration_cast<std::chrono::microseconds>(
                releaseTime - pressStartTime
            ).count();

            sessionData.back().holdDuration = hold;
        }
    }

    // Redraw
    setCursorPosition(0, currentRow);
    std::cout << std::string(100, ' ') << "\r";
    for (auto& k : sessionData) std::cout << k.character;
    setCursorPosition(cursorBufferPos, currentRow);
    }
    return sessionData;
}
int main() {
    std::cout << "--- TEST 1 ---" << std::endl;
    // Use row 2 so it doesn't overwrite the header
    std::vector<KeyData> test1 = handleUserInputWithTiming(2);

    std::cout << "\n[DEBUG] Captured " << test1.size() << " keys." << std::endl;

    if (test1.empty()) {
        std::cout << "Nothing was captured! Check your 'if' conditions." << std::endl;
    } else {
        for(auto& k : test1) {
            std::cout << "Char: " << k.character
          << " | PressDelta(us): " << k.pressDown
          << " | Hold(us): " << k.holdDuration
          << std::endl;
        }
    }
    Enigma machine;

    std::string encrypted;

    for (auto& k : test1) {

        long long chaos = k.pressDown + k.holdDuration;

        char enc = machine.encryptWithChaos(k.character, chaos);

        encrypted += enc;
    }

    std::cout << "\nEncrypted: " << encrypted << std::endl;
    return 0;
}
