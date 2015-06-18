#include "utils.h"

Utils::Utils() {

}

QImage Utils::mat2QImage(const Mat &mat) { // from http://www.qtcentre.org/threads/56482-efficient-way-to-display-opencv-image-into-Qt
    if(mat.type()==CV_8UC1) {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    else if(mat.type()==CV_8UC3) {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else {
        //        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

Scalar Utils::bgr2hsv(Scalar rgb) {
        Scalar         out;
        double      min, max, delta;

        min = rgb.val[2] < rgb.val[1] ? rgb.val[2] : rgb.val[1];
        min = min  < rgb.val[0] ? min  : rgb.val[0];

        max = rgb.val[2] > rgb.val[1] ? rgb.val[2] : rgb.val[1];
        max = max  > rgb.val[0] ? max  : rgb.val[0];

        out.val[2] = max;                                // v
        delta = max - min;
        if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
            out.val[1] = (delta / max);                  // s
        } else {
            // if max is 0, then r = g = b = 0
                // s = 0, v is undefined
            out.val[1] = 0.0;
            out.val[0] = NAN;                            // its now undefined
            return out;
        }
        if( rgb.val[2] >= max )                           // > is bogus, just keeps compilor happy
            out.val[0] = ( rgb.val[1] - rgb.val[0] ) / delta;        // between yellow & magenta
        else
        if( rgb.val[1] >= max )
            out.val[0] = 2.0 + ( rgb.val[0] - rgb.val[2] ) / delta;  // between cyan & yellow
        else
            out.val[0] = 4.0 + ( rgb.val[2] - rgb.val[1]) / delta;  // between magenta & cyan

        out.val[0] *= 60.0;                              // degrees

        if( out.val[0] < 0.0 )
            out.val[0] += 360.0;

        return out;


}

