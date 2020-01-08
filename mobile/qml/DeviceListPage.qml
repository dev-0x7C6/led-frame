import QtQuick 2.14
import QtQuick.Controls 2.14

import "delegates"

Item {
	ListView {
		id: clientListView
		anchors.fill: parent
		model: clientModel
		clip: true
		delegate: DeviceDelegate {
			width: parent.width
			height: 100
			selected: clientListView.currentIndex === index
			opacity: 1.0
			source: "qrc:/devices/usb.png"

			MouseArea {
				anchors.fill: parent
				onClicked: {
					clientListView.currentIndex = index
					configuration.name = device
					onClientSelected(host, port, id)
				}
			}
		}
	}

	function insert(arg) {
		clientModel.append(arg)
	}
}
