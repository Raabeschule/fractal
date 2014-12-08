#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(saveImage()));

    // Some fractals to try
    //std::complex<double> c(0, 1);
    //std::complex<double> c(0.2323, 0.2334);
    //std::complex<double> c(0.2323, 0.73051);
    std::complex<double> c(0, 0.73051);

    size = 1024;
    upperLimit = 1e120;
    maxIterations = 1000;
    maxActualIterations = 0;

    image = QImage(size, size, QImage::Format_RGB32);
    image.fill(Qt::white);

    for(int xPos = 0; xPos < size; xPos++) {
        for(int yPos = 0; yPos < size; yPos++) {
            double rl = (xPos - (size/2)) * 0.004;
            double im = -((yPos - (size/2)) * 0.004);
            double dist = isConvergent(rl, im, c);

            if(dist > maxActualIterations) maxActualIterations = dist;
        }
    }

    for(int xPos = 0; xPos < size; xPos++) {
        for(int yPos = 0; yPos < size; yPos++) {
            double rl = (xPos - (size/2)) * 0.004;
            double im = -((yPos - (size/2)) * 0.004);
            double dist = isConvergent(rl, im, c);

            if(dist == -1) {
                image.setPixel(xPos, yPos, qRgb(0,0,0));
            }
            else {
                int color = map(dist, 0, maxActualIterations, 0, 255);
                image.setPixel(xPos, yPos, qRgb(0,color,color/5));
            }
        }
    }

    ui->label->setPixmap(QPixmap::fromImage(image));
}

double MainWindow::isConvergent(double rl, double im, std::complex<double> c) {
    std::complex<double> curr(rl, im);

    std::complex<double> prev;
    for(int i = 0; i < maxIterations; i++) {
        prev = curr;
        curr = julia(curr, c);

        if(std::abs(curr) > upperLimit) {
            return i;
        }
    }
    return -1;
}

std::complex<double> MainWindow::julia(std::complex<double> a, std::complex<double> c) {
    return a*a + c;
}

double MainWindow::complexDistance(std::complex<double> prev, std::complex<double> curr) {
    return std::abs(prev - curr);
}

double MainWindow::map(double value, double in_min, double in_max, double out_min, double out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void MainWindow::saveImage() {
    if(!image.save("/Users/benni/Downloads/fractal.jpg", "jpg", 100)) {
        qDebug() << "Error saving image.";
    }
}

MainWindow::~MainWindow() {
    delete ui;
}
