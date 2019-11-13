wget -O qt5.zip https://github.com/francescmm/ci-utils/releases/download/qt_5.13/qt5.zip;
unzip -qq qt5.zip;
wget -O qtcreator.zip https://github.com/francescmm/ci-utils/releases/download/qt_5.13/qt-creator-opensource-src-4.10.0.zip;
unzip -qq qtcreator-src.zip
wget -O qtcreator.zip https://github.com/francescmm/ci-utils/releases/download/qt_5.13/Tools.zip;
unzip -qq qtcreator-bin.zip
export QTDIR=$PWD/qt5
export PATH=$QTDIR/bin:$PATH;
export QT_PLUGIN_PATH=$PWD/qt5/plugins;
$QTDIR/bin/qmake GitQlientPlugin.pro QTC_SOURCE=qtcreator-src QTC_BUILD=qtcreator-bin
make -j 4
ls -lsh
