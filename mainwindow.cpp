#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "qfiledialog.h"
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->installEventFilter(this);
    ui->realEdit->setValidator(new QDoubleValidator());
    ui->imagEdit->setValidator(new QDoubleValidator());
    ui->zoomEdit->setValidator(new QDoubleValidator(0, 1, 9, this));
    ui->sizeEdit->setValidator(new QIntValidator());
    ui->iterationsEdit->setValidator(new QIntValidator());

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(saveImage()));
    connect(ui->drawButton, SIGNAL(clicked()), this, SLOT(drawFractal()));
    connect(ui->imagSlider, SIGNAL(valueChanged(int)), this, SLOT(on_imagSlider_valueChanged(int)));
    connect(ui->realSlider, SIGNAL(valueChanged(int)), this, SLOT(on_realSlider_valueChanged(int)));
    connect(ui->zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(on_zoomSlider_valueChanged(int)));

    mThread = new FractalThread(this);
    connect(mThread, SIGNAL(imageDone(QImage)), this, SLOT(onImageDone(QImage)));
    connect(mThread, SIGNAL(progressChanged(double)), this, SLOT(onProgressChanged(double)));

    // Some fractals to try
    //std::complex<double> c(0, 1);
    //std::complex<double> c(0.2323, 0.2334);
    //std::complex<double> c(0.2323, 0.73051);
    std::complex<double> c(0, 0.73051);
    double zoom = 0.004;
    int maxIterations = 100;
    size = 1024;

    QStringList colors;
    colors << "Green" << "Red" << "Blue";

    ui->comboBox->addItems(colors);

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, (size * size) * 2);
    ui->statusBar->addPermanentWidget(progressBar);
    progressBar->setVisible(true);

    ui->realEdit->setText(QString::number(c.real()));
    ui->imagEdit->setText(QString::number(c.imag()));
    ui->zoomEdit->setText(QString::number(zoom));
    ui->sizeEdit->setText(QString::number(size));
    ui->iterationsEdit->setText(QString::number(maxIterations));
    ui->realSlider->setValue(c.real() * 100);
    ui->imagSlider->setValue(c.imag() * 100);
    ui->zoomSlider->setValue(zoom * 25000);

    mThread->setParameters(c, zoom, size, maxIterations, 0);
    mThread->start();
}

void MainWindow::saveImage() {
    // see https://stackoverflow.com/questions/14033720/qfiledialog-how-to-specify-home-directory#comment19386951_14033968
    QString filename = QFileDialog::getSaveFileName(this, "Save image", QDir::homePath() + QDir::separator() + "fractal.jpg", "JPEG Files (*.jpg, *.jpeg)");
    if(!image_global.save(filename, "jpg", 100)) {
        qDebug() << "Error saving image.";
    }
}

void MainWindow::drawFractal() {
    double imag = ui->imagEdit->text().toDouble();
    double real = ui->realEdit->text().toDouble();
    double zoom = ui->zoomEdit->text().toDouble();
    double user_size = ui->sizeEdit->text().toInt();
    double iterations = ui->iterationsEdit->text().toInt();

    int base_color = ui->comboBox->currentIndex();
    std::complex<double> c(real, imag);
    mThread->setParameters(c, zoom, user_size, iterations, base_color);
    mThread->start();
}

void MainWindow::onImageDone(QImage image) {
    image_global = image;
    ui->label->setPixmap(QPixmap::fromImage(image));
    progressBar->setValue(0);
}

void MainWindow::onProgressChanged(double progress) {
    progressBar->setValue(progress);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_realSlider_valueChanged(int value) {
    ui->realEdit->setText(QString::number(value * 0.01));
}

void MainWindow::on_imagSlider_valueChanged(int value) {
    ui->imagEdit->setText(QString::number(value * 0.01));
}

void MainWindow::on_zoomSlider_valueChanged(int value) {
    ui->zoomEdit->setText(QString::number(value * 0.00004));
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if(event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if((keyEvent->key() == Qt::Key_Enter) || (keyEvent->key() == Qt::Key_Return)) {
            drawFractal();
        }
        return true;
    }
    else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
