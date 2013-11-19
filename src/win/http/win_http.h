/*
** Taiga
** Copyright (C) 2010-2013, Eren Okka
** 
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef WIN_HTTP_H
#define WIN_HTTP_H

#include <windows.h>
#include <winhttp.h>
#include <map>
#include <string>
#include <vector>

namespace win {
namespace http {

typedef std::multimap<std::wstring, std::wstring> header_t;
typedef std::multimap<std::wstring, std::wstring> query_t;

enum ContentEncoding {
  kContentEncodingNone,
  kContentEncodingGzip
};

class Request {
public:
  Request();
  virtual ~Request() {}

  void Clear();

  std::wstring method;
  std::wstring host;
  std::wstring path;
  query_t query;
  header_t header;
  std::wstring body;

  std::wstring uuid;
  LPARAM parameter;
};

class Response {
public:
  Response();
  virtual ~Response() {}

  void Clear();

  unsigned int code;
  header_t header;
  std::wstring body;

  std::wstring uuid;
  LPARAM parameter;
};

class Client {
public:
  Client();
  virtual ~Client();

  void Cleanup();
  bool MakeRequest(Request request);

  const Request& request() const;
  const Response& response() const;

  void set_auto_redirect(bool enabled);
  void set_download_path(const std::wstring& download_path);
  void set_proxy(const std::wstring& host,
                 const std::wstring& username, const std::wstring& password);
  void set_referer(const std::wstring& referer);
  void set_user_agent(const std::wstring& user_agent);

  virtual void OnError(DWORD error) {}
  virtual bool OnSendRequestComplete() { return false; }
  virtual bool OnHeadersAvailable() { return false; }
  virtual bool OnDataAvailable() { return false; }
  virtual bool OnReadData() { return false; }
  virtual bool OnReadComplete() { return true; }  // TODO: Why "true"?
  virtual bool OnRedirect(const std::wstring& address) { return false; }

protected:
  Request request_;
  Response response_;

  LPSTR buffer_;
  ContentEncoding content_encoding_;
  DWORD content_length_;
  DWORD current_length_;

  bool auto_redirect_;
  std::wstring download_path_;
  std::wstring proxy_host_;
  std::wstring proxy_password_;
  std::wstring proxy_username_;
  std::wstring referer_;
  std::wstring user_agent_;

private:
  static void CALLBACK Callback(HINTERNET hInternet,
                                DWORD_PTR dwContext,
                                DWORD dwInternetStatus,
                                LPVOID lpvStatusInformation,
                                DWORD dwStatusInformationLength);
  void StatusCallback(HINTERNET hInternet,
                      DWORD dwInternetStatus,
                      LPVOID lpvStatusInformation,
                      DWORD dwStatusInformationLength);

  HINTERNET OpenSession();
  HINTERNET ConnectToSession();
  HINTERNET OpenRequest();
  BOOL SetRequestOptions();
  BOOL SendRequest();

  std::wstring BuildRequestHeader();
  bool GetResponseHeader(const std::wstring& header);
  bool ParseResponseHeader();

  HINTERNET connection_handle_;
  HINTERNET request_handle_;
  HINTERNET session_handle_;
};

class Url {
public:
  Url() {}
  Url(const std::wstring& url);
  virtual ~Url() {}

  void Crack(std::wstring url);

  Url& operator=(const Url& url);
  void operator=(const std::wstring& url);

  std::wstring scheme;
  std::wstring host;
  std::wstring path;
};

std::wstring GetUrlEncodedString(const std::wstring& str,
                                 bool encode_unreserved);

}  // namespace http
}  // namespace win

#endif  // WIN_HTTP_H