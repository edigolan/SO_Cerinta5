#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
using namespace std;
int main() {
    auto Prime = [&](int x) {
        if (x < 2)
            return false;
        for (int d = 2; d * d <= x; d++)
            if (x % d == 0)
                return false;
        return true;
    };
    const int N = 10000;
    const int PROC = 10;
    int step = N / PROC;
    int pipes[PROC][2];
    for (int i = 0; i < PROC; i++) {
        pipe(pipes[i]);
        pid_t pid = fork();
        if (pid == 0) {
            close(pipes[i][0]); 
            int start = i * step + 1;
            int end = (i + 1) * step;
            string primes = "";
            for (int x = start; x <= end; x++)
                if (Prime(x))
                    primes += to_string(x) + " ";
            write(pipes[i][1], primes.c_str(), primes.size());
            close(pipes[i][1]);
            return 0; 
        }
        else 
            close(pipes[i][1]); 
    }
    char buffer[4096];
    for (int i = 0; i < PROC; i++) {
        int n = read(pipes[i][0], buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            cout << "Numerele prime din procesul " << i + 1 << " sunt: " << buffer << "\n";
        }
        close(pipes[i][0]);
    }
    for (int i = 0; i < PROC; i++)
        wait(nullptr);
    return 0;
}
