#include "serviceinfo.h"

#include <QTimer>

ServiceInfo::ServiceInfo()
{
}

ServiceInfo::ServiceInfo(QLowEnergyService *service):
    m_service(service)
{
    m_service->setParent(this);
}

QLowEnergyService *ServiceInfo::service() const
{
    return m_service;
}

QString ServiceInfo::getName() const
{
    if (!m_service)
        return QString();

    return m_service->serviceName();
}

QString ServiceInfo::getType() const
{
    if (!m_service)
        return QString();

    QString result;
    if (m_service->type() & QLowEnergyService::PrimaryService)
        result += QStringLiteral("primary");
    else
        result += QStringLiteral("secondary");

    if (m_service->type() & QLowEnergyService::IncludedService)
        result += QStringLiteral(" included");

    result.prepend('<').append('>');

    return result;
}

QString ServiceInfo::getUuid() const
{
    if (!m_service)
        return QString();

    const QBluetoothUuid uuid = m_service->serviceUuid();
    bool success = false;
    quint16 result16 = uuid.toUInt16(&success);
    if (success)
        return QStringLiteral("0x") + QString::number(result16, 16);

    quint32 result32 = uuid.toUInt32(&success);
    if (success)
        return QStringLiteral("0x") + QString::number(result32, 16);

    return uuid.toString().remove(QLatin1Char('{')).remove(QLatin1Char('}'));
}

void ServiceInfo::connectToService()
{
    if (!m_service)
        return;

    qDeleteAll(m_characteristics);
    m_characteristics.clear();
    if (m_service->state() == QLowEnergyService::DiscoveryRequired) {
        connect(m_service, SIGNAL(characteristicChanged(QLowEnergyCharacteristic,QByteArray)),
                this, SIGNAL(dataReceived(QLowEnergyCharacteristic,QByteArray)));
        connect(m_service, SIGNAL(characteristicWritten(QLowEnergyCharacteristic,QByteArray)),
                this, SIGNAL(dataWritten(QLowEnergyCharacteristic,QByteArray)));
        connect(m_service, SIGNAL(descriptorWritten(QLowEnergyDescriptor,QByteArray)),
                this, SIGNAL(descriptorWritten(QLowEnergyDescriptor,QByteArray)));
        connect(m_service, SIGNAL(error(QLowEnergyService::ServiceError)),
                this, SIGNAL(serviceError(QLowEnergyService::ServiceError)));
        connect(m_service, SIGNAL(stateChanged(QLowEnergyService::ServiceState)),
                this, SLOT(serviceStateChanged(QLowEnergyService::ServiceState)));

        m_service->discoverDetails();
        return;
    }
}

void ServiceInfo::serviceStateChanged(QLowEnergyService::ServiceState state)
{
    switch (state) {
    case QLowEnergyService::InvalidService:
    case QLowEnergyService::DiscoveryRequired:
    case QLowEnergyService::DiscoveringServices:
        break;
    case QLowEnergyService::ServiceDiscovered:
    {
        QList<QLowEnergyCharacteristic> characteristic = m_service->characteristics();
        foreach (QLowEnergyCharacteristic charac, characteristic) {
            CharacteristicInfo *characteristicInfo = new CharacteristicInfo(charac);
            m_characteristics.append(characteristicInfo);
            if(characteristicInfo->getUuid().toLower().contains(QString(RX_CHARACTERISTIC).toLower()))
                bleRxCharacteristic = charac;
            if(characteristicInfo->getUuid().toLower().contains(QString(TX_CHARACTERISTIC).toLower()))
                bleTxCharacteristic = charac;
        }
        if(bleRxCharacteristic.isValid() && bleTxCharacteristic.isValid())
            qDebug()<<"BLE CHARECTERSTICS FOUND "<<bleRxCharacteristic.uuid() <<" "<<bleTxCharacteristic.uuid();

        //************************************************************************************************************************//
        const QLowEnergyCharacteristic rxCharacteristic = m_service->characteristic(QBluetoothUuid(QString(RX_CHARACTERISTIC)));
        if (!rxCharacteristic.isValid()) {
            qDebug()<<"RX not found.";
            break;
        }
        const QLowEnergyDescriptor m_notificationDesc = rxCharacteristic.descriptor(QBluetoothUuid::ClientCharacteristicConfiguration);
        if (m_notificationDesc.isValid()) {
            m_service->writeDescriptor(m_notificationDesc, QByteArray::fromHex("0100"));
            qDebug() << "Enabled Notification";
            QTimer::singleShot(25, this, SIGNAL(serviceReady()));
        }
        break;
    }
    default:
        break;
    }
}

void ServiceInfo::sendData(const QByteArray &data)
{
    if(!m_service)
        return;
    m_service->writeCharacteristic(bleTxCharacteristic, data, QLowEnergyService::WriteWithResponse);
}
