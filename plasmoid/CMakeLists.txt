kpackage_install_package(package org.kde.kdeconnect plasmoids plasma NO_DEPRECATED_WARNING)

install(FILES package/metadata.desktop
        DESTINATION ${KDE_INSTALL_KSERVICESDIR}
        RENAME plasma-kdeconnect.desktop)
