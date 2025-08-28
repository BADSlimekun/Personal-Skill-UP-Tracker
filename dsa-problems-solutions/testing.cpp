#include <iostream>
using namespace std;


int main() {
    int a = 10;
    int b = a;
    int* c = new int(a);
    int& d = a;
    int* e = &a;

    cout << a << " " << b << " " << d << " " << e << c << '\n';

    // d = 15;
    *e = 20;

    cout << a << " " << b << " " << d << " " << e << c <<'\n';
}