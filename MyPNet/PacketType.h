//
// Created by Berlingot on 2023-04-19.
//

#ifndef MYPNET_PACKETTYPE_H
#define MYPNET_PACKETTYPE_H


#include <cstdint>

namespace MyPNet{
    enum PacketType : uint16_t {
        PT_invalid,
        PT_ChatMessage,
        PT_IntegerArray,
        PT_FIleTransferRequest,
        PT_EndOfAFileTransfer,
        PT_CMDcommand,

    };
}

#endif //MYPNET_PACKETTYPE_H
