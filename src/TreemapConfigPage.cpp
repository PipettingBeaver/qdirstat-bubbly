/* Modified for QDirStat Bubbly on 2026-11-10. */

#include "TreemapConfigPage.h"
#include "ExtensionColorMap.h"
#include "Settings.h"
#include "SettingsHelpers.h"

#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QSpinBox>
#include <QVBoxLayout>


using namespace QDirStat;


static void selectData( QComboBox * combo, const QString & data )
{
    int index = combo->findData( data );

    if ( index >= 0 )
        combo->setCurrentIndex( index );
}


ColorButton::ColorButton( QWidget * parent ):
    QPushButton( parent ),
    _color( Qt::black )
{
    setMinimumWidth( 110 );
    connect( this, &QPushButton::clicked, this, &ColorButton::pickColor );
    updateIcon();
}


void ColorButton::setColor( const QColor & color )
{
    if ( ! color.isValid() )
        return;

    _color = color;
    updateIcon();
}


void ColorButton::pickColor()
{
    QColor color = QColorDialog::getColor( _color, this, tr( "Select Color" ) );

    if ( color.isValid() )
        setColor( color );
}


void ColorButton::updateIcon()
{
    QPixmap pixmap( 16, 16 );
    pixmap.fill( _color );

    QPainter painter( &pixmap );
    painter.setPen( QColor( 0, 0, 0, 160 ) );
    painter.drawRect( 0, 0, 15, 15 );
    painter.end();

    setIcon( QIcon( pixmap ) );
    setText( _color.name( QColor::HexRgb ).toUpper() );
}


