QT += widgets
QT += network
CONFIG += c++17

RC_ICONS=icon.ico
DESTDIR = ./bin
QMAKE_PROJECT_DEPTH = 0
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adduseritem.cpp \
    applyfriend.cpp \
    applyfrienditem.cpp \
    applyfriendlist.cpp \
    applyfriendpage.cpp \
    bubbleframe.cpp \
    chatdialog.cpp \
    chatitembase.cpp \
    chatpage.cpp \
    chatuserlist.cpp \
    chatuserwid.cpp \
    chatview.cpp \
    clickedbtn.cpp \
    clickedlabel.cpp \
    clickedoncelabel.cpp \
    contactuserlist.cpp \
    conuseritem.cpp \
    customizeedit.cpp \
    findsuccessdlg.cpp \
    friendlabel.cpp \
    gloal.cpp \
    grouptipitem.cpp \
    httpmgl.cpp \
    listitembase.cpp \
    loadingdlg.cpp \
    longindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    messagetextedit.cpp \
    picturebubble.cpp \
    registerdialong.cpp \
    resetdialong.cpp \
    searchlist.cpp \
    statewidget.cpp \
    tcpmgr.cpp \
    textbubble.cpp \
    timerbtn.cpp \
    userdata.cpp \
    usermgr.cpp

HEADERS += \
    adduseritem.h \
    applyfriend.h \
    applyfrienditem.h \
    applyfriendlist.h \
    applyfriendpage.h \
    bubbleframe.h \
    chatdialog.h \
    chatitembase.h \
    chatpage.h \
    chatuserlist.h \
    chatuserwid.h \
    chatview.h \
    clickedbtn.h \
    clickedlabel.h \
    clickedoncelabel.h \
    contactuserlist.h \
    conuseritem.h \
    customizeedit.h \
    findsuccessdlg.h \
    friendlabel.h \
    gloal.h \
    grouptipitem.h \
    httpmgl.h \
    listitembase.h \
    loadingdlg.h \
    longindialog.h \
    mainwindow.h \
    messagetextedit.h \
    picturebubble.h \
    registerdialong.h \
    resetdialong.h \
    searchlist.h \
    singletion.h \
    statewidget.h \
    tcpmgr.h \
    textbubble.h \
    timerbtn.h \
    userdata.h \
    usermgr.h

FORMS += \
    adduseritem.ui \
    applyfriend.ui \
    applyfrienditem.ui \
    applyfriendpage.ui \
    chatdialog.ui \
    chatpage.ui \
    chatuserwid.ui \
    conuseritem.ui \
    findsuccessdlg.ui \
    friendlabel.ui \
    grouptipitem.ui \
    loadingdlg.ui \
    longindialog.ui \
    mainwindow.ui \
    registerdialong.ui \
    resetdialong.ui

# Default rules for deployment.
CONFIG(debug, debug | release) {
    TargetConfig = $${PWD}/config.ini
    OutputDir =  $${OUT_PWD}/$${DESTDIR}
    QMAKE_POST_LINK += $$QMAKE_COPY_FILE $$shell_path($${TargetConfig}) $$shell_path($${OutputDir}) $$escape_expand(\\n\\t)

    StaticDir = $${PWD}/static
    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_path($${StaticDir}) $$shell_path($${OutputDir})

}else{
    #release
    message("release mode")
    TargetConfig = $${PWD}/config.ini
    OutputDir =  $${OUT_PWD}/$${DESTDIR}
    QMAKE_POST_LINK += $$QMAKE_COPY_FILE $$shell_path($${TargetConfig}) $$shell_path($${OutputDir}) $$escape_expand(\\n\\t)

    StaticDir = $${PWD}/static
    QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_path($${StaticDir}) $$shell_path($${OutputDir})
}

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rec.qrc

DISTFILES += \
    config.ini
