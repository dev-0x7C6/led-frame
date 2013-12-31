import QtQuick 2.0

Rectangle {
    width: 380
    height: 380
    color: "black"

    property int size: 160

    Item {
      id: whole
      x: 0
      y: 0
      width :380
      height :380

      Item {
        anchors.centerIn: whole
        width: 256
        height: 256

        Row {
          x: -size*0.38
          y: -size*0.32
          spacing: -size*0.806
          LedAmbient { id: led01; width: size; height: size; }
          LedAmbient { id: led02; width: size; height: size; }
          LedAmbient { id: led03; width: size; height: size; }
          LedAmbient { id: led04; width: size; height: size; }
          LedAmbient { id: led05; width: size; height: size; }
          LedAmbient { id: led06; width: size; height: size; }
          LedAmbient { id: led07; width: size; height: size; }
          LedAmbient { id: led08; width: size; height: size; }
        }

        Row {
          x: -size*0.38
          y: size*0.60
          spacing: -size*0.806
          LedAmbient { id: led09; width: size; height: size; }
          LedAmbient { id: led10; width: size; height: size; }
          LedAmbient { id: led11; width: size; height: size; }
          LedAmbient { id: led12; width: size; height: size; }
          LedAmbient { id: led13; width: size; height: size; }
          LedAmbient { id: led14; width: size; height: size; }
          LedAmbient { id: led15; width: size; height: size; }
          LedAmbient { id: led16; width: size; height: size; }
        }

        Column {
          x: -size*0.38
          y: -size*0.20
          spacing: -size*0.905
          LedAmbient { id: led17; width: size; height: size; }
          LedAmbient { id: led18; width: size; height: size; }
          LedAmbient { id: led19; width: size; height: size; }
          LedAmbient { id: led20; width: size; height: size; }
          LedAmbient { id: led21; width: size; height: size; }
          LedAmbient { id: led22; width: size; height: size; }
          LedAmbient { id: led23; width: size; height: size; }
          LedAmbient { id: led24; width: size; height: size; }
        }

        Column {
          x: size*0.98
          y: -size*0.20
          spacing: -size*0.905
          LedAmbient { id: led25; width: size; height: size; }
          LedAmbient { id: led26; width: size; height: size; }
          LedAmbient { id: led27; width: size; height: size; }
          LedAmbient { id: led28; width: size; height: size; }
          LedAmbient { id: led29; width: size; height: size; }
          LedAmbient { id: led30; width: size; height: size; }
          LedAmbient { id: led31; width: size; height: size; }
          LedAmbient { id: led32; width: size; height: size; }
        }

        Image {
          x: 0
          y: 0
          source: "qrc:/256x256/display.png"
        }
     }
   }


   function onShit(arg) {
     led03.setColor(arg)
   }
}
