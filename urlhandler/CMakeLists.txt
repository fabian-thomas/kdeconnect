add_definitions(-DTRANSLATION_DOMAIN="kdeconnect-urlhandler")

ki18n_wrap_ui(telhandler_SRCS dialog.ui)

qt5_add_resources(kdeconnect_custom_icons_SRCS ${CMAKE_SOURCE_DIR}/icons/custom_icons.qrc)

add_executable(kdeconnect-handler kdeconnect-handler.cpp ${telhandler_SRCS} ${kdeconnect_custom_icons_SRCS})


target_link_libraries(kdeconnect-handler
    kdeconnectinterfaces
    kdeconnectcore
    kdeconnectversion
    Qt::Widgets
    KF5::CoreAddons
    KF5::DBusAddons
    KF5::I18n
    KF5::KIOCore KF5::KIOFileWidgets KF5::KIOWidgets
)

install(TARGETS kdeconnect-handler ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.kde.kdeconnect.handler.desktop DESTINATION ${KDE_INSTALL_APPDIR} )
