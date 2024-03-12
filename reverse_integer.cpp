#include <iostream>
#include <climits>

using namespace std;

// int pow(int n) {
//     int x = 1;
//     if (n == 0) return 1;
//     for (size_t i = 0; i < n; i++) {
//         x *= 10;
//     }
//     return x;
// }

int main() {
    int x = 321;
    // cin >> x;
    int new_one = 0;
    for (size_t i = 0; x != 0; i++) {
        if (INT_MAX)
        new_one *= 10;
        new_one += (x % 10);
        cout << new_one << endl;
        x /= 10;
    }
    cout << new_one;
}
