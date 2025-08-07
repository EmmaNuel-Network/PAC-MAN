#ifndef JUGADOR_H
#define JUGADOR_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QKeyEvent>
#include <vector>
#include <string>

class GameController;  // Declaración adelantada para evitar dependencia circular

class Jugador : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT

public:
    Jugador(const std::vector<std::string>& lab, GameController* controlador);
    void keyPressEvent(QKeyEvent* event) override;
    void setActivo(bool estado);
    void reiniciarPosicion();


private:
    std::vector<std::string> laberinto;
    GameController* controlador;
    int fila, col;
    int tamCelda;

    bool puedeMover(int f, int c);
    void mover(int f, int c);
    bool activo = true;
    int filaInicial, colInicial;

    bool bocaAbierta;
    QTimer* animacionTimer;
    void alternarSprite();
};

#endif // JUGADOR_H
