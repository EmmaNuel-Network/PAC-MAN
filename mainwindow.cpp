#include "mainwindow.h"
#include "jugador.h"
#include "gamecontroller.h"
#include "fantasma.h"

#include <QFile>
#include <QTextStream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <vector>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {
    // Tamaño original de la escena
    int anchoOriginal = 224;
    int altoOriginal = 228;
    int escala = 2;

    // Escalar ventana para que no se recorte nada
    resize(anchoOriginal * escala + 128, altoOriginal * escala + 256);

    // Crear escena y vista
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setGeometry(0, 0, anchoOriginal * escala + 64, altoOriginal * escala + 64);
    view->setSceneRect(0, 0, anchoOriginal, altoOriginal);

    // Escalar vista
    view->scale(2.0, 2.0);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    // Cargar fondo del laberinto
    QPixmap fondo(":/sprites/laberinto_recortado.png");
    if (fondo.isNull()) {
        qDebug() << "❌ ERROR: No se pudo cargar la imagen del fondo";
    } else {
        qDebug() << "✅ Imagen de fondo cargada correctamente";
        QGraphicsPixmapItem* fondoItem = new QGraphicsPixmapItem(fondo);
        scene->addItem(fondoItem);
    }

    // Leer archivo laberinto.txt desde recursos
    std::vector<std::string> matriz;
    QFile file(":/sprites/laberinto.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "❌ No se pudo abrir laberinto.txt desde recursos";
    } else {
        qDebug() << "✅ laberinto.txt cargado correctamente desde recursos";
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            matriz.push_back(line.toStdString());
        }
    }

    // Clonamos matriz para usar como mapa de galletas
    std::vector<std::string> matrizGalletas = matriz;
    for (auto& fila : matrizGalletas) {
        for (char& celda : fila) {
            if (celda == '0') celda = '1';  // Donde hay camino, ahora hay galleta
        }
    }




    // Crear GameController con la escena y el mapa de galletas
    GameController* controlador = new GameController(scene, matrizGalletas);

    // Crear jugador y agregarlo a la escena
    Jugador* jugador = new Jugador(matriz, controlador);
    scene->addItem(jugador);
    jugador->setFocus();  // Importante para recibir eventos de teclado

    // Coordenadas centrales de la cárcel (ajústalas si tu mapa tiene otras)
    std::vector<std::pair<int, int>> posicionesFantasmas = {
        {10,12},
        {10,13},
        {10,14},
        {10,15}

    };

    // Crear 4 fantasmas
    for (const auto& [fila, col] : posicionesFantasmas) {
        Fantasma* f = new Fantasma(matriz, controlador, fila, col);
        scene->addItem(f);
        controlador->registrarFantasma(f);
    }

}

MainWindow::~MainWindow() {
    delete view;
    delete scene;
}
