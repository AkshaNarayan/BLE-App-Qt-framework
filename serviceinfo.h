


#ifndef SERVICEINFO_H
#define SERVICEINFO_H

#define ENDEX_SERVICE_UUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define RX_CHARACTERISTIC "6e400003-b5a3-f393-e0a9-e50e24dcca9e"
#define TX_CHARACTERISTIC "6e400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CLIENT_CHARACTERISTIC_CONFIG "00002902-0000-1000-8000-00805f9b34fb"

#include <QtBluetooth/QLowEnergyService>

#include "characteristicinfo.h"

class ServiceInfo: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString serviceName READ getName NOTIFY serviceChanged)
    Q_PROPERTY(QString serviceUuid READ getUuid NOTIFY serviceChanged)
    Q_PROPERTY(QString serviceType READ getType NOTIFY serviceChanged)
public:
    ServiceInfo();
    ServiceInfo(QLowEnergyService *service);
    QLowEnergyService *service() const;
    QString getUuid() const;
    QString getName() const;
    QString getType() const;

public slots:
    void connectToService();
    void serviceStateChanged(QLowEnergyService::ServiceState state);
    void sendData(const QByteArray &data);

signals:
    void dataWritten(QLowEnergyCharacteristic characteristic, QByteArray data);
    void dataReceived(QLowEnergyCharacteristic characteristic, QByteArray data);
    void descriptorWritten(QLowEnergyDescriptor characteristic, QByteArray data);
    void serviceError(QLowEnergyService::ServiceError state);
    void serviceReady();

Q_SIGNALS:
    void serviceChanged();

private:
    QLowEnergyService *m_service;

    QList<CharacteristicInfo*> m_characteristics;
    QLowEnergyCharacteristic bleRxCharacteristic, bleTxCharacteristic;
};

#endif // SERVICEINFO_H
