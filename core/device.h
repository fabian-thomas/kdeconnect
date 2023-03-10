/**
 * SPDX-FileCopyrightText: 2013 Albert Vaca <albertvaka@gmail.com>
 *
 * SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
 */

#ifndef DEVICE_H
#define DEVICE_H

#include <QHostAddress>
#include <QObject>
#include <QString>

#include "backends/devicelink.h"
#include "networkpacket.h"

class DeviceLink;
class KdeConnectPlugin;

class KDECONNECTCORE_EXPORT Device : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.kde.kdeconnect.device")
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString iconName READ iconName CONSTANT)
    Q_PROPERTY(QString statusIconName READ statusIconName NOTIFY statusIconNameChanged)
    Q_PROPERTY(bool isReachable READ isReachable NOTIFY reachableChanged)
    Q_PROPERTY(bool isTrusted READ isTrusted NOTIFY trustedChanged)
    Q_PROPERTY(QStringList supportedPlugins READ supportedPlugins NOTIFY pluginsChanged)
    Q_PROPERTY(bool hasPairingRequests READ hasPairingRequests NOTIFY hasPairingRequestsChanged)

public:
    enum DeviceType {
        Unknown,
        Desktop,
        Laptop,
        Phone,
        Tablet,
        Tv,
    };

    /**
     * Restores the @p device from the saved configuration
     *
     * We already know it but we need to wait for an incoming DeviceLink to communicate
     */
    Device(QObject *parent, const QString &id);

    /**
     * Device known via an incoming connection sent to us via a devicelink.
     *
     * We know everything but we don't trust it yet
     */
    Device(QObject *parent, const NetworkPacket &np, DeviceLink *dl);

    ~Device() override;

    QString id() const;
    QString name() const;
    QString dbusPath() const
    {
        return QStringLiteral("/modules/kdeconnect/devices/") + id();
    }
    QString type() const;
    QString iconName() const;
    QString statusIconName() const;
    Q_SCRIPTABLE QByteArray verificationKey() const;
    Q_SCRIPTABLE QString encryptionInfo() const;

    // Add and remove links
    void addLink(const NetworkPacket &identityPacket, DeviceLink *);
    void removeLink(DeviceLink *);

    Q_SCRIPTABLE bool isTrusted() const;

    Q_SCRIPTABLE QStringList availableLinks() const;
    virtual bool isReachable() const;

    Q_SCRIPTABLE QStringList loadedPlugins() const;
    Q_SCRIPTABLE bool hasPlugin(const QString &name) const;

    Q_SCRIPTABLE QString pluginsConfigFile() const;

    KdeConnectPlugin *plugin(const QString &pluginName) const;
    Q_SCRIPTABLE void setPluginEnabled(const QString &pluginName, bool enabled);
    Q_SCRIPTABLE bool isPluginEnabled(const QString &pluginName) const;

    void cleanUnneededLinks();

    int protocolVersion();
    QStringList supportedPlugins() const;

    QHostAddress getLocalIpAddress() const;

public Q_SLOTS:
    /// sends a @p np packet to the device
    /// virtual for testing purposes.
    virtual bool sendPacket(NetworkPacket &np);

    // Dbus operations
public Q_SLOTS:
    Q_SCRIPTABLE void requestPair(); // to all links
    Q_SCRIPTABLE void unpair(); // from all links
    Q_SCRIPTABLE void reloadPlugins(); // from kconf

    Q_SCRIPTABLE void acceptPairing();
    Q_SCRIPTABLE void rejectPairing();
    Q_SCRIPTABLE bool hasPairingRequests() const;

    Q_SCRIPTABLE QString pluginIconName(const QString &pluginName);
private Q_SLOTS:
    void privateReceivedPacket(const NetworkPacket &np);
    void linkDestroyed(QObject *o);
    void pairStatusChanged(DeviceLink::PairStatus current);
    void addPairingRequest(PairingHandler *handler);
    void removePairingRequest(PairingHandler *handler);

Q_SIGNALS:
    Q_SCRIPTABLE void pluginsChanged();
    Q_SCRIPTABLE void reachableChanged(bool reachable);
    Q_SCRIPTABLE void trustedChanged(bool trusted);
    Q_SCRIPTABLE void pairingError(const QString &error);
    Q_SCRIPTABLE void nameChanged(const QString &name);
    Q_SCRIPTABLE void typeChanged(const QString &type);
    Q_SCRIPTABLE void statusIconNameChanged();

    Q_SCRIPTABLE void hasPairingRequestsChanged(bool hasPairingRequests);

private: // Methods
    static DeviceType str2type(const QString &deviceType);
    static QString type2str(DeviceType deviceType);

    void setName(const QString &name);
    void setType(const QString &strtype);
    QString iconForStatus(bool reachable, bool paired) const;
    QSslCertificate certificate() const;

private:
    class DevicePrivate;
    DevicePrivate *d;
};

Q_DECLARE_METATYPE(Device *)

#endif // DEVICE_H
