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

    double upperLimit;
};

#endif // MAINWINDOW_H
