DEFINES += GITQLIENTPLUGIN_LIBRARY

# GitEditorPlugin files

CONFIG += qt warn_on c++17

SOURCES += \
    GitQlientPlugin.cpp

HEADERS += \
    GitQlientPlugin.h \
    GitQlientPlugin_global.h \
    GitQlientPluginConstants.h

INCLUDEPATH+= src

# Qt Creator linking

## Either set the IDE_SOURCE_TREE when running qmake,
## or set the QTC_SOURCE environment variable, to override the default setting
isEmpty(IDE_SOURCE_TREE): IDE_SOURCE_TREE = $$(QTC_SOURCE)
isEmpty(IDE_SOURCE_TREE): IDE_SOURCE_TREE = $$(HOME)"/Qt/qt-creator-opensource-src-4.10.0"

## Either set the IDE_BUILD_TREE when running qmake,
## or set the QTC_BUILD environment variable, to override the default setting
isEmpty(IDE_BUILD_TREE): IDE_BUILD_TREE = $$(QTC_BUILD)
isEmpty(IDE_BUILD_TREE): IDE_BUILD_TREE = $$(HOME)"/Qt/Tools/QtCreator"

## uncomment to build plugin into user config directory
## <localappdata>/plugins/<ideversion>
##    where <localappdata> is e.g.
##    "%LOCALAPPDATA%QtProjectqtcreator" on Windows Vista and later
##    "$XDG_DATA_HOME/data/QtProject/qtcreator" or "~/.local/share/data/QtProject/qtcreator" on Linux
##    "~/Library/Application Support/QtProject/Qt Creator" on OS X
# USE_USER_DESTDIR = yes

###### If the plugin can be depended upon by other plugins, this code needs to be outsourced to
###### <dirname>_dependencies.pri, where <dirname> is the name of the directory containing the
###### plugin's sources.

INCLUDEPATH += \
    GitQlient \
    GitQlient/app \
    GitQlient/QLogger

include(GitQlientPlugin_dependencies.pri)
include(GitQlient/app/GitQlient.pri)
include(GitQlient/QLogger/QLogger.pri)

###### End _dependencies.pri contents ######

include($$IDE_SOURCE_TREE/src/qtcreatorplugin.pri)
#include($$IDE_SOURCE_TREE/src/plugin/git/git_dependencies.pri)

RESOURCES += \
    GitQlientPlugin.qrc

OTHER_FILES += \
    $$PWD/README.md \
    $$PWD/.travis.yml
