#include <QApplication>
#include <QMessageBox>

#include <sbml/SBMLTypes.h>
#include <sbml/extension/SBMLDocumentPlugin.h>
#include <sbml/packages/spatial/extension/SpatialExtension.h>
#include <sbml/packages/spatial/common/SpatialExtensionTypes.h>

#include "mainwindow.h"

LIBSBML_CPP_NAMESPACE_USE

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SpatialPkgNamespaces sbmlns(3,1,1);

    // create the L3V1 document with spatial package
    SBMLDocument document(&sbmlns);

    return a.exec();
}
