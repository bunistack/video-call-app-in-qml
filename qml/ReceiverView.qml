import QtQuick 2.12
import VCVideoImage 1.0
import VCAudio 1.0

Item {

    VCVideoImage{
        id: videoImage
        anchors.fill: parent
        fillMode: VCVideoImage.PreserveAspectCrop
    }

    Connections{
        target: ws

        onImageReceived: {
            //console.log("received....");
            videoImage.updateImage(data);
        }
    }

}
