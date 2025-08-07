// fantasma.h
#ifndef FANTASMA_H
#define FANTASMA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <vector>
#include <string>

class GameController;

class Fantasma : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Fantasma(const std::vector<std::string>& laberinto, GameController* controlador, int filaInicio, int colInicio);
    void moverFantasma();
    void detener();


private:
    std::vector<std::string> lab;
    GameController* controlador;
    int fila, col;
    int tamCelda;

    QTimer* timer;

    bool puedeMover(int f, int c);
    int dirFila, dirCol;
};

#endif // FANTASMA_H
