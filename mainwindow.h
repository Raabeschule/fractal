#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include <complex>
#include "fractalthread.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QProgressBar *progressBar;

    double maxActualIterations;
    double size;

    FractalThread *mThread;

    QImage image_global;

    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void saveImage();
    void drawFractal();
    void onImageDone(QImage image_global);
    void onProgressChanged(double progress);
    void on_realSlider_valueChanged(int value);
    void on_imagSlider_valueChanged(int value);
    void on_zoomSlider_valueChanged(int value);
};

#endif // MAINWINDOW_H
