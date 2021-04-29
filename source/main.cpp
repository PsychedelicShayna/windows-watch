#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstdio>
#include <memory>

#include <thread>
#include <chrono>
#include <string>
#include <vector>

#ifndef WIN32_MEAN_AND_LEAN
#define WIN32_MEAN_AND_LEAN
#include <Windows.h>
#endif

#define ThreadSleep(sleep_ms) std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms))

int ExecuteCommand(const char* command, std::string& command_output, size_t buffer_size = 512) {
    std::unique_ptr<char> output_buffer(new char[buffer_size + 1]);

    FILE* command_pipe = _popen(command, "r");

    if (!command_pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (!feof(command_pipe)) {
        if (fgets(output_buffer.get(), buffer_size, command_pipe) != nullptr) {
            output_buffer.get()[buffer_size] = '\0';
            command_output += output_buffer.get();
        }
    }

    return _pclose(command_pipe);
}

void ClearConsoleBuffer(char fill_character = ' ') {
    static HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(console_handle, &csbi);

    unsigned long written;

    FillConsoleOutputCharacter(console_handle, fill_character, csbi.dwSize.X * csbi.dwSize.Y, {0, 0}, &written);
    FillConsoleOutputAttribute(console_handle, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, {0, 0}, &written);
    SetConsoleCursorPosition(console_handle, {0, 0});
}

constexpr const char* HELP_TEXT =
    "--help (-h)      |     Prints This Help Page\n"
    "                 |\n"
    "--interval (-i)  |     Sets the command repetition interval in milliseconds. Defaults to 1000ms.\n"
    "                 |\n"
    "--verbose (-v)   |     Toggles the printing of additional information related to the command's execution.\n"
    "                 |     Currently only displays the return code after every execution.\n"
    "                 |\n"
    "--command (-c)   |     The most important argument. Everything after this will be treated as the command that\n"
    "                 |     should be watched. This should be placed at the very end of all other arguments.\n"
    "                 |     In some some cases (e.g. pipes) encapsulating the command in quotes may be necessary.\n"
    "\n";

int main(int argc, char* argv[]) {
    uint32_t sleep_interval_ms = 1000;
    std::string command;

    bool verbose = false;

    if(argc > 1) {
        for(int i=0; i<argc; ++i) {
            const char* next_argument = (i + 1) < argc ? argv[i + 1] : nullptr;
            const char* argument = argv[i];

            // Arguments that don't rely on subsequent parameters.
            if(!_stricmp(argument, "--help") || !_stricmp(argument, "-h")) {
                std::cout << HELP_TEXT << std::endl;
                return 0;
            }

            else if(!_stricmp(argument, "--verbose") || !_stricmp(argument, "-v")) {
                verbose = true;
            }

            // Arguments that rely on subsequent parameters.
            if(next_argument != nullptr) {
                if(!_stricmp(argument, "--interval") || !_stricmp(argument, "-i")) {
                    if(std::all_of(next_argument, next_argument + (strlen(next_argument) - 1), isdigit)) {
                        sleep_interval_ms = static_cast<uint32_t>(atoi(next_argument));
                    } else {
                        std::cout << "Parameter supplied for --interval (-i) contains non-digit characters. Defaulting to 1000ms." << std::endl;
                    }
                }
                
                else if(!_stricmp(argument, "--command") || !_stricmp(argument, "-c")) {
                    for(int c = (i + 1); c < argc; ++c) {
                        command += argv[c];

                        if((c + 1) < argc) {
                            command += ' ';
                        }
                    }

                    break;
                }
            }

        }
    }

    if(!command.size()) {
        std::cerr << "No command has been supplied. Read the help page via --help (-h) for command usage." << std::endl;
        return 1;
    }

    if(verbose) {
        std::cout << "Running Command >> " << command << std::endl;
        std::cout << "With a " << sleep_interval_ms << " millisecond interval after 500ms.." << std::endl;
        ThreadSleep(500);
    }

    ClearConsoleBuffer();

    int last_return_code = 0;

    for(;true;ThreadSleep(sleep_interval_ms)) {
        std::string command_output;

        try {
            last_return_code = ExecuteCommand(command.c_str(), command_output, 5);
        } catch(const std::runtime_error& exception) {
            std::cerr << "Encountered a runtime exception when attempting to execute the command." << std::endl;
            std::cerr << "Exception message:" << exception.what() << std::endl;
            return 1;
        }

        ClearConsoleBuffer();
        std::cout << command_output << std::endl;

        if(verbose) {
            std::cout << std::endl << "(Watch (Return Code:" << last_return_code << "))" << std::endl;
        }
    }

    return last_return_code;
}