TreemapConfigPage::TreemapConfigPage( QWidget * parent ):
    QWidget( parent ),
    _cushionShadingCheckBox( 0 ),
    _useDirGradientCheckBox( 0 ),
    _enforceContrastCheckBox( 0 ),
    _forceCushionGridCheckBox( 0 ),
    _useDevicePixelRatioCheckBox( 0 ),
    _ambientLightSpinBox( 0 ),
    _minTileSizeSpinBox( 0 ),
    _cushionGridColorButton( 0 ),
    _outlineColorButton( 0 ),
    _colorByComboBox( 0 ),
    _paletteComboBox( 0 ),
    _dirFillColorButton( 0 ),
    _dirGradientStartButton( 0 ),
    _dirGradientEndButton( 0 ),
    _folderOutlinesCheckBox( 0 ),
    _folderLabelsCheckBox( 0 ),
    _folderOutlineMinSizeSpinBox( 0 ),
    _folderLabelMinSizeSpinBox( 0 ),
    _folderPaddingSpinBox( 0 ),
    _folderPaddingMaxDepthSpinBox( 0 ),
    _folderOutlineColorButton( 0 ),
    _wizTreePresetButton( 0 )
{
    QVBoxLayout * mainLayout = new QVBoxLayout( this );

    QLabel * hint = new QLabel( tr( "WizTree style: cushion shading, extension colors, "
                                    "folder outlines, and labels off." ), this );
    hint->setWordWrap( true );
    mainLayout->addWidget( hint );

    QGroupBox * appearanceGroup = new QGroupBox( tr( "Appearance" ), this );
    QFormLayout * appearanceLayout = new QFormLayout( appearanceGroup );

    _cushionShadingCheckBox = new QCheckBox( tr( "Cushion shading (3D tiles)" ), appearanceGroup );
    appearanceLayout->addRow( _cushionShadingCheckBox );

    _useDirGradientCheckBox = new QCheckBox( tr( "Directory gradient" ), appearanceGroup );
    appearanceLayout->addRow( _useDirGradientCheckBox );

    _enforceContrastCheckBox = new QCheckBox( tr( "Enforce tile edge contrast" ), appearanceGroup );
    appearanceLayout->addRow( _enforceContrastCheckBox );

    _forceCushionGridCheckBox = new QCheckBox( tr( "Draw cushion grid lines" ), appearanceGroup );
    appearanceLayout->addRow( _forceCushionGridCheckBox );

    _useDevicePixelRatioCheckBox = new QCheckBox( tr( "High-DPI cushion rendering" ), appearanceGroup );
    appearanceLayout->addRow( _useDevicePixelRatioCheckBox );

    _ambientLightSpinBox = new QSpinBox( appearanceGroup );
    _ambientLightSpinBox->setRange( 0, 200 );
    appearanceLayout->addRow( tr( "Ambient light:" ), _ambientLightSpinBox );

    _minTileSizeSpinBox = new QSpinBox( appearanceGroup );
    _minTileSizeSpinBox->setRange( 1, 50 );
    appearanceLayout->addRow( tr( "Minimum tile size:" ), _minTileSizeSpinBox );

    _cushionGridColorButton = new ColorButton( appearanceGroup );
    appearanceLayout->addRow( tr( "Cushion grid color:" ), _cushionGridColorButton );

    _outlineColorButton = new ColorButton( appearanceGroup );
    appearanceLayout->addRow( tr( "Flat tile outline:" ), _outlineColorButton );

    mainLayout->addWidget( appearanceGroup );

    QGroupBox * colorGroup = new QGroupBox( tr( "Colors" ), this );
    QFormLayout * colorLayout = new QFormLayout( colorGroup );

    _colorByComboBox = new QComboBox( colorGroup );
    _colorByComboBox->addItem( tr( "MIME category" ), "MimeCategory" );
    _colorByComboBox->addItem( tr( "File extension" ), "Extension" );
    _colorByComboBox->addItem( tr( "Top-level folder" ), "TopLevelFolder" );
    _colorByComboBox->addItem( tr( "File size" ), "FileSize" );
    colorLayout->addRow( tr( "Color files by:" ), _colorByComboBox );

    _paletteComboBox = new QComboBox( colorGroup );
    _paletteComboBox->addItem( tr( "WizTree" ), "WizTree" );
    _paletteComboBox->addItem( tr( "Colorblind" ), "Colorblind" );
    _paletteComboBox->addItem( tr( "Muted" ), "Muted" );
    colorLayout->addRow( tr( "Extension palette:" ), _paletteComboBox );

    _dirFillColorButton = new ColorButton( colorGroup );
    colorLayout->addRow( tr( "Directory fill:" ), _dirFillColorButton );

    _dirGradientStartButton = new ColorButton( colorGroup );
    colorLayout->addRow( tr( "Folder gradient start:" ), _dirGradientStartButton );

    _dirGradientEndButton = new ColorButton( colorGroup );
    colorLayout->addRow( tr( "Folder gradient end:" ), _dirGradientEndButton );

    mainLayout->addWidget( colorGroup );

    QGroupBox * folderGroup = new QGroupBox( tr( "Folder Structure" ), this );
    QFormLayout * folderLayout = new QFormLayout( folderGroup );

    _folderOutlinesCheckBox = new QCheckBox( tr( "Draw folder outlines over tiles" ), folderGroup );
    folderLayout->addRow( _folderOutlinesCheckBox );

    _folderOutlineColorButton = new ColorButton( folderGroup );
    folderLayout->addRow( tr( "Folder outline color:" ), _folderOutlineColorButton );

    _folderPaddingSpinBox = new QSpinBox( folderGroup );
    _folderPaddingSpinBox->setRange( 0, 10 );
    folderLayout->addRow( tr( "Folder gap:" ), _folderPaddingSpinBox );

    _folderPaddingMaxDepthSpinBox = new QSpinBox( folderGroup );
    _folderPaddingMaxDepthSpinBox->setRange( 0, 20 );
    folderLayout->addRow( tr( "Folder gap max depth:" ), _folderPaddingMaxDepthSpinBox );

    _folderOutlineMinSizeSpinBox = new QSpinBox( folderGroup );
    _folderOutlineMinSizeSpinBox->setRange( 1, 50 );
    folderLayout->addRow( tr( "Outline minimum size:" ), _folderOutlineMinSizeSpinBox );

    _folderLabelsCheckBox = new QCheckBox( tr( "Show file and folder names" ), folderGroup );
    folderLayout->addRow( _folderLabelsCheckBox );

    _folderLabelMinSizeSpinBox = new QSpinBox( folderGroup );
    _folderLabelMinSizeSpinBox->setRange( 20, 1000 );
    folderLayout->addRow( tr( "Label minimum size:" ), _folderLabelMinSizeSpinBox );

    mainLayout->addWidget( folderGroup );

    QHBoxLayout * presetLayout = new QHBoxLayout;
    _wizTreePresetButton = new QPushButton( tr( "Use WizTree defaults" ), this );
    presetLayout->addWidget( _wizTreePresetButton );
    presetLayout->addStretch();
    mainLayout->addLayout( presetLayout );

    mainLayout->addStretch();

    connect( _wizTreePresetButton, &QPushButton::clicked,
             this,                 &TreemapConfigPage::applyWizTreePreset );

    readSettings();
}


