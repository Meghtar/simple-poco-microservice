#include "BaseServerRequestHandler.h"

#include "Poco/JSON/Object.h"
#include <glog/logging.h>

std::string BaseServerRequestHandler::createUrl(const std::string& protocol, const std::string& host, const uint16_t port, const std::string& endpoint)
{
    return protocol + "://" + host + ":" + std::to_string(port) + "/" + endpoint;
}

void BaseServerRequestHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
    {
        response.setChunkedTransferEncoding(true);
        response.setContentType("application/json");

        std::ostream& out = response.send();
        LOG(INFO) << "Received request";

        std::string content = getRequestContent(request);

        std::string externalResponse = "";

        if (_useRemote)
        {
            std::string body = "{\"Example\":\"content\"}";
            std::map<std::string,std::string> headers;

            LOG(INFO) << "sending to " << _remoteHost << " on " << _remotePort;

            std::string addr = createUrl("http", _remoteHost, _remotePort);
            
            externalResponse = sendRequest(addr, body, headers);

            LOG(INFO) << externalResponse;

        }

        const std::string defaultText = "default text";
        const std::string responseText = _useRemote ? prepareResponse(request, externalResponse) : prepareResponse(request, defaultText);

        out << responseText;

        out.flush();
    }

std::string BaseServerRequestHandler::getRequestContent(Poco::Net::HTTPServerRequest& request)
{
    auto & stream = request.stream();
    const size_t len = request.getContentLength();
    std::string buffer(len, 0);
    stream.read(buffer.data(), len);

    return buffer;
}

std::string BaseServerRequestHandler::sendRequest(std::string url, std::string body, std::map<std::string,std::string> headers) const
{
try
    {
        // prepare session
        URI uri(url);
        HTTPClientSession session(uri.getHost(), uri.getPort());

        // prepare path
        std::string path(uri.getPathAndQuery());
        if (path.empty()) path = "/";
            
        // send request
        HTTPRequest req(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);
        req.setContentType("application/json");
            
        // Set headers here
        for(std::map<std::string,std::string>::iterator it = headers.begin();
            it != headers.end(); it++) {
            req.set(it->first, it->second);
        }

        // Set the request body
        req.setContentLength( body.length() );
            
        std::ostream& os = session.sendRequest(req);
        os << body;
            
        // get response
        HTTPResponse res;
        LOG(INFO) << res.getStatus() << " " << res.getReason();
            
        std::istream &is = session.receiveResponse(res);
        std::stringstream ss;
        StreamCopier::copyStream(is, ss);
            
        return ss.str();
    }
    catch (Exception &ex)
    {
        LOG(ERROR) << ex.displayText();
        return "";
    }
}

std::string BaseServerRequestHandler::prepareResponse(const Poco::Net::HTTPServerRequest& request, const std::string& additionalText)
{
    Poco::JSON::Object response;

    response.set("URI", request.getURI());
    response.set("contentType", request.getContentType());
    response.set("miscText", _text);
    response.set("additionalText", additionalText);

    Poco::Dynamic::Var response_var = response;

    return response_var.toString();
}