import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: abc
    width: 300
    height: 300


    property color initColor: "lightblue"
    property color goColor: "green"

    RadialGradient {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: initColor }
            GradientStop { position: 0.5; color: "#00000000" }
        }
    }

    signal data(color c)
    onData: {
      goColor = c
      animateColor.start()
    }


   PropertyAnimation {
     id: animateColor
     target: abc
     properties: "initColor"
     to: goColor
     duration: 13


   }

   function setColor(col) {
     initColor = col
   }


   MouseArea {
       anchors.fill: parent
       onClicked: {
           animateColor.start()
       }
   }


}
