#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
using namespace std;
int main() {
    auto Prime = [&](int x) {
        if (x < 2)
            return false;
        for (int d = 2; d * d <= x; d++)
        {
            if (x % d == 0)
                return false;
        }
        return true;
    };
    const int N = 10000;
    const int PROC = 10;
    int step = N / PROC;
    for (int i = 0; i < PROC; i++) {
        HANDLE hRead, hWrite;
        SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
        CreatePipe(&hRead, &hWrite, &sa, 0);
        int start = i * step + 1;
        int end = (i + 1) * step;
        string primes = "";
        for (int x = start; x <= end; x++) {
            if (Prime(x))
                primes += to_string(x) + " ";
        }
        DWORD written;
        WriteFile(hWrite, primes.c_str(), (DWORD)primes.size(), &written, NULL);
        CloseHandle(hWrite);
        char buffer[4096];
        DWORD readBytes;
        ReadFile(hRead, buffer, sizeof(buffer) - 1, &readBytes, NULL);
        buffer[readBytes] = '\0';
        cout << "Numerele prime din procesul " << i + 1 << " sunt: " << buffer << "\n";
        CloseHandle(hRead);
    }
    return 0;
}
