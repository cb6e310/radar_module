#pragma once

#include "can_thread.h"
#include <QList>
#include <QMutex>
#include "connection_cfg.h"

extern QList<received_info> global_buffer;
extern QMutex mtx;
extern Connection_cfg* cfg;