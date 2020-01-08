import QtQuick 2.14
import QtQuick.Controls 2.14

Item {
	id: main
	property int size : 72
	property int borderSize : 4
	property string color : "#000000"

	ListModel {
		id: colorListModel

		Component.onCompleted: {
			for (var i = 0; i < 240; i++) {
				var colorValue = Qt.hsla(i/240, 0.5, 0.5, 1);
				append({"colorValue":  colorValue.toString()})
			}
		}
	}

	GridView {
		id: grid
		anchors.fill: parent
		cellHeight: size
		cellWidth: size
		delegate: Rectangle {
			height: grid.cellHeight
			width: grid.cellWidth
			border.width: borderSize
			border.color: Qt.darker(colorValue, 2);
			color: colorValue;
			Behavior on opacity { NumberAnimation{} }
			opacity: GridView.isCurrentItem ? 1.0 : 0.33

			MouseArea {
				anchors.fill: parent
				onClicked: {
					grid.currentIndex = index
					main.color = colorValue
				}
			}
		}

		model: colorListModel

		ScrollIndicator.vertical: ScrollIndicator { }
	}
}
