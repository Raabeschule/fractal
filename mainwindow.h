#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <complex>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    double isConvergent(double rl, double im, std::complex<double> c = 0);
    std::complex<double> julia(std::complex<double> a, std::complex<double> c = 0);
    double complexDistance(std::complex<double> prev, std::complex<double> curr);
    double map(double value, double in_min, double in_max, double out_min, double out_max);
    QImage getFractalImage(std::complex<double> c, double zoom, double size);

    double upperLimit;
    double maxIterations;
    double maxActualIterations;
    double size;

    QImage image;

private slots:
    void saveImage();
    void drawFractal();
};

#endif // MAINWINDOW_H
