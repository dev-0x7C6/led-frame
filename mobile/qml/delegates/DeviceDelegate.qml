import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "../components"
import "../components/labels"
import "../components/layouts"
import "custom"

BaseDelegate {
	id: delegate
	property string source

	RowLayout {
		anchors.fill: parent

		Image {
			Layout.fillHeight: true
			fillMode: Image.PreserveAspectFit
			source: delegate.source
		}

		Description {
			name: device;
			detail: "computer: " + computer;
			description: "hostname: " + host + ":" + port;
		}
	}
}
