#pragma once

#include <QMainWindow>
#include <QTreeWidgetItem>
#include "ext/qcustomplot/qcustomplot.h"

#include "sbml.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = nullptr);
  virtual ~MainWindow() override;

 private slots:
  void on_action_About_triggered();

  void on_actionE_xit_triggered();

  void on_action_Open_SBML_file_triggered();

  void on_action_Save_SBML_file_triggered();

  void on_actionAbout_Qt_triggered();

  void on_actionGeometry_from_image_triggered();

  void on_lblGeometry_mouseClicked(QRgb col);

  void on_chkSpeciesIsSpatial_stateChanged(int arg1);

  void on_chkShowSpatialAdvanced_stateChanged(int arg1);

  void on_listReactions_itemActivated(QTreeWidgetItem *item, int column);

  void on_listReactions_itemClicked(QTreeWidgetItem *item, int column);

  void on_btnSimulate_clicked();

  void on_listFunctions_currentTextChanged(const QString &currentText);

  void on_listSpecies_itemActivated(QTreeWidgetItem *item, int column);

  void on_listSpecies_itemClicked(QTreeWidgetItem *item, int column);

  void on_graphClicked(QCPAbstractPlottable *plottable, int dataIndex);

  void on_btnChangeCompartment_clicked();

  void on_listCompartments_currentTextChanged(const QString &currentText);

  void on_listCompartments_itemDoubleClicked(QListWidgetItem *item);

  void on_hslideTime_valueChanged(int value);

  void on_tabMain_currentChanged(int index);

  void on_btnImportConcentration_clicked();

  void on_listMembranes_currentTextChanged(const QString &currentText);

  void on_btnSpeciesDisplaySelect_clicked();

 private:
  Ui::MainWindow *ui;
  SbmlDocWrapper sbmlDoc;
  bool waitingForCompartmentChoice = false;

  // temp: vector of simulation images to display
  QVector<QImage> images;

  // update list of species to display in simulation result image
  void updateSpeciesDisplaySelect();
};
