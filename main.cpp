#include "core/containers/application-info-container.h"
#include "core/functionals/remote-controller.h"
#include "core/managers/main-manager.h"
#include "core/managers/session-manager.h"
#include "core/networking/web-socket-server.h"
#include "gui/dialogs/about-dialog.h"
#include "gui/tray/system-tray.h"

#include <QApplication>
#include <QSettings>

using namespace Container;
using namespace Enum;
using namespace Functional;
using namespace Manager;
using namespace Network;

using namespace Corrector::Concrete;
using namespace Emitter::Concrete;
using namespace Receiver::Concrete;

int main(int argc, char *argv[]) {
	auto applicationName = QString(ApplicationInfoContainer::name());
	auto applicationVersion = QString::fromStdString(ApplicationInfoContainer::versionToString());

	QApplication application(argc, argv);
	application.setQuitOnLastWindowClosed(false);
	application.setApplicationName(applicationName);
	application.setApplicationVersion(applicationVersion);
	application.setApplicationDisplayName(QString("%1 %2").arg(applicationName, applicationVersion));

	QSettings settings(applicationName, applicationName);
	MainManager manager(settings);
	SessionManager session(settings, manager);
	RemoteController controller(manager);
	WebSocketConnectionManager webSocketServer(manager, controller);

	Tray::SystemTray tray;
	manager.attach(tray);

	tray.setAboutRequestCallback([] {
		static bool dialogGuardVisible = false;

		if (dialogGuardVisible)
			return;

		dialogGuardVisible = true;
		Widget::AboutDialog dialog;
		dialog.exec();
		dialogGuardVisible = false;
	});

	tray.setCloseRequestCallback([&application] { application.quit(); });

	manager.run();
	return application.exec();
}
