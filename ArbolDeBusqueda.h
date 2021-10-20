//
// Created by Catoras on 19/10/2021.
//

#ifndef LABERINTO_BUSQUEDA_EN_PROFUNDIDAD_ARBOLDEBUSQUEDA_H
#define LABERINTO_BUSQUEDA_EN_PROFUNDIDAD_ARBOLDEBUSQUEDA_H

#include "Plano.h"

#include "string"
#include "vector"
#include <iostream>

using namespace std;


class Nodo {
private:
    Nodo* padre = nullptr;
    Nodo* hermano = nullptr;
    Nodo* primerHijo = nullptr;
    Coordenada* coordenada;
    int profundidad;
    bool estaEstancado = false;
public:
    Nodo() {}

    Nodo(Coordenada* cordenada) {
        this->coordenada = cordenada;
    }

    Nodo(Coordenada* cordenada, Nodo* padre, int profundidad) {
        this->coordenada = cordenada;
        this->padre = padre;
        this->profundidad = profundidad;
    }

    Nodo* getPadre() {
        return padre;
    }

    Coordenada* getCoordenada() {
        return coordenada;
    }

    void setProfundidad(int profundidad){
        this->profundidad = profundidad;
    }

    int getProfundidad(){
        return this->profundidad;
    }

    bool getEstaEstancado () {
        return this->estaEstancado;
    }
    void setEstaEstancado (bool estaEstancado) {
        this->estaEstancado = estaEstancado;
    }

    void setHermano(Nodo* hermano) {
        this->hermano = hermano;
    }

    Nodo* getHermano() {
        return this->hermano;
    }

    Nodo* getPrimerHijo() const {
        return primerHijo;
    }

    void setPrimerHijo(Nodo* primerHijo) {
        this->primerHijo = primerHijo;
    }

    void print() {
        cout << "Actual:" << this->coordenada->toString() << " - Padre:" << this->padre->coordenada->toString() << endl;
    }

};

class ArbolDeBusqueda {
private:

    Coordenada* makeCoordenada(int x, int y){
        Coordenada* nuevaCoordena = new Coordenada(x,y);

        return nuevaCoordena;
    }

    vector<Coordenada*> getMovimientosPosibles(Coordenada* coordenada) {
        vector<Coordenada*> arregloDePosiblesResultados;
        if (coordenada->getY() >= 0 && coordenada->getY() < Y_SIZE - 1)
            arregloDePosiblesResultados.push_back(makeCoordenada(coordenada->getX(),coordenada->getY()+1));
        if (coordenada->getY() > 0 && coordenada->getY() < Y_SIZE )
            arregloDePosiblesResultados.push_back(makeCoordenada(coordenada->getX(),coordenada->getY()-1));
        if (coordenada->getX() >= 0 && coordenada->getX() < X_SIZE - 1)
            arregloDePosiblesResultados.push_back(makeCoordenada(coordenada->getX()+1,coordenada->getY()));
        if (coordenada->getX() > 0 && coordenada->getX() < X_SIZE )
            arregloDePosiblesResultados.push_back(makeCoordenada(coordenada->getX()-1,coordenada->getY()));

        return arregloDePosiblesResultados;
    }

    vector<string> getPath(Nodo* resultado) {
        vector<string> path;
        Nodo* c = resultado;
        while (c) {
            path.push_back(c->getCoordenada()->toString());
            c = c->getPadre();
        }

        return path;
    }

    bool checarSiLaRamaSeEstanco (Nodo* nodoAVerificar) {
        Coordenada* coordenadaDelNodoAVerificar = nodoAVerificar->getCoordenada();

        nodoAVerificar = nodoAVerificar->getPadre();

        while (nodoAVerificar) {
            if (nodoAVerificar->getCoordenada()->getY() == coordenadaDelNodoAVerificar->getY())
                if (nodoAVerificar->getCoordenada()->getX() == coordenadaDelNodoAVerificar->getX())
                    return true;
            nodoAVerificar = nodoAVerificar->getPadre();
        }

        return false;
    }

    Nodo* ancla;
    Coordenada* punto_de_partida;
    Plano* mapa;
    Coordenada* resultado_esperado;
    uint64_t nodos_creados = 0;
public:
    ArbolDeBusqueda(Coordenada* punto_de_partida, Coordenada* punto_objetivo, Plano* mapa) {
        this->punto_de_partida = punto_de_partida;
        this->resultado_esperado = punto_objetivo;
        this->mapa = mapa;
        ancla = new Nodo(punto_de_partida);
        ancla->setProfundidad(0);
    };

    uint64_t getNodosCreados() {
        return this->nodos_creados;
    }
    string getPuntoObjetivo() {
        return this->resultado_esperado->toString();
    }
    vector<string> busquedaPorProfundidad () {
        Nodo* resultado = ancla;
        if (resultado->getCoordenada()->getY() == this->resultado_esperado->getY()) {
            if (resultado->getCoordenada()->getX() == this->resultado_esperado->getX()) {
                cout << "Resultado Encontrado" << endl;
                return getPath(resultado);
            }
        }
        while (true) {
            if (!resultado->getEstaEstancado() && !mapa->isPosicionObstaculizada(resultado->getCoordenada())) {
                vector<Coordenada*> proximos_hijos = getMovimientosPosibles(resultado->getCoordenada());
                Nodo *hijoPasado = nullptr;
                for (Coordenada* cadena_del_hijo: proximos_hijos) {
                    Nodo *hijo = new Nodo(cadena_del_hijo, resultado, (resultado->getProfundidad()) + 1);
                    if (hijo->getCoordenada()->getY() == this->resultado_esperado->getY())
                        if (hijo->getCoordenada()->getX() == this->resultado_esperado->getX()) {
                            cout << "Resultado Encontrado" << endl;
                            return getPath(hijo);
                        }
                    //hijo->print();
                    nodos_creados++;
                    if (hijoPasado) {
                        hijoPasado->setHermano(hijo);
                    } else {
                        hijo->getPadre()->setPrimerHijo(hijo);
                    }
                    hijo->setEstaEstancado(checarSiLaRamaSeEstanco(hijo));
                    hijoPasado = hijo;
                }
            }
            if (resultado->getPrimerHijo()) {
                resultado = resultado->getPrimerHijo();
            } else if (resultado->getHermano()) {
                resultado = resultado->getHermano();
            } else {
                do {
                    resultado = resultado->getPadre();
                } while (!resultado->getHermano());
                resultado = resultado->getHermano();
            }
            if (resultado)
                cout <<"Nodos creados: "<< this->nodos_creados << " - Profunfidad actual:" <<resultado->getProfundidad()<< endl;
        }
    }

};


#endif //LABERINTO_BUSQUEDA_EN_PROFUNDIDAD_ARBOLDEBUSQUEDA_H
