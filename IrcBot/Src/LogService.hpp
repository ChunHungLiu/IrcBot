#pragma once

#include "Client.hpp"

class LogService : public Client::Listener
{
public:

	LogService(const std::string& pUser) : mUser(pUser){}

	void OnConnected(Client* pClient){}
	void OnMessage(Client* pClient, const std::string& pSender, const std::string& pChannel, const std::string& pMessage)
	{
		std::ostringstream os;
		os << "[" << pSender << "] @" << pChannel << " : " << pMessage;

		if(pSender != mUser)
		{
			pClient->SendToChannel(mUser, os.str());
		}
	}

private:

	std::string mUser;
};