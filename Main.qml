import QtQuick
import qt_bodies
import QtQuick.Layouts
import QtGraphs
import QtQuick.Controls.Basic

Window {
    id: root

    width: 700
    height: 700
    visible: true
    title: qsTr("Hello World")

    NBodies {
        id: nbodies
        configuration: ":/qt/qml/qt_bodies/data/earth_moon.txt"
        stepTime: 3600
    }

    Timer {
        id: runningTimer
        running: false
        repeat: true
        interval: 17
        onTriggered: {
            nbodies.step();
            scatter.clear();
            for (var i = 0; i < nbodies.bodies.length; i++) {
                scatter.append(Qt.point(nbodies.bodies[i].dx, nbodies.bodies[i].dy));
            }
        }
    }

    GraphsView {
        anchors.fill: parent

        axisX: ValueAxis {
            min: -1.5
            max: 1.5
        }
        axisY: ValueAxis {
            min: -1.5
            max: 1.5
        }
        ScatterSeries {
            id: scatter
            color: "#00ff00"
            pointDelegate: Rectangle {
                width: 10
                height: 10
                radius: 5
                color: "yellow"
            }
        }
    }
    Rectangle {
        anchors.fill: guigrid
        color: "#33ff4400"
    }

    Grid {
        id: guigrid
        columns: 2
        Text {
            text: "timestep"
        }
        TextField {
            id: timestepInput
            validator: DoubleValidator {
                bottom: 1000
                top: 1e6
            }

            Component.onCompleted: {
                text = nbodies.stepTime
            }

            onTextChanged: {
                nbodies.stepTime = text
            }
        }
        Button {
            id: startstop
            text: runningTimer.running ? "Stop" : "Start"
            onClicked: {
                runningTimer.running = !runningTimer.running
            }
        }
        Button {
            id: reset
            text: "reset"
            onClicked: {
                nbodies.reset();
            }
        }
        Text {
            text: "configuration"
        }
        ComboBox {
            id: configurationSelect
            model: ListModel {
                ListElement { text: "Earth and Moon"; value: "earth_moon" }
                ListElement { text: "Sun, Earth and Moon"; value: "sun_earth_moon" }
                ListElement { text: "Sun, Mercury, Venus, Earth and Mars"; value: "sun_mercury_venus_earth_mars" }
                ListElement { text: "Solar System"; value: "solar_system" }
            }
            textRole: "text"
            valueRole: "value"
            onCurrentValueChanged: {
                runningTimer.running = false
                nbodies.configuration = `:/qt/qml/qt_bodies/data/${configurationSelect.currentValue}.txt`
            }
        }

        Text {
            text: "bodies"
        }

        Column {
            Repeater {
                id: repeater
                model: nbodies.bodies

                Text {
                    required property body modelData
                    text: modelData.name
                }
            }
        }
    }
}

// TODO
// - add table to show previous simulation steps
//  - data not yet available: should also be supplied by NBodies?
// - add configuration selector based on available data files from resources
// - show current configuration text contents
// - add configuration editor to create custom configurations
//  - probably requires more robust configuration processing
