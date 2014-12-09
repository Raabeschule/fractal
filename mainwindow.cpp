#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->realEdit->setValidator( new QDoubleValidator());
    ui->imagEdit->setValidator( new QDoubleValidator());
    ui->zoomEdit->setValidator( new QDoubleValidator(0, 1, 9, this));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(saveImage()));
    connect(ui->drawButton, SIGNAL(clicked()), this, SLOT(drawFractal()));

    mThread = new FractalThread(this);
    connect(mThread, SIGNAL(imageDone(QImage)), this, SLOT(onImageDone(QImage)));

    // Some fractals to try
    //std::complex<double> c(0, 1);
    //std::complex<double> c(0.2323, 0.2334);
    //std::complex<double> c(0.2323, 0.73051);
    std::complex<double> c(0, 0.73051);

    size = 1024;

    mThread->setParameters(c, 0.004, size);
    mThread->start();
}



void MainWindow::saveImage() {
    if(!image.save("/home/seven/Downloads/fractal.jpg", "jpg", 100)) {
        qDebug() << "Error saving image.";
    }
}

void MainWindow::drawFractal() {
    double imag = ui->imagEdit->text().toDouble();
    double real = ui->realEdit->text().toDouble();
    double zoom = ui->zoomEdit->text().toDouble();
    std::complex<double> c(real, imag);
    mThread->setParameters(c, zoom, size);
    mThread->start();
}

void MainWindow::onImageDone(QImage image) {
    ui->label->setPixmap(QPixmap::fromImage(image));
}


MainWindow::~MainWindow() {
    delete ui;
}
