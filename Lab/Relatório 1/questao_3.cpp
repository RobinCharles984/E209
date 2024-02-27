#include <iostream>

using namespace std;

void Cont(int x, unsigned int &par, unsigned int &impar, unsigned int &pos, unsigned int &neg){
    if(x%2 == 0)
        par++;
    else
        impar++;

    if(x>=0)
        pos++;
    else
        neg++;
}

int main(){
    //Variables
    unsigned int n;//Times for test
    signed int x;//Var for values
    unsigned int par = 0, 
    impar = 0, 
    pos = 0, 
    neg = 0;

    cin >> n;//Input

    //Counting
    for (int i = 0; i < n; i++)
    {
        for ( int j = 0; j < 5; j++)
        {
            cin >> x;
            Cont(x, par, impar, pos, neg);
        }
        //Outputs
        cout << "Quantidade de numeros pares: " << par << endl;
        cout << "Quantidade de numeros impares: " << impar << endl;
        cout << "Quantidade de numeros positivos: " << pos << endl;
        cout << "Quantidade de numeros negativos: " << neg << endl;

        //Reseting
        par = 0, 
        impar = 0, 
        pos = 0, 
        neg = 0;
    }
}
