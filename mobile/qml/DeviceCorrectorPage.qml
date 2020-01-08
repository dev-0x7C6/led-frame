import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import QtQuick.Layouts 1.14

import "components"

Item {
	ColumnLayout {
		spacing: 10
		anchors.fill: parent
		anchors.topMargin: 20
		anchors.bottomMargin: 20
		width: parent.width

		DisplayIcon {
			id: icon
			width: 200
			height: 200
			Layout.alignment: Qt.AlignCenter
			borderRadius: 4
			ambientRadius: 32
		}

		Item {
			Layout.fillHeight: true
			Layout.fillWidth: true

			ColumnLayout {
				anchors.fill: parent
				anchors.leftMargin: 30
				anchors.rightMargin: 30

				Slider {
					Layout.fillWidth: true
					id: lSlider
					value: 1
					from: 0
					to: 255
					stepSize: 1
					Material.accent: "white"

					onValueChanged: {
						if (pressed) configuration.globalLValue = value
						icon.color.a = position;
					}
				}

				Slider {
					id: rSlider
					live: false
					value: 1
					from: 0
					to: 255
					stepSize: 1
					Layout.fillWidth: true
					Material.accent: "tomato"
					onValueChanged: {
						if (pressed) configuration.globalRValue = value
						icon.color.r = position;
					}
				}

				Slider {
					id: gSlider
					live: false
					value: 1
					from: 0
					to: 255
					stepSize: 1
					Layout.fillWidth: true
					Material.accent: "lightGreen"
					onValueChanged: {
						if (pressed) configuration.globalGValue = value
						icon.color.g = position;
					}
				}

				Slider {
					id: bSlider
					live: false
					value: 1
					from: 0
					to: 255
					stepSize: 1
					Layout.fillWidth: true
					Material.accent: "lightBlue"
					onValueChanged: {
						if (pressed) configuration.globalBValue = value
						icon.color.b = position;
					}
				}
			}
		}
	}

	readonly property double l: configuration.globalLValue
	readonly property double r: configuration.globalRValue
	readonly property double g: configuration.globalGValue
	readonly property double b: configuration.globalBValue

	onLChanged: if (!lSlider.pressed) lSlider.value = l
	onRChanged: if (!rSlider.pressed) rSlider.value = r
	onGChanged: if (!gSlider.pressed) gSlider.value = g
	onBChanged: if (!bSlider.pressed) bSlider.value = b

	Component.onCompleted: {
		lSlider.value = configuration.globalLValue;
		rSlider.value = configuration.globalRValue;
		gSlider.value = configuration.globalGValue;
		bSlider.value = configuration.globalBValue;
	}
}


