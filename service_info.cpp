#include "consul/service_info.h"

ServiceInfo::ServiceInfo() {
    serviceCheck = nullptr;
}

ServiceInfo::~ServiceInfo() {
}

ServiceInfo::ServiceCheck::ServiceCheck() {
    protocol = "TCP";
    url = "";
    status = "passing";
    intervalMs = 10000;
    timeoutMs = 3000;
}

ServiceInfo& ServiceInfo::setServiceId(std::string id) {
    serviceId = id;
    return *this;
}

std::string ServiceInfo::getServiceId() const {
    return serviceId;
}

ServiceInfo& ServiceInfo::setServiceName(std::string name) {
    serviceName = name;
    return *this;
}

std::string ServiceInfo::getServiceName() const {
    return serviceName;
}

ServiceInfo& ServiceInfo::setServiceIp(std::string address) {
    serviceIp = address;
    return *this;
}

std::string ServiceInfo::getServiceIp() const {
    return serviceIp;
}

ServiceInfo& ServiceInfo::setServicePort(int port) {
    servicePort = port;
    return *this;
}

int ServiceInfo::getServicePort() const {
    return servicePort;
}

ServiceInfo& ServiceInfo::setServiceTags(std::vector<std::string> tags) {
    serviceTags = tags;
    return *this;
}

std::vector<std::string> ServiceInfo::getServiceTags() const {
    return serviceTags;
}

ServiceInfo& ServiceInfo::setServiceCheck(ServiceCheck* check) {
    serviceCheck = check;
    return *this;
}

ServiceInfo::ServiceCheck* ServiceInfo::getServiceCheck() const {
    return serviceCheck;
}