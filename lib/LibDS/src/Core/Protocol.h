/*
 * Copyright (c) 2016 Alex Spataru <alex_spataru@outlook.com>
 *
 * This file is part of the LibDS, which is released under the MIT license.
 * For more information, please read the LICENSE file in the root directory
 * of this project.
 */

#ifndef _LIB_DS_PROTOCOL_H
#define _LIB_DS_PROTOCOL_H

#include <QtMath>
#include <DriverStation.h>
#include <Core/DS_Config.h>

/**
 * \brief Base class for implementing communication protocols
 *
 * The protocol class allows developers to create their own communication
 * protocols for use with applications that support the LibDS.
 */
class Protocol {
  public:
    explicit Protocol() {
        m_sentFmsPackets = 0;
        m_sentRadioPackets = 0;
        m_sentRobotPackets = 0;
        m_receivedFmsPackets = 0;
        m_receivedRadioPackets = 0;
        m_receivedRobotPackets = 0;

        m_recvRobotPacketsSinceConnect = 0;
        m_sentRobotPacketsSinceConnect = 0;
    }

    /**
     * Returns the name of the protocol.
     * This is used by the \c DriverStation to notify the user when the protocol
     * is loaded.
     */
    virtual QString name() {
        return QObject::tr ("Generic Protocol");
    }

    /**
     * Returns the number of packets that we send to the FMS per second.
     * For example, if we want to send 50 packets/second, the frequency should
     * be set to 50.
     *
     * \note If you do not re-implement this function, then the Driver Station
     *       will not send data to the FMS.
     */
    virtual int fmsFrequency() {
        return 1;
    }

    /**
     * Returns the number of packets that we send to the robot per second.
     * For example, if we want to send 50 packets/second, the frequency should
     * be set to 50.
     *
     * \note If you do not re-implement this function, then the Driver Station
     *       will not send data to the radio.
     */
    virtual int radioFrequency() {
        return 1;
    }

    /**
     * Returns the number of packets that we send to the robot per second.
     * For example, if we want to send 50 packets/second, the frequency should
     * be set to 50.
     *
     * \note If you do not re-implement this function, then the Driver Station
     *       will not send data to the robot.
     */
    virtual int robotFrequency() {
        return 1;
    }

    /**
     * Returns the maximum number of joysticks supported
     */
    virtual int maxJoystickCount() {
        return 6;
    }

    /**
     * Returns the maximum ammount of POVs supported by the protocol
     */
    virtual int maxPOVCount() {
        return 12;
    }

    /**
     * Returns the maximum ammount of axes supported by the protocol
     */
    virtual int maxAxisCount() {
        return 12;
    }

    /**
     * Returns the maximum ammount of buttons supported by the protocol
     */
    virtual int maxButtonCount() {
        return 24;
    }

    /**
     * Returns the port in which we receive data from the FMS.
     *
     * \note If you do not re-implement this function, the protocol will not be
     *    able to receive data from the FMS.
     */
    virtual int fmsInputPort() {
        return DS_DISABLED_PORT;
    }

    /**
     * Returns the port in which we send data to the FMS.
     *
     * \note If you do not re-implement this function, the protocol will not be
     *    able to send data to the FMS.
     */
    virtual int fmsOutputPort() {
        return DS_DISABLED_PORT;
    }

    /**
     * Returns the port in which we receive data from the radio.
     *
     * \note If you do not re-implement this function, the protocol will not be
     *    able to receive data from the radio.
     */
    virtual int radioInputPort() {
        return DS_DISABLED_PORT;
    }

    /**
     * Returns the port in which we receive data from the robot.
     *
     * \note If you do not re-implement this function, the protocol will not be
     *    able to receive data from the robot.
     */
    virtual int robotInputPort() {
        return DS_DISABLED_PORT;
    }

    /**
     * Returns the port in which we send data to the radio.
     *
     * \note If you do not re-implement this function, the protocol will not be
     *    able to send data to the robot radio.
     */
    virtual int radioOutputPort() {
        return DS_DISABLED_PORT;
    }

    /**
     * Returns the port in which we send data to the robot.
     *
     * \note If you do not re-implement this function, the protocol will not be
     *    able to send data to the robot.
     */
    virtual int robotOutputPort() {
        return DS_DISABLED_PORT;
    }

    /**
     * Returns the port in which we receive broadcasted messages from the robot.
     *
     * \note If you do not re-implement this function, the protocol will not be
     *    able to receive broadcasted messages from the robot.
     */
    virtual int netconsoleInputPort() {
        return DS_DISABLED_PORT;
    }

    /**
     * Returns the port in which we send broadcasted messages to the robot.
     *
     * \note If you do not re-implement this function, the protocol will not be
     *    able to send broadcasted commands to the robot.
     */
    virtual int netconsoleOutputPort() {
        return DS_DISABLED_PORT;
    }

