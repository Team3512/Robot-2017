// Copyright (c) FRC Team 3512, Spartatroniks 2013-2017. All Rights Reserved.

#pragma once

#include <unistd.h>

template <class T>
void SocketConnection::queueWrite(T& buf) {
    m_writequeue.emplace(reinterpret_cast<const char*>(&buf), sizeof(T));

    // Select on write
    selectflags |= SocketConnection::Write;
    write(m_ipcfd_w, "r", 1);
}
