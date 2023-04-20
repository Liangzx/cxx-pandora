//
// Created by dingjia on 2022/11/1.
//

#ifndef VIRT_SAMPLES_CONNECTION_H
#define VIRT_SAMPLES_CONNECTION_H

#include <libvirt/libvirt.h>

using ConnectCredentialType = virConnectCredentialType;
using ConnectAuthCallback = virConnectAuthCallbackPtr;

struct ConnectAuth {
    Callback ConnectAuthCallback;
    CredType []ConnectCredentialType;
};

class Connection {
public:
    Connection();

private:
    virConnectPtr conn_;
};


#endif//VIRT_SAMPLES_CONNECTION_H
