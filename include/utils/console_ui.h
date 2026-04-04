#pragma once

#include <cstdlib>
#include <iostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

namespace consoleui
{

    enum class Color
    {
        Default,
        Red,
        Green,
        Yellow,
        Blue,
        Cyan,
        Magenta,
        White
    };

    inline std::string colorCode(Color color)
    {
        switch (color)
        {
        case Color::Red:
            return "\033[31m";
        case Color::Green:
            return "\033[32m";
        case Color::Yellow:
            return "\033[33m";
        case Color::Blue:
            return "\033[34m";
        case Color::Magenta:
            return "\033[35m";
        case Color::Cyan:
            return "\033[36m";
        case Color::White:
            return "\033[37m";
        default:
            return "\033[0m";
        }
    }

    inline void initializeConsole()
    {
#ifdef _WIN32
        static bool initialized = false;
        if (initialized)
            return;

        HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (outputHandle == INVALID_HANDLE_VALUE)
            return;

        DWORD mode = 0;
        if (GetConsoleMode(outputHandle, &mode))
            SetConsoleMode(outputHandle, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

        initialized = true;
#endif
    }

    inline std::string colorText(const std::string &text, Color color, bool bold = false)
    {
        initializeConsole();

        std::string prefix;
        if (bold)
            prefix += "\033[1m";
        prefix += colorCode(color);
        return prefix + text + "\033[0m";
    }

    inline void clearScreen()
    {
#ifdef _WIN32
        initializeConsole();

        HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (outputHandle == INVALID_HANDLE_VALUE)
            return;

        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        if (!GetConsoleScreenBufferInfo(outputHandle, &bufferInfo))
            return;

        const DWORD cellCount = static_cast<DWORD>(bufferInfo.dwSize.X) * static_cast<DWORD>(bufferInfo.dwSize.Y);
        const COORD home = {0, 0};
        DWORD written = 0;

        FillConsoleOutputCharacterA(outputHandle, ' ', cellCount, home, &written);
        FillConsoleOutputAttribute(outputHandle, bufferInfo.wAttributes, cellCount, home, &written);
        SetConsoleCursorPosition(outputHandle, home);
#else
        std::cout << "\033[2J\033[H";
#endif
    }

    inline void printLine(char ch = '=', int width = 40)
    {
        for (int i = 0; i < width; ++i)
            std::cout << ch;
        std::cout << '\n';
    }

    inline void printTitleBox(const std::string &title)
    {
        printLine('=');
        std::cout << colorText(title, Color::Cyan, true) << '\n';
        printLine('=');
    }

    inline void waitForEnter(const std::string &message = "Tekan Enter untuk melanjutkan...")
    {
        std::cout << message;
        std::cout.flush();
        std::string dummy;
        std::getline(std::cin, dummy);
    }

} // namespace consoleui
