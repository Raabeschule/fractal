#include "fractalthread.h"
#include "qdebug.h"

FractalThread::FractalThread(QObject *parent) :
    QThread(parent) {
    upperLimit = 1e120;
    maxIterations = 100;
}


void FractalThread::run() {
    QImage image = QImage(size, size, QImage::Format_RGB32);
    image = getFractalImage(c, zoom, size, base_color);
    emit imageDone(image);
}

void FractalThread::setParameters(std::complex<double> c_param, double zoom_param, double size_param, int base_color_param) {
    c = c_param;
    zoom = zoom_param;
    size = size_param;
    base_color = base_color_param;
}

QImage FractalThread::getFractalImage(std::complex<double> c, double zoom, double size, int base_color) {
    double progress = 0;
    double maxActualIterations = 0;
    QImage image = QImage(size, size, QImage::Format_RGB32);
    image.fill(Qt::white);
    std::vector<double> distArray(size*size);

    for(int xPos = 0; xPos < size; xPos++) {
        for(int yPos = 0; yPos < size; yPos++) {
            double rl = (xPos - (size/2)) * zoom;
            double im = -((yPos - (size/2)) * zoom);
            double dist = isConvergent(rl, im, c);

            if(dist > maxActualIterations) maxActualIterations = dist;
            distArray[progress] = dist;
            progress++;
        }
        emit progressChanged(progress);
    }
    qDebug()<< progress-size*size;

    for(int xPos = 0; xPos < size; xPos++) {
        for(int yPos = 0; yPos < size; yPos++) {
            double dist = distArray[progress-size*size];

            if(dist == -1) {
                image.setPixel(xPos, yPos, qRgb(0,0,0));
            }
            else {
                int color = map(dist, 0, maxActualIterations, 0, 255);

                QRgb pixel_color;
                switch(base_color){
                    case 0:
                        pixel_color = qRgb(0, color, color/5); // Green
                        break;
                    case 1:
                        pixel_color = qRgb(color, color/5, 0); // Red
                        break;
                    case 2:
                        pixel_color = qRgb(color/5, 0, color); // Blue
                        break;
                    default:
                        pixel_color = qRgb(0, color, color/5); // Green
                }

                image.setPixel(xPos, yPos, pixel_color);
            }
            progress++;
        }
        emit progressChanged(progress);
    }
    return image;
}

double FractalThread::isConvergent(double rl, double im, std::complex<double> c) {
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

std::complex<double> FractalThread::julia(std::complex<double> a, std::complex<double> c) {
    return a*a + c;
}

double FractalThread::complexDistance(std::complex<double> prev, std::complex<double> curr) {
    return std::abs(prev - curr);
}

double FractalThread::map(double value, double in_min, double in_max, double out_min, double out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
