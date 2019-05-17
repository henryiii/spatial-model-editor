#include <QErrorMessage>
#include <QMessageBox>
#include <QString>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <sbml/SBMLTypes.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("About");
    QString info("Spatial Model Editor\n\n");
    info.append("github.com/lkeegan/spatial-model-editor\n");
    info.append("Included libraries:\n");
    info.append("\nQt5:\t");
    info.append(QT_VERSION_STR);
    info.append("\nlibSBML:\t");
    info.append(libsbml::getLibSBMLDottedVersion());
    for(const auto& dep : {"expat", "libxml", "xerces-c", "bzip2", "zip"}){
        if(libsbml::isLibSBMLCompiledWith(dep) != 0){
            info.append("\n");
            info.append(dep);
            info.append(":\t");
            info.append(libsbml::getLibSBMLDependencyVersionOf(dep));
        }
    }
    msgBox.setText(info);
    msgBox.exec();
}

void MainWindow::on_actionE_xit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_action_Open_SBML_file_triggered()
{
    QString filename = QFileDialog::getOpenFileName();
    libsbml::SBMLDocument *doc = libsbml::readSBMLFromFile(qPrintable(filename));
    if (doc->getErrorLog()->getNumFailsWithSeverity(libsbml::LIBSBML_SEV_ERROR) > 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Error: Invalid SBML file");
        msgBox.exec();
        // doc->printErrors(stream)
    }
    else
    {
        ui->textBrowser->setText(libsbml::writeSBMLToString(doc));
    }

}

void MainWindow::on_action_Save_SBML_file_triggered()
{
    QMessageBox msgBox;
    msgBox.setText("todo");
    msgBox.exec();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this);
}
