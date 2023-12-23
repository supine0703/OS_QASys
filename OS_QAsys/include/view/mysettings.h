#ifndef MYSETTINGS_H
#define MYSETTINGS_H

#include <QSettings>

#define SCFG MySettings::Get

class MySettings
{
    MySettings() = delete;
    ~MySettings() = delete;
    MySettings(const MySettings&) = delete;
    MySettings(const MySettings&&) = delete;
public:
    static QSettings& Get()
    {
        static QSettings settings(CONFIG_INI, QSettings::IniFormat);
        return settings;
    }
};

// start widget
#define PRODUCE_WAY       "/option/produce_way"
#define _ALGORITHM_       "/option/_algorithm_"
#define BUFFER_SIZE       "/option/buffer_size"
#define MEMORY_SIZE       "/option/memory_size"
#define PRODUCT_FILE_PATH "/option/product_file_path"

// main window
#define _MAIN_WINDOW_MAXIMIZE_ "state/main_window_maximize"
#define _MAIN_WINDOW_GEOMETRY_ "state/main_window_geometry"
#define _RECORD_DOCK_GEOMETRY_ "state/record_dock_geometry"
#define _RECORD_DOCK_ISHIDDEN_ "state/record_dock_ishidden"
#define _RECORD_DOCK_TRACKING_ "state/record_dock_tracking"

#endif // MYSETTINGS_H
