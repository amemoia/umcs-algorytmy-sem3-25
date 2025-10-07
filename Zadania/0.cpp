#include <iostream>
#include <vector>
using namespace std;

int main() {
    short count;
    std::cin >> count;
    for (short i = 0; i < count; i++)
    {
        long n, m;
        std::cin >> n >> m;
        std::cout << n+m << "\n";
    }
    return 0;
}