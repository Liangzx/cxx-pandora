#include <iostream>
#include <libvirt/libvirt.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>

#include <algorithm>
#include <memory>
#include <string>

/* Struct to pass the credentials to the auth callback via the cbdata pointer */
struct AuthData {
    char *username;
    char *password;
};

/* This function will be called by libvirt to obtain credentials in order to
 * authenticate to the hypervisor */
static int
authCallback(virConnectCredentialPtr cred, unsigned int ncred, void *cbdata) {
    size_t i;
    auto *authData = static_cast<AuthData *>(cbdata);

    /* libvirt might request multiple credentials in a single call.
     * This example supports VIR_CRED_AUTHNAME and VIR_CRED_PASSPHRASE
     * credentials only, but there are several other types.
     *
     * A request may also contain a prompt message that can be displayed
     * to the user and a challenge. The challenge is specific to the
     * credential type and hypervisor type.
     *
     * For example the ESX driver passes the hostname of the ESX or vCenter
     * server as challenge. This allows a auth callback to return the
     * proper credentials. */
    for (i = 0; i < ncred; ++i) {
        switch (cred[i].type) {
            case VIR_CRED_AUTHNAME:
                cred[i].result = strdup(authData->username);

                if (cred[i].result == nullptr)
                    return -1;

                cred[i].resultlen = strlen(cred[i].result);
                break;

            case VIR_CRED_PASSPHRASE:
                cred[i].result = strdup(authData->password);

                if (cred[i].result == nullptr)
                    return -1;

                cred[i].resultlen = strlen(cred[i].result);
                break;

            default:
                return -1;
        }
    }

    return 0;
}


/* The list of credential types supported by our auth callback */
static int credTypes[] = {
        VIR_CRED_AUTHNAME,
        VIR_CRED_PASSPHRASE};


/* The auth struct that will be passed to virConnectOpenAuth */
static virConnectAuth auth = {
        credTypes,
        sizeof(credTypes) / sizeof(int),
        authCallback,
        nullptr, /* cbdata will be initialized in main */
};


virConnectPtr ConnectOpenAuth() {
    // 172.20.10.151 | 172.20.10.128
    std::string host_ip = "172.20.10.128";
    auto uri = "qemu+tcp://" + host_ip + "/system";
    AuthData authData{};
    authData.username = "root";
    authData.password = "dingjia";
    auth.cbdata = &authData;

    auto conn = virConnectOpenAuth(uri.c_str(), &auth, 0);

    return conn;
}
//
virConnectPtr ConnectOpen() {
    //  driver[+transport]://[username@][hostname][:port]/[path][?extraparameters]
    auto conn = virConnectOpen("qemu:///system");

    //    auto conn = virConnectOpen("xenapi://root@172.20.60.131");
    //    auto conn = virConnectOpenAuth("xenapi://root@172.20.60.131", virConnectAuthPtrDefault, 0);
    //    auto dom_count = virConnectNumOfDomains(conn);
    //    std::cout << "dom_count:" << dom_count << std::endl;
    //    auto capabilities = virConnectGetCapabilities(conn);
    //    std::cout << "capabilities:" << capabilities << std::endl;
    //    free(capabilities);
    //    if (conn != nullptr) {
    //        virConnectClose(conn);
    //    }
    return conn;
}

bool StorageVolLookupByPath(virConnectPtr conn, const std::string &path) {
    auto vol_ptr = virStorageVolLookupByPath(conn, path.c_str());
    if (nullptr == vol_ptr) {
        std::cout << "nullptr == vol_ptr" << std::endl;
        return false;
    }

    return true;
}

void ConnectClose(virConnectPtr conn) {
    if (nullptr != conn) {
        virConnectClose(conn);
    }
}

enum Color : short {
    RED = 0,
    GREEN = 1
};

