#include "connection_cfg.h"

Connection_cfg::Connection_cfg()
{

}

Connection_cfg::~Connection_cfg() {

}

Connection_cfg::Connection_cfg(const Connection_cfg& other)
	: m_hDBC(other.m_hDBC), file_info(other.file_info), DBC_message_list(other.DBC_message_list) {
}