set(debug_file_SRCS)
ecm_qt_declare_logging_category(
    debug_file_SRCS HEADER plugin_runcommand_debug.h
    IDENTIFIER KDECONNECT_PLUGIN_RUNCOMMAND CATEGORY_NAME kdeconnect.plugin.runcommand
    DEFAULT_SEVERITY Warning
    EXPORT kdeconnect-kde DESCRIPTION "kdeconnect (plugin runcommand)")

set(kdeconnect_runcommand_SRCS
    runcommandplugin.cpp
    ${debug_file_SRCS}
)

kdeconnect_add_plugin(kdeconnect_runcommand SOURCES ${kdeconnect_runcommand_SRCS})

target_link_libraries(kdeconnect_runcommand
    kdeconnectcore
    Qt${QT_MAJOR_VERSION}::DBus
    KF5::KCMUtils
    KF5::I18n)

#----------------------
if(NOT SAILFISHOS)
    kcoreaddons_add_plugin(kdeconnect_runcommand_config SOURCES runcommand_config.cpp INSTALL_NAMESPACE "kdeconnect/kcms")
    target_link_libraries( kdeconnect_runcommand_config
        kdeconnectcore
        kdeconnectpluginkcm
        KF5::I18n
        KF5::CoreAddons
        KF5::ConfigWidgets

    )
endif()
