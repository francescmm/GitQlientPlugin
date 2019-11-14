wget -O qt5.zip https://github.com/francescmm/ci-utils/releases/download/qt_5.13/qt5.zip;
unzip -qq qt5.zip;
wget -O qtcreator-src.zip https://github.com/francescmm/ci-utils/releases/download/qt_5.13/qt-creator-opensource-src-4.10.0.zip;
unzip -qq qtcreator-src.zip
wget -O qtcreator-bin.zip https://github.com/francescmm/ci-utils/releases/download/qt_5.13/Tools.zip;
unzip -qq qtcreator-bin.zip
export QTDIR=$PWD/qt5
export QT_CREATOR=$HOME/Qt/Tools/QtCreator/bin
export PATH=$QTDIR/bin:$QT_CREATOR:$PATH;
export QT_PLUGIN_PATH=$PWD/qt5/plugins;
$QTDIR/bin/qmake QTC_SOURCE=./qt-creator-opensource-src-4.10.0 QTC_BUILD=./Tools GitQlientPlugin.pro
make -j 4
ls -lsh Tools/QtCreator/lib/qtcreator/plugins/libGitQlientPlugin*
