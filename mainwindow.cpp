#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    int size = 1024;
    std::complex<double> c(0, 1);
    upperLimit = 1e120;

    QImage image(size, size, QImage::Format_RGB32);

    QRgb value;

    for(int xPos = 0; xPos < size; xPos++) {
        for(int yPos = 0; yPos < size; yPos++) {
            double rl = (xPos - (size/2)) * 0.004;
            double im = -((yPos - (size/2)) * 0.004);
            double dist = isConvergent(rl, im, c);

            if(dist == -1) {
                value = qRgb(255, 255, 255);
                image.setPixel(xPos, yPos, value);
            }
            else {
                value = qRgb(0, 0, 0);
                image.setPixel(xPos, yPos, value);
            }
        }
    }

    ui->label->setPixmap(QPixmap::fromImage(image));
}

double MainWindow::isConvergent(double rl, double im, std::complex<double> c) {
    std::complex<double> start(rl, im);
    std::complex<double> curr(rl, im);

    std::complex<double> prev;
    for(int i = 0; i < 20; i++) {
        prev = curr;
        curr = julia(curr, c);

        if(std::abs(curr) > upperLimit) {
            return -1;
        }
    }

    return 6;
}

std::complex<double> MainWindow::julia(std::complex<double> a, std::complex<double> c) {
    return a*a + c;
}

double MainWindow::complexDistance(std::complex<double> prev, std::complex<double> curr)
{
    return std::abs(prev - curr);
}

MainWindow::~MainWindow() {
    delete ui;
}
