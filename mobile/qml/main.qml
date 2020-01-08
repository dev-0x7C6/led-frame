import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import QtWebSockets 1.14

import "components"

ApplicationWindow {
	id: window
	height: 520
	width: 360
	visible: true

	Material.background: Qt.darker("#101020", 2)

	Image {
		id: background
		anchors.centerIn: parent
		width: parent.width
		source: "qrc:/background/background-blured.png"
		fillMode: Image.PreserveAspectFit
		cache: true
		opacity: 0.025
	}

	WebSocket {
		id: webSocket

		onTextMessageReceived: configuration.recv(message)

		onStatusChanged: {
			if (webSocket.status == WebSocket.Error) {
				mainStackView.pop(deviceListPage)
				url = ""
			}

			if (webSocket.status == WebSocket.Open) {
				mainStackView.push(deviceControlPage)
				emitterModel.clear()
				correctorModel.clear()
			}
		}

		function connect(host, port) {
			webSocket.url = ""
			var address = "ws://"
			address = address.concat(host, ":", port);
			webSocket.url = address
			webSocket.active = true
		}
	}

	ListModel { id: clientModel }
	ListModel { id: emitterModel; signal selectEmitter(int index); }
	ListModel { id: correctorModel; }

	Configuration {
		id: configuration

		function commit(command) {
			if (webSocket.status == WebSocket.Open)
				webSocket.sendTextMessage(JSON.stringify(command))
		}

		function correctorAttached(arg) {
			correctorModel.append(arg)

			if (arg.datagram.owner !== -1)
				return;

			switch (arg.datagram.type) {
			case "brightness":
				globalLValue = arg.datagram.factor;
				globalLId = arg.datagram.id;
				break;
			case "red_channel":
				globalRValue = arg.datagram.factor;
				globalRId = arg.datagram.id;
				break;
			case "green_channel":
				globalGValue = arg.datagram.factor;
				globalGId = arg.datagram.id;
				break;
			case "blue_channel":
				globalBValue = arg.datagram.factor;
				globalBId = arg.datagram.id;
				break;
			}
		}

		function correctorModified(arg) {
			for (var i = 0; i < correctorModel.count; ++i)
			{
				if (correctorModel.get(i).datagram.id === arg.datagram.id) {
					correctorModel.set(i, JSON.parse(JSON.stringify(arg)))
					correctorModel.get(i).value = arg.datagram.factor;
				}
			}

			if (arg.datagram.owner !== -1)
				return;

			switch (arg.datagram.type) {
			case "brightness":
				globalLValue = arg.datagram.factor;
				globalLId = arg.datagram.id;
				break;
			case "red_channel":
				globalRValue = arg.datagram.factor;
				globalRId = arg.datagram.id;
				break;
			case "green_channel":
				globalGValue = arg.datagram.factor;
				globalGId = arg.datagram.id;
				break;
			case "blue_channel":
				globalBValue = arg.datagram.factor;
				globalBId = arg.datagram.id;
				break;
			}
		}

		function emitterAttached(arg) {
			emitterModel.append(arg)
			for (var i = 0; i < emitterModel.count; ++i)
				if (emitterModel.get(i).datagram.id  === configuration.emitter)
					emitterModel.selectEmitter(i)
		}

		function receiverAttached(arg) {
			emitter = arg.datagram.emitter;
			for (var i = 0; i < emitterModel.count; ++i)
				if (emitterModel.get(i).datagram.id  === configuration.emitter)
					emitterModel.selectEmitter(i)
		}

		function receiverModified(arg) {
			emitter = arg.datagram.emitter;
			for (var i = 0; i < emitterModel.count; ++i)
				if (emitterModel.get(i).datagram.id  === configuration.emitter)
					emitterModel.selectEmitter(i)
		}

		function emitterDetached(arg) {}

		function recv(arg) {
			fetch(arg)
		}
	}

	header: ApplicationHeader {
		id: applicationHeader
	}

	ApplicationDrawer {

	}

	StackView {
		id: mainStackView
		anchors.fill: parent

		initialItem: WaitPage {
			id: waitingForConnectionPage
			text: "Searching for devices..."
		}

		DeviceListPage {
			id: deviceListPage
			visible: false

			function onClientSelected(host, port, id) {
				configuration.device = id
				webSocket.connect(host, port)
			}

		}

		Component {
			id: deviceControlPage
			DeviceControlPage {
			}
		}

		onCurrentItemChanged: {
			applicationHeader.title = configuration.name
			if (currentItem == deviceListPage) applicationHeader.title = "Available devices"
			if (currentItem == waitingForConnectionPage) applicationHeader.title = "Searching..."

			if (currentItem == deviceListPage) {
				if (webSocket.status == WebSocket.Open)
					webSocket.active = false;
			}
		}
	}

	Connections {
		target: broadcast

		onDeviceDetected: {
			if (mainStackView.currentItem == waitingForConnectionPage)
				mainStackView.push(deviceListPage)
			deviceListPage.insert(arg)
		}
	}
}
