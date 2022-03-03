#include "SuplaRestLib.h"

SuplaRestClient::SuplaRestClient() :
		accessToken(""), targetUrl(""), accessTokenCreationTime(0), accessTokenExpireTime(
				0), userName(""), password(""), headers(""), body("") {
	iterate();
}

void SuplaRestClient::begin(String userName, String password) {
	this->userName = userName;
	this->password = password;
}

void SuplaRestClient::iterate() {

	int expiringIn = accessTokenCreationTime + accessTokenExpireTime
			- (millis() / 1000);
	if (accessToken.length() != 0 && expiringIn > 0) {
		log_d("Token valid, expiring in %d.", expiringIn);
		return;
	}
	if (loadAccessToken()) {
		log_i("Token loaded.");
		return;
	}

	log_w("Invalidating token.");

	accessToken = "";
	targetUrl = "";
	accessTokenExpireTime = 0;
}

bool SuplaRestClient::openRollerShutter(int channelId) {
	String path = "/api/channels/";
	path += channelId;
	return patch(path, "{\"action\": \"REVEAL\"}");
}

bool SuplaRestClient::closeRollerShutter(int channelId) {
	String path = "/api/channels/";
	path += channelId;
	return patch(path, "{\"action\": \"SHUT\"}");
}

bool SuplaRestClient::loadAccessToken() {
	if (userName.length() == 0 || password.length() == 0) {
		log_e("No user and password specified");
		return false;
	}

	String data = "{ \"username\": \"" + userName + "\", \"password\": \""
			+ password + "\" }";
	char *buf = new char[data.length() + 1];
	data.toCharArray(buf, data.length() + 1);
	bool callSuccess = post("svr49.supla.org", "/api/webapp-auth", buf, "");
	delete buf;

	if (!callSuccess) {
		log_w("Error by trying to get token - request failed.");
		return false;
	}

	StaticJsonDocument < 1024 > doc;
	DeserializationError error = deserializeJson(doc, body);

	if (error) {
		log_e("Token response serialization failed (return code: %d).", error);
		return false;
	}

	accessTokenCreationTime = millis() / 1000;
	const char *token = doc["access_token"];
	accessToken = token;
	const char *url = doc["target_url"];
	targetUrl = String(url + 8);
	accessTokenExpireTime = doc["expires_in"];

	return true;
}

bool SuplaRestClient::patch(String path, const char *data) {
	if (accessToken.length() == 0 || targetUrl.length() == 0) {
		log_d("No token for request available.")
		return false;
	}

	String additionalHeaders = "Authorization: Bearer " + accessToken + "\r\n";
	return patch(targetUrl, path, data, additionalHeaders);
}

bool SuplaRestClient::patch(String host, String path, const char *data,
		String additionalHeaders) {
	return call(host, path, "PATCH", data, additionalHeaders);
}

bool SuplaRestClient::post(String host, String path, const char *data,
		String additionalHeaders) {
	return call(host, path, "POST", data, additionalHeaders);
}

bool SuplaRestClient::call(String host, String path, const char *type,
		const char *data, String additionalHeaders) {

	headers = "";
	body = "";

	WiFiClientSecure httpClient;
	httpClient.setInsecure();

	if (!httpClient.connect(host.c_str(), 443)) {
		return false;
	}

	httpClient.print(type);
	httpClient.print(" ");
	httpClient.print(path);
	httpClient.println(" HTTP/1.1");
	httpClient.print("Host: ");
	httpClient.println(host);
	if (additionalHeaders.length() > 0) {
		httpClient.print(additionalHeaders);
	}
	httpClient.println("Connection: close");
	httpClient.print("Content-Length: ");
	httpClient.println(strlen(data));
	httpClient.println();
	httpClient.println(data);
	httpClient.println();

	while (httpClient.connected()) {
		String line = httpClient.readStringUntil('\n');
		headers += line;
		if (headers.length() > 0) {
			headers += "\n";
		}
		if (line == "\r") {
			break;
		}
	}

	while (httpClient.available()) {
		char c = httpClient.read();
		body += c;
	}

	httpClient.stop();

	return headers.length() > 0 && body.length() > 0
			&& headers.startsWith("HTTP/1.1 200 OK");
}

SuplaRestClient SuplaRest;
