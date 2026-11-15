/* Modified for QDirStat Bubbly on 2026-11-10. */

#ifndef ExtensionColorMap_h
#define ExtensionColorMap_h

#include <QColor>
#include <QList>
#include <QString>


namespace QDirStat
{
    class FileInfo;


    class ExtensionColorMap
    {
    public:
        enum Palette
        {
            WizTreePalette = 0,
            ColorblindPalette,
            MutedPalette
        };

        static ExtensionColorMap * instance();

        QColor colorForFile( const FileInfo * file ) const;
        QColor colorForName( const QString & fileName ) const;
        QColor colorForSuffix( const QString & suffix ) const;
        QColor colorForKey( const QString & key ) const;

        Palette palette() const { return _palette; }
        void setPalette( Palette palette ) { _palette = palette; }

        static QString paletteName( Palette palette );
        static Palette paletteFromName( const QString & name );

        static QString suffixForFile( const FileInfo * file );
        static QString suffixForFileName( const QString & fileName );

        void readSettings();
        void writeSettings();

    private:
        ExtensionColorMap();

        QList<QColor> paletteColors( Palette palette ) const;
        static QList<QColor> wizTreeColors();
        static quint32 hashKey( const QString & key );
        static QString normalizeSuffix( const QString & suffix );

        Palette _palette;

        static ExtensionColorMap * _instance;
    };
}

#endif
