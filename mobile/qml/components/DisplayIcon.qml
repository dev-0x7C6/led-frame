import QtQuick 2.14
import QtGraphicalEffects 1.14

Item {
	property color color: "orange"
	property double borderRadius: 4
	property double ambientRadius: 16
	readonly property double marginSize: 32

	RectangularGlow {
		id: effect
		anchors.topMargin: marginSize
		anchors.bottomMargin: marginSize
		anchors.leftMargin: marginSize
		anchors.rightMargin: marginSize
		anchors.fill: parent
		glowRadius: ambientRadius
		spread: 0.1
		color: parent.color
		cornerRadius: glowRadius * 1.5
		cached: false
	}

	Image {
		id: image
		anchors.fill: parent
		fillMode: Image.PreserveAspectFit
		source: "qrc:/splash/desktop.png"
		smooth: true
		cache: false
	}
}
