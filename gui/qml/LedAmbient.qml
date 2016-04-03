import QtQuick 2.2
import QtGraphicalEffects 1.0

RadialGradient {
    property color sample: "#00000000"
    gradient: Gradient {
        GradientStop { position: 0.0; color: sample }
        GradientStop { position: 0.5; color: "#00000000" }
    }
}
