#include <iostream>
using namespace std;
#include <cstdlib>
#include <ctime>

// PROTOTIPOS
void imprimirTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas, unsigned char** tablerofijo);
void inicializarTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas);
void crearMatriz(unsigned char** ptr, unsigned int filas, unsigned int columnas);
void ingresarFicha(unsigned char** ptr, unsigned char piezas[5][4],unsigned int columnas, unsigned int columnas_bits,unsigned int &filaficha,unsigned int &dimensiony,unsigned char** tablerofijo,bool &juegoActivo,int &posicionx,unsigned int &dimensionx,unsigned char piezaActiva[4]);
void bajar(unsigned char** ptr,unsigned char** tablerofijo,unsigned int &filaFicha,unsigned int columnas_bits,unsigned int filas, unsigned char piezas[5][4],unsigned int columnas,unsigned int &dimensiony,bool &juegoActivo,int &posicionx,unsigned int &dimensionx,unsigned char piezaActiva[4]);
void MoverDerecha(unsigned char** ptr,unsigned int filaFicha,unsigned int columnas_bits,unsigned int filas,unsigned int dimensiony,int &posicionx,unsigned int dimensionx);
void MoverIzquierda(unsigned char** ptr,unsigned int filaFicha,unsigned int columnas_bits,unsigned int dimensiony,int &posicionx);
bool hayColisionAbajo(unsigned char** ptr,unsigned char** tablerofijo,unsigned int filaFicha,unsigned int columnas_bits,unsigned int filas, unsigned int dimensiony);
void borrarFilas(unsigned char** tablerofijo, unsigned int filas, unsigned int columnas_bits);
void rotarDerecha(unsigned char piezaActual[4], unsigned int &dimensionx, unsigned int &dimensiony);
bool esPosibleRotar(unsigned char piezaRotada[4], unsigned int dimX, unsigned int dimY, unsigned int filaFicha, int posicionx, unsigned char** tablerofijo, unsigned int filas, unsigned int columnas_bits);


