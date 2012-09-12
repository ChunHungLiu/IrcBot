#pragma once

#define MessageSplit(x) \
	if(msg.find(x) == 0) \
	{ \
		msg.erase(0, strlen(x) + 1);

std::string NextToken(std::string& pTokenized, char pSpliter);

class Client
{
public:

	struct Listener
	{
		typedef std::shared_ptr<Listener> pointer;

		virtual void OnMessage  (Client* pClient, const std::string& pSender, const std::string& pChannel, const std::string& pMessage) = 0;
		virtual void OnConnected(Client* pClient) = 0;
	};

	Client(const std::string& pAddress, uint16_t pPort);

	void Poll();

	void SendIdent();
	void SetUser(const std::string& pUser);
	void SetNick(const std::string& pNick);
	void Join(const std::string& pChannel);
	void Leave(const std::string& pChannel);
	void SendToChannel(const std::string& pChannel, const std::string& pMessage);

	void AddListener(Listener* pListener);
	void RemoveListener(Listener* pListener);

private:

	void Parse(const std::string& pMessage);

	void Send(const std::string& pData);

	void SendUser();
	void SendNick();

	void _OnConnected(const std::string& pHost, const std::string& pContent);
	void _OnMessage(const std::string& pHost, const std::string& pContent);
	void _OnNotice(const std::string& pHost, const std::string& pContent);

	std::map<std::string, std::function<void(const std::string&, const std::string&)>> mHandlers;

	std::string mUser, mNick;
	std::string mBuffer;

	bool mConnected;

	std::list<Listener::pointer> mListeners;

	sf::TcpSocket mSocket;
	sf::SocketSelector mSelector;
};