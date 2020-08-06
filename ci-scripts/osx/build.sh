export LDFLAGS="-L/usr/local/opt/qt/lib"
export CPPFLAGS="-I/usr/local/opt/qt/include"
wget -O qtcreator-src.zip https://github.com/francescmm/ci-utils/releases/download/qt/qt-creator-opensource-src-${QTC_VERSION}.0.zip;
unzip -qq qtcreator-src.zip
wget -O qtcreator-bin.zip https://github.com/francescmm/ci-utils/releases/download/qt/qt-creator-bin-osx.zip;
unzip -qq qtcreator-bin.zip
export QT_CREATOR=Tools/QtCreator/bin
export PATH="/usr/local/opt/qt/bin:$QT_CREATOR:$PATH"
export QT_PLUGIN_PATH=$PWD/qt5/plugins;
export PLUGIN_VERSION="1.0.1";
source ~/.bash_profile
qmake QTC_SOURCE=./qt-creator-opensource-src-${QTC_VERSION}.0 QTC_BUILD=. GitQlientPlugin.pro
make
cp bin/Qt\ Creator.app/Contents/PlugIns/libGitQlientPlugin.dylib libGitQlientPlugin_$PLUGIN_VERSION-qtc${QTC_COMPAT}.0.dylib
