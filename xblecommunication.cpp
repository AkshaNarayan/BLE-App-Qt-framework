#include "xblecommunication.h"
//#include "utils/retrytimeout.h"

#include <QDebug>
#include <QTimer>
#include <QState>
#include <QFinalState>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>

XBleCommunication::XBleCommunication(QObject *parent) :
    QObject(parent)
{
    deviceMacAdd = "";
    isXBleConnected = false;
    m_controller = 0; m_service = 0, retryCount= 3;
    tmrRetry.setSingleShot(true);
    tmrRetry.setInterval(3000);
    connect(&tmrRetry, SIGNAL(timeout()), this, SLOT(retryConnect()));

    m_deviceDiscoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(m_deviceDiscoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)), this, SLOT(scanDeviceDiscovered(QBluetoothDeviceInfo)));
    connect(m_deviceDiscoveryAgent, SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)), this, SLOT(scanDeviceError(QBluetoothDeviceDiscoveryAgent::Error)));
    connect(m_deviceDiscoveryAgent, SIGNAL(finished()), this, SIGNAL(scanFinished()));
}

XBleCommunication::~XBleCommunication()
{

}

bool XBleCommunication::isBLEActive()
{
    QBluetoothLocalDevice localDevice;
    if (!localDevice.isValid())
        return false;
    if(localDevice.hostMode() != QBluetoothLocalDevice::HostPoweredOff)
        return true;
//    localDevice.powerOn();
//    return true;
    return false;
}

bool XBleCommunication::isBLESupported()
{
    return true;
}

void XBleCommunication::scan()
{
//    if(m_deviceDiscoveryAgent->isActive())
  //      return;
    emit bleInfoMsg(tr("Scanning"),4);
    stop();
    m_deviceDiscoveryAgent->start();
}

void XBleCommunication::stop()
{
    if(m_deviceDiscoveryAgent->isActive())
        m_deviceDiscoveryAgent->stop();
}

void XBleCommunication::clearScannedList()
{
    m_devices.clear();
}

void XBleCommunication::scanDeviceDiscovered(const QBluetoothDeviceInfo &info)
{
    if (info.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration) {
        qWarning() << "Discovered LE Device name: " << info.name() << " Address: " << info.address().toString() << m_devices.count() << info.rssi();
        emit scanDevRange(info.name(), info.address().toString(), info.rssi());
        foreach (DeviceInfo *device, m_devices) {
            if(device->getAddress() == info.address().toString())
                return;
        }
        m_devices.append(new DeviceInfo(info));
        qDebug()<<"Device found :"<<info.name();
        emit scanDevFound(info.name(), info.address().toString());
    }
}

void XBleCommunication::scanDeviceError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    qCritical() << error << m_deviceDiscoveryAgent->errorString();
    emit bleInfoMsg(tr(m_deviceDiscoveryAgent->errorString().toLatin1().data()), 2);
    if(error == QBluetoothDeviceDiscoveryAgent::PoweredOffError)
        emit bleInfoMsg(tr("Turn phone ble on..."), 2);
}

void XBleCommunication::connectToDevice(QString mac)
{
    emit bleInfoMsg(tr("Connect to device"));
    if(deviceMacAdd != mac)
        deviceMacAdd = mac;
    firstLaunchSM.stop();
    retryConnect();
}

void XBleCommunication::disConnectFromDevice()
{
    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    qDeleteAll(m_services);
    m_services.clear();

    if (m_controller) {
        m_controller->disconnectFromDevice();
        delete m_controller;
        m_controller = 0;
    }
    isXBleConnected = false;
    emit disconnected2();
}

void XBleCommunication::retryConnect()
{
    if(isXBleConnected)
        return;

    if(retryCount < 0) {
        emit bleInfoMsg(tr("Connection failed"), 3);
        retryCount = 3;
        clearScannedList();
        disConnectFromDevice();
        return;
    }
    emit bleInfoMsg(tr("Connecting..."));
    retryCount--;

    foreach(DeviceInfo *device, m_devices) {
        if (device->getAddress() == deviceMacAdd) {
            m_currentDevice.setDevice(device->getDevice());
            break;
        }
    }

    if (!m_currentDevice.getDevice().isValid()) {
        qWarning() << "Not a valid device";
        return;
    }
    stop();
    disConnectFromDevice();

    m_controller = new QLowEnergyController(m_currentDevice.getDevice(), this);
    connect(m_controller, SIGNAL(connected()), this, SLOT(discoverServices()));
    connect(m_controller, SIGNAL(serviceDiscovered(QBluetoothUuid)), this, SLOT(serviceDiscovered(QBluetoothUuid)));
    connect(m_controller, SIGNAL(discoveryFinished()), this, SLOT(serviceScanDone()));
    connect(m_controller, SIGNAL(error(QLowEnergyController::Error)), this, SLOT(controllerError(QLowEnergyController::Error)));
    connect(m_controller, SIGNAL(disconnected()), this, SLOT(disconnected()));
    m_controller->connectToDevice();

    QTimer::singleShot(3000, this, SLOT(retryConnect()));
}

