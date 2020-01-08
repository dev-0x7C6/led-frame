import QtGraphicalEffects 1.14
import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import QtQuick.Layouts 1.14

import "../js/functions.js" as Logic
import "../components"
import "../components/labels"
import "../components/layouts"
import ".."

BaseDelegate {
	id: base
	property int min: datagram.min
	property int max: datagram.max
	property int value: datagram.factor
	property string type : datagram.type;

	selected: slider.from !== slider.value

	RowLayout {
		anchors.fill: parent
		anchors.rightMargin: 40

		Image {
			id: image
			source: Logic.correctorIconFromType(type);
			Layout.fillHeight: true
			Layout.maximumWidth: 120
			fillMode: Image.PreserveAspectFit
			opacity: slider.position + 0.3
			Behavior on opacity { NumberAnimation{} }
		}

		ColumnLayout {
			LayoutExpander {}

			LargeLabel {
				property string title: (selected) ? " (" + parseInt(slider.position * 100) + "%)" : " (off)"
				font.pixelSize: 14
				opacity: base.selected ? 1.0 : 0.3
				text: Logic.correctorTextFromType(type) + title;
			}

			Slider {
				id: slider
				Layout.fillWidth: true
				from: base.min
				to: base.max
				stepSize: 1
				value: base.value
				onValueChanged: base.value = value
				Behavior on opacity { NumberAnimation{} }
			}

			LayoutExpander {}
		}
	}
}
