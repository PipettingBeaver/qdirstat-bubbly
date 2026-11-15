/* Modified for QDirStat Bubbly on 2026-11-10. */

#ifndef TreemapConfigPage_h
#define TreemapConfigPage_h

#include <QColor>
#include <QPushButton>
#include <QWidget>


class QCheckBox;
class QComboBox;
class QSpinBox;


namespace QDirStat
{
    class ColorButton: public QPushButton
    {
    public:
        explicit ColorButton( QWidget * parent = 0 );

        QColor color() const { return _color; }
        void setColor( const QColor & color );

    private:
        void pickColor();
        void updateIcon();

        QColor _color;
    };


    class TreemapConfigPage: public QWidget
    {
        Q_OBJECT

    public:
        explicit TreemapConfigPage( QWidget * parent = 0 );
        virtual ~TreemapConfigPage();

    public slots:
        void setup();
        void applyChanges();
        void discardChanges();

    protected slots:
        void applyWizTreePreset();

    private:
        void readSettings();
        void writeSettings();

        QCheckBox * _cushionShadingCheckBox;
        QCheckBox * _useDirGradientCheckBox;
        QCheckBox * _enforceContrastCheckBox;
        QCheckBox * _forceCushionGridCheckBox;
        QCheckBox * _useDevicePixelRatioCheckBox;
        QSpinBox  * _ambientLightSpinBox;
        QSpinBox  * _minTileSizeSpinBox;
        ColorButton * _cushionGridColorButton;
        ColorButton * _outlineColorButton;

        QComboBox * _colorByComboBox;
        QComboBox * _paletteComboBox;
        ColorButton * _dirFillColorButton;
        ColorButton * _dirGradientStartButton;
        ColorButton * _dirGradientEndButton;

        QCheckBox * _folderOutlinesCheckBox;
        QCheckBox * _folderLabelsCheckBox;
        QSpinBox  * _folderOutlineMinSizeSpinBox;
        QSpinBox  * _folderLabelMinSizeSpinBox;
        QSpinBox  * _folderPaddingSpinBox;
        QSpinBox  * _folderPaddingMaxDepthSpinBox;
        ColorButton * _folderOutlineColorButton;

        QPushButton * _wizTreePresetButton;
    };
}

#endif
