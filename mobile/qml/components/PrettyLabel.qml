import QtQuick 2.14
import QtQuick.Controls 2.14

import "../components/labels"

Item {
	property alias text: title.text
	property alias fontSize : title.font.pixelSize

	Image {
		source: "qrc:/devices/title.png"
		anchors.fill: parent
		fillMode: Image.Stretch
		opacity: 0.25
	}

	LargeLabel {
		id: title
		anchors.centerIn: parent
	}
}