    /**
     * Returns the nominal voltage given by the battery.
     * This value can be used by the client to draw graphs, create car-like
     * dashboards and other cool things.
     *
     * It is also used to avoid reporting 37.00 V or 16'674'591 V.
     * Trust me, it happened: http://imgur.com/a/6ibog
     */
    virtual qreal nominalBatteryVOltage() {
        return 12.8;
    }

    /**
     * Returns the maximum amperage given by the battery.
     * This value can be used by the client to draw graphs, calculate current
     * power output (in watts), do car-like dashboards and other cool things.
     */
    virtual qreal nominalBatteryAmperage() {
        return 17;
    }

    /**
     * Reboots the robot controller
     */
    virtual void rebootRobot() {}

    /**
     * Restarts the robot code
     */
    virtual void restartRobotCode() {}

    /**
     * Called when the FMS watchdog expires
     */
    virtual void onFMSWatchdogExpired() {}

    /**
     * Called when the radio watchdog expires
     */
    virtual void onRadioWatchdogExpired() {}

    /**
     * Called when the robot watchdog expires
     */
    virtual void onRobotWatchdogExpired() {}

    /**
     * Returns the socket type (UDP or TCP) used for client/FMS interacion.
     *
     * \note If you do not re-implement this function, the Driver Station will
     *       asume that the FMS uses UDP communications.
     *
     * \note We recommend you to redefine this function in your protocol to
     *       avoid breaking it if the core library is changed.
     */
    virtual DS::SocketType fmsSocketType() {
        return DS::kSocketTypeUDP;
    }

    /**
     * Returns the socket type (UDP or TCP) used for client/radio interaction.
     *
     * \note If you do not re-implement this function, the Driver Station will
     *       asume that the radio uses UDP communications.
     *
     * \note We recommend you to redefine this function in your protocol to
     *       avoid breaking it if the core library is changed.
     */
    virtual DS::SocketType radioSocketType() {
        return DS::kSocketTypeUDP;
    }

    /**
     * Returns the socket type (UDP or TCP) used for client/robot interacion.
     *
     * \note If you do not re-implement this function, the Driver Station will
     *       asume that the robot uses UDP communications.
     *
     * \note We recommend you to redefine this function in your protocol to
     *       avoid breaking it if the core library is changed.
     */
    virtual DS::SocketType robotSocketType() {
        return DS::kSocketTypeUDP;
    }

    /**
     * Returns the IP address in which we should be able to find the FMS.
     * Contrary to how the radio and the robot protocols work, we will not
     * send FMS packets until we have received a packet from the FMS.
     *
     * The address is set to \a empty by default, the DS will set the appropriate
     * FMS IP once it receives a packet from it.
     */
    virtual QString fmsAddress() {
        return "";
    }

    /**
     * Returns the IP address in which we should be able to find the radio.
     *
     * If the returned address is invalid, unresolved (e.g. mDNS) or empty,
     * the Driver Station will lookup the address, while broadcasting the
     * generated radio packages until the lookup receives a response.
     *
     * The default value that this function returns is 10.xx.yy.1
     */
    virtual QString radioAddress() {
        return DS::getStaticIP (10, config()->team(), 1);
    }

    /**
     * Returns the IP address in which we should be able to find the robot.
     *
     * If the returned address is invalid, unresolved (e.g. mDNS) or empty,
     * the Driver Station will lookup the address, while broadcasting the
     * generated robot packages until the lookup receives a response.
     *
     * The default value that this function returns is 10.xx.yy.2
     */
    virtual QString robotAddress() {
        return DS::getStaticIP (10, config()->team(), 2);
    }

    /**
     * Updates the sent FMS packets counter and generates a client-to-FMS
     * packet using the protocol implementation code.
     */
    QByteArray generateFMSPacket() {
        ++m_sentFmsPackets;
        return getFMSPacket();
    }

    /**
     * Updates the sent radio packets counter and generates a client-to-radio
     * packet using the protocol implementation code.
     */
    QByteArray generateRadioPacket() {
        ++m_sentRadioPackets;
        return getRadioPacket();
    }

    /**
     * Updates the sent robot packets counter and generates a client-to-robot
     * packet using the protocol implementation code.
     */
    QByteArray generateRobotPacket() {
        ++m_sentRobotPackets;
        ++m_sentRobotPacketsSinceConnect;

        return getRobotPacket();
    }

    /**
     * Lets the protocol implementation interpret the given \a data and updates
     * the received FMS packets counter.
     */
    bool readFMSPacket (const QByteArray& data) {
        ++m_receivedFmsPackets;

        if (interpretFMSPacket (data)) {
            config()->updateFMSCommStatus (DS::kCommsWorking);
            return true;
        }

        return false;
    }

