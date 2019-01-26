HEADERS       = glwidget.h \
                window.h \
                serialport.h

SOURCES       = glwidget.cpp \
                main.cpp \
                window.cpp \
                serialport.cpp

RESOURCES     = textures.qrc

QT           += widgets serialport

LIBS += -LD:\install\qt5.8\5.8\mingw53_32\bin \
        -LD:\install\qt5.8\5.8\mingw53_32\lib \
        -lQt5SerialPortd

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/textures
INSTALLS += target
