#include "gamecontroller.h"
#include <QGraphicsRectItem>
#include <QBrush>
#include <QMessageBox>

GameController::GameController(QGraphicsScene* scene, const std::vector<std::string>& mapaGalletasOriginal)
    : scene(scene), mapaGalletas(mapaGalletasOriginal), galletasComidas(0), vidas(3), jugadorInvulnerable(false) {

    // Contar galletas totales al inicio (donde hay '1')
    galletasTotales = 0;
    for (const auto& fila : mapaGalletas) {
        for (char celda : fila) {
            if (celda == '1') {
                galletasTotales++;
            }
        }
    }

    // Crear texto para el score (abajo del laberinto)
    textoScore = new QGraphicsTextItem(QString("Galletas: %1").arg(galletasComidas));
    textoScore->setDefaultTextColor(Qt::white);
    textoScore->setFont(QFont("Pac-Font", 10));
    textoScore->setPos(8, 240);  // debajo del laberinto
    textoScore->setZValue(10);
    scene->addItem(textoScore);

    // Crear texto para las vidas
    textoVidas = new QGraphicsTextItem(QString("Vidas: %1").arg(vidas));
    textoVidas->setDefaultTextColor(Qt::white);
    textoVidas->setFont(QFont("Pac-Font", 10));
    textoVidas->setPos(120, 240);  // a la derecha del score
    textoVidas->setZValue(10);
    scene->addItem(textoVidas);

    // Inicializar el timer de invulnerabilidad
    invulnerableTimer = new QTimer(this);
    invulnerableTimer->setSingleShot(true);
    connect(invulnerableTimer, &QTimer::timeout, this, &GameController::finInvulnerabilidad);
}


bool GameController::comerGalleta(int fila, int col) {
    if(juegoTerminado) return false;
    if (fila < 0 || col < 0 || fila >= (int)mapaGalletas.size() || col >= (int)mapaGalletas[0].size())
        return false;

    if (mapaGalletas[fila][col] == '1') {

        qDebug() << "Galleta comida en [" << fila << "," << col << "]";
        // Marcar como comida
        mapaGalletas[fila][col] = '3';
        galletasComidas++;

        //Actuliza el score
        textoScore->setPlainText(QString("Galletas: %1").arg(galletasComidas));

        // Tapar visualmente la galleta
        QGraphicsRectItem* tapa = new QGraphicsRectItem(col * 8, fila * 8, 8, 8);
        tapa->setBrush(QBrush(Qt::blue));
        tapa->setPen(Qt::NoPen);
        tapa->setZValue(1);

        scene->addItem(tapa);



        verificarVictoria();
        return true;
    }

    return false;
}

void GameController::verificarVictoria() {
    if (galletasComidas >= 254 && !juegoTerminado) {
        detenerJuego();
        QMessageBox::information(nullptr, "Victoria", "¡Has comido todas las galletas!");
    }
}

void GameController::perderVida() {
    if (jugadorInvulnerable || juegoTerminado) return;

    vidas--;
    textoVidas->setPlainText(QString("Vidas: %1").arg(vidas));

    // Activar invulnerabilidad temporal
    jugadorInvulnerable = true;
    invulnerableTimer->start(2000);  // 2 segundos

    // Reiniciar la posición del jugador
    if (jugador) {
        jugador->reiniciarPosicion();
    }

    if (vidas <= 0) {
        gameOver();
    }
}



void GameController::gameOver() {
    detenerJuego();
    QMessageBox::critical(nullptr, "Game Over", "¡Te has quedado sin vidas!");
    // Puedes cerrar la app o reiniciar si lo deseas
}

int GameController::getGalletasComidas() const {
    return galletasComidas;
}

int GameController::getVidas() const {
    return vidas;
}

void GameController::actualizarPosJugador(int f, int c) {
    jugadorFila = f;
    jugadorCol = c;
}

int GameController::getFilaJugador() const {
    return jugadorFila;
}

int GameController::getColJugador() const {
    return jugadorCol;
}

void GameController::registrarJugador(Jugador* j) {
    jugador = j;
}

void GameController::registrarFantasma(Fantasma* f) {
    fantasmas.push_back(f);
}

void GameController::detenerJuego() {
    juegoTerminado = true;

    if (jugador) {
        jugador->setActivo(false);  // bloquea el movimiento del jugador
        jugador->clearFocus();       // quita el foco de teclado para evitar input
    }

    for (Fantasma* f : fantasmas) {
        f->detener();  // detiene el timer interno de cada fantasma
    }
}

void GameController::reiniciarJugador() {
    if (jugador) {
        jugador->reiniciarPosicion();
    }
}

bool GameController::estaInvulnerable() const {
    return jugadorInvulnerable;
}

void GameController::finInvulnerabilidad() {
    jugadorInvulnerable = false;
}










