#ifndef FRACTALTHREAD_H
#define FRACTALTHREAD_H

#include <QThread>
#include <QImage>
#include <complex>

class FractalThread : public QThread
{
    Q_OBJECT
public:
    explicit FractalThread(QObject *parent = 0);
    void run();
    void setParameters(std::complex<double> c_param, double zoom_param, double size_param, int base_color_param);

    bool Stop;

private:
    double isConvergent(double rl, double im, std::complex<double> c = 0);
    std::complex<double> julia(std::complex<double> a, std::complex<double> c = 0);
    double complexDistance(std::complex<double> prev, std::complex<double> curr);
    double map(double value, double in_min, double in_max, double out_min, double out_max);
    QImage getFractalImage(std::complex<double> c, double zoom, double size, int colors);

    double upperLimit;
    double maxIterations;
    std::complex<double> c;
    double zoom;
    double size;
    int base_color;

signals:
    void imageDone(QImage);
    void progressChanged(double);

public slots:

};

#endif // FRACTALTHREAD_H
