#pragma once

#include <coreplugin/imode.h>

#include <extensionsystem/iplugin.h>

using namespace Core;

class GitQlient;

namespace GitQlientNamespace
{
namespace Internal
{

class GitQlientMode : public Core::IMode
{
   Q_OBJECT

public:
   GitQlientMode();

   GitQlient *mGitImpl = nullptr;
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
   GitQlientMode *mGitQlientMode = nullptr;
   QString mCurrentProject;

   void aboutToChange(Core::Id mode);
};

} // namespace Internal
} // namespace GitQlientPlugin