    /**
     * Lets the protocol implementation interpret the given \a data and updates
     * the received radio packets counter.
     */
    bool readRadioPacket (const QByteArray& data) {
        ++m_receivedRadioPackets;

        if (interpretRadioPacket (data)) {
            config()->updateRadioCommStatus (DS::kCommsWorking);
            return true;
        }

        return false;
    }

    /**
     * Lets the protocol implementation interpret the given \a data and updates
     * the received robot packets counter.
     */
    bool readRobotPacket (const QByteArray& data) {
        ++m_receivedRobotPackets;
        ++m_recvRobotPacketsSinceConnect;

        if (interpretRobotPacket (data)) {
            if (!config()->isConnectedToRobot())
                resetLossCounter();

            config()->updateRobotCommStatus (DS::kCommsWorking);
            return true;
        }

        return false;
    }

    /**
     * Ensures that the packet loss is calculated based on current values and
     * not on previous robot packet loss data.
     */
    void resetLossCounter() {
        m_recvRobotPacketsSinceConnect = 0;
        m_sentRobotPacketsSinceConnect = 0;
    }

    /**
     * Returns the number of packets sent to the FMS
     */
    int sentFMSPackets() {
        return m_sentFmsPackets;
    }

    /**
     * Returns the number of packets sent to the radio
     */
    int sentRadioPackets() {
        return m_sentRadioPackets;
    }

    /**
     * Returns the number of packets sent to the robot
     */
    int sentRobotPackets() {
        return m_sentRobotPackets;
    }

    /**
     * Returns the number of packets received from the FMS
     */
    int receivedFMSPackets() {
        return m_receivedRobotPackets;
    }

    /**
     * Returns the number of packets received from the radio
     */
    int receivedRadioPackets() {
        return m_receivedRobotPackets;
    }

    /**
     * Returns the number of packets received from the robot
     */
    int receivedRobotPackets() {
        return m_receivedRobotPackets;
    }

    /**
     * Returns the number of packets received since robot connection.
     * This is used to get an real-time packet loss percentage.
     * If we use the \c receivedRobotPackets() to do that, we risk having
     * an unreal packet-loss ratio, since we would take into account the
     * number of packets received before a reset or something.
     */
    int recvRobotPacketsSinceConnect() {
        return m_recvRobotPacketsSinceConnect;
    }

    /**
     * Returns the number of packets sent since robot connection.
     * This is used to get an real-time packet loss percentage.
     * If we use the \c sentRobotPackets() to do that, we risk having
     * an unreal packet-loss ratio, since we would take into account the
     * number of packets sent before robot connection (which can be a lot).
     */
    int sentRobotPacketsSinceConnect() {
        return m_sentRobotPacketsSinceConnect;
    }

  protected:
    /**
     * Gives direct access to the Driver Station variables/configs
     */
    DS_Config* config() {
        return DS_Config::getInstance();
    }

    /**
     * Gives direct access to the registered joysticks of the DS
     */
    DS_Joysticks* joysticks() {
        return DriverStation::getInstance()->joysticks();
    }

    /**
     * Returns a packet that is sent to the FMS.
     *
     * \note If you do not re-implement this function, it will generate an
     *       empty packet. Empty packets are ignored by the Driver Station.
     */
    virtual QByteArray getFMSPacket() {
        return QByteArray();
    }

    /**
     * Returns a packet that is sent to the robot radio.
     *
     * \note If you do not re-implement this function, it will generate an
     *       empty packet. Empty packets are ignored by the Driver Station.
     */
    virtual QByteArray getRadioPacket() {
        return QByteArray();
    }

    /**
     * Returns a packet that is sent to the robot.
     *
     * \note If you do not re-implement this function, it will generate an
     *       empty packet. Empty packets are ignored by the Driver Station.
     */
    virtual QByteArray getRobotPacket() {
        return QByteArray();
    }

    /**
     * Interprets the \a data received from the FMS and adjusts the Driver
     * Station properties accordingly.
     */
    virtual bool interpretFMSPacket (const QByteArray& data) {
        Q_UNUSED (data);
        return false;
    }

    /**
     * Interprets the \a data received from the radio and adjusts the Driver
     * Station properties accordingly.
     */
    virtual bool interpretRadioPacket (const QByteArray& data) {
        Q_UNUSED (data);
        return false;
    }

    /**
     * Interprets the \a data received from the robot and adjusts the Driver
     * Station properties accordingly.
     */
    virtual bool interpretRobotPacket (const QByteArray& data) {
        Q_UNUSED (data);
        return false;
    }

  private:
    int m_sentFmsPackets;
    int m_sentRadioPackets;
    int m_sentRobotPackets;
    int m_receivedFmsPackets;
    int m_receivedRadioPackets;
    int m_receivedRobotPackets;

    int m_recvRobotPacketsSinceConnect;
    int m_sentRobotPacketsSinceConnect;
};

#endif
