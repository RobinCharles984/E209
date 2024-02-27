#include <iostream>

using namespace std;

int main(){
    //Variables
    unsigned int n;

    cin >> n;//Input

    if(n < 50 || n > 0)
        for (int i = n-1; i > 0; i--)
        {
            cout << "Resto da divisao de " << n << " por " << i << ": " << n%i << endl;
        }
    else{
        cout << "Error" << endl;
    }

    return 0;
}