int main()
{
    unsigned int filas,columnas,columnas_bits,dimensiony = 0,dimensionx=0,filaFicha = 0;// filas
    int posicionx= 0;
    unsigned char** ptr;
    unsigned char** tablerofijo;
    bool juegoActivo = true;
    cout << "numero del tamaño de las filas: " << endl;
    cin >> filas;
    cout << "tamaño de las columnas" << endl;
    cin >> columnas;
    columnas_bits = columnas/8;
    ptr = new unsigned char*[filas];
    tablerofijo = new unsigned char*[filas];
    //piezas
    unsigned char piezas[5][4] = // creo una matriz con todas las fichas a nivel de columnas_bits
        {
            {0b10000000,0b10000000,0b10000000,0b10000000},      // I
            {0b11100000,0b01000000,0,0}, // T
            {0b11000000,0b01100000,0,0}, // Z
            {0b10000000,0b10000000,0b11000000,0}, // L
            {0b11000000,0b11000000,0,0}  // O
        };

    unsigned char piezaActiva[4];

    //inicializo la aleatoridad
    srand(time(0));
    // Aqui inicializamos y creamos la matriz que maneja la ficha
    crearMatriz(ptr,filas,columnas_bits);
    inicializarTablero(ptr,filas,columnas_bits);

    // creo el tablero donde se guarden las fichas
    crearMatriz(tablerofijo,filas,columnas_bits);
    inicializarTablero(tablerofijo,filas,columnas_bits);
    // Agregamos la primer ficha
    ingresarFicha(ptr,piezas,columnas,columnas_bits,filaFicha,dimensiony,tablerofijo,juegoActivo,posicionx,dimensionx,piezaActiva);

    cout << "\n=======================================================\n";
    cout << "  _____     _       _      __  __           _   _ \n";
    cout << " |_   _|___| |_ _ __(_)___|  \\/  | __ _ ___| |_ ___ _ __ \n";
    cout << "   | |/ _ \\ __| '__| / __| |\\/| |/ _` / __| __/ _ \\ '__|\n";
    cout << "   | |  __/ |_| |  | \\__ \\ |  | | (_| \\__ \\ ||  __/ |   \n";
    cout << "   |_|\\___|\\__|_|  |_|___/_|  |_|\\__,_|___/\\__\\___|_|   \n";
    cout << "=======================================================\n";
    while(juegoActivo){
        system("cls"); // limpia la pantalla
        // Mostrar el tablero en cada iteración
        imprimirTablero(ptr, filas, columnas_bits,tablerofijo);
        unsigned char opcion;
        cout << "ingrese la opcion: ";
        cin >> opcion;

        switch(opcion) {
        case 'S':
        case 's':
            bajar(ptr,tablerofijo,filaFicha,columnas_bits,filas,piezas,columnas,dimensiony,juegoActivo,posicionx,dimensionx,piezaActiva);
            break;
        case 'Q':
        case 'q':
            juegoActivo = false;
            cout << "Gracias por jugar!!" << endl;
            break;
        case 'D':
        case 'd':
            MoverDerecha(ptr,filaFicha,columnas_bits,filas,dimensiony,posicionx,dimensionx);
            break;
        case 'A':
        case 'a':
            MoverIzquierda(ptr,filaFicha,columnas_bits,dimensiony,posicionx);
            break;
        case 'W':
        case 'w': {
            // Creamos copias temporales para no perder la forma original si la rotación falla
            unsigned char copiaPieza[4];
            for(int i=0; i<4; i++) copiaPieza[i] = piezaActiva[i];
            unsigned int copiaDimX = dimensionx;
            unsigned int copiaDimY = dimensiony;

            // 1. Rotamos las copias primero (Simulación)
            rotarDerecha(copiaPieza, copiaDimX, copiaDimY);

            // 2. Validamos si esa nueva forma cabe en el tablero
            if (esPosibleRotar(copiaPieza, copiaDimX, copiaDimY, filaFicha, posicionx, tablerofijo, filas, columnas_bits)) {

                // Si es posible, borramos la vieja del tablero de movimiento (ptr)
                for (unsigned int i = 0; i < dimensiony; i++) {
                    ptr[filaFicha + i][posicionx / 8] = 0;
                }

                // Aplicamos los cambios definitivos
                for(int i=0; i<4; i++) piezaActiva[i] = copiaPieza[i];
                dimensionx = copiaDimX;
                dimensiony = copiaDimY;

                // Redibujamos la nueva pieza rotada en ptr
                for (unsigned int i = 0; i < dimensiony; i++) {
                    unsigned char fila = piezaActiva[i];
                    for (int d = 0; d < posicionx; d++) fila >>= 1;
                    ptr[filaFicha + i][posicionx / 8] = fila;
                }
            }
            break;
        }
        default:
            cout << "Opcion no valida" << endl;
            break;

        }
    }
    cout << "\n GAME OVER !!!\n";
    // liberar memoria
    for(unsigned int i = 0; i < filas; i++){
        delete[] ptr[i];
        delete[] tablerofijo[i];
    }
    delete[] ptr;
    delete[] tablerofijo;


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

void imprimirTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas, unsigned char** tablerofijo){

    cout << "\n=======================================================\n";
    cout << "  _____     _       _      __  __           _   _ \n";
    cout << " |_   _|___| |_ _ __(_)___|  \\/  | __ _ ___| |_ ___ _ __ \n";
    cout << "   | |/ _ \\ __| '__| / __| |\\/| |/ _` / __| __/ _ \\ '__|\n";
    cout << "   | |  __/ |_| |  | \\__ \\ |  | | (_| \\__ \\ ||  __/ |   \n";
    cout << "   |_|\\___|\\__|_|  |_|___/_|  |_|\\__,_|___/\\__\\___|_|   \n";
    cout << "=======================================================\n";

    for(unsigned int i = 0; i < filas; i++){
        cout << "|";
        for(unsigned int j = 0; j < columnas; j++){

            unsigned char valor = ptr[i][j] | tablerofijo[i][j]; // para superponer ambos tableros el ptr y tablero fijo

            for(int bit = 7; bit >=0; bit--){ // es un int normal porque en la ultima iteracion queda 0-1 y ahi se vuelve un overflow y nunca acabaria, un ciclo infinito


                if((valor >> bit) & 1){ // ptr[i][j] lo que significa es tomar el char de esa posicion y tomar los valores bit a bit

                    cout << " [ ] ";
                }
                else{

                    cout << "  .  ";
                }

            }
        }
        cout << "|" << endl;
    }
    cout << "Accion: [A]Izq  [D]Der [S]Bajar [W]Rotar [Q]Salir: ";

}



