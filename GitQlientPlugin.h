#ifndef GitQlientPlugin_H
#define GitQlientPlugin_H

#include "GitQlientPlugin_global.h"
#include <coreplugin/imode.h>

#include <extensionsystem/iplugin.h>
#include <coreplugin/idocument.h>

using namespace Core;

class MainWindow;

namespace GitQlientPlugin
{
namespace Internal
{

class MyMode : public Core::IMode
{
   Q_OBJECT

public:
   MyMode();

   MainWindow *mGitImpl = nullptr;
};

class GitQlientPlugin : public ExtensionSystem::IPlugin
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "GitQlientPlugin.json")

public:
   GitQlientPlugin();
   ~GitQlientPlugin() override;

   bool initialize(const QStringList &arguments, QString *errorString) override;
   void extensionsInitialized() override;
   ShutdownFlag aboutToShutdown() override;

private:
   MyMode *myMode = nullptr;
   QString mCurrentProject;
};

} // namespace Internal
} // namespace GitQlientPlugin

#endif // GitQlientPlugin_H
