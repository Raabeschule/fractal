#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "qfiledialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->realEdit->setValidator( new QDoubleValidator());
    ui->imagEdit->setValidator( new QDoubleValidator());
    ui->zoomEdit->setValidator( new QDoubleValidator(0, 1, 9, this));
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
    size = 1024;

    QStringList colors;
    colors << "Green" << "Red" << "Blue";

    ui->comboBox->addItems(colors);

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, (size*size)*2);
    ui->statusBar->addPermanentWidget(progressBar);
    progressBar->setVisible(true);

    mThread->setParameters(c, 0.004, size, 0);
    mThread->start();
}



void MainWindow::saveImage() {
    qDebug() << QDir::homePath();
    // see https://stackoverflow.com/questions/14033720/qfiledialog-how-to-specify-home-directory#comment19386951_14033968
    QString filename = QFileDialog::getSaveFileName(this, "Save image", QDir::homePath() + QDir::separator() + "fractal.jpg", "JPEG Files (*.jpg, *.jpeg)");
    qDebug() << filename;
    if(!image_global.save(filename, "jpg", 100)) {
        qDebug() << "Error saving image.";
    }
}

void MainWindow::drawFractal() {
    double imag = ui->imagEdit->text().toDouble();
    double real = ui->realEdit->text().toDouble();
    double zoom = ui->zoomEdit->text().toDouble();
    int base_color = ui->comboBox->currentIndex();
    std::complex<double> c(real, imag);
    mThread->setParameters(c, zoom, size, base_color);
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

void MainWindow::on_realSlider_valueChanged(int value)
{
    ui->realEdit->setText(QString::number(value*0.01));
}

void MainWindow::on_imagSlider_valueChanged(int value)
{
    ui->imagEdit->setText(QString::number(value*0.01));
}

void MainWindow::on_zoomSlider_valueChanged(int value)
{
    ui->zoomEdit->setText(QString::number(value*0.00004));
}
