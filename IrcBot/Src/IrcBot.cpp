#include "stdafx.h"
#include "Client.hpp"

#include "LogService.hpp"
#include "AdminService.hpp"
#include "QuidditchService.hpp"

int main(int argc, char* argv[])
{
	try
	{
		Client client("minefrance.com", 6667);

		client.AddListener(new AdminService);

		client.SetUser("Yamasushi");
		client.SetNick("Yamasushi");
		client.SendIdent();

		client.SendToChannel("root", "bot bot");
		client.SendToChannel("root", "remove zen");
		client.SendToChannel("root", "create zen zen-attitude.org");
		client.Join("#taki");
		
		while(true)
		{
			client.Poll();
		}
	}
	catch(std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	exit(0);

	return 0;
}

