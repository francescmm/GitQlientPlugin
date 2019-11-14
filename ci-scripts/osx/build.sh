export LDFLAGS="-L/usr/local/opt/qt/lib"
export CPPFLAGS="-I/usr/local/opt/qt/include"
wget -O qtcreator-src.zip https://github.com/francescmm/ci-utils/releases/download/qt_5.13/qt-creator-opensource-src-4.10.0.zip;
unzip -qq qtcreator-src.zip
wget -O qtcreator-bin.zip https://github.com/francescmm/ci-utils/releases/download/qt_5.13/qt-creator-bin-osx.zip;
unzip -qq qtcreator-bin.zip
export QT_CREATOR=Tools/QtCreator/bin
export PATH="/usr/local/opt/qt/bin:$QT_CREATOR:$PATH"
export QT_PLUGIN_PATH=$PWD/qt5/plugins;
source ~/.bash_profile
qmake QTC_SOURCE=./qt-creator-opensource-src-4.10.0 QTC_BUILD=. GitQlientPlugin.pro
make
cp Tools/QtCreator/bin/Qt\ Creator.app/Contents/PlugIns/libGitQlientPlugin.dylib libGitQlientPlugin-qtc${QTC_VERSION}_${TRAVIS_OS_NAME}.dylib
