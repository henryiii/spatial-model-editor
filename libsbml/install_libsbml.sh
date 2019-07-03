# script to download & compile libSBML experimental branch for linux, including spatial extension
# creates `sbml` dir containing header files and `libsbml-static.a` library in the current directory
svn co https://svn.code.sf.net/p/sbml/code/branches/libsbml-experimental tmp
cd tmp
mkdir build
cd build
cmake -G Ninja -DENABLE_SPATIAL=ON -DCMAKE_INSTALL_PREFIX=../install ..
ninja install
cd ../../
mv tmp/install/lib/libsbml-static.a .
mv tmp/install/include/sbml sbml
rm -rf tmp
