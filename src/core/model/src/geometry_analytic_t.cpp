#include "catch_wrapper.hpp"
#include "geometry_analytic.hpp"
#include "geometry_sampled_field.hpp"
#include "model.hpp"
#include "model_test_utils.hpp"
#include "utils.hpp"
#include <QFile>
#include <QImage>

using namespace sme;
using namespace sme::test;

SCENARIO("Analytic geometry", "[core/model/geometry_analytic][core/"
                              "model][core][model][geometry_analytic]") {
  GIVEN("SBML model with 2d analytic geometry") {
    auto s{getTestModel("analytic_2d")};
    REQUIRE(s.getGeometry().getIsValid() == true);
    REQUIRE(s.getGeometry().getHasImage() == true);
    const auto &img{s.getGeometry().getImage()};
    REQUIRE(img.colorCount() == 3);
    REQUIRE(img.size() == QSize(200, 200));
    REQUIRE(img.pixel(100, 100) == utils::indexedColours()[0].rgb());
    REQUIRE(img.pixel(80, 80) == utils::indexedColours()[1].rgb());
    REQUIRE(img.pixel(30, 20) == utils::indexedColours()[2].rgb());
  }
  GIVEN("SBML model with 3d analytic geometry") {
    auto s{getTestModel("analytic_3d")};
    REQUIRE(s.getGeometry().getIsValid() == true);
    REQUIRE(s.getGeometry().getHasImage() == true);
    const auto &img{s.getGeometry().getImage()};
    REQUIRE(img.colorCount() == 3);
    REQUIRE(img.size() == QSize(200, 200));
    REQUIRE(img.pixel(100, 100) == utils::indexedColours()[0].rgb());
    REQUIRE(img.pixel(80, 80) == utils::indexedColours()[1].rgb());
    REQUIRE(img.pixel(30, 20) == utils::indexedColours()[2].rgb());
  }
}
