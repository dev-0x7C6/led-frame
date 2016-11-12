#include <core/containers/application-info-container.h>
#include <core/emitters/concretes/emitter-manager.h>
#include <core/emitters/concretes/screen-emitter.h>
#include <core/emitters/factories/emitter-factory.h>
#include <core/networking/web-socket-server.h>
#include <gui/dialogs/about-dialog.h>
#include <gui/tray/system-tray.h>
#include "core/managers/main-manager.h"
#include "core/functionals/remote-controller.h"
#include "core/managers/session-manager.h"

#include <QApplication>
#include <QSettings>

#include <memory>

using namespace Container;
using namespace Enum;
using namespace Functional;
using namespace Manager;

using namespace Corrector::Concrete;
using namespace Corrector::Concrete::Manager;
using namespace Emitter::Concrete;
using namespace Emitter::Concrete::Manager;
using namespace Emitter::Factory;
using namespace Receiver::Concrete;
using namespace Receiver::Concrete::Manager;

int main(int argc, char *argv[]) {
	ApplicationInfoContainer info;
	QApplication application(argc, argv);
	application.setQuitOnLastWindowClosed(false);
	application.setApplicationName(info.applicationName());
	application.setApplicationVersion(info.versionToString());
	application.setApplicationDisplayName(QString("%1 %2").arg(info.applicationName(), info.versionToString()));

	QSettings settings(info.applicationName(), info.applicationName());

	MainManager manager(settings);
	SessionManager session(settings, manager);
	RemoteController controller(manager);

	Network::WebSocketConnectionManager webSocketServer(manager, controller);

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
