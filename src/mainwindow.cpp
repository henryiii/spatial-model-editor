#include <QErrorMessage>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QString>
#include <QStringListModel>

#include "mainwindow.h"
#include "sbml.h"
#include "simulate.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // for debugging: import an image on startup
  ui->lblGeometry->importGeometry(
      "../spatial-model-editor/two-blobs-100x100.bmp");

  compartmentMenu = new QMenu(ui->btnChangeCompartment);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_action_About_triggered() {
  QMessageBox msgBox;
  msgBox.setWindowTitle("About");
  QString info("Spatial Model Editor\n");
  info.append("github.com/lkeegan/spatial-model-editor\n\n");
  info.append("Included libraries:\n");
  info.append("\nQt5:\t");
  info.append(QT_VERSION_STR);
  info.append("\nlibSBML:\t");
  info.append(libsbml::getLibSBMLDottedVersion());
  for (const auto &dep : {"expat", "libxml", "xerces-c", "bzip2", "zip"}) {
    if (libsbml::isLibSBMLCompiledWith(dep) != 0) {
      info.append("\n");
      info.append(dep);
      info.append(":\t");
      info.append(libsbml::getLibSBMLDependencyVersionOf(dep));
    }
  }
  msgBox.setText(info);
  msgBox.exec();
}

void MainWindow::on_actionE_xit_triggered() { QApplication::quit(); }

void MainWindow::on_action_Open_SBML_file_triggered() {
  // load SBML file
  QString filename = QFileDialog::getOpenFileName();
  if (!filename.isEmpty()) {
    sbml_doc.loadFile(qPrintable(filename));
    if (sbml_doc.isValid) {
      update_ui();
    }
  }
}

void MainWindow::update_ui() {
  // update raw XML display
  ui->txtSBML->setText(sbml_doc.xml);

  // update list of reactions
  ui->listReactions->clear();
  ui->listReactions->insertItems(0, sbml_doc.reactions);

  // update list of functions
  ui->listFunctions->clear();
  ui->listFunctions->insertItems(0, sbml_doc.functions);

  // update tree list of species
  ui->listSpecies->clear();
  QList<QTreeWidgetItem *> items;
  for (auto c : sbml_doc.compartments) {
    // add compartments as top level items
    QTreeWidgetItem *comp =
        new QTreeWidgetItem(ui->listSpecies, QStringList({c}));
    ui->listSpecies->addTopLevelItem(comp);
    for (auto s : sbml_doc.species[c]) {
      // add each species as child of compartment
      comp->addChild(new QTreeWidgetItem(comp, QStringList({s})));
    }
  }
  ui->listSpecies->expandAll();

  // update possible compartments in compartmentMenu
  compartmentMenu->clear();
  compartmentMenu->addAction("none");
  for (auto c : sbml_doc.compartments) {
    compartmentMenu->addAction(c);
  }
  ui->btnChangeCompartment->setMenu(compartmentMenu);
}

void MainWindow::on_action_Save_SBML_file_triggered() {
  QMessageBox msgBox;
  msgBox.setText("todo");
  msgBox.exec();
}

void MainWindow::on_actionAbout_Qt_triggered() { QMessageBox::aboutQt(this); }

void MainWindow::on_actionGeometry_from_image_triggered() {
  QString filename =
      QFileDialog::getOpenFileName(this, "Import geometry from image", "",
                                   "Image Files (*.png *.jpg *.bmp)");
  ui->lblGeometry->importGeometry(filename);
}

void MainWindow::on_lblGeometry_mouseClicked() {
  // update colour box
  QPalette palette;
  palette.setColor(QPalette::Window, QColor::fromRgb(ui->lblGeometry->colour));
  ui->lblCompartmentColour->setPalette(palette);
  // update drop-down compartement selector
  QString compID = ui->lblGeometry->compartmentID;
  ui->btnChangeCompartment->setText(compID);
}

void MainWindow::on_chkSpeciesIsSpatial_stateChanged(int arg1) {
  ui->grpSpatial->setEnabled(arg1);
}

void MainWindow::on_chkShowSpatialAdvanced_stateChanged(int arg1) {
  ui->grpSpatialAdavanced->setEnabled(arg1);
}

void MainWindow::on_btnChangeCompartment_triggered(QAction *arg1) {
  // todo: make sure each comp only has one colour
  QString compID = arg1->text();
  ui->lblGeometry->colour_to_comp[ui->lblGeometry->colour] = compID;
  ui->lblGeometry->compartmentID = compID;
  on_lblGeometry_mouseClicked();
}