TreemapConfigPage::~TreemapConfigPage()
{
}


void TreemapConfigPage::setup()
{
    readSettings();
}


void TreemapConfigPage::applyChanges()
{
    writeSettings();
}


void TreemapConfigPage::discardChanges()
{
    readSettings();
}


void TreemapConfigPage::readSettings()
{
    Settings settings;
    settings.beginGroup( "Treemaps" );

    _cushionShadingCheckBox->setChecked( settings.value( "CushionShading", true ).toBool() );
    _useDirGradientCheckBox->setChecked( settings.value( "UseDirGradient", true ).toBool() );
    _enforceContrastCheckBox->setChecked( settings.value( "EnforceContrast", false ).toBool() );
    _forceCushionGridCheckBox->setChecked( settings.value( "ForceCushionGrid", false ).toBool() );
    _useDevicePixelRatioCheckBox->setChecked( settings.value( "UseDevicePixelRatio", true ).toBool() );

    _ambientLightSpinBox->setValue( settings.value( "AmbientLight", 40 ).toInt() );
    _minTileSizeSpinBox->setValue( settings.value( "MinTileSize", 3 ).toInt() );

    selectData( _colorByComboBox, settings.value( "ColorBy", "Extension" ).toString() );
    selectData( _paletteComboBox, settings.value( "ExtensionPalette", "WizTree" ).toString() );

    _folderOutlinesCheckBox->setChecked( settings.value( "FolderOutlines", true ).toBool() );
    _folderLabelsCheckBox->setChecked( settings.value( "TreemapLabels", true ).toBool() );
    _folderOutlineMinSizeSpinBox->setValue( settings.value( "FolderOutlineMinSize", 2 ).toInt() );
    _folderLabelMinSizeSpinBox->setValue( settings.value( "TreemapLabelMinSize", 80 ).toInt() );
    _folderPaddingSpinBox->setValue( settings.value( "FolderPadding", 1 ).toInt() );
    _folderPaddingMaxDepthSpinBox->setValue( settings.value( "FolderPaddingMaxDepth", 2 ).toInt() );

    _cushionGridColorButton->setColor( readColorEntry( settings, "CushionGridColor", QColor( 0x80, 0x80, 0x80 ) ) );
    _outlineColorButton->setColor( readColorEntry( settings, "OutlineColor", Qt::black ) );
    _dirFillColorButton->setColor( readColorEntry( settings, "DirFillColor", QColor( 0x10, 0x7d, 0xb4 ) ) );
    _dirGradientStartButton->setColor( readColorEntry( settings, "DirGradientStart", QColor( 0x20, 0x20, 0x28 ) ) );
    _dirGradientEndButton->setColor( readColorEntry( settings, "DirGradientEnd", QColor( 0x30, 0x30, 0x3c ) ) );
    _folderOutlineColorButton->setColor( readColorEntry( settings, "FolderOutlineColor", QColor( 0x10, 0x10, 0x10 ) ) );

    settings.endGroup();
}


