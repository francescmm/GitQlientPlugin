#include "GitQlientPlugin.h"

#include <coreplugin/modemanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <utils/macroexpander.h>
#include <projectexplorer/session.h>
#include <projectexplorer/project.h>
#include <coreplugin/editormanager/editormanager.h>

#include <GitQlient.h>

#include <utils/icon.h>

namespace GitQlientNamespace
{
namespace Internal
{

GitQlientMode::GitQlientMode()
{
   const Utils::Icon FLAT({ { ":/git.png", Utils::Theme::IconsBaseColor } });
   const Utils::Icon FLAT_ACTIVE({ { ":/git.png", Utils::Theme::IconsModeEditActiveColor } });
   setIcon(Utils::Icon::modeIcon(FLAT, FLAT, FLAT_ACTIVE));

   Core::Id id("GitQlient");

   setDisplayName("GitQlient");
   setPriority(0);
   setEnabled(true);
   setId(id);

   setWidget(mGitImpl = new GitQlient({ "-noLog" }));

   mGitImpl->setObjectName("mainWindow");

   connect(mGitImpl, &GitQlient::signalEditDocument, [](const QString &fileName, int row, int col) {
      Core::EditorManager::instance()->openEditorAt(fileName, row, col);
   });
}

GitQlientPlugin::GitQlientPlugin() { }

GitQlientPlugin::~GitQlientPlugin() { }

bool GitQlientPlugin::initialize(const QStringList &arguments, QString *errorString)
{
   Q_UNUSED(arguments)
   Q_UNUSED(errorString)

   mGitQlientMode = new GitQlientMode();

   connect(Core::ModeManager::instance(), &Core::ModeManager::currentModeAboutToChange, this,
           &GitQlientPlugin::aboutToChange);

   return true;
}

void GitQlientPlugin::aboutToChange(Core::Id mode)
{
   if (mode == mGitQlientMode->id())
   {
      bool found;
      const auto workingDirectory = Utils::globalMacroExpander()->value("CurrentProject:Path", &found);
      QStringList currentOpenedProjects;

      for (auto project : ProjectExplorer::SessionManager::projects())
         currentOpenedProjects.append(project->projectDirectory().toString());

      mGitQlientMode->mGitImpl->setRepositories(currentOpenedProjects);

      Core::ModeManager::instance()->setFocusToCurrentMode();
   }
}

void GitQlientPlugin::extensionsInitialized() { }

ExtensionSystem::IPlugin::ShutdownFlag GitQlientPlugin::aboutToShutdown()
{
   return SynchronousShutdown;
}

} // namespace Internal
} // namespace GitQlientPlugin
