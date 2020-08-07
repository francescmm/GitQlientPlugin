wget -O qtcreator-src.zip https://github.com/francescmm/ci-utils/releases/download/qt/qt-creator-opensource-src-${QTC_VERSION}.0.zip;
unzip -qq qtcreator-src.zip
mv /Applications/Qt\ Creator.app/ /Applications/QtCreator.app/
export QT_CREATOR=/Applications/QtCreator.app/
export PATH="/usr/local/opt/qt/bin:$QT_CREATOR:$PATH"
export QT_PLUGIN_PATH=$PWD/qt5/plugins;
qmake QTC_SOURCE=./qt-creator-opensource-src-${QTC_VERSION}.0 QTC_BUILD=${QT_CREATOR} GitQlientPlugin.pro
make
cp /Applications/QtCreator.app/Contents/PlugIns/libGitQlientPlugin.dylib libGitQlientPlugin_$PLUGIN_VERSION-qtc${QTC_COMPAT}.0.dylib
