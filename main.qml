import QtQuick 2.15
import QtMultimedia 5.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import App 1.0
ApplicationWindow {
    visible:true
    width:640
    height:480
    TestSql {
        id:tSql
    }

    ColumnLayout {
        width: parent.width
        height: parent.height
        Text {
            Layout.fillWidth:  parent
            horizontalAlignment: Text.AlignHCenter
            text:'DRIVER SQL AVAILABLE'
            font.bold: true
        }
        Text {
            id:log
            wrapMode:Text.WrapAnywhere
            Layout.fillWidth: parent
            Layout.fillHeight: parent
            horizontalAlignment: Text.AlignHCenter
            text:_drivers || ""
        }
        Text {
            wrapMode:Text.WrapAnywhere
            Layout.fillWidth: parent
            horizontalAlignment: Text.AlignHCenter
            text:"DATALOCATION"
            font.bold: true
        }
        TextArea {
            Layout.fillWidth: parent
            Layout.fillHeight: parent
            placeholderText: ""
            text: tSql.dataLocation()
            readOnly: true
        }
        ListView {
            Layout.fillWidth: parent
            Layout.fillHeight: parent
            Layout.minHeight:100
            model: QtMultimedia.availableCameras
            delegate: Text {
                text: modelData.displayName
            }
        }
        RowLayout {
            Layout.fillWidth: parent
            Button {
                text: "OPEN"
                Layout.fillWidth: parent
                onClicked: log.text = tSql.openPsql()
            }
            Button {
                text: "CLOSE"
                Layout.fillWidth: parent
                onClicked: log.text = tSql.closePsql()
            }
            Button {
                text: "VERSION"
                Layout.fillWidth: parent
                onClicked: log.text=tSql.versionPsql()
            }

        }
        RowLayout {
            Layout.fillWidth: parent
            Button {
                text: "WRITEBLE"
                Layout.fillWidth: parent
                onClicked: log.text=tSql.writebleAppLocation()
            }
            Button {
                text: "COPY"
                Layout.fillWidth: parent
                onClicked: log.text=tSql.copyDB()
            }
            Button {
                text: "QUIT"
                Layout.fillWidth: parent
                onClicked: Qt.quit()
            }
        }
    }
}
