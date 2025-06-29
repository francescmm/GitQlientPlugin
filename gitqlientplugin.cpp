#include "gitqlientpluginconstants.h"
#include "gitqlientplugintr.h"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <coreplugin/imode.h>
#include <coreplugin/rightpane.h>
#include <coreplugin/coreicons.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/session.h>
#include <projectexplorer/projectmanager.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectexplorer.h>
#include <utils/macroexpander.h>
#include <extensionsystem/iplugin.h>

//#include <GitQlient.h>

#include <QAction>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QIcon>

#include <QLogger.h>
#include <GitQlient.h>

using namespace Core;
using namespace Core::Internal;

namespace GitQlientPlugin::Internal {

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
    setIcon(Utils::Icon::combinedIcon({FLAT, FLAT}));
    setPriority(0);
    Utils::Id id("GitQlient");
    setId(id);
    setEnabled(true);

    setWidget(mGitImpl = new GitQlient());
    mGitImpl->setArgumentsPostInit({ "-noLog" });
    mGitImpl->setObjectName("mainWindow");
}

class GitQlientPluginPlugin final : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "GitQlientPlugin.json")

    GitQlientMode *mGitQlientMode = nullptr;

public:
    GitQlientPluginPlugin() = default;

    ~GitQlientPluginPlugin() = default;

    void initialize()
    {
        mGitQlientMode = new GitQlientMode();

        connect(Core::ModeManager::instance(), &Core::ModeManager::currentModeAboutToChange, this,
                &GitQlientPluginPlugin::aboutToChange);
    }

    void extensionsInitialized()
    {
        // Retrieve objects from the plugin manager's object pool, if needed. (rare)
        // In the extensionsInitialized function, a plugin can be sure that all
        // plugins that depend on it have passed their initialize() and
        // extensionsInitialized() phase.
    }

    ShutdownFlag aboutToShutdown()
    {
        // Save settings
        // Disconnect from signals that are not needed during shutdown
        // Hide UI (if you add UI that is not in the main window directly)
        return SynchronousShutdown;
    }

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

} // namespace GitQlientPlugin::Internal

#include <gitqlientplugin.moc>
