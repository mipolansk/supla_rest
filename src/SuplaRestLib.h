#ifndef SUPLARESTLIB_H_
#define SUPLARESTLIB_H_

#ifndef TEST

#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#else

#include "types.h";

#endif

class SuplaRestClient {
	String accessToken;
	String targetUrl;
	unsigned long accessTokenCreationTime;
	unsigned long accessTokenExpireTime;

	String userName;
	String password;

	String headers;
	String body;

	bool patch(String path, const char *data);

	bool patch(String host, String path, const char *data,
			String additionalHeaders);
	bool post(String host, String path, const char *data,
			String additionalHeaders);

	bool call(String host, String path, const char *type, const char *data,
			String additionalHeaders);

	bool loadAccessToken();

public:
	SuplaRestClient();

	void begin(String userName, String password);
	void iterate();

	bool openRollerShutter(int channelId);
	bool closeRollerShutter(int channelId);
};

extern SuplaRestClient SuplaRest;

#endif /* SUPLARESTLIB_H_ */
