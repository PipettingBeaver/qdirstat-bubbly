/* Modified for QDirStat Bubbly on 2026-11-10. */

#include "ExtensionColorMap.h"
#include "FileInfo.h"
#include "Settings.h"

#include <QStringList>


using namespace QDirStat;


ExtensionColorMap * ExtensionColorMap::_instance = 0;


ExtensionColorMap * ExtensionColorMap::instance()
{
    if ( ! _instance )
        _instance = new ExtensionColorMap();

    return _instance;
}


ExtensionColorMap::ExtensionColorMap():
    _palette( WizTreePalette )
{
}


QColor ExtensionColorMap::colorForFile( const FileInfo * file ) const
{
    return colorForSuffix( suffixForFile( file ) );
}


QColor ExtensionColorMap::colorForName( const QString & fileName ) const
{
    return colorForSuffix( suffixForFileName( fileName ) );
}


QColor ExtensionColorMap::colorForSuffix( const QString & suffix ) const
{
    QString normalized = normalizeSuffix( suffix );

    if ( normalized.isEmpty() )
        return QColor( 0x70, 0x70, 0x70 );

    return colorForKey( "ext:" + normalized );
}


QColor ExtensionColorMap::colorForKey( const QString & key ) const
{
    QList<QColor> colors = paletteColors( _palette );

    if ( colors.isEmpty() )
        return QColor( 0x70, 0x70, 0x70 );

    return colors.at( hashKey( key ) % (quint32) colors.size() );
}


QString ExtensionColorMap::suffixForFile( const FileInfo * file )
{
    if ( ! file )
        return QString();

    return suffixForFileName( file->name() );
}


QString ExtensionColorMap::suffixForFileName( const QString & fileName )
{
    QString name = fileName;
    int slash = qMax( name.lastIndexOf( '/' ), name.lastIndexOf( '\\' ) );

    if ( slash >= 0 )
        name = name.mid( slash + 1 );

    QString lower = name.toLower();

    static const char * multiSuffixes[] =
    {
        "tar.gz",
        "tar.bz2",
        "tar.xz",
        "tar.zst",
        "tar.lz",
        "tar.lzo",
        "cpio.gz",
        "dvi.bz2",
        "xcf.gz",
        "xcf.bz2",
        0
    };

    for ( int i=0; multiSuffixes[i]; ++i )
    {
        QString suffix = QLatin1String( multiSuffixes[i] );

        if ( lower.endsWith( "." + suffix ) )
            return suffix;
    }

    int dot = name.lastIndexOf( '.' );

    if ( dot <= 0 || dot == name.size() - 1 )
        return QString();

    return normalizeSuffix( name.mid( dot + 1 ) );
}


QString ExtensionColorMap::normalizeSuffix( const QString & suffix )
{
    QString result = suffix.trimmed().toLower();

    if ( result.startsWith( "*." ) )
        result.remove( 0, 2 );
    else if ( result.startsWith( '.' ) )
        result.remove( 0, 1 );

    if ( result == "jpeg" || result == "jfif" )
        result = "jpg";
    else if ( result == "tiff" )
        result = "tif";
    else if ( result == "htm" )
        result = "html";
    else if ( result == "yml" )
        result = "yaml";
    else if ( result == "mpeg" )
        result = "mpg";

    return result;
}


QString ExtensionColorMap::paletteName( Palette palette )
{
    switch ( palette )
    {
        case ColorblindPalette: return "Colorblind";
        case MutedPalette:      return "Muted";
        default:                return "WizTree";
    }
}


ExtensionColorMap::Palette ExtensionColorMap::paletteFromName( const QString & name )
{
    if ( name.compare( "Colorblind", Qt::CaseInsensitive ) == 0 )
        return ColorblindPalette;

    if ( name.compare( "Muted", Qt::CaseInsensitive ) == 0 )
        return MutedPalette;

    return WizTreePalette;
}


QList<QColor> ExtensionColorMap::paletteColors( Palette palette ) const
{
    if ( palette == ColorblindPalette )
    {
        return QList<QColor>()
            << QColor( "#4477aa" )
            << QColor( "#ee6677" )
            << QColor( "#228833" )
            << QColor( "#ccbb44" )
            << QColor( "#66ccee" )
            << QColor( "#aa3377" )
            << QColor( "#bbbbbb" )
            << QColor( "#77aadd" )
            << QColor( "#ee8866" )
            << QColor( "#eedd88" )
            << QColor( "#ffaabb" )
            << QColor( "#99ddff" )
            << QColor( "#44bb99" )
            << QColor( "#666666" );
    }

    QList<QColor> colors = wizTreeColors();

    if ( palette == MutedPalette )
    {
        for ( int i=0; i < colors.size(); ++i )
        {
            QColor color = colors.at( i );
            int hue = color.hsvHue();

            if ( hue >= 0 )
                color.setHsv( hue,
                              qRound( color.hsvSaturation() * 0.55 ),
                              qRound( color.value() * 0.90 ) );
            else
                color.setHsv( 0, 0, qRound( color.value() * 0.90 ) );

            colors[i] = color;
        }
    }

    return colors;
}


QList<QColor> ExtensionColorMap::wizTreeColors()
{
    return QList<QColor>()
        << QColor( "#e6194b" )
        << QColor( "#3cb44b" )
        << QColor( "#ffe119" )
        << QColor( "#4363d8" )
        << QColor( "#f58231" )
        << QColor( "#911eb4" )
        << QColor( "#42d4f4" )
        << QColor( "#f032e6" )
        << QColor( "#bfef45" )
        << QColor( "#469990" )
        << QColor( "#9a6324" )
        << QColor( "#800000" )
        << QColor( "#808000" )
        << QColor( "#000075" )
        << QColor( "#a9a9a9" )
        << QColor( "#ff7f50" )
        << QColor( "#40e0d0" )
        << QColor( "#da70d6" )
        << QColor( "#7cfc00" )
        << QColor( "#ff6347" );
}


quint32 ExtensionColorMap::hashKey( const QString & key )
{
    quint32 hash = 2166136261u;

    for ( int i=0; i < key.size(); ++i )
    {
        hash ^= key.at( i ).unicode();
        hash *= 16777619u;
    }

    hash ^= hash >> 16;
    hash *= 0x45d9f3bu;
    hash ^= hash >> 16;

    return hash;
}


void ExtensionColorMap::readSettings()
{
    Settings settings;
    settings.beginGroup( "Treemaps" );
    _palette = paletteFromName( settings.value( "ExtensionPalette", "WizTree" ).toString() );
    settings.endGroup();
}


void ExtensionColorMap::writeSettings()
{
    Settings settings;
    settings.beginGroup( "Treemaps" );
    settings.setValue( "ExtensionPalette", paletteName( _palette ) );
    settings.endGroup();
}
