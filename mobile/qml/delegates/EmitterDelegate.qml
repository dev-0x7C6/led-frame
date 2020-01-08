import QtQuick 2.14
import QtGraphicalEffects 1.14
import QtQuick.Layouts 1.14

import "../components"
import "../components/labels"
import "../components/layouts"
import "custom"
import "../js/functions.js" as Logic

BaseDelegate {
	property double iconOpacity : 1.0
	property bool iconRotation : false
	property string iconSource

	RowLayout {
		anchors.fill: parent

		Image {
			id: image
			opacity: iconOpacity
			Layout.fillHeight: true
			fillMode: Image.PreserveAspectFit
			source: iconSource

			RotationAnimator {
				target: image;
				from: 0;
				to: 360;
				loops: Animation.Infinite
				duration: 5000
				running: iconRotation
			}
		}

		Description {
			name: datagram.name
			detail: datagram.type
			description: datagram.name
		}
	}
}
