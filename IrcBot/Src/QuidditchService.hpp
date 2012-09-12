#pragma once

#include "Client.hpp"

class QuidditchService : public Client::Listener
{
public:

	QuidditchService(const std::string& pUser) : mUser(pUser){}
	void OnConnected(Client* pClient){}
	void OnMessage(Client* pClient, const std::string& pSender, const std::string& pChannel, const std::string& pMessage)
	{
		if(pSender == mUser)
		{
			if(pMessage.find("Le Vif d'Or se lance dans un fantastique") != std::string::npos)
			{
				pClient->SendToChannel(pChannel, "!fait un plongeon");
			}
			else if(pMessage.find("Le Vif d'Or tourne brutalement à") != std::string::npos && pMessage.find("droite") != std::string::npos)
			{
				pClient->SendToChannel(pChannel, "!vire à droite");
			}
			else if(pMessage.find("Le Vif d'Or monte en") != std::string::npos)
			{
				pClient->SendToChannel(pChannel, "!monte en chandelle");
			}
			else if(pMessage.find("Le Vif d'Or tourne brutalement") != std::string::npos && pMessage.find("gauche") != std::string::npos)
			{
				pClient->SendToChannel(pChannel, "!vire à gauche");
			}
			else if(pMessage.find("Nouveau Vif d'Or dans 5 secondes !") != std::string::npos)
			{
				pClient->SendToChannel(pChannel, "!joueur");
			}
			else if(pMessage.find("Mais.... non... ca ne serait pas le Vif d'Or pas hasard ??") != std::string::npos)
			{
				pClient->SendToChannel(pChannel, "!suit le vif d'or");
			}
			else if(pMessage.find("est en position pour attraper le Vif d'Or !") != std::string::npos)
			{
				pClient->SendToChannel(pChannel, "!attrape le vif d'or");
			}
		}
	}
private:

	std::string mUser;
};