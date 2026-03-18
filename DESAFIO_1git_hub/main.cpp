#include <iostream>
using namespace std;
#include <cstdlib>
#include <ctime>

// PROTOTIPOS
void imprimirTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas);
void inicializarTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas);
void crearMatriz(unsigned char** ptr, unsigned int filas, unsigned int columnas);
void ingresarFicha(unsigned char** ptr, unsigned char piezas[5][4],unsigned int columnas,unsigned int columnas_bits);
int main()
{
    unsigned int filas,columnas,columnas_bits;// filas
    unsigned char** ptr;
    cout << "numero del tamaño de la lista: " << endl;
    cin >> filas;
    cout << "tamaño de las columnas" << endl;
    cin >> columnas;
    columnas_bits = columnas/8;
    ptr = new unsigned char*[filas];
    unsigned char piezas[5][4] = // creo una matriz con todas las fichas a nivel de columnas_bits
        {
            {0b11110000,0,0,0},      // I
            {0b011100000,0b1000000,0,0}, // T
            {0b11000000,0b11000000,0,0}, // Z
            {0b11100000,0b10000000,0,0}, // L
            {0b11000000,0b11000000,0,0}  // O
        };

    crearMatriz(ptr,filas,columnas_bits);
    inicializarTablero(ptr,filas,columnas_bits);
    //    imprimirTablero(ptr,filas,columnas_bits);
    ingresarFicha(ptr,piezas,columnas,columnas_bits);
    imprimirTablero(ptr,filas,columnas_bits);
    // liberar memoria
    for(unsigned int i = 0; i < filas; i++)
        delete[] ptr[i];

    delete[] ptr;


    return 0;
}


void crearMatriz(unsigned char** ptr, unsigned int filas, unsigned int columnas){

    for(unsigned int i = 0; i < filas ; i++){

        ptr[i] = new unsigned char[columnas];

    }
}

void inicializarTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas){

    for(unsigned int i = 0; i < filas; i++){
        for(unsigned int j = 0; j < columnas; j++){

            ptr[i][j] = 0;

        }
    }
}

void imprimirTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas){

    for(unsigned int i = 0; i < filas; i++){
        cout << "|";
        for(unsigned int j = 0; j < columnas; j++){


            for(int bit = 7; bit >=0; bit--){ // es un int normal porque en la ultima iteracion queda 0-1 y ahi se vuelve un overflow y nunca acabaria, un ciclo infinito

                if((ptr[i][j] >> bit) & 1){ // ptr[i][j] lo que significa es tomar el char de esa posicion y tomar los valores bit a bit

                    cout << " [ ] ";
                }
                else{

                    cout << "  .  ";
                }

            }
        }
        cout << "|" << endl;
    }
}

void ingresarFicha(unsigned char** ptr, unsigned char piezas[5][4],unsigned int columnas, unsigned int columnas_bits)
{
    unsigned int ficha = 1, mitad = (columnas/2) - 2, contador_mitad = 0; // cuadrado

    for(unsigned int i = 0; i < 4; i++)
    {
        if(piezas[ficha][i] != 0)
        {
            ptr[i][0] |= piezas[ficha][i];
        }
    }
    //ingresar en el medio
    for(unsigned int desplazamiento = 0; desplazamiento < mitad; desplazamiento++)//cuantas veces mover la ficha
    {
        for(unsigned int i = 0; i < 4; i++) // recorrer filas de la ficha
        {
            for(int j = columnas_bits - 1; j >= 0; j--)
            {
                unsigned char carry = (ptr[i][j] & 1) << 7;

                ptr[i][j] >>= 1;

                if(j + 1 < columnas_bits){
                    ptr[i][j+1] |= carry;
                }
            }
        }
    }

}

