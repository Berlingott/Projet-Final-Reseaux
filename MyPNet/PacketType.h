//
// Created by Berlingot on 2023-04-19.
//

#ifndef MYPNET_PACKETTYPE_H
#define MYPNET_PACKETTYPE_H


#include <cstdint>

namespace MyPNet{
    enum PacketType : int32_t {
        ETVOILA,
        Instruction,
        CMDCommand,
        Warning,
        ChatMessage,
        FileTransferRequestFile, //Sent to request a file
        FileTransfer_EndOfFile, //Sent for when file transfer is complete
        FileTransferByteBuffer, //Sent before sending a byte buffer for file transfer
        FileTransferRequestNextBuffer, //Sent to request the next buffer for file
        PT_invalid,
        PT_ChatMessage,
        PT_IntegerArray,
        PT_FIleTransferRequest,
        PT_EndOfAFileTransfer,
        PT_CMDcommand
        };
}

#endif //MYPNET_PACKETTYPE_H
