#pragma once
// MESSAGE HOVER_GAMES_STATUS PACKING

#define MAVLINK_MSG_ID_HOVER_GAMES_STATUS 12916


typedef struct __mavlink_hover_games_status_t {
 uint32_t time_boot_ms; /*< [ms] Timestamp (time since system boot).*/
 uint8_t HoverGames_SM; /*<  Value of the main state machine*/
 uint8_t HoverGames_ActiveSM; /*<  Value of the Active state machine*/
} mavlink_hover_games_status_t;

#define MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN 6
#define MAVLINK_MSG_ID_HOVER_GAMES_STATUS_MIN_LEN 6
#define MAVLINK_MSG_ID_12916_LEN 6
#define MAVLINK_MSG_ID_12916_MIN_LEN 6

#define MAVLINK_MSG_ID_HOVER_GAMES_STATUS_CRC 52
#define MAVLINK_MSG_ID_12916_CRC 52



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_HOVER_GAMES_STATUS { \
    12916, \
    "HOVER_GAMES_STATUS", \
    3, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_hover_games_status_t, time_boot_ms) }, \
         { "HoverGames_SM", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_hover_games_status_t, HoverGames_SM) }, \
         { "HoverGames_ActiveSM", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_hover_games_status_t, HoverGames_ActiveSM) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_HOVER_GAMES_STATUS { \
    "HOVER_GAMES_STATUS", \
    3, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_hover_games_status_t, time_boot_ms) }, \
         { "HoverGames_SM", NULL, MAVLINK_TYPE_UINT8_T, 0, 4, offsetof(mavlink_hover_games_status_t, HoverGames_SM) }, \
         { "HoverGames_ActiveSM", NULL, MAVLINK_TYPE_UINT8_T, 0, 5, offsetof(mavlink_hover_games_status_t, HoverGames_ActiveSM) }, \
         } \
}
#endif

/**
 * @brief Pack a hover_games_status message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp (time since system boot).
 * @param HoverGames_SM  Value of the main state machine
 * @param HoverGames_ActiveSM  Value of the Active state machine
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hover_games_status_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, uint8_t HoverGames_SM, uint8_t HoverGames_ActiveSM)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint8_t(buf, 4, HoverGames_SM);
    _mav_put_uint8_t(buf, 5, HoverGames_ActiveSM);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN);
#else
    mavlink_hover_games_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.HoverGames_SM = HoverGames_SM;
    packet.HoverGames_ActiveSM = HoverGames_ActiveSM;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HOVER_GAMES_STATUS;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_MIN_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_CRC);
}

/**
 * @brief Pack a hover_games_status message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp (time since system boot).
 * @param HoverGames_SM  Value of the main state machine
 * @param HoverGames_ActiveSM  Value of the Active state machine
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_hover_games_status_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,uint8_t HoverGames_SM,uint8_t HoverGames_ActiveSM)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint8_t(buf, 4, HoverGames_SM);
    _mav_put_uint8_t(buf, 5, HoverGames_ActiveSM);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN);
#else
    mavlink_hover_games_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.HoverGames_SM = HoverGames_SM;
    packet.HoverGames_ActiveSM = HoverGames_ActiveSM;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_HOVER_GAMES_STATUS;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_MIN_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_CRC);
}

/**
 * @brief Encode a hover_games_status struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param hover_games_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_hover_games_status_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_hover_games_status_t* hover_games_status)
{
    return mavlink_msg_hover_games_status_pack(system_id, component_id, msg, hover_games_status->time_boot_ms, hover_games_status->HoverGames_SM, hover_games_status->HoverGames_ActiveSM);
}

/**
 * @brief Encode a hover_games_status struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param hover_games_status C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_hover_games_status_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_hover_games_status_t* hover_games_status)
{
    return mavlink_msg_hover_games_status_pack_chan(system_id, component_id, chan, msg, hover_games_status->time_boot_ms, hover_games_status->HoverGames_SM, hover_games_status->HoverGames_ActiveSM);
}

/**
 * @brief Send a hover_games_status message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp (time since system boot).
 * @param HoverGames_SM  Value of the main state machine
 * @param HoverGames_ActiveSM  Value of the Active state machine
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_hover_games_status_send(mavlink_channel_t chan, uint32_t time_boot_ms, uint8_t HoverGames_SM, uint8_t HoverGames_ActiveSM)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint8_t(buf, 4, HoverGames_SM);
    _mav_put_uint8_t(buf, 5, HoverGames_ActiveSM);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVER_GAMES_STATUS, buf, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_MIN_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_CRC);
#else
    mavlink_hover_games_status_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.HoverGames_SM = HoverGames_SM;
    packet.HoverGames_ActiveSM = HoverGames_ActiveSM;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVER_GAMES_STATUS, (const char *)&packet, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_MIN_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_CRC);
#endif
}

/**
 * @brief Send a hover_games_status message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_hover_games_status_send_struct(mavlink_channel_t chan, const mavlink_hover_games_status_t* hover_games_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_hover_games_status_send(chan, hover_games_status->time_boot_ms, hover_games_status->HoverGames_SM, hover_games_status->HoverGames_ActiveSM);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVER_GAMES_STATUS, (const char *)hover_games_status, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_MIN_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_CRC);
#endif
}

#if MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This varient of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_hover_games_status_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, uint8_t HoverGames_SM, uint8_t HoverGames_ActiveSM)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_uint8_t(buf, 4, HoverGames_SM);
    _mav_put_uint8_t(buf, 5, HoverGames_ActiveSM);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVER_GAMES_STATUS, buf, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_MIN_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_CRC);
#else
    mavlink_hover_games_status_t *packet = (mavlink_hover_games_status_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->HoverGames_SM = HoverGames_SM;
    packet->HoverGames_ActiveSM = HoverGames_ActiveSM;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_HOVER_GAMES_STATUS, (const char *)packet, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_MIN_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_CRC);
#endif
}
#endif

#endif

// MESSAGE HOVER_GAMES_STATUS UNPACKING


/**
 * @brief Get field time_boot_ms from hover_games_status message
 *
 * @return [ms] Timestamp (time since system boot).
 */
static inline uint32_t mavlink_msg_hover_games_status_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field HoverGames_SM from hover_games_status message
 *
 * @return  Value of the main state machine
 */
static inline uint8_t mavlink_msg_hover_games_status_get_HoverGames_SM(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  4);
}

/**
 * @brief Get field HoverGames_ActiveSM from hover_games_status message
 *
 * @return  Value of the Active state machine
 */
static inline uint8_t mavlink_msg_hover_games_status_get_HoverGames_ActiveSM(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  5);
}

/**
 * @brief Decode a hover_games_status message into a struct
 *
 * @param msg The message to decode
 * @param hover_games_status C-struct to decode the message contents into
 */
static inline void mavlink_msg_hover_games_status_decode(const mavlink_message_t* msg, mavlink_hover_games_status_t* hover_games_status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    hover_games_status->time_boot_ms = mavlink_msg_hover_games_status_get_time_boot_ms(msg);
    hover_games_status->HoverGames_SM = mavlink_msg_hover_games_status_get_HoverGames_SM(msg);
    hover_games_status->HoverGames_ActiveSM = mavlink_msg_hover_games_status_get_HoverGames_ActiveSM(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN? msg->len : MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN;
        memset(hover_games_status, 0, MAVLINK_MSG_ID_HOVER_GAMES_STATUS_LEN);
    memcpy(hover_games_status, _MAV_PAYLOAD(msg), len);
#endif
}
