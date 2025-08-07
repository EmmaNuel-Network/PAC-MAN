// fantasma.cpp
#include "fantasma.h"
#include "gamecontroller.h"
#include <QBrush>
#include <QRandomGenerator>

Fantasma::Fantasma(const std::vector<std::string>& laberinto, GameController* controlador, int filaInicio, int colInicio)
    : lab(laberinto), controlador(controlador), fila(filaInicio), col(colInicio), tamCelda(8) {

    setPixmap(QPixmap(":/sprites/fantasma.png").scaled(tamCelda, tamCelda));
    setPos(col * tamCelda, fila * tamCelda);
    setZValue(2);

    // Inicializar dirección aleatoria
    std::vector<std::pair<int, int>> direcciones = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    std::shuffle(direcciones.begin(), direcciones.end(), *QRandomGenerator::global());
    dirFila = direcciones[0].first;
    dirCol = direcciones[0].second;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Fantasma::moverFantasma);
    timer->start(150); // cada 300 ms se mueve
}

bool Fantasma::puedeMover(int f, int c) {
    if (f < 0 || c < 0 || f >= (int)lab.size() || c >= (int)lab[0].size())
        return false;

    return lab[f][c] == '0';
}

void Fantasma::moverFantasma() {
    int nf = fila + dirFila;
    int nc = col + dirCol;

    // Seguir en la misma dirección si se puede
    if (puedeMover(nf, nc)) {
        if (nf == controlador->getFilaJugador() && nc == controlador->getColJugador()) {
            if (!controlador->estaInvulnerable()) {
                controlador->perderVida();  // Pacman pierde vida
            }
            return;
        }

        fila = nf;
        col = nc;
        setPos(col * tamCelda, fila * tamCelda);
        return;
    }

    // Si no puede avanzar, elegir nueva dirección aleatoria
    std::vector<std::pair<int, int>> direcciones = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1}
    };
    std::shuffle(direcciones.begin(), direcciones.end(), *QRandomGenerator::global());

    for (const auto& [df, dc] : direcciones) {
        nf = fila + df;
        nc = col + dc;

        if (puedeMover(nf, nc)) {
            if (nf == controlador->getFilaJugador() && nc == controlador->getColJugador()) {
                if (!controlador->estaInvulnerable()) {
                    controlador->perderVida();  // Pacman pierde vida
                }
                return;
            }

            fila = nf;
            col = nc;
            setPos(col * tamCelda, fila * tamCelda);
            dirFila = df;
            dirCol = dc;
            return;
        }
    }
}


void Fantasma::detener() {
    if (timer->isActive()) {
        timer->stop();
    }
}

