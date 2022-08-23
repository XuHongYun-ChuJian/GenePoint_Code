import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.4

Window {
    width: 800
    height: 600
    visible: true

    Rectangle {
        id: mainRect
        anchors.fill: parent
        property var mainRectComponent: null

        Column {
            id: mainColumn
            spacing: 5

            width: parent.width
            property var count: 0

            function deleteItems(object) {
                object.destroy()
            }

            function createItem() {
                var color = 'red'
                if (mainColumn.count % 3 === 1)
                    color = 'yellow'
                else if (mainColumn.count % 3 === 2)
                    color = 'blue'
                mainColumn.count++

                // 创建一个组件
                var obj = itemCompont.createObject(mainColumn, {"color": color, "width": mainRect.width})
                //obj.setCurentObject(obj)
                obj.setCurrentText('Component' + mainColumn.count.toString())
                obj.deleteThis.connect(mainColumn.deleteItems)

                // 创建文件中的组件
                var obj2 = mainRect.mainRectComponent.createObject(mainColumn,
                                                        {'color': Qt.rgba(0.4, 0.8, 0.6, 1.0)
                                                        ,'width': mainRect.width})
                obj2.setCurrentText('Component' + mainColumn.count.toString() + ', From File TestComponent')
                obj2.deleteThis.connect(mainColumn.deleteItems)
            }
        }

        Button {
            anchors.top: mainColumn.bottom
            anchors.topMargin: 10
            anchors.right: mainRect.right
            anchors.rightMargin: 10
            text: '添加'

            onClicked: {
                mainColumn.createItem()
            }
        }

        Component.onCompleted: {
            if (mainRectComponent == null)
                mainRectComponent = mainRectComponent = Qt.createComponent('qrc:/QML/TestCompont.qml')
        }

        Component {
            id: itemCompont
            Rectangle {
                id: compontRect
                color: 'blue'
                implicitWidth: 200
                implicitHeight: 50
                property var currentObject: ''

                signal deleteThis(var obj)

                // 设置文字的内容
                function setCurrentText(textName) {
                    interText.text = textName
                }

                Text {
                    id: interText
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("text")
                }
                Button {
                    anchors.margins: 5
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    text: '删除'

                    onClicked: {
                        compontRect.deleteThis(compontRect)
                    }
                }

                Component.onCompleted: {
                    compontRect.currentObject = parent
                }
            }
        }
    }
}
