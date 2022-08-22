import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 1.4
import Qt.labs.platform 1.1
import QtQml 2.12

import TaskManage 1.0

Window{
    id: root_window;

    property int m_hours : 0;
    property int m_minutes : 0;
    property int m_seconds : 0;

    color: 'transparent';
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowMinimizeButtonHint | Qt.Popup;

    x: Screen.desktopAvailableWidth - width;
    y: Screen.desktopAvailableHeight - height;

    enum HoursNum{
        Twelve = 12
    }

    TaskManage{
        id: taskManager;
    }

    Window{
        id: _popupWindow;
        title: "我是弹出窗口";
        opacity: 0.5;
        property string extraStr: "10";
        property int elapse: 0;
        property int totalTime: 100;

        Text {
            id: lab
            property string content: "";
            font{ pixelSize: 70; }
            anchors.centerIn: parent
        }

        Timer{
            id: delayTimer;
            repeat: true;
            interval: 1000;
            onTriggered: {
                console.log("遮罩层窗口定时器开始计时：" , _popupWindow.elapse);
                _popupWindow.extraStr =  _popupWindow.totalTime - _popupWindow.elapse;
                lab.text = lab.content + "\n" + "   倒计时：" + _popupWindow.extraStr + "s";

                if(_popupWindow.elapse++ == _popupWindow.totalTime)
                {
                    _popupWindow.elapse = 0;
                    _popupWindow.close();
                    root_window.show();
                    delayTimer.stop();
                }

            }

        }

        function setText(_text)
        {
            lab.content = _text;
        }

        function startTimer()
        {
            delayTimer.start();
            root_window.hide();
        }
    }

    MouseArea{
        id: _mouseArea;

        property point clickPos : '0,0';
        property bool isClicked : false;

        anchors.fill: parent;
        acceptedButtons: Qt.LeftButton | Qt.RightButton | Qt.MidButton;
        onPressed: {
            console.log(_mouseArea.pressedButtons)
            isClicked = false;
            switch(_mouseArea.pressedButtons)
            {
            case Qt.MidButton:
            {
                console.log("鼠标中键退出" , Qt.formatDateTime( new Date , "yyyy-MM-dd hh:mm:ss") );

                console.log(taskManager.m_taskGroup);
                console.log(taskManager.getJsonObject());
                console.log("=========================我是分割线==========================");
                console.log(taskManager.getJsonString());

                Qt.quit();
                break;
            }

            case Qt.LeftButton:
            {
                isClicked = true;
                clickPos = Qt.point(mouse.x , mouse.y);
                break;
            }

            case Qt.RightButton:
            {
                _rightMenu.open();
                break;
            }
            default:
                break;
            }
        }

        //鼠标按下移动时触发
        onPositionChanged: {

            if( !isClicked )
                return;

            var delta = Qt.point(mouse.x - clickPos.x , mouse.y - clickPos.y);
            root_window.setX(delta.x + root_window.x);
            root_window.setY(delta.y + root_window.y);
        }
    }

    Menu{
        id: _rightMenu;
        visible: true;

        Menu{
            id: _secondLevelMenu;
            visible: true;
            title: "设置样式";

            MenuItem{
                text: "字体";
                onTriggered: {
                    console.log( text );
                    console.log("点击了" , text , "菜单");
                    _fontDialog.open();
                }
            }
            MenuItem{
                text: "颜色";
                onTriggered: {
                    console.log( text );
                    console.log("点击了" , text , "菜单");
                    _colorDialog.open();
                }
            }
        }
    }

    ColorDialog{
        id: _colorDialog;
        title: "选择颜色";
        onAccepted: {
            console.log("你选择的颜色是：" , _colorDialog.color);
            lcdNumber.color = _colorDialog.color;
            _colorDialog.close();
        }
        onRejected: {
            console.log("取消颜色选择...");
            _colorDialog.close();
        }
    }

    FontDialog{
        id: _fontDialog;
        title: "选择字体";
        onAccepted: {

            console.log("你选择的字体是：" , _fontDialog.font);
            lcdNumber.font = _fontDialog.font;

            {
                root_window.x = root_window.x;
                root_window.y = root_window.y;
            }

            _colorDialog.close();
        }
        onRejected: {
            console.log("取消字体选择...");
            _colorDialog.close();
        }
    }

    Text {
        id: lcdNumber;
        font.pixelSize: 30
        font.bold: true
        color: 'green'
        anchors.centerIn: parent
        text: getCurrentDateTime();
    }

    function getCurrentDateTime()
    {
        return Qt.formatDateTime( new Date , "yyyy-MM-dd hh:mm:ss");
    }

    function getCurDate()
    {
        var date = new Date;
        var week = "未知日期";
        switch(date.getDay())
        {
        case 1:
        {
            week = "星期一";
            break;
        }
        case 2:
        {
            week = "星期二";
            break;
        }
        case 3:
        {
            week = "星期三";
            break;
        }
        case 4:
        {
            week = "星期四";
            break;
        }
        case 5:
        {
            week = "星期五";
            break;
        }
        case 6:
        {
            week = "星期六";
            break;
        }
        case 7:
        {
            week = "星期日";
            break;
        }
        default:
            break;
        }

        var text = getCurrentDateTime() + " " + week;
        return text;
    }

    //休息时间
    function excuteTask()
    {
        var date = new Date;
        var hour = date.getHours();
        var minute = date.getMinutes();
        var second = date.getSeconds();

        if(hour === 17 && minute === 40 && second === 0)
        {
            var content = ("大兄弟，该休息了...");
            console.log(content);
            _popupWindow.setText(content);
            _popupWindow.startTimer();
            _popupWindow.showFullScreen();
        }
    }

    Timer
    {
        id: timer;
        interval: 1000;
        repeat: true;
        onTriggered: {
            lcdNumber.text = getCurDate().toString();

            excuteTask();

            root_window.width = lcdNumber.contentWidth + 50;
            root_window.height = lcdNumber.contentHeight + 10;
        }
    }

    //<组件加载完成时执行，类似于构造函数？
    Component.onCompleted: {
        root_window.visible = true;
        timer.start();
    }
}
