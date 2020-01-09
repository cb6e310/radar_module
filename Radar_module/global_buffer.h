#pragma once

#include "can_thread.h"
#include <QList>
#include <QMutex>

extern QList<received_info> global_buffer;
extern QMutex mtx;