#include <fcntl.h>
void openfile() {
    auto path = "/var/lib/dbackup3/storaged/data/305b3e76537311ed800000505696ea55/vmserverd/KvmBackupStorage/18060319-81e9-4b8f-a81c-4650ae60e6e7#1666663104/vmbackup_1666608815_vmbackup_1666608484_vmbackup_1666604000_vmbackup_1666603295_vmbackup_1666601867_vmbackup_1662622359_vmbackup_1639634370_ubuntu-5G.qcow2";
    auto fd = open(path, O_WRONLY | O_CREAT, 0640);
    if (fd < 0) {
        std::cout << "openfile" << std::endl;
    }
}
// 18060319-81e9-4b8f-a81c-4650ae60e6e7
void DomainLookupByUUIDString(virConnectPtr conn) {
    auto vm_uuid = "18060319-81e9-4b8f-a81c-4650ae60e6e7";
    std::shared_ptr<virDomain> domain(virDomainLookupByUUIDString(conn, vm_uuid), [](virDomainPtr vp) {
        if (nullptr != vp) {
            virDomainFree(vp);
        }
    });
    //

    std::shared_ptr<char> domain_xml(virDomainGetXMLDesc(domain.get(), VIR_DOMAIN_XML_SECURE), [](char *p) {
        if (nullptr != p) {
            free(p);
        }
    });
    //
    std::string ss{domain_xml.get()};
    //
    std::cout << ss << std::endl;
}
//
void DomainListAllSnapshots(virConnectPtr conn) {
    auto vm_uuid = "59c8413f-933d-4415-b7d9-a8daa8b55a31";
    std::unique_ptr<virDomain, void (*)(virDomainPtr)> domain(virDomainLookupByUUIDString(conn, vm_uuid), [](virDomainPtr vp) {
        if (nullptr != vp) {
            virDomainFree(vp);
        }
    });
    virDomainSnapshotPtr *snaps = nullptr;
    int flag = 0;
    flag |= VIR_DOMAIN_SNAPSHOT_LIST_EXTERNAL;
    int snap_num = virDomainListAllSnapshots(domain.get(), &snaps, flag);

    for (int i = 0; i < snap_num; i++) {
        std::string snapshot_name = virDomainSnapshotGetName(snaps[i]);
        std::cout << snapshot_name << std::endl;
    }
    std::cout << virDomainSnapshotNum(domain.get(), flag) << std::endl;
}

void DomainGetXMLDesc(virConnectPtr conn) {
    auto domain_uuid_str = "13cf615f-cefd-4442-9c93-772bc512e8df";

    std::shared_ptr<virDomain> d(virDomainLookupByUUIDString(conn, domain_uuid_str), [](virDomainPtr vp) {
        if (nullptr != vp) {
            virDomainFree(vp);
        }
    });
    //    auto domain_name = "ubuntu.1804.5G-external";
    //    std::shared_ptr<virDomain> d(virDomainLookupByName(conn, domain_name), [](virDomainPtr vp) {
    //        if (nullptr != vp) {
    //            virDomainFree(vp);
    //        }
    //    });

    char *domain_xml = virDomainGetXMLDesc(d.get(), VIR_DOMAIN_XML_SECURE);
    std::cout << domain_xml << std::endl;
    free(domain_xml);
}
void StorageVolLookupByPath(virConnectPtr conn) {
    auto disk_path = "/var/lib/libvirt/images/vmbackup_1671171831_0_ubuntu16.04-true.qcow2";
    virStorageVolPtr vol = virStorageVolLookupByPath(conn, disk_path);
    char *vol_xml = virStorageVolGetXMLDesc(vol, 0);
    std::cout << vol_xml << std::endl;
}

void DomainSnapshotCurrent(virConnectPtr conn) {
    auto domain_name = "ubuntu16.04-test";
    std::shared_ptr<virDomain> domain(virDomainLookupByName(conn, domain_name), [](virDomainPtr vp) {
        if (nullptr != vp) {
            virDomainFree(vp);
        }
    });
    auto snap = virDomainSnapshotCurrent(domain.get(), 0);
    int flag = 0;
    flag |= VIR_DOMAIN_XML_SECURE;
    char *snapshot_xml = virDomainSnapshotGetXMLDesc(snap, flag);
    std::cout << snapshot_xml << std::endl;
}

void DomainBlockCommit(virConnectPtr conn) {
    auto domain_uuid_str = "5e15f0cb-870a-4237-af4c-47890c50bb34";

    std::shared_ptr<virDomain> domain(virDomainLookupByUUIDString(conn, domain_uuid_str), [](virDomainPtr vp) {
        if (nullptr != vp) {
            virDomainFree(vp);
        }
    });
    unsigned int commit_flag = VIR_DOMAIN_BLOCK_COMMIT_SHALLOW;
    commit_flag |= VIR_DOMAIN_BLOCK_COMMIT_ACTIVE;
    auto disk = "/var/lib/libvirt/images/vmbackup_1671171948_0_ubuntu16.04-true.qcow2";
    auto base = "/var/lib/libvirt/images/vmbackup_1671171831_0_ubuntu16.04-true.qcow2";
    auto top = "/var/lib/libvirt/images/vmbackup_1671171948_0_ubuntu16.04-true.qcow2";

    virDomainBlockCommit(domain.get(), disk, base, top, 0, commit_flag);
}

