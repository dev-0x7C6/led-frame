import QtQuick 2.0

Rectangle {
    id: whole
    width: 500
    height: 400
    color: "black"

    property int size: 100

    Item {
      id: scene
      anchors.centerIn: whole;
      width: 500
      height: 400


//      Item {
//        id: topStrip
//        y: 0
//        width: parent.width
//        height: 20

//        Repeater {
//          model: 16;
//          LedAmbient { id: led; x:(-size*0.5)+(width/16*index);
//              y:(-size*0.5); width: size; height: size;}
//        }
//      }

//      Item {
//        id: bottomStrip
//        y: parent.height
//        width: parent.width
//        height: 20

//        Repeater {
//          model: 16;
//          LedAmbient { id: led; x:(-size*0.5)+(width/16*index); y:-size*0.5; width: size; height: size;}
//        }
//      }


//      Item {
//        id: bottomStrip
//        anchors.bottom: scene
//        width: parent.width
//        height: 20

//        Repeater {
//          model: 16;
//          LedAmbient {  x:(-size*0.5)+(mantle.width/15*index); y:-size*0.5; width: size; height: size;}
//        }
//      }

//        Repeater {
//          model: 16;
//          LedAmbient { id: led; x:(-size*0.5); y:-size*0.5+(mantle.height/24*index); width: size; height: size;}
//        }

//        Repeater {
//          model: 16;
//          LedAmbient { id: led; x:(-size*0.5)+(mantle.width/15*index); y:size*0.5; width: size; height: size;}
//        }

//        Repeater {
//          model: 16;
//          LedAmbient { id: led; x:(-size*0.5); y:-size*0.5+(mantle.height/24*index); width: size; height: size;}
//        }



//        Row {
//          x: -size*0.38
//          y: size*0.60
//          spacing: -size*0.806
//          LedAmbient { id: led09; width: size; height: size; }
//          LedAmbient { id: led10; width: size; height: size; }
//          LedAmbient { id: led11; width: size; height: size; }
//          LedAmbient { id: led12; width: size; height: size; }
//          LedAmbient { id: led13; width: size; height: size; }
//          LedAmbient { id: led14; width: size; height: size; }
//          LedAmbient { id: led15; width: size; height: size; }
//          LedAmbient { id: led16; width: size; height: size; }
//        }

//        Column {
//          x: -size*0.38
//          y: -size*0.20
//          spacing: -size*0.905
//          LedAmbient { id: led17; width: size; height: size; }
//          LedAmbient { id: led18; width: size; height: size; }
//          LedAmbient { id: led19; width: size; height: size; }
//          LedAmbient { id: led20; width: size; height: size; }
//          LedAmbient { id: led21; width: size; height: size; }
//          LedAmbient { id: led22; width: size; height: size; }
//          LedAmbient { id: led23; width: size; height: size; }
//          LedAmbient { id: led24; width: size; height: size; }
//        }

//        Column {
//          x: size*0.98
//          y: -size*0.20
//          spacing: -size*0.905
//          LedAmbient { id: led25; width: size; height: size; }
//          LedAmbient { id: led26; width: size; height: size; }
//          LedAmbient { id: led27; width: size; height: size; }
//          LedAmbient { id: led28; width: size; height: size; }
//          LedAmbient { id: led29; width: size; height: size; }
//          LedAmbient { id: led30; width: size; height: size; }
//          LedAmbient { id: led31; width: size; height: size; }
//          LedAmbient { id: led32; width: size; height: size; }
//        }

        Image {
          anchors.centerIn: scene
          source: "qrc:/256x256/display.png"
        }


   }

   function onShit(arg) {
     led03.setColor(arg)
   }
}
