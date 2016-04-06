#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QHash>
#include <QStringRef>

namespace nq {

/**
 * @brief The Settings class.
 *
 *  All settings.
 *  Should be instanciated once and before other objects.
 */
class Settings : public QSettings
{
    Q_OBJECT
public:

    /**
     * @brief The Keys struct
     * Key for ini-file
     */
    struct Keys {
        /**
         * @brief The General struct
         * Section "general"
         */
        struct General{
            static const QString Locale;        ///< Localization
            static const QString Geometry;      ///< Geometry for MainWindow
            static const QString WindowState;   ///< State for MainWindow
        };
    };

    /**
     * @brief instance (As Meyers singleton)
     * @return singleton.
     *
     */
    static Settings &instance();

//    Settings &operator()() {return instance();}

    /**
     * @brief enumLanguages
     * @return map of locales
     */
    QHash<QString, QString> enumLanguages();

    /**
     * @brief setApplicationLocale
     * @param localeName
     *
     * Set locale for application.
     */
    void setApplicationLocale(const QString &localeName);

    QString localeName() const;

    /**
     * @brief makeAppTitle
     * @param extraString
     * @return
     *
     * Makes Application title from base string and extraString.
     */
    QString makeAppTitle(const QString &extraString = "");

signals:
    void fireTitleChanged(const QString &title);

public slots:

private:
    QString m_title;
    QString m_localeName;

    // Methods
    explicit Settings();
    ~Settings();
    Settings(Settings &);
    Settings &operator =(Settings &);
    // ~Methods

};

} // namespace nq;

#endif // SETTINGS_H
