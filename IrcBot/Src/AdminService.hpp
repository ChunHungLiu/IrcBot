#pragma once

#include "Client.hpp"
#include "QuidditchService.hpp"
#include "LogService.hpp"

class AdminService : public Client::Listener
{
public:

	void OnConnected(Client* pClient){}
	void OnMessage(Client* pClient, const std::string& pSender, const std::string& pChannel, const std::string& pMessage)
	{
		if(IsValid(pSender) && pMessage.size() && pMessage[0] == ':')
		{
			std::string msg = pMessage;
			msg.erase(0,1);
			MessageSplit("join")
				pClient->Join(msg);
			}
			else MessageSplit("leave")
				pClient->Leave(msg);
			}
			else MessageSplit("nick")
				pClient->SetNick(msg);
			}
			else MessageSplit("register")
				mAdmins.push_back(msg);
				mAdmins.unique();
				pClient->SendToChannel(pSender, std::string("Utilisateur ") + msg + std::string(" ajoute aux admins."));
				pClient->SendToChannel(msg, "Vous etes admin !");
			}
			else MessageSplit("log")
				std::string user = NextToken(msg, ' ');
				mLogs[user] = new LogService(user);
				pClient->AddListener(mLogs[user]);
			}
			else MessageSplit("unlog")
				std::string user = NextToken(msg, ' ');
				auto it = mLogs.find(user);
				if(it != mLogs.end())
				{
					pClient->RemoveListener(it->second);
					mLogs.erase(it);
				}
			}
			else MessageSplit("remove")
				mAdmins.remove(msg);
				pClient->SendToChannel(pSender, std::string("Utilisateur ") + msg + std::string(" supprime des admins."));
				pClient->SendToChannel(msg, "Vous n'etes plus admin !");
			}
			else MessageSplit("say")
				std::string channel = NextToken(msg, ' ');
				pClient->SendToChannel(channel, msg);
			}
			else MessageSplit("quid")
				std::string user = NextToken(msg, ' ');
				mQuids[user] = new QuidditchService(user);
				pClient->AddListener(mQuids[user]);
			}
			else MessageSplit("stopquid")
				std::string user = NextToken(msg, ' ');
				auto it = mQuids.find(user);
				if(it != mQuids.end())
				{
					pClient->RemoveListener(it->second);
					mQuids.erase(it);
				}
			}
		}
	}

	bool IsValid(const std::string& pUser)
	{
		if(pUser == std::string("Yamashi")) return true;
		for(auto itor = mAdmins.begin(), end = mAdmins.end(); itor != end; ++itor)
		{
			if(*itor == pUser)
				return true;
		}
		return false;
	}

private:

	std::map<std::string, QuidditchService*> mQuids;
	std::map<std::string, LogService*> mLogs;
	std::list<std::string> mAdmins;
};