void MainWindow::on_listReactions_currentTextChanged(
    const QString &currentText) {
  ui->listProducts->clear();
  ui->listReactants->clear();
  ui->listReactionParams->clear();
  ui->lblReactionRate->clear();
  if (currentText.size() > 0) {
    qDebug() << currentText;
    const auto *reac = sbml_doc.model->getReaction(qPrintable(currentText));
    for (unsigned i = 0; i < reac->getNumProducts(); ++i) {
      ui->listProducts->addItem(reac->getProduct(i)->getSpecies().c_str());
    }
    for (unsigned i = 0; i < reac->getNumReactants(); ++i) {
      ui->listReactants->addItem(reac->getReactant(i)->getSpecies().c_str());
    }
    for (unsigned i = 0; i < reac->getKineticLaw()->getNumParameters(); ++i) {
      ui->listReactionParams->addItem(
          reac->getKineticLaw()->getParameter(i)->getId().c_str());
    }
    ui->lblReactionRate->setText(reac->getKineticLaw()->getFormula().c_str());
  }
}

void MainWindow::on_btnSimulate_clicked() {
  // do simple simulation of model

  // compile reaction expressions
  simulate sim(sbml_doc);
  sim.compile_reactions();
  // set initial concentrations
  for (unsigned int i = 0; i < sbml_doc.model->getNumSpecies(); ++i) {
    const auto *spec = sbml_doc.model->getSpecies(i);
    if (spec->isSetInitialAmount()) {
      // SBML file specifies amount: convert to concentration
      double vol =
          sbml_doc.model->getCompartment(spec->getCompartment())->getSize();
      sim.species_values[i] = spec->getInitialAmount() / vol;
    } else {
      sim.species_values[i] = spec->getInitialConcentration();
    }
  }

  // generate vector of resulting concentrations at each timestep
  sim.euler_timestep(0.0);
  QVector<double> time;
  double t = 0;
  double dt = ui->txtSimDt->text().toDouble();
  std::vector<QVector<double>> conc(sim.species_values.size());
  time.push_back(t);
  for (std::size_t i = 0; i < sim.species_values.size(); ++i) {
    conc[i].push_back(sim.species_values[i]);
  }
  for (int i = 0;
       i < static_cast<int>(ui->txtSimLength->text().toDouble() / dt); ++i) {
    // do an euler integration timestep
    sim.euler_timestep(dt);
    t += dt;
    time.push_back(t);
    for (std::size_t i = 0; i < sim.species_values.size(); ++i) {
      conc[i].push_back(sim.species_values[i]);
    }
  }
  // plot results
  ui->pltPlot->clearGraphs();
  ui->pltPlot->setInteraction(QCP::iRangeDrag, true);
  ui->pltPlot->setInteraction(QCP::iRangeZoom, true);
  ui->pltPlot->setInteraction(QCP::iSelectPlottables, true);

  std::vector<QColor> col{{0, 0, 0},       {230, 25, 75},   {60, 180, 75},
                          {255, 225, 25},  {0, 130, 200},   {245, 130, 48},
                          {145, 30, 180},  {70, 240, 240},  {240, 50, 230},
                          {210, 245, 60},  {250, 190, 190}, {0, 128, 128},
                          {230, 190, 255}, {170, 110, 40},  {255, 250, 200},
                          {128, 0, 0},     {170, 255, 195}, {128, 128, 0},
                          {255, 215, 180}, {0, 0, 128},     {128, 128, 128}};
  ui->pltPlot->legend->setVisible(true);
  for (std::size_t i = 0; i < sim.species_values.size(); ++i) {
    ui->pltPlot->addGraph();
    ui->pltPlot->graph(static_cast<int>(i))->setData(time, conc[i]);
    ui->pltPlot->graph(static_cast<int>(i))->setPen(col[i]);
    ui->pltPlot->graph(static_cast<int>(i))
        ->setName(sbml_doc.speciesID[i].c_str());
  }
  ui->pltPlot->xAxis->setLabel("time");
  ui->pltPlot->xAxis->setRange(time.front(), time.back());
  ui->pltPlot->yAxis->setLabel("concentration");
  ui->pltPlot->replot();
}

void MainWindow::on_listFunctions_currentTextChanged(
    const QString &currentText) {
  ui->listFunctionParams->clear();
  ui->lblFunctionDef->clear();
  if (currentText.size() > 0) {
    qDebug() << currentText;
    const auto *func =
        sbml_doc.model->getFunctionDefinition(qPrintable(currentText));
    for (unsigned i = 0; i < func->getNumArguments(); ++i) {
      ui->listFunctionParams->addItem(
          libsbml::SBML_formulaToL3String(func->getArgument(i)));
    }
    ui->lblFunctionDef->setText(
        libsbml::SBML_formulaToL3String(func->getBody()));
  }
}

void MainWindow::on_listSpecies_itemActivated(QTreeWidgetItem *item,
                                              int column) {
  // if user selects a species (i.e. an item with a parent)
  if ((item != nullptr) && (item->parent() != nullptr)) {
    qDebug() << item->text(column);
    // display species information
    auto *spec = sbml_doc.model->getSpecies(qPrintable(item->text(column)));
    ui->txtInitialConcentration->setText(
        QString::number(spec->getInitialConcentration()));
  }
}

void MainWindow::on_listSpecies_itemClicked(QTreeWidgetItem *item, int column) {
  on_listSpecies_itemActivated(item, column);
}
