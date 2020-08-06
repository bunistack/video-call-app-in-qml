import QtQuick 2.12
import QtMultimedia 5.13
import VCFilter 1.0
import QtGraphicalEffects 1.0

Item {

    Camera {
        id: camera
        position: Camera.FrontFace
        imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

        exposure {
            exposureCompensation: -1.0
            exposureMode: Camera.ExposurePortrait
        }

    }

    VCFilter{
        id: vcFilter

        onImageDataReady: {

            var msg = {
                type: "video",
                data: data
            };

            ws.wsSend(JSON.stringify(msg));

        }
    }

    VideoOutput {
        id: output
        source: camera
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectCrop
        filters: [vcFilter]
        visible: false
        autoOrientation: true
    }

    OpacityMask{
        source: output
        anchors.fill: output
        maskSource: Rectangle{
            radius: 10
            visible: false
            width: output.width
            height: output.height
        }
    }

}
