QT += charts

SOURCES += \
    main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/charts/splinechart
INSTALLS += target

HEADERS += \
    data.hpp \
    filter.hpp \
    filter_order1.h \
    filter_order5.h
