#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include <vector>
#include <string>
#include "jugador.h"
#include "fantasma.h"

class GameController : public QObject {
    Q_OBJECT

public:
    GameController(QGraphicsScene* scene, const std::vector<std::string>& mapaGalletasOriginal);

    bool comerGalleta(int fila, int col);
    void perderVida();
    int getGalletasComidas() const;
    int getVidas() const;
    void actualizarScore();

    int getFilaJugador() const;
    int getColJugador() const;
    void actualizarPosJugador(int f, int c);

    void registrarJugador(Jugador* j);
    void registrarFantasma(Fantasma* f);
    void detenerJuego();
    void reiniciarJugador();
    bool estaInvulnerable() const;




private:
    QGraphicsScene* scene;
    std::vector<std::string> mapaGalletas;
    int galletasTotales;
    int galletasComidas;
    int vidas;

    void verificarVictoria();
    void gameOver();
    //
    QGraphicsTextItem* textoScore;
    QGraphicsTextItem* textoVidas;

    bool juegoTerminado = false;
    int jugadorFila;
    int jugadorCol;

    Jugador* jugador;
    std::vector<Fantasma*> fantasmas;

    bool jugadorInvulnerable;

    QTimer* invulnerableTimer;

private slots:
    void finInvulnerabilidad();


};

#endif // GAMECONTROLLER_H