void ingresarFicha(unsigned char** ptr, unsigned char piezas[5][4],unsigned int columnas, unsigned int columnas_bits,unsigned int &filaficha,unsigned int &dimensiony,unsigned char** tablerofijo,bool &juegoActivo,int &posicionx,unsigned int &dimensionx,unsigned char piezaActiva[4])
{
    unsigned int ficha,mitad;
    //reinicio la posicion en x
    posicionx = 0;
    //ficha ramdom
    ficha = rand() % 5;

    //dimensiones de la ficha
    // I
    if(ficha == 0){
        dimensionx = 1;
        dimensiony = 4;
    }
    //T
    if(ficha == 1){
        dimensionx = 3;
        dimensiony = 2;

    } // esto verifica la altura de la ficha y en que fila esta la parte mas baja de la ficha
    //z
    if(ficha == 2){
        dimensionx = 3;
        dimensiony = 2;

        //L
    }
    if(ficha == 3){
        dimensionx = 2;
        dimensiony = 3;
        //O
    }
    if(ficha == 4){
        dimensionx = 2;
        dimensiony = 2;

    }



    for(unsigned int i= 0; i < 4; i++){

        piezaActiva[i] = piezas[ficha][i];
    }

    mitad = (columnas/2) - (dimensionx/2);
    posicionx = mitad;
    for(unsigned int i = 0; i < dimensiony; i++)
    {
        if(piezas[ficha][i] != 0)
        {
            ptr[i][0] |= piezas[ficha][i];
        }
    }
    //ingresar en el medio
    for(unsigned int desplazamiento = 0; desplazamiento < mitad; desplazamiento++)//cuantas veces mover la ficha
    {
        for(unsigned int i = 0; i < dimensiony; i++) // recorrer filas de la ficha
        {
            for(int j = columnas_bits - 1; j >= 0; j--)
            {
                unsigned char llevar_bit = (ptr[i][j] & 1) << 7;

                ptr[i][j] >>= 1;

                if(j + 1 < columnas_bits){
                    ptr[i][j+1] |= llevar_bit;
                }
            }
        }
        //posicionx++;
    }

    for(unsigned int i = 0; i < dimensiony; i++)
    {
        if((ptr[i][0] & tablerofijo[i][0]) != 0)
        {
            juegoActivo = false;
            return; // game over

        }
    }

}

void bajar(unsigned char** ptr,unsigned char** tablerofijo,unsigned int &filaFicha,unsigned int columnas_bits,unsigned int filas, unsigned char piezas[5][4],unsigned int columnas,unsigned int &dimensiony,bool &juegoActivo,int &posicionx,unsigned int &dimensionx,unsigned char piezaActiva[4])
{


    if(hayColisionAbajo(ptr, tablerofijo, filaFicha, columnas_bits, filas,dimensiony))
    {
        // fijar ficha en tablero fijo
        for(unsigned int i = 0; i < dimensiony; i++)
        {
            for(unsigned int j = 0; j < columnas_bits; j++)
            {
                tablerofijo[filaFicha + i][j] |= ptr[filaFicha + i][j];
                ptr[filaFicha + i][j] = 0;
            }
        }
        //verifico que no este la fila completa
        borrarFilas(tablerofijo, filas, columnas_bits);
        // nueva ficha
        ingresarFicha(ptr,piezas,columnas,columnas_bits,filaFicha,dimensiony,tablerofijo,juegoActivo,posicionx,dimensionx,piezaActiva);
        filaFicha = 0;//reinicio la posicion inicial


        return; // hayo una colision entonces termina la funcion
    }


    // mover ficha hacia abajo
    for(int i = (dimensiony - 1); i >= 0; i--)
    {
        for(unsigned int j = 0; j < columnas_bits; j++)
        {
            ptr[filaFicha + i + 1][j] = ptr[filaFicha + i][j];
            ptr[filaFicha + i][j] = 0;
        }
    }

    filaFicha++;
}

void MoverDerecha(unsigned char** ptr,unsigned int filaFicha,unsigned int columnas_bits,unsigned int filas,unsigned int dimensiony,int &posicionx,unsigned int dimensionx){

    unsigned int columnas = columnas_bits * 8,posicionxfinal;

    posicionxfinal = posicionx + dimensionx;

    if((posicionxfinal + 1)<= columnas){
        //aca solo hace una iteracion
        for(unsigned int i = 0; i < dimensiony; i++) // recorrer filas de la ficha
        {
            for(int j = columnas_bits - 1; j >= 0; j--)
            {
                //necesito condiciones para que no haga overflow
                unsigned char llevar_bit = (ptr[i + filaFicha][j] & 1) << 7;

                ptr[i+filaFicha][j] >>= 1;

                if(j + 1 < columnas_bits){
                    ptr[i + filaFicha][j+1] |= llevar_bit ;

                }

            }
        }
        posicionx++;
    }
}

void MoverIzquierda(unsigned char** ptr,unsigned int filaFicha,unsigned int columnas_bits,unsigned int dimensiony,int &posicionx){
    if(posicionx - 1 >= 0){
        //aca solo hace una iteracion
        for(unsigned int i = 0; i < dimensiony; i++) // recorrer filas de la ficha
        {
            for(int j = 0; j < columnas_bits; j++)
            {

                unsigned char llevar_bit = (ptr[i + filaFicha][j] & 128) >> 7;

                ptr[i+filaFicha][j] <<= 1;

                if(j > 0){
                    ptr[i + filaFicha][j-1] |= llevar_bit;
                }

            }
        }
        posicionx--;
    }
}

