#include "GitQlientPlugin.h"
#include "GitQlientPluginConstants.h"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/fancytabwidget.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <coreplugin/id.h>
#include <coreplugin/modemanager.h>
#include <extensionsystem/pluginmanager.h>

#include <QAction>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>

#include <coreplugin/coreconstants.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/minisplitter.h>
#include <coreplugin/navigationwidget.h>
#include <coreplugin/outputpane.h>
#include <coreplugin/rightpane.h>

#include <plugins/projectexplorer/projectexplorer.h>

#include <QDebug>
#include <GitQlient.h>

#include <projectexplorer/session.h>
#include <projectexplorer/project.h>

namespace GitQlientNamespace
{

namespace Internal
{

MyMode::MyMode()
{
   const Utils::Icon FLAT({ { ":/git.png", Utils::Theme::IconsBaseColor } });
   const Utils::Icon FLAT_ACTIVE({ { ":/git.png", Utils::Theme::IconsModeEditActiveColor } });
   setIcon(Utils::Icon::modeIcon(FLAT, FLAT, FLAT_ACTIVE));

   Core::Id id("GitEditor");

   setDisplayName("Git Editor");
   setPriority(88);
   setEnabled(true);
   setId(id);

   setWidget(mGitImpl = new GitQlient());

   mGitImpl->setObjectName("mainWindow");
}

GitQlientPlugin::GitQlientPlugin()
{
   // Create your members
}

GitQlientPlugin::~GitQlientPlugin()
{
   // Unregister objects from the plugin manager's object pool
   // Delete members
}

bool GitQlientPlugin::initialize(const QStringList &arguments, QString *errorString)
{
   // Register objects in the plugin manager's object pool
   // Load settings
   // Add actions to menus
   // Connect to other plugins' signals
   // In the initialize function, a plugin can be sure that the plugins it
   // depends on have initialized their members.

   Q_UNUSED(arguments)
   Q_UNUSED(errorString)

   myMode = new MyMode();

   connect(Core::ModeManager::instance(), &Core::ModeManager::currentModeAboutToChange, this, [this](Core::Id mode) {
      if (mode == myMode->id())
      {
         bool found;
         const auto workingDirectory = Utils::globalMacroExpander()->value("CurrentProject:Path", &found);
         const auto projects = ProjectExplorer::SessionManager::projects();
         QStringList currentOpenedProjects;

         for (auto project : projects)
         {
            qDebug() << project->projectDirectory().toString();
            currentOpenedProjects.append(project->projectDirectory().toString());
         }

         myMode->mGitImpl->setRepositories(currentOpenedProjects);

         Core::ModeManager::instance()->setFocusToCurrentMode();
      }
   });

   return true;
}

void GitQlientPlugin::extensionsInitialized()
{
   // Retrieve objects from the plugin manager's object pool
   // In the extensionsInitialized function, a plugin can be sure that all
   // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag GitQlientPlugin::aboutToShutdown()
{
   // Save settings
   // Disconnect from signals that are not needed during shutdown
   // Hide UI (if you add UI that is not in the main window directly)
   return SynchronousShutdown;
}

} // namespace Internal
} // namespace GitQlientPlugin
