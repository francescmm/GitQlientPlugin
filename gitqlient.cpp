#include "gitqlientconstants.h"
#include "gitqlienttr.h"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <coreplugin/imode.h>
#include <coreplugin/coreicons.h>
#include <coreplugin/modemanager.h>
#include <projectexplorer/projectmanager.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <utils/macroexpander.h>
#include <extensionsystem/iplugin.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <cppeditor/cppeditorconstants.h>

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QIcon>

#include <GitQlient.h>

using namespace Core;
using namespace Core::Internal;
using namespace ProjectExplorer::Constants;

namespace GitQlient::Internal
{

class GitQlientMode final : public IMode
{
   Q_OBJECT

public:
   GitQlientMode();

   ~GitQlientMode() final = default;

   GitQlient *mGitImpl = nullptr;
};

GitQlientMode::GitQlientMode()
{
   const Utils::Icon FLAT({ { ":/git.png", Utils::Theme::IconsBaseColor } });

   setObjectName(QLatin1String("GitQlient"));
   setDisplayName(Tr::tr("GitQlient"));
   setIcon(Utils::Icon::combinedIcon({ FLAT, FLAT }));
   setPriority(0);
   Utils::Id id("GitQlient");
   setId(id);
   setEnabled(true);

   setWidget(mGitImpl = new GitQlient());
   mGitImpl->setArgumentsPostInit({ "-noLog" });
   mGitImpl->setObjectName("mainWindow");
}

class GitQlientClass final : public ExtensionSystem::IPlugin
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "GitQlient.json")

   GitQlientMode *mGitQlientMode = nullptr;
   QAction *mBlame = nullptr;

public:
   GitQlientClass() = default;

   ~GitQlientClass() = default;

   void initialize()
   {
      mBlame = new QAction(tr("Blame"), this);
      connect(mBlame, &QAction::triggered, this, &GitQlientClass::onBlame);

      auto am = Core::ActionManager::instance();
      Core::Context globalcontext(Core::Constants::C_GLOBAL);

      auto command = am->registerAction(mBlame, "Blame", globalcontext);
      command->setAttribute(Core::Command::CA_UpdateText);
      // TODO: Set a shortcut
      // command->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+F5")));

      auto menu = am->createMenu("GitQlient");
      menu->menu()->setTitle("GitQlient");
      menu->addAction(command);

      if (auto contextMenu = ActionManager::actionContainer(CppEditor::Constants::M_CONTEXT))
      {
         contextMenu->addSeparator();
         contextMenu->addMenu(menu);
      }

      auto fileMenu = ActionManager::actionContainer(M_FILECONTEXT);
      fileMenu->addMenu(menu, G_FILE_OTHER);

      mGitQlientMode = new GitQlientMode();

      connect(Core::ModeManager::instance(), &Core::ModeManager::currentModeAboutToChange, this,
              &GitQlientClass::aboutToChange);
   }

   void onBlame()
   {
      QMessageBox::information(Core::ICore::mainWindow(), tr("Blame"), tr("Hello from GitQlient!"));
   }

   ShutdownFlag aboutToShutdown() { return SynchronousShutdown; }

   void aboutToChange(Utils::Id mode)
   {
      if (mode == mGitQlientMode->id())
      {
         bool found;
         Utils::globalMacroExpander()->value("CurrentProject:Path", &found);
         QStringList currentOpenedProjects;

         for (auto project : ProjectExplorer::ProjectManager::projects())
            currentOpenedProjects.append(project->projectDirectory().toUrlishString());

         mGitQlientMode->mGitImpl->setRepositories(currentOpenedProjects);

         Core::ModeManager::instance()->setFocusToCurrentMode();
      }
   }
};

} // namespace GitQlient::Internal

#include <gitqlient.moc>
