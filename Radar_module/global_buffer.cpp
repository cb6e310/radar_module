#include "global_buffer.h"

QList<received_info> global_buffer;
QMutex mtx;
Connection_cfg* cfg;
bool pause_flag;