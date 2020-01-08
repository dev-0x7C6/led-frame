import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import QtQuick.Layouts 1.14

import "delegates"
import "../js/functions.js" as Logic

Item {
	id: canvas

	ColumnLayout {
		anchors.fill: canvas

		TabBar {
			id: tabBar
			Layout.fillWidth: true

			currentIndex: view.currentIndex
			font.pixelSize: 12

			TabButton {
				text: qsTr("Emitters")
			}
			TabButton {
				text: qsTr("Correctors")
			}
			TabButton {
				text: qsTr("Output")
			}
		}

		SwipeView {
			id: view
			Layout.fillWidth: true
			Layout.fillHeight: true
			currentIndex: tabBar.currentIndex

			ListView {
				id: emitterListView
				model: emitterModel
				currentIndex: -1
				clip: true

				delegate: EmitterDelegate {
					id: delegate
					width: parent.width
					height: 120
					iconSource: Logic.emitterIconFromType(datagram.type)
					iconRotation: emitterListView.currentIndex === index && datagram.type === "animation"
					selected: emitterListView.currentIndex === index
					opacity: emitterListView.currentIndex === index ? 1.0 : 0.4

					MouseArea {
						anchors.fill: parent
						onClicked: {
							emitterListView.currentIndex = index
							configuration.selectedEmitter = JSON.stringify(datagram)
							configuration.emitter = emitterModel.get(index).datagram.id
						}
					}
				}

				Component.onCompleted: {
					emitterModel.selectEmitter.connect(select)
				}

				function select(arg) {
					emitterListView.currentIndex = arg
				}

				ScrollIndicator.vertical: ScrollIndicator { }
			}

			ListView {
				id: correctorListView
				model: correctorModel
				currentIndex: -1
				clip: true

				delegate: CorrectorDelegate {
					height: configuration.device === datagram.owner ? 120 : 0
					visible: configuration.device === datagram.owner
					width: parent.width
					onValueChanged: {
						configuration.changeCorrector(datagram.id, value, value > datagram.min)
					}
				}
				ScrollIndicator.vertical: ScrollIndicator { }

				Component.onCompleted: {
					//correctorModel.updateItem.connect(select)
				}

				function select(arg) {
				}
			}

			DeviceCorrectorPage {
				id: correction
			}
		}
	}
}
