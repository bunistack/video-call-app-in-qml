import Felgo 3.0
import QtQuick 2.0
import VCAudio 1.0

App {
    width: 900
    height: 600

    Item{
        anchors.fill: parent

        ReceiverView{
            anchors.fill: parent
        }

        CallerView{
            width: 200
            height: 300
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
        }

        VCAudio{
            id: vcAudio

            onAudioDataReady: {

                var msg = {
                    type: "audio",
                    data: data
                };

                ws.wsSend(JSON.stringify(msg));

            }

        }
    }

    Connections{
        target: ws

        onAudioReceived: {
            vcAudio.appendData(data);
        }

    }
}
