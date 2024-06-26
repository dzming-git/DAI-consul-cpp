#include "consul/consul_client.h"
#include "hv/requests.h"
#include "hv/json.hpp"
#include "hv/hurl.h"
#include <string>

ConsulClient::ConsulClient(): consulIp("127.0.0.1"), consulPort(8500) {
    
}

ConsulClient& ConsulClient::setConsulIp(std::string ip) {
    consulIp = ip;
    return *this;
}

ConsulClient& ConsulClient::setConsulPort(int port) {
    consulPort = port;
    return *this;
}

bool ConsulClient::registerService(const ServiceInfo& serverInfo) {
    HttpRequest req;
    req.method = HTTP_PUT;
    req.url = "http://" + consulIp + ":" + std::to_string(consulPort) + "/v1/agent/service/register";
    req.content_type = APPLICATION_JSON;
    nlohmann::json jserviceInfo;
    jserviceInfo["ID"] = serverInfo.getServiceId();
    jserviceInfo["Name"] = serverInfo.getServiceName();
    jserviceInfo["Address"] = serverInfo.getServiceIp();
    jserviceInfo["Port"] = serverInfo.getServicePort();    
    nlohmann::json jtags = nlohmann::json::array();
    for (auto& tag : serverInfo.getServiceTags()) {
        jtags.push_back(tag);
    }
    jserviceInfo["Tags"] = jtags;
    nlohmann::json jserviceCheckInfo;
    ServiceInfo::ServiceCheck* check = serverInfo.getServiceCheck();
    bool needDeleteCheckPtr = false;
    if (nullptr == check) {
        needDeleteCheckPtr = true;
        check = new ServiceInfo::ServiceCheck();
        check->url = serverInfo.getServiceIp() + ":" + std::to_string(serverInfo.getServicePort());
    }
    jserviceCheckInfo[check->protocol] = check->url;
    jserviceCheckInfo["Interval"] = std::to_string(check->intervalMs) + "ms";
    jserviceCheckInfo["DeregisterCriticalServiceAfter"] = std::to_string(check->intervalMs * 3) + "ms";
    if (needDeleteCheckPtr) {
        delete check;
    }
    jserviceInfo["Check"] = jserviceCheckInfo;
    req.body = jserviceInfo.dump();
    std::cout << "PUT " << req.url << std::endl
              << req.body << std::endl;
    HttpResponse res;
    int ret = http_client_send(&req, &res);
    std::cout << res.body << std::endl;
    return ret;
}

bool ConsulClient::discoverServices(std::string serviceName, std::vector<ServiceInfo>& services) {
    HttpRequest req;
    req.method = HTTP_GET;
    req.url = "http://" + consulIp + ":" + std::to_string(consulPort) + "/v1/catalog/service/" + HUrl::escape(serviceName);
    std::cout << "GET" << std::endl << req.url << std::endl;
    HttpResponse res;

    int ret = http_client_send(&req, &res);
    if (ret != 0) return false;
    std::cout << res.body << std::endl;

    nlohmann::json jroot = nlohmann::json::parse(res.body);
    if (!jroot.is_array()) return false;
    if (jroot.size() == 0) return true;

    for (size_t i = 0; i < jroot.size(); ++i) {
        ServiceInfo serverInfo;
        auto jservice = jroot[i];
        serverInfo.setServiceName(jservice["ServiceName"]);
        serverInfo.setServiceId(jservice["ServiceID"]);
        if (jservice.contains("ServiceAddress")) {
            serverInfo.setServiceIp(jservice["ServiceAddress"]);
        } 
        else {
            continue;
        }
        if (jservice.contains("ServiceTags")) {
            auto serviceTagsArray = jservice["ServiceTags"];
            std::vector<std::string> tags(serviceTagsArray.size());
            for (size_t j = 0; j < serviceTagsArray.size(); ++j) {
                tags[j] = serviceTagsArray[j];
            }
            serverInfo.setServiceTags(tags);
        }
        serverInfo.setServicePort(static_cast<uint16_t>(jservice["ServicePort"]));
        services.emplace_back(serverInfo);
    }
    return true;
}
