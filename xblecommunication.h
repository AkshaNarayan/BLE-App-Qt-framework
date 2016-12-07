#ifndef XBLECOMMUNICATION_H
#define XBLECOMMUNICATION_H

#define ENDEX_SERVICE_UUID "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define RX_CHARACTERISTIC "6e400003-b5a3-f393-e0a9-e50e24dcca9e"
#define TX_CHARACTERISTIC "6e400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CLIENT_CHARACTERISTIC_CONFIG "00002902-0000-1000-8000-00805f9b34fb"

#include "deviceinfo.h"
#include "serviceinfo.h"
#include "characteristicinfo.h"

#include <QTimer>
#include <QObject>
#include <QStateMachine>
#include <QLowEnergyController>
#include <QBluetoothDeviceDiscoveryAgent>

class XBleCommunication : public QObject
{
    Q_OBJECT
public:
    explicit XBleCommunication(QObject *parent = 0);
    ~XBleCommunication();

public slots:
    bool isBLEActive();
    bool isBLESupported();
    void scan();
    void stop();
    void clearScannedList();
    void connectToDevice(QString mac);
    void disConnectFromDevice();
    void firstLaunch();
    void bulbOn();
    void bulbOff();

    void sendData(const QByteArray &data);

signals:
    void scanDevFound(QString name, QString mac);
    void scanDevRange(QString name, QString mac, qint16 signalStrength);
    void scanFinished();

    void connected();
    void disconnected2();

    void sigDataSendSuccessfully();
    void cmdReceivedSuccessfully();
    void communicationError();

    void bleInfoMsg(QString msg, int timeOut = 0, bool bgEnabled = true);

private slots:
    void scanDeviceDiscovered(const QBluetoothDeviceInfo & info);
    void scanDeviceError(QBluetoothDeviceDiscoveryAgent::Error error);
    void retryConnect();
    void disconnected();
    void discoverServices();
    void serviceDiscovered(const QBluetoothUuid &uuid);
    void serviceScanDone();
    void controllerError(QLowEnergyController::Error error);

    void dataReceived(QLowEnergyCharacteristic characteristic, QByteArray data);
    void dataWritten(QLowEnergyCharacteristic characteristic, QByteArray data);
    void serviceError(QLowEnergyService::ServiceError state);
    void serviceReady();

private:
    QBluetoothDeviceDiscoveryAgent *m_deviceDiscoveryAgent;
    ServiceInfo *m_service;
    QLowEnergyController *m_controller;
    QList<DeviceInfo*> m_devices;
    QList<ServiceInfo*> m_services;
    QList<CharacteristicInfo*> m_characteristics;
    DeviceInfo m_currentDevice;
    QLowEnergyCharacteristic bleRxCharacteristic, bleTxCharacteristic;

    QString deviceMacAdd;
    QStateMachine firstLaunchSM, sendDataSM;
    QByteArray dataToSend;
    char data[20], cmdSend, lastCmdSend;
    int retryCount;
    bool isXBleConnected;
    QTimer tmrRetry;
};

#endif // XBLECOMMUNICATION_H
