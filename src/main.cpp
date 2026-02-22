#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

// Цветной вывод в консоль
void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Поиск процесса по имени
DWORD GetProcessIdByName(const std::wstring& processName) {
    DWORD processId = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W processEntry;
        processEntry.dwSize = sizeof(processEntry);
        if (Process32FirstW(snapshot, &processEntry)) {
            do {
                if (_wcsicmp(processEntry.szExeFile, processName.c_str()) == 0) {
                    processId = processEntry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snapshot, &processEntry));
        }
        CloseHandle(snapshot);
    }
    return processId;
}

// Получение базового адреса модуля
uintptr_t GetModuleBaseAddress(DWORD processId, const std::wstring& moduleName) {
    uintptr_t baseAddress = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if (snapshot != INVALID_HANDLE_VALUE) {
        MODULEENTRY32W moduleEntry;
        moduleEntry.dwSize = sizeof(moduleEntry);
        if (Module32FirstW(snapshot, &moduleEntry)) {
            do {
                if (_wcsicmp(moduleEntry.szModule, moduleName.c_str()) == 0) {
                    baseAddress = reinterpret_cast<uintptr_t>(moduleEntry.modBaseAddr);
                    break;
                }
            } while (Module32NextW(snapshot, &moduleEntry));
        }
        CloseHandle(snapshot);
    }
    return baseAddress;
}

// Паттерн-сканирование (AOB scan) — демонстрационная версия
std::vector<uintptr_t> PatternScan(HANDLE hProcess, uintptr_t start, size_t size, const std::vector<byte>& pattern, const std::string& mask) {
    std::vector<uintptr_t> results;
    std::vector<byte> buffer(size);
    SIZE_T bytesRead;

    if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(start), buffer.data(), size, &bytesRead))
        return results;

    for (size_t i = 0; i <= bytesRead - pattern.size(); ++i) {
        bool found = true;
        for (size_t j = 0; j < pattern.size(); ++j) {
            if (mask[j] == 'x' && buffer[i + j] != pattern[j]) {
                found = false;
                break;
            }
        }
        if (found)
            results.push_back(start + i);
    }
    return results;
}

// Глобальные переменные для хоткеев
HHOOK keyboardHook;
bool godModeActive = false;
bool infiniteHealthActive = false;

// Обработчик клавиш (демо)
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* p = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
        if (wParam == WM_KEYDOWN) {
            switch (p->vkCode) {
            case VK_F1:
                godModeActive = !godModeActive;
                SetColor(10);
                std::cout << "[Trainer] God Mode " << (godModeActive ? "ACTIVATED" : "DEACTIVATED") << std::endl;
                SetColor(7);
                break;
            case VK_F2:
                infiniteHealthActive = !infiniteHealthActive;
                SetColor(10);
                std::cout << "[Trainer] Infinite Health " << (infiniteHealthActive ? "ACTIVATED" : "DEACTIVATED") << std::endl;
                SetColor(7);
                break;
            case VK_F3:
                SetColor(10);
                std::cout << "[Trainer] Speed Hack toggled (demo)" << std::endl;
                SetColor(7);
                break;
            }
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

// Загрузка конфигурации (демо)
void LoadConfig() {
    std::ifstream config("trainer.ini");
    if (config.is_open()) {
        std::string line;
        while (std::getline(config, line)) {
            // Просто выводим для вида
            std::cout << "[Config] " << line << std::endl;
        }
        config.close();
    }
}

int main() {
    SetColor(11);
    std::cout << R"(
    ████████╗██████╗  █████╗ ██╗███╗   ██╗███████╗██████╗ 
    ╚══██╔══╝██╔══██╗██╔══██╗██║████╗  ██║██╔════╝██╔══██╗
       ██║   ██████╔╝███████║██║██╔██╗ ██║█████╗  ██████╔╝
       ██║   ██╔══██╗██╔══██║██║██║╚██╗██║██╔══╝  ██╔══██╗
       ██║   ██║  ██║██║  ██║██║██║ ╚████║███████╗██║  ██║
       ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝╚══════╝╚═╝  ╚═╝
    )" << std::endl;
    SetColor(7);
    std::cout << "REANIMAL Trainer v2.1 (Educational Demo)" << std::endl;
    std::cout << "=========================================" << std::endl;
    std::cout << "Loading configuration..." << std::endl;
    LoadConfig();

    std::wstring targetProcess = L"REANIMAL.exe";
    std::wstring targetModule = L"REANIMAL.exe";

    std::cout << "Scanning for REANIMAL process..." << std::endl;
    DWORD pid = GetProcessIdByName(targetProcess);
    if (pid == 0) {
        SetColor(12);
        std::cout << "[-] REANIMAL not found. Please launch the game first." << std::endl;
        SetColor(7);
        system("pause");
        return 1;
    }
    SetColor(10);
    std::cout << "[+] Found REANIMAL (PID: " << pid << ")" << std::endl;
    SetColor(7);

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hProcess) {
        SetColor(12);
        std::cout << "[-] Failed to open process. Run as Administrator." << std::endl;
        SetColor(7);
        system("pause");
        return 1;
    }

    uintptr_t baseAddress = GetModuleBaseAddress(pid, targetModule);
    if (baseAddress == 0) {
        SetColor(12);
        std::cout << "[-] Could not get module base." << std::endl;
        SetColor(7);
        CloseHandle(hProcess);
        system("pause");
        return 1;
    }
    SetColor(10);
    std::cout << "[+] Module base: 0x" << std::hex << baseAddress << std::endl;
    SetColor(7);

    // Демо-паттерн для God Mode (маска и паттерн не настоящие)
    std::vector<byte> pattern = { 0x48, 0x8B, 0x45, 0x08, 0x48, 0x89, 0x45, 0xF8 };
    std::string mask = "xxxxxxxx";
    std::cout << "Scanning for God Mode pattern..." << std::endl;
    auto results = PatternScan(hProcess, baseAddress, 0x1000000, pattern, mask);
    if (!results.empty()) {
        SetColor(10);
        std::cout << "[+] Found pattern at: 0x" << std::hex << results[0] << std::endl;
    }
    else {
        SetColor(12);
        std::cout << "[-] Pattern not found. Game version might be different." << std::endl;
    }
    SetColor(7);

    // Установка глобального хука клавиатуры
    keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
    if (!keyboardHook) {
        std::cout << "Warning: Could not install keyboard hook. Hotkeys disabled." << std::endl;
    }
    else {
        std::cout << "Hotkeys active: F1 (God Mode), F2 (Infinite Health), F3 (Speed Hack)" << std::endl;
    }

    std::cout << "\nTrainer is running. Press 'Q' to quit." << std::endl;

    // Основной цикл сообщений (для хуков)
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (GetAsyncKeyState('Q') & 0x8000) break;
    }

    if (keyboardHook) UnhookWindowsHookEx(keyboardHook);
    CloseHandle(hProcess);
    SetColor(10);
    std::cout << "Trainer shut down. Goodbye!" << std::endl;
    SetColor(7);
    return 0;
}