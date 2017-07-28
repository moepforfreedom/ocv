INCLUDEPATH += . C:\opencv-build\install\include
TEMPLATE = app
CONFIG += console warn_on


LIBS += C:\opencv-build\bin\libopencv_calib3d320.dll
LIBS += C:\opencv-build\bin\libopencv_flann320.dll
LIBS += C:\opencv-build\bin\libopencv_highgui320.dll
LIBS += C:\opencv-build\bin\libopencv_ml320.dll
LIBS += C:\opencv-build\bin\libopencv_objdetect320.dll
LIBS += C:\opencv-build\bin\libopencv_photo320.dll
LIBS += C:\opencv-build\bin\libopencv_shape320.dll
LIBS += C:\opencv-build\bin\libopencv_stitching320.dll
LIBS += C:\opencv-build\bin\libopencv_videostab320.dll
LIBS += C:\opencv-build\bin\libopencv_core320.dll
LIBS += C:\opencv-build\bin\libopencv_imgcodecs320.dll
LIBS += C:\opencv-build\bin\libopencv_imgproc320.dll
LIBS += C:\opencv-build\bin\libopencv_features2d320.dll
LIBS += C:\opencv-build\bin\libopencv_superres320.dll
LIBS += C:\opencv-build\bin\libopencv_video320.dll
LIBS += C:\opencv-build\bin\libopencv_videoio320.dll
LIBS += C:\opencv-build\bin\libopencv_optflow320.dll


QT += gui
QT += core
QT += widgets

HEADERS += commontypes.h interpolate.h \
           mainwindow.h \
           vid.h \
           rollingshutter.h \
           guicontrols\roll_time_group.h \
           guicontrols\exportgroup.h \
           guicontrols\interpolatecontrolgroup.h

SOURCES += main.cpp interpolate.cpp \
           mainwindow.cpp \
           vid.cpp \
           rollingshutter.cpp \
           guicontrols\roll_time_group.cpp \
           guicontrols\exportgroup.cpp \
           guicontrols\interpolatecontrolgroup.cpp

TARGET = ocv
INSTALLS += target

