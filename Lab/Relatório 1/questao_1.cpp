#include <iostream>
#include <iomanip>

using namespace std;

int main(){
    //Variables
    unsigned int oranges;

    cin >> oranges;//Input

    cout << fixed << setprecision(2);//Output precision
    if(oranges < 12)
    {
        cout << "Preco da unidade R$0,80" << endl;
        cout << "Preco total: R$" << oranges * 0.80f << endl;
    }   else
    {
        cout << "Preco da unidade R$0,65" << endl;
        cout << "Preco total: R$" << oranges * 0.65f << endl;
    }

}