void XBleCommunication::disconnected()
{
    emit bleInfoMsg(tr("Ble disconnected, retrying connection"), 3);
     qDebug() << "Disconnnnnnnnnnnnnnnneeeeeeeeeeeeeeeeeeeeeeected!";
    isXBleConnected = false;
    firstLaunchSM.stop();
    QTimer::singleShot(3000, this, SLOT(retryConnect()));
}

void XBleCommunication::discoverServices()
{
    m_controller->discoverServices();
}

void XBleCommunication::serviceDiscovered(const QBluetoothUuid &uuid)
{
    QLowEnergyService *service = m_controller->createServiceObject(uuid);
    if (!service) {
        qWarning() << "Cannot create service for uuid";
        return;
    }
    ServiceInfo *serv = new ServiceInfo(service);
    m_services.append(serv);
}

void XBleCommunication::serviceScanDone()
{
    qDebug() << "Service scan done!";
    ServiceInfo *serv = 0;
    foreach (serv, m_services) {
        qWarning()<<"SERVICE : " << serv->getUuid() << "  " << serv->getName();
        if (serv->getUuid().toLower().contains(ENDEX_SERVICE_UUID)) {
            qDebug() << "BLE Service found!";
            m_service = serv;
            break;
        }
    }

    if(m_service) {
        connect(m_service, SIGNAL(dataReceived(QLowEnergyCharacteristic,QByteArray)), this, SLOT(dataReceived(QLowEnergyCharacteristic,QByteArray)));
        connect(m_service, SIGNAL(dataWritten(QLowEnergyCharacteristic,QByteArray)), this, SLOT(dataWritten(QLowEnergyCharacteristic,QByteArray)));
        connect(m_service, SIGNAL(serviceError(QLowEnergyService::ServiceError)), this, SLOT(serviceError(QLowEnergyService::ServiceError)));
        connect(m_service, SIGNAL(serviceReady()), this, SLOT(serviceReady()));
        m_service->connectToService();
    }
}

void XBleCommunication::controllerError(QLowEnergyController::Error error)
{
    qWarning() << "Error: " << error << m_controller->errorString();
    emit bleInfoMsg(tr(m_controller->errorString().toLatin1().data()), 2);
}

void XBleCommunication::dataWritten(QLowEnergyCharacteristic characteristic, QByteArray data)
{
    CharacteristicInfo cInfo(characteristic);
    qDebug()<<"CharacteristicWritten "<< data << cInfo.getHandle() << cInfo.getPermission() << cInfo.getName() << cInfo.getUuid() << cInfo.getValue();
}

void XBleCommunication::serviceError(QLowEnergyService::ServiceError state) { qDebug() << "CharacteristicError : "<< state; }

void XBleCommunication::serviceReady()
{
    qDebug() << "SERVICE READY : ";
    retryCount = 3;
    isXBleConnected = true;
    QTimer::singleShot(250, this, SLOT(firstLaunch()));
}

void XBleCommunication::dataReceived(QLowEnergyCharacteristic characteristic, QByteArray data)
{
    qDebug() << "DATA RECEIVED : " << data;
}

void XBleCommunication::firstLaunch()
{
    emit bleInfoMsg(tr("Connected"), 3);
    emit connected();
}

void XBleCommunication::bulbOn()
{
      qDebug() << "BULB ON IS BEING CALLEDDDD " ;
      memset(data, 0, sizeof(data));
        data[0] = 'a';
        data[1] = 'k';
        data[2] = 'o';
        sendData(QByteArray((char*)data, sizeof(data)));
}

void XBleCommunication::bulbOff()
{
         qDebug() << "BULB OFFF IS BEING CALLEDDDD " ;
         memset(data, 0, sizeof(data));
           data[0] = 'a';
           data[1] = 'k';
           data[2] = 'f';
           sendData(QByteArray((char*)data, sizeof(data)));
}

void XBleCommunication::sendData(const QByteArray &data)
{
    m_service->sendData(data);
}
