#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sbml.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_About_triggered();

    void on_actionE_xit_triggered();

    void on_action_Open_SBML_file_triggered();

    void on_action_Save_SBML_file_triggered();

    void on_actionAbout_Qt_triggered();

    void on_actionGeometry_from_image_triggered();

    void on_lblGeometry_mouseClicked();

    void on_chkEnableSpatial_stateChanged(int arg1);

    void on_chkShowSpatialAdvanced_stateChanged(int arg1);

    void on_listSpecies_currentTextChanged(const QString &currentText);

    void on_btnChangeCompartment_triggered(QAction *arg1);

    void on_listReactions_currentTextChanged(const QString &currentText);

private:
    Ui::MainWindow *ui;
    sbmlDocWrapper sbml_doc;
    std::unique_ptr<QMenu> compartmentMenu;
};

#endif // MAINWINDOW_H