void DomainBlockJobAbort(virConnectPtr conn) {
    auto domain_uuid_str = "5e15f0cb-870a-4237-af4c-47890c50bb34";

    std::shared_ptr<virDomain> domain(virDomainLookupByUUIDString(conn, domain_uuid_str), [](virDomainPtr vp) {
        if (nullptr != vp) {
            virDomainFree(vp);
        }
    });
    auto disk = "/var/lib/libvirt/images/vmbackup_1671171948_0_ubuntu16.04-true.qcow2";
    virDomainBlockJobAbort(domain.get(), disk, VIR_DOMAIN_BLOCK_JOB_ABORT_PIVOT | VIR_DOMAIN_BLOCK_JOB_ABORT_ASYNC);
}

void ConnectGetLibVersion(virConnectPtr conn) {
    int long lib_ver = 0;
    virConnectGetLibVersion(conn, reinterpret_cast<unsigned long *>(&lib_ver));
    std::cout << lib_ver << std::endl;
}

// driver version
void ConnectGetVersion(virConnectPtr conn) {

    int long lib_ver = 0;
    virConnectGetVersion(conn, reinterpret_cast<unsigned long *>(&lib_ver));
    std::cout << lib_ver << std::endl;
}

bool ShutDown(virConnectPtr conn) {
    auto domain_uuid_str = "13cf615f-cefd-4442-9c93-772bc512e8df";
    std::shared_ptr<virDomain> domain(virDomainLookupByUUIDString(conn, domain_uuid_str), [](virDomainPtr vp) {
        if (nullptr != vp) {
            virDomainFree(vp);
        }
    });

    if (domain == nullptr) {
        return false;
    }

    int state = 0, reason = 0;
    if (0 != virDomainGetState(domain.get(), &state, &reason, 0)) {
        return false;
    }

    if (VIR_DOMAIN_SHUTOFF == state) {
        std::cout << "is shutdown" << std::endl;
        return true;
    }


    std::cout << "shutdown" << std::endl;
    std::cout << virDomainShutdown(domain.get()) << std::endl;

    return true;
}

bool ForceOff(virConnectPtr conn) {
    // virDomainDestroy()
    return false;
}


bool PowerOn(virConnectPtr conn) {
    auto domain_uuid_str = "13cf615f-cefd-4442-9c93-772bc512e8df";
    std::shared_ptr<virDomain> domain(virDomainLookupByUUIDString(conn, domain_uuid_str), [](virDomainPtr vp) {
        if (nullptr != vp) {
            virDomainFree(vp);
        }
    });
    virDomainCreate(domain.get());
}


bool Upload() {
    // virStreamNew
    // virStorageVolLookupByPath
    // virStorageVolUpload
    // virStreamSendAll
    // virStreamFinish
    // virStorageVolFree
    // virStoragePoolFree
    return false;
}

bool Download() {
    // virStorageVolLookupByPath
    // virStreamNew
    // virStorageVolDownload
    return false;
}

int main(int argc, char **argv) {
    //    auto conn = ConnectOpenAuth();
    auto conn = ConnectOpen();
    if (conn == nullptr) {
        std::cout << "conn == nullptr" << std::endl;
        return 0;
    }

    //    std::string path = "/var/lib/libvirt/images/vmbackup_1666604000_vmbackup_1666603295_vmbackup_1666601867_vmbackup_1662622359_vmbackup_1639634370_ubuntu-5G.qcow2";
    //    StorageVolLookupByPath(conn, path);
    //    DomainLookupByUUIDString(conn);

    //    DomainListAllSnapshots(conn);
    //    DomainGetXMLDesc(conn);
    //    StorageVolLookupByPath(conn);
    //    ConnectClose(conn);
    //    DomainSnapshotCurrent(conn);
    //    DomainBlockCommit(conn);
    //    DomainBlockJobAbort(conn);
    //    ConnectGetLibVersion(conn);
    //    ConnectGetVersion(conn);
    //    ShutDown(conn);
    PowerOn(conn);
    return 0;
}
