#include "../precompiled.hpp"
#include "tcp_session_base.hpp"
#define POSEIDON_TCP_SESSION_SSL_IMPL_
#include "tcp_session_ssl_impl.hpp"
#include <boost/thread/once.hpp>
#include <errno.h>
#include <openssl/ssl.h>
#include "exception.hpp"
#include "log.hpp"
using namespace Poseidon;

namespace {

boost::once_flag g_sslInitFlag;

void initSsl(){
	LOG_INFO("Initializing SSL library...");

	::OpenSSL_add_all_algorithms();
	::SSL_library_init();

	std::atexit(&::EVP_cleanup);
}

}

namespace Poseidon {

void requireSsl(){
	boost::call_once(&initSsl, g_sslInitFlag);
}

}

TcpSessionBase::SslImpl::SslImpl(Move<SslPtr> ssl, int fd)
	: m_ssl(STD_MOVE(ssl)), m_established(false)
{
	if(!::SSL_set_fd(m_ssl.get(), fd)){
		DEBUG_THROW(Exception, "::SSL_set_fd() failed");
	}
}
TcpSessionBase::SslImpl::~SslImpl(){
	if(m_established){
		::SSL_shutdown(m_ssl.get());
	}
}

long TcpSessionBase::SslImpl::doRead(void *data, unsigned long size){
	if(!m_established && !(m_established = establishConnection())){
		LOG_DEBUG("Waiting for SSL handshake...");
		errno = EAGAIN;
		return -1;
	}
	return ::SSL_read(m_ssl.get(), data, size);
}
long TcpSessionBase::SslImpl::doWrite(const void *data, unsigned long size){
	if(!m_established && !(m_established = establishConnection())){
		LOG_DEBUG("Waiting for SSL handshake...");
		errno = EAGAIN;
		return -1;
	}
	return ::SSL_write(m_ssl.get(), data, size);
}