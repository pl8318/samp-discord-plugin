#include "discord.h"
#include "env.h"

namespace Discord
{
	static void ready(const DiscordUser* request) {}

	static void errored(int errorCode, const char* message) {}

	static void disconnected(int errorCode, const char* message) {}

	static void joinGame(const char* joinSecret) {}

	static void spectateGame(const char* spectateSecret) {}

	static void joinRequest(const DiscordUser* request) {}

	void initialize()
	{
		DiscordEventHandlers handlers = { 0 };
		handlers.ready = ready;
		handlers.errored = errored;
		handlers.disconnected = disconnected;
		handlers.joinGame = joinGame;
		handlers.spectateGame = spectateGame;
		handlers.joinRequest = joinRequest;
		Discord_Initialize("1153658677249720431", &handlers, 1, nullptr);
		//Discord_Initialize(DiscordAPPID, &handlers, 1, nullptr);
		Discord_ClearPresence();
	}

	void update(const time_t time, const char* state, const char* details, const char* image, const char* imageDetails, const char* infoDetails)
	{
		DiscordRichPresence discordPresence = { 0 };
		discordPresence.state = state;
		discordPresence.details = details;
		discordPresence.startTimestamp = time;
		discordPresence.largeImageKey = image;
		discordPresence.largeImageText = imageDetails;
		discordPresence.smallImageKey = "info";
		discordPresence.smallImageText = infoDetails;

		std::string website = "https://www.youtube.com/playlist?list=PLrTPcZSrghctk2kS3vr9VO6SsEKoxp46s";
		std::string modpack = "https://www.mediafire.com/folder/bubagwr56md0u/MNGC";

		DiscordButton buttons[] = {
		  {"Visit website", website.c_str()},
		  {"Get MNGC modpack", modpack.c_str()},
		};

		discordPresence.buttons = buttons;

		Discord_UpdatePresence(&discordPresence);
	}
}
