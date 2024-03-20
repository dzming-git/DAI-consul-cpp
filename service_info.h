/*****************************************************************************
*  Copyright © 2023 - 2023 dzming.                                           *
*                                                                            *
*  @file     service_info.h                                                  *
*  @brief    Service information for registration or discovery               *
*  @author   dzming                                                          *
*  @email    dzm_work@163.com                                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark  :                                                                 *
*****************************************************************************/

#ifndef _SERVICE_INFO_H_
#define _SERVICE_INFO_H_

#include <vector>
#include <string>

class ServiceInfo {
public:
    ServiceInfo();
    ~ServiceInfo();
    struct ServiceCheck;

    ServiceInfo& setServiceId(std::string id);
    std::string getServiceId() const;

    ServiceInfo& setServiceName(std::string name);
    std::string getServiceName() const;

    ServiceInfo& setServiceIp(std::string address);
    std::string getServiceIp() const;

    ServiceInfo& setServicePort(int port);
    int getServicePort() const;

    ServiceInfo& setServiceTags(std::vector<std::string> tags);
    std::vector<std::string> getServiceTags() const;

    ServiceInfo& setServiceCheck(ServiceCheck* check);
    ServiceCheck* getServiceCheck() const;
private:
    std::string serviceId;
    std::string serviceName;
    std::string serviceIp;
    int servicePort;
    std::vector<std::string> serviceTags;
    ServiceCheck* serviceCheck;
};

struct ServiceInfo::ServiceCheck {
    ServiceCheck();

    std::string protocol;
    std::string url;
    std::string status;
    int intervalMs;
    int timeoutMs;
};

#endif /* _SERVICE_INFO_H_ */
