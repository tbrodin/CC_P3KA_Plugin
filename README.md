# CC_P3KA_Plugin


Add this line in CMakeList.txt in  source ode directory (..\CloudCompare\plugins\)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../CC_P3KA_Plugin ${CMAKE_CURRENT_BINARY_DIR}/CC_P3KA_Plugin)
 
Set CMAKE_INSTALL_PREFIX to other directory than "Program Files" since you might not have access to write to this directory.

Make sure you have the correct default verions of QT (qmake) set.
Run qmake --version in cmd to se active QT-version.
If this is incorrect you might need to add correct version to Environment PATH and reorder it so that i ends up before the other existing installations of qmake

Place the folder of this project at a sibling level to the folder where the source code of CloudCompare is.


Generate the CloudCompare project using cmake (for example using Cmake-gui) according to the CloudCompare projects README. If everyting is done correctly you will be able to choose to generate the Plugin project together with the CloudCompare project.