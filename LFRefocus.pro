#-------------------------------------------------
#
# Project created by QtCreator 2019-06-16T13:01:16
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = LFRefocus
TEMPLATE = app

MOC_DIR = $$OUT_PWD/mocfile
OBJECTS_DIR = $$OUT_PWD/objfile
UI_DIR = $$OUT_PWD/uifile
RCC_DIR = $$OUT_PWD/rcfile


SOURCES += main.cpp\
    maincontent/controlwidget/common.cpp \
    maincontent/controlwidget/imagelabel.cpp \
    maincontent/controlwidget/configparam.cpp \
    maincontent/controlwidget/loadimagewgt.cpp \
    maincontent/controlwidget/loadpos.cpp \
    maincontent/display.cpp \
    maincontent/controlwidget/refocus.cpp \
    maincontent/controlwidget/saveresult.cpp \
        mainwindow.cpp \
    maincontent/controlwidget/selectrendermodelwidget.cpp \
    titlebar/maintitlebar.cpp \
    titlebar/titlebar.cpp \
    maincontent/controltabwidget.cpp \
    maincontent/controltreewidget.cpp \
    systemtray/systemtray.cpp \
    flowlayout/flowlayout.cpp \
    qcustomplot/qcustomplot.cpp \
    numberanimation/numberanimation.cpp \
    screen/mainobject.cpp \
    screen/splashscreen.cpp


HEADERS  += mainwindow.h \
    maincontent/controlwidget/common.h \
    globaldef.hpp \
    maincontent/controlwidget/crashprocess.h \
    maincontent/controlwidget/imagelabel.h \
    maincontent/controlwidget/configparam.h \
    maincontent/controlwidget/loadimagewgt.h \
    maincontent/controlwidget/loadpos.h \
    maincontent/display.h \
    maincontent/controlwidget/refocus.h \
    maincontent/controlwidget/cudarefocus.h \
    maincontent/controlwidget/saveresult.h \
    maincontent/controlwidget/selectrendermodelwidget.h \
    titlebar/maintitlebar.h \
    titlebar/titlebar.h \
    maincontent/controltabwidget.h \
    maincontent/controltreewidget.h \
    systemtray/systemtray.h \
    flowlayout/flowlayout.h \
    qcustomplot/qcustomplot.h \
    numberanimation/numberanimation.h \
    screen/mainobject.h \
    screen/splashscreen.h


FORMS    += mainwindow.ui \
    maincontent/controlwidget/configparam.ui \
    maincontent/controlwidget/loadimage.ui \
    maincontent/controlwidget/loadpos.ui \
    maincontent/controlwidget/refocus.ui \
    maincontent/controlwidget/saveresult.ui \
    maincontent/controlwidget/selectrendermodelwidget.ui \
    titlebar/maintitlebar.ui \
    maincontent/controltabwidget.ui \
    maincontent/controltreewidget.ui \

RESOURCES += \
    res.qrc \
    res/shaders.qrc \
    res/textures.qrc

RC_FILE += myapp.rc

INCLUDEPATH+=F:\QT\opencv\build\include\opencv2 \
F:\QT\opencv\build\include\opencv \
F:\QT\opencv\build\include \


CONFIG(debug,debug|release) {
LIBS += -LF:\QT\opencv\build\x64\vc15\lib \
    -lopencv_world345 \
    -lopencv_world345d
} else {
LIBS += -LF:\QT\opencv\build\x64\vc15\lib \
-lopencv_world345 \
-lopencv_world345d
}
#LIBS += F:\QT\opencv\cvbase\lib\libopencv_*.a \




#=============CUDA==============================
# CUDA 设置
CUDA_SOURCES += maincontent/controlwidget/refocus.cu

CUDA_DIR = "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v11.1/"
SYSTEM_NAME = x64
SYSTEM_TYPE = 64
CUDA_ARCH = compute_60
CUDA_CODE = sm_60
NVCC_OPTIONS = --use_fast_math

# 头文件路径
INCLUDEPATH += "$$CUDA_DIR/include" \
"C:\ProgramData\NVIDIA Corporation\CUDA Samples\v11.1\common\inc"\

# 导入库文件路径
QMAKE_LIBDIR += "$$CUDA_DIR/lib/x64"
CUDA_INC = $$join(INCLUDEPATH,'" -I"','-I"','"')
# 依赖库

CUDA_LIB_NAMES += \
cuda \
cudadevrt \
cudart \


CONFIG(debug, debug|release) {
CUDA_LIB_NAMES += ucrtd \
}
else{
CUDA_LIB_NAMES += ucrt \
}

for(lib, CUDA_LIB_NAMES) {
    CUDA_LIBS += $$lib.lib
}
for(lib, CUDA_LIB_NAMES) {
    NVCC_LIBS += -l$$lib
}
LIBS += $$NVCC_LIBS


MSVCRT_LINK_FLAG_DEBUG   = "/MDd"
MSVCRT_LINK_FLAG_RELEASE = "/MD"

CONFIG(debug, debug|release) {
    # Debug 模式
    OBJECTS_DIR = debug/obj
    CUDA_OBJECTS_DIR = debug/cuda
    cuda_d.input = CUDA_SOURCES
    cuda_d.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda_d.commands = $$CUDA_DIR/bin/nvcc.exe -D_DEBUG $$NVCC_OPTIONS $$CUDA_INC $$LIBS \
                      --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -code=$$CUDA_CODE \
                      --compile -cudart static -g -DWIN32 -D_MBCS \
                      -Xcompiler "/wd4819,/EHsc,/W3,/nologo,/Od,/Zi,/RTC1" \
                      -Xcompiler $$MSVCRT_LINK_FLAG_DEBUG \
                      -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda_d.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda_d
}
else {
    # Release 模式
    OBJECTS_DIR = release/obj
    CUDA_OBJECTS_DIR = release/cuda
    cuda.input = CUDA_SOURCES
    cuda.output = $$CUDA_OBJECTS_DIR/${QMAKE_FILE_BASE}_cuda.o
    cuda.commands = $$CUDA_DIR/bin/nvcc.exe $$NVCC_OPTIONS $$CUDA_INC $$LIBS \
                    --machine $$SYSTEM_TYPE -arch=$$CUDA_ARCH -code=$$CUDA_CODE \
                    --compile -cudart static -D_MBCS \
                    -Xcompiler "/wd4819,/EHsc,/W3,/nologo,/O3,/Zi" \
                    -Xcompiler $$MSVCRT_LINK_FLAG_RELEASE \
                    -c -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
    cuda.dependency_type = TYPE_C
    QMAKE_EXTRA_COMPILERS += cuda
}

#-----------------crashed protect-------------------
LIBS += -lDbgHelp
