#ifndef FUNCIONES_H
#define FUNCIONES_H

#endif // FUNCIONES_H

#include <iostream>

// --- PROTOTIPOS DE LAS FUNCIONES ---

void imprimirTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas, unsigned char** tablerofijo);
void inicializarTablero(unsigned char** ptr, unsigned int filas, unsigned int columnas);
void crearMatriz(unsigned char** ptr, unsigned int filas, unsigned int columnas);
void ingresarFicha(unsigned char** ptr, unsigned char piezas[5][4],unsigned int columnas, unsigned int columnas_bits,unsigned int &filaficha,unsigned int &dimensiony,unsigned char** tablerofijo,bool &juegoActivo,int &posicionx,unsigned int &dimensionx,unsigned char piezaActiva[4]);
void bajar(unsigned char** ptr,unsigned char** tablerofijo,unsigned int &filaFicha,unsigned int columnas_bits,unsigned int filas, unsigned char piezas[5][4],unsigned int columnas,unsigned int &dimensiony,bool &juegoActivo,int &posicionx,unsigned int &dimensionx,unsigned char piezaActiva[4]);
void MoverDerecha(unsigned char** ptr,unsigned char** tablerofijo,unsigned int filaFicha,unsigned int columnas_bits,unsigned int filas,unsigned int dimensiony,int &posicionx,unsigned int dimensionx);
void MoverIzquierda(unsigned char** ptr,unsigned char** tablerofijo,unsigned int filaFicha,unsigned int columnas_bits,unsigned int dimensiony,int &posicionx);
bool hayColisionAbajo(unsigned char** ptr,unsigned char** tablerofijo,unsigned int filaFicha,unsigned int columnas_bits,unsigned int filas, unsigned int dimensiony);
void borrarFilas(unsigned char** tablerofijo, unsigned int filas, unsigned int columnas_bits);
void rotarDerecha(unsigned char piezaActual[4], unsigned int &dimensionx, unsigned int &dimensiony);
bool esPosibleRotar(unsigned char piezaRotada[4], unsigned int dimX, unsigned int dimY, unsigned int filaFicha, int posicionx, unsigned char** tablerofijo, unsigned int filas, unsigned int columnas_bits);
bool hayColisionDerecha(unsigned char** ptr, unsigned char** tablerofijo, unsigned int filaFicha, unsigned int columnas_bits, unsigned int dimensiony, int posicionx, unsigned int dimensionx);
bool hayColisionIzquierda(unsigned char** ptr, unsigned char** tablerofijo, unsigned int filaFicha, unsigned int columnas_bits, unsigned int dimensiony, int posicionx);
