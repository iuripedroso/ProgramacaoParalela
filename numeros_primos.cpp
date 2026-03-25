#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int main() {
    int N = 1000;

    vector<char> primo(N + 1);

    #pragma omp parallel for
    for (int i = 0; i <= N; i++) {
        primo[i] = 1;
    }

    primo[0] = primo[1] = 0;

    for (int i = 2; i * i <= N; i++) {
        if (primo[i]) {
            #pragma omp parallel for
            for (int j = i * i; j <= N; j += i) {
                primo[j] = 0;
            }
        }
    }

    cout << "Numeros primos de 1 ate 1000:\n";

    for (int i = 2; i <= N; i++) {
        if (primo[i]) {
            cout << i << " ";
        }
    }

    cout << endl;

    return 0;
}