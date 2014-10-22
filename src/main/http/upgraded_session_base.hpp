#ifndef POSEIDON_HTTP_UPGRADED_SESSION_BASE_HPP_
#define POSEIDON_HTTP_UPGRADED_SESSION_BASE_HPP_

#include "../session_base.hpp"
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "../optional_map.hpp"

namespace Poseidon {

class OptionalMap;
class HttpSession;

class HttpUpgradedSessionBase : public SessionBase {
	friend class HttpSession;

private:
	const boost::weak_ptr<HttpSession> m_parent;

protected:
	explicit HttpUpgradedSessionBase(const boost::shared_ptr<HttpSession> &parent);

private:
	virtual void onInitContents(const void *data, std::size_t size);

	void onReadAvail(const void *data, std::size_t size) = 0;

public:
	bool send(StreamBuffer buffer, bool final = false);
	bool hasBeenShutdown() const;
	bool forceShutdown();

	boost::shared_ptr<const HttpSession> getParent() const {
		return m_parent.lock();
	}
	boost::shared_ptr<HttpSession> getParent(){
		return m_parent.lock();
	}

	boost::shared_ptr<const HttpSession> getSafeParent() const {
		return boost::shared_ptr<const HttpSession>(m_parent);
	}
	boost::shared_ptr<HttpSession> getSafeParent(){
		return boost::shared_ptr<HttpSession>(m_parent);
	}

	const std::string &getUri() const;
	const OptionalMap &getGetParams() const;
	const OptionalMap &getHeaders() const;
};

}

#endif
