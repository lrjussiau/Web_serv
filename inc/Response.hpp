#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "WebServ.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Utils.hpp"

class Response {

	private:
		std::string							_status_line;
		std::map<std::string, std::string>	_headers;
		std::string							_content;
		std::string							_final_reply;
		Client								_client;

		void 		buildResponse(void);
		void 		createContent(std::string path, int status_code, std::string status_message);
		void		buildRedirectResponse(std::string redirect_path);
		void		init_headers(void);
		bool		checkMimeType(std::string mime);
		void		generateAutoIndex(std::string dir_requested);

	public:
		Response(void);
		Response(Client client, ServerConfig server);
		~Response(void);
		Response(const Response& src);


		std::string getFinalReply(void) const;

		
};

#endif