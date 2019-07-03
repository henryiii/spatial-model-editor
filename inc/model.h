// Spatial Geometry
//  - Geometry class: defines set of points that make up compartment
//  - Field class: species concentrations within compartment

#ifndef MODEL_H
#define MODEL_H

#include <QDebug>
#include <QImage>

class Geometry {
 private:
  QImage img_comp;

 public:
  void init(QImage img, QRgb col);
  // vector of points that make up compartment
  std::vector<QPoint> ix;
  // size of QImage
  QSize img_size;
  // return a QImage of the compartment geometry
  const QImage &compartment_image();
};

class Field {
  enum BOUNDARY_CONDITION { DIRICHLET, NEUMANN };

 private:
  std::size_t n_bcs = 1;
  // vector of indices of the nearest-neighbours
  // of the point i:
  // [4*i+0] = +x neighbour
  // [4*i+1] = -x neighbour
  // [4*i+2] = +y neighbour
  // [4*i+3] = -y neighbour
  // size: 4*n_pixels
  std::vector<std::size_t> nn;
  Geometry *geometry;

 public:
  std::size_t n_species;
  std::size_t n_pixels;
  // field of species concentrations
  std::vector<double> conc;
  // field of dcdt values
  std::vector<double> dcdt;
  // vector of diffusion constants for each species
  std::vector<double> diffusion_constant;
  // a set of colours for display purposes
  std::vector<QColor> speciesColour{
      {230, 25, 75},  {60, 180, 75},   {255, 225, 25}, {0, 130, 200},
      {245, 130, 48}, {145, 30, 180},  {70, 240, 240}, {240, 50, 230},
      {210, 245, 60}, {250, 190, 190}, {0, 128, 128},  {230, 190, 255},
      {170, 110, 40}, {255, 250, 200}, {128, 0, 0},    {170, 255, 195},
      {128, 128, 0},  {255, 215, 180}, {0, 0, 128},    {128, 128, 128}};

  void init(Geometry *geom, std::size_t n_species_ = 1,
            BOUNDARY_CONDITION bc = NEUMANN);
  void setConstantConcentration(std::size_t species_index,
                                double concentration);
  void importConcentration(std::size_t species_index, QImage img,
                           double scale_factor = 1.0);
  QImage img_conc;
  // return a QImage of the concentration of given species
  const QImage &getConcentrationImage(std::size_t species_index);
  // return a QImage of the concentration of the given set of species
  const QImage &getConcentrationImage(
      const std::vector<std::size_t> &species_indices);
  // return a QImage of the concentration of all species
  const QImage &getConcentrationImage();
  // field.dcdt = result of the diffusion operator acting on field.conc
  void diffusion_op();
  double get_mean_concentration(std::size_t species_index);
};

#endif  // MODEL_H
