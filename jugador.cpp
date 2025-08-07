#include "jugador.h"
#include "gamecontroller.h"
#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QTimer>


Jugador::Jugador(const std::vector<std::string>& lab, GameController* controlador)
    : laberinto(lab), controlador(controlador),
    fila(1), col(1), tamCelda(8),
    filaInicial(1), colInicial(1), bocaAbierta(true)  // ← añadimos esto
{
    setPixmap(QPixmap(":/sprites/pacman_abierto.png").scaled(tamCelda, tamCelda));
    setPos(col * tamCelda, fila * tamCelda);
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
    setZValue(2);

    animacionTimer = new QTimer(this);
    connect(animacionTimer, &QTimer::timeout, this, &Jugador::alternarSprite);
    animacionTimer->start(200);  // Cambia cada 200 ms
}

void Jugador::alternarSprite() {
    bocaAbierta = !bocaAbierta;
    if (bocaAbierta) {
        setPixmap(QPixmap(":/sprites/pacman_abierto.png").scaled(tamCelda, tamCelda));
    } else {
        setPixmap(QPixmap(":/sprites/pacman_cerrado.png").scaled(tamCelda, tamCelda));
    }
}


bool Jugador::puedeMover(int f, int c) {
    if (f < 0 || c < 0 || f >= (int)laberinto.size() || c >= (int)laberinto[0].size())
        return false;
    return laberinto[f][c] != '1';
}

#include <QTransform>  // Asegúrate de tener este include arriba

void Jugador::mover(int f, int c) {
    int df = f - fila;
    int dc = c - col;

    QPixmap base(":/sprites/pacman_abierto.png");
    QPixmap rotada;

    if (df == -1) {  // Arriba
        rotada = base.transformed(QTransform().rotate(-90));
    } else if (df == 1) {  // Abajo
        rotada = base.transformed(QTransform().rotate(90));
    } else if (dc == -1) {  // Izquierda
        rotada = base.transformed(QTransform().rotate(180));
    } else if (dc == 1) {  // Derecha (sin rotar)
        rotada = base;
    }

    setPixmap(rotada.scaled(tamCelda, tamCelda));

    fila = f;
    col = c;
    setPos(col * tamCelda, fila * tamCelda);
    controlador->actualizarPosJugador(fila, col);
}


void Jugador::keyPressEvent(QKeyEvent* event) {
    if (!activo) return;  // si está desactivado, no se mueve

    int nf = fila, nc = col;

    switch (event->key()) {
    case Qt::Key_W: nf--; break;
    case Qt::Key_S: nf++; break;
    case Qt::Key_A: nc--; break;
    case Qt::Key_D: nc++; break;
    }

    if (puedeMover(nf, nc)) {
        mover(nf, nc);
        controlador->comerGalleta(fila, col);
    }
}

void Jugador::setActivo(bool estado) {
    activo = estado;
}

void Jugador::reiniciarPosicion() {
    fila = filaInicial;
    col = colInicial;
    setPos(col * tamCelda, fila * tamCelda);
    controlador->actualizarPosJugador(fila, col);  // actualiza posición en GameController
}



