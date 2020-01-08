
function emitterIconFromType(arg) {
  switch (arg) {
  case "animation":
    return "qrc:/emitters/animation.png"
    case "display": return "qrc:/emitters/display.png"
    case "image": return "qrc:/emitters/landscape.png"
    case "color": return "qrc:/emitters/color.png"
    case "camera": return "qrc:/emitters/camera.png"
    case "off": return "qrc:/emitters/off.png"
  }
}

function correctorTextFromType(arg) {
  switch (arg) {
  case "color_enhancer":
    return "Color enhancer"
    case "brightness": return "Brightness"
    case "red_channel": return "Red"
    case "green_channel": return "Green"
    case "blue_channel": return "Blue"
    case "flickr_effect": return "Flickr effect"
  }

  return arg;
}

function correctorIconFromType(arg) {
  switch (arg) {
  case "color_enhancer":
    return "qrc:/correctors/color-enhancer.png"
    case "brightness": return "qrc:/correctors/brightness.png"
    case "red_channel": return "qrc:/correctors/red.png"
    case "green_channel": return "qrc:/correctors/green.png"
    case "blue_channel": return "qrc:/correctors/blue.png"
    case "flickr_effect": return "qrc:/correctors/flickr.png"
    case "backlight": return "qrc:/correctors/backlight.png"
    case "gamma": return "qrc:/correctors/gamma.png"
  }

  return arg;
}