void TreemapConfigPage::writeSettings()
{
    Settings settings;
    settings.beginGroup( "Treemaps" );

    settings.setValue( "CushionShading", _cushionShadingCheckBox->isChecked() );
    settings.setValue( "UseDirGradient", _useDirGradientCheckBox->isChecked() );
    settings.setValue( "EnforceContrast", _enforceContrastCheckBox->isChecked() );
    settings.setValue( "ForceCushionGrid", _forceCushionGridCheckBox->isChecked() );
    settings.setValue( "UseDevicePixelRatio", _useDevicePixelRatioCheckBox->isChecked() );
    settings.setValue( "AmbientLight", _ambientLightSpinBox->value() );
    settings.setValue( "MinTileSize", _minTileSizeSpinBox->value() );
    settings.setValue( "ColorBy", _colorByComboBox->currentData().toString() );
    settings.setValue( "ExtensionPalette", _paletteComboBox->currentData().toString() );
    settings.setValue( "FolderOutlines", _folderOutlinesCheckBox->isChecked() );
    settings.setValue( "TreemapLabels", _folderLabelsCheckBox->isChecked() );
    settings.setValue( "FolderOutlineMinSize", _folderOutlineMinSizeSpinBox->value() );
    settings.setValue( "TreemapLabelMinSize", _folderLabelMinSizeSpinBox->value() );
    settings.setValue( "FolderPadding", _folderPaddingSpinBox->value() );
    settings.setValue( "FolderPaddingMaxDepth", _folderPaddingMaxDepthSpinBox->value() );

    writeColorEntry( settings, "CushionGridColor", _cushionGridColorButton->color() );
    writeColorEntry( settings, "OutlineColor", _outlineColorButton->color() );
    writeColorEntry( settings, "DirFillColor", _dirFillColorButton->color() );
    writeColorEntry( settings, "DirGradientStart", _dirGradientStartButton->color() );
    writeColorEntry( settings, "DirGradientEnd", _dirGradientEndButton->color() );
    writeColorEntry( settings, "FolderOutlineColor", _folderOutlineColorButton->color() );

    settings.endGroup();
    ExtensionColorMap::instance()->readSettings();
}


void TreemapConfigPage::applyWizTreePreset()
{
    _cushionShadingCheckBox->setChecked( true );
    _useDirGradientCheckBox->setChecked( true );
    _enforceContrastCheckBox->setChecked( true );
    _forceCushionGridCheckBox->setChecked( false );
    _useDevicePixelRatioCheckBox->setChecked( true );

    _ambientLightSpinBox->setValue( 40 );
    _minTileSizeSpinBox->setValue( 2 );

    selectData( _colorByComboBox, "Extension" );
    selectData( _paletteComboBox, "WizTree" );

    _folderOutlinesCheckBox->setChecked( true );
    _folderLabelsCheckBox->setChecked( true );
    _folderOutlineMinSizeSpinBox->setValue( 2 );
    _folderLabelMinSizeSpinBox->setValue( 80 );
    _folderPaddingSpinBox->setValue( 1 );
    _folderPaddingMaxDepthSpinBox->setValue( 2 );

    _cushionGridColorButton->setColor( QColor( "#303030" ) );
    _outlineColorButton->setColor( QColor( "#000000" ) );
    _dirFillColorButton->setColor( QColor( "#2b2b33" ) );
    _dirGradientStartButton->setColor( QColor( "#202028" ) );
    _dirGradientEndButton->setColor( QColor( "#303040" ) );
    _folderOutlineColorButton->setColor( QColor( "#101010" ) );
}
