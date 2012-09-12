#include "stdafx.h"
#include "Client.hpp"

std::string NextToken(std::string& pTokenized, char pSpliter)
{
	size_t pos = pTokenized.find(pSpliter); 
	auto tmp = pTokenized.substr(0, pos); 
	pTokenized.erase(0, pos + 1);
	return tmp;
}

Client::Client(const std::string& pAddress, uint16_t pPort)
	:mConnected(false)
{
	if(mSocket.connect(pAddress, pPort) != sf::Socket::Done)
	{
		throw std::runtime_error("Unable to connect.");
	}
	mSelector.add(mSocket);

	mHandlers["376"] = std::bind(&Client::_OnConnected, this, std::placeholders::_1, std::placeholders::_2);
	mHandlers["NOTICE"] = std::bind(&Client::_OnNotice, this, std::placeholders::_1, std::placeholders::_2);
	mHandlers["PRIVMSG"] = std::bind(&Client::_OnMessage, this, std::placeholders::_1, std::placeholders::_2);
}

void Client::Poll()
{
	unsigned int NbSockets = mSelector.wait(sf::microseconds(0));

	std::string buffer;

	if(mSelector.isReady(mSocket))
	{
		char tmp[1024];
		size_t received = 0;
		do 
		{
			mSocket.receive((void*)tmp, 1024, received);
			mBuffer.append(tmp, received);
		} while (received == 1024);

		size_t pos = mBuffer.find("\n");
		while(pos != std::string::npos)
		{
			Parse(mBuffer.substr(0, pos - 1));
			mBuffer.erase(0, pos + 1);
			pos = mBuffer.find("\n");
		}
		
	}
}

void Client::SetUser(const std::string& pUser)
{
	mUser = pUser;
	if(mConnected)
		SendUser();
}

void Client::SetNick(const std::string& pNick)
{
	mNick = pNick;
	if(mConnected)
		SendNick();
}

void Client::Send(const std::string& pData)
{
	mSocket.send((void*)pData.data(), pData.size());
}

void Client::SendIdent()
{
	SendUser();
	SendNick();
}

void Client::SendUser()
{
	std::ostringstream os;
	os << "USER " << mUser << " * * :Some guy\r\n";
	Send(os.str());
}

void Client::SendNick()
{
	std::ostringstream os;
	os << "NICK " << mNick << "\r\n";
	Send(os.str());
}

void Client::Join(const std::string& pChannel)
{
	std::ostringstream os;
	os << "JOIN " << pChannel << "\r\n";
	Send(os.str());
}

void Client::Leave(const std::string& pChannel)
{
	std::ostringstream os;
	os << "PART " << pChannel << "\r\n";
	Send(os.str());
}

void Client::SendToChannel(const std::string& pChannel, const std::string& pMessage)
{
	std::ostringstream os;
	os << "PRIVMSG " << pChannel << " :" << pMessage << "\r\n";
	Send(os.str());
}

void Client::Parse(const std::string& pMessage)
{
	std::string msg = pMessage;
	//std::cout << pMessage << std::endl;
	if(msg.find("PING") == 0)
	{
		msg[1] = 'O';
		Send(msg + std::string("\r\n"));
		return;
	}
	
	msg.erase(0, 1);
	std::string host, opcode;
	size_t pos = msg.find(" ");
	host = msg.substr(0, pos);
	msg.erase(0, pos + 1);
	pos = msg.find(" ");
	opcode = msg.substr(0, pos);
	msg.erase(0, pos + 1);

	//std::cout << host << " " << opcode << " " << msg << std::endl;

	auto itor = mHandlers.find(opcode);
	if(itor != mHandlers.end())
	{
		itor->second(host, msg);
	}
}

void Client::AddListener(Listener* pListener)
{
	mListeners.push_back(std::shared_ptr<Listener>(pListener));
	mListeners.unique();
}

void Client::RemoveListener(Listener* pListener)
{
	for(auto itor = mListeners.begin(), end = mListeners.end(); itor != end; ++itor)
	{
		if(itor->get() == pListener)
		{
			mListeners.erase(itor);
			return;
		}
	}
}

void Client::_OnConnected(const std::string& pHost, const std::string& pContent)
{
	std::for_each(mListeners.begin(), mListeners.end(), 
		[this](Client::Listener::pointer pListener)
		{
			pListener->OnConnected(this);
		});
}

void Client::_OnMessage(const std::string& pHost, const std::string& pContent)
{
	std::string sender = pHost.substr(0, pHost.find("!"));
	size_t pos = pContent.find(" ");
	std::string channel = pContent.substr(0, pos);
	pos += 2;
	std::string message = pContent.substr(pos);

	std::for_each(mListeners.begin(), mListeners.end(), 
		[this, &sender, &channel, &message](Client::Listener::pointer pListener)
		{
			pListener->OnMessage(this, sender, channel, message);
		});
}

void Client::_OnNotice(const std::string& pHost, const std::string& pContent)
{
	if(!mConnected && pContent.find("Checking") != std::string::npos)
	{
		mConnected = true;
		SendIdent();
	}
}