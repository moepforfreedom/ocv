INCLUDEPATH += . R:\OpenCV\opencv_repo\opencv\build_vc17\install\include
TEMPLATE = app
CONFIG += console warn_on


LIBS += R:\OpenCV\opencv_repo\opencv\build_vc17\install\x64\vc15\lib\opencv_world343.lib


QT += gui
QT += core
QT += widgets

HEADERS += commontypes.h ./videoproc/interpolate.h inputparser.h videoproc/vid.h \

           
SOURCES += main_test.cpp ./videoproc/interpolate.cpp ./videoproc/vid.cpp\


TARGET = upsampvid
target.path += ./install/
INSTALLS += target

