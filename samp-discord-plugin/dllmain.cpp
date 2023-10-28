#include <windows.h>
#include <process.h>
#include <iostream>
#include "client.h"
#include "discord.h"
#include "query.h"
#include "http.h"

static void process(void*)
{
	SAMP::ServerData data;
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	Discord::initialize();
	if (SAMP::readServerData(GetCommandLine(), data)) {
		std::string logo = "logo";
		{
			
			std::stringstream httpResponseStream;
			if (
				HTTP::WebRequest(
					[&httpResponseStream](auto data, auto len)
					{
						httpResponseStream.write(data, len);
						return true;
						// THIS ONLY WORKS PROPERLY ON WINDOWS 10 LOL
					}, "Mozilla/5.0", "raw.githubusercontent.com", INTERNET_DEFAULT_HTTPS_PORT)
					.get("pl8318/samp-discord-plugin/server_logos/asset.txt")
			   ) { 
				logo = data.logoFromStream(httpResponseStream, logo);
			   }
		}

		auto start = std::time(0);
		if (data.connect == SAMP::SAMP_CONNECT_SERVER) {
			SAMP::Query query(data.address, std::stoi(data.port));
			while (true) {
				SAMP::Query::Information information;
				if (query.info(information)) {
				//	auto fullAddress = data.address + ':' + data.port;
					auto fullAddress = "";
					auto players = std::to_string(information.basic.players) + "/" + std::to_string(information.basic.maxPlayers) + " players online";
					auto info = "Playing " + information.gamemode + " as " + data.username + " in " + information.language;
					auto fuck = "";
					auto really = "is this real?";
					auto image = logo;
					if (image == "logo") {
						if (information.basic.password) {
							image = "lock";
						}
						else if (information.basic.players < 10) {
							image = "metaicon";
						}
					}
					if (image == "comrp") {
						std::string strand = "Playing " + information.gamemode + " as " + data.username;
						fullAddress = strand.c_str();
					} else if (image == "foep") {
						fullAddress = "is this real?";
					}
					Discord::update(start, fuck, fullAddress, image, really, fuck);
					Sleep(15000-QUERY_DEFAULT_TIMEOUT*2);
				}
			}
		}
		else if (data.connect == SAMP::SAMP_CONNECT_DEBUG) {
			while (true) {
				Discord::update(start, "localhost", "Debug server", "metaicon", "Playing debug mode in English", "Most likely 1 player online as it's debug mode");
				Sleep(15000);
			}
		}
	}
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    switch (reason)
    {
		case DLL_PROCESS_ATTACH: {
			DisableThreadLibraryCalls(module);
			_beginthread(&process, 0, nullptr);
			break;
		}
		case DLL_PROCESS_DETACH: {
			WSACleanup();
			break;
		}
    }
    return TRUE;
}
