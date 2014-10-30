#ifndef POSEIDON_MODULE_RAII_HPP_
#define POSEIDON_MODULE_RAII_HPP_

#include "cxx_util.hpp"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace Poseidon {

class ModuleRaiiBase : boost::noncopyable {
public:
	ModuleRaiiBase();
	virtual ~ModuleRaiiBase();

public:
	virtual boost::shared_ptr<void> init() const = 0;
};

}

#define MODULE_RAII_BEGIN	\
	namespace {	\
		class UNIQUE_ID : public ::Poseidon::ModuleRaiiBase {	\
			::boost::shared_ptr<void> init() const {

#define MODULE_RAII_END	\
				return ::boost::shared_ptr<void>();	\
			}	\
		} const TOKEN_CAT2(UNIQUE_ID, mraii_);	\
	}

#define MODULE_RAII(...)	\
	MODULE_RAII_BEGIN __VA_ARGS__ MODULE_RAII_END

#endif