bool hayColisionAbajo(unsigned char** ptr,unsigned char** tablerofijo,unsigned int filaFicha,unsigned int columnas_bits,unsigned int filas, unsigned int dimensiony)
{
    for(unsigned int i = 0; i < dimensiony ; i++)
    {
        if(filaFicha + i + 1 >= filas)
            return true;

        for(unsigned int j = 0; j < columnas_bits; j++)
        {
            unsigned char ficha = ptr[filaFicha + i][j]; // ficha
            unsigned char abajo = tablerofijo[filaFicha + i + 1][j]; //tablero fijo para luego hacer un and y comprobar la colision


            if((ficha & abajo) != 0)
                return true;
        }
    }

    return false;
}

void borrarFilas(unsigned char** tablerofijo, unsigned int filas, unsigned int columnas_bits)
{
    for(int i = filas - 1; i >= 0; i--)
    {
        bool llena = true;

        for(unsigned int j = 0; j < columnas_bits; j++)
        {
            if(tablerofijo[i][j] != 255)
            {
                llena = false;
                break;
            }
        }

        if(llena)
        {
            // bajar todas las filas
            for(int k = i; k > 0; k--)
            {
                for(unsigned int j = 0; j < columnas_bits; j++)
                {
                    tablerofijo[k][j] = tablerofijo[k-1][j];
                }
            }

            // limpiar la fila de arriba
            for(unsigned int j = 0; j < columnas_bits; j++)
            {
                tablerofijo[0][j] = 0;
            }

            i++; // volver a revisar la misma fila
        }
    }
}

void rotarDerecha(unsigned char piezaActual[4], unsigned int &dimensionx, unsigned int &dimensiony) {
    // Buffer temporal para construir la nueva forma sin destruir la original durante el proceso
    unsigned char piezaRotada[4] = {0, 0, 0, 0};

    // Al rotar 90 grados, el ancho se convierte en alto y viceversa
    unsigned int nuevaDimX = dimensiony;
    unsigned int nuevaDimY = dimensionx;

    // Recorremos la pieza original bit a bit usando sus dimensiones actuales
    for (unsigned int y = 0; y < dimensiony; y++) {
        for (unsigned int x = 0; x < dimensionx; x++) {

            // Extraemos el estado del bit en (x, y).
            // (7 - x) ajusta la lectura: el bit 7 es la columna 0 (izquierda)
            bool bitEncendido = (piezaActual[y] >> (7 - x)) & 1;

            if (bitEncendido) {
                /* Aplicamos la fórmula de rotación de matrices 90° horario:
                   NuevaFila = x_vieja
                   NuevaColumna = (Alto_viejo - 1) - y_vieja */
                unsigned int nuevaFila = x;
                unsigned int nuevaCol = (dimensiony - 1) - y;

                // Insertamos el bit en la nueva posición dentro del buffer temporal
                piezaRotada[nuevaFila] |= (1 << (7 - nuevaCol));
            }
        }
    }

    /* NORMALIZACIÓN AL BIT 7 (Izquierda):
       Tras rotar, la pieza puede quedar desplazada a la derecha del byte.
       Este bucle empuja los bits a la izquierda hasta que alguna fila toque el borde (bit 128).
     */
    while (!(piezaRotada[0] & 128 || piezaRotada[1] & 128 || piezaRotada[2] & 128 || piezaRotada[3] & 128)) {
        for (int i = 0; i < 4; i++) {
            piezaRotada[i] <<= 1;
        }
    }

    // Transferimos la pieza rotada y actualizamos las dimensiones por referencia
    for (int i = 0; i < 4; i++) {
        piezaActual[i] = piezaRotada[i];
    }
    dimensionx = nuevaDimX;
    dimensiony = nuevaDimY;
}
bool esPosibleRotar(unsigned char piezaRotada[4], unsigned int dimX, unsigned int dimY, unsigned int filaFicha, int posicionx, unsigned char** tablerofijo, unsigned int filas, unsigned int columnas_bits) {

    // 1. Verificar límites del tablero (bordes)
    if (filaFicha + dimY > filas) return false; // Se sale por abajo
    if (posicionx < 0 || (posicionx + dimX) > (columnas_bits * 8)) return false; // Se sale por los lados

    // 2. Verificar colisión con piezas fijas
    for (unsigned int i = 0; i < dimY; i++) {
        unsigned char filaDesplazada = piezaRotada[i];

        // Desplazamos los bits de la pieza rotada a su posición X actual
        for (int d = 0; d < posicionx; d++) {
            filaDesplazada >>= 1;
        }

        // Si al hacer un AND con el tablero fijo el resultado no es 0, hay choque
        // (Nota: Esto asume que la pieza cabe en el byte actual del tablero fijo)
        if ((filaDesplazada & tablerofijo[filaFicha + i][posicionx / 8]) != 0) {
            return false;
        }
    }

    return true; // Si pasó todas las pruebas, la rotación es segura
}