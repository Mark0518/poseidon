// 这个文件是 Poseidon 服务器应用程序框架的一部分。
// Copyleft 2014 - 2017, LH_Mouse. All wrongs reserved.

#ifndef POSEIDON_WEBSOCKET_CLIENT_HPP_
#define POSEIDON_WEBSOCKET_CLIENT_HPP_

#include "low_level_client.hpp"

namespace Poseidon {

namespace WebSocket {
	class Client : public LowLevelClient {
	private:
		class SyncJobBase;
		class ConnectJob;
		class ReadHupJob;
		class DataMessageJob;
		class ControlMessageJob;

	private:
		OpCode m_opcode;
		StreamBuffer m_payload;

	public:
		explicit Client(const boost::shared_ptr<Http::LowLevelClient> &parent);
		~Client();

	protected:
		OpCode get_low_level_opcode() const {
			return m_opcode;
		}
		const StreamBuffer &get_low_level_payload() const {
			return m_payload;
		}

		// UpgradedSessionBase
		void on_connect() OVERRIDE;
		void on_read_hup() OVERRIDE;

		// LowLevelClient
		void on_low_level_message_header(OpCode opcode) OVERRIDE;
		void on_low_level_message_payload(boost::uint64_t whole_offset, StreamBuffer payload) OVERRIDE;
		bool on_low_level_message_end(boost::uint64_t whole_size) OVERRIDE;

		bool on_low_level_control_message(OpCode opcode, StreamBuffer payload) OVERRIDE;

		// 可覆写。
		virtual void on_sync_connect();

		virtual void on_sync_data_message(OpCode opcode, StreamBuffer payload) = 0;
		virtual void on_sync_control_message(OpCode opcode, StreamBuffer payload);
	};
}

}

#endif
