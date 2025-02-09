#include "config.h"

namespace NYT::NContainers {

////////////////////////////////////////////////////////////////////////////////

void TMockedDiskConfig::Register(TRegistrar registrar)
{
    registrar.Parameter("disk_id", &TThis::DiskId)
        .Default();
    registrar.Parameter("device_path", &TThis::DevicePath)
        .Default();
    registrar.Parameter("device_name", &TThis::DeviceName)
        .Default();
    registrar.Parameter("disk_model", &TThis::DiskModel)
        .Default();
    registrar.Parameter("partition_fs_labels", &TThis::PartitionFsLabels)
        .Default();
    registrar.Parameter("state", &TThis::State)
        .Default(EDiskState::OK);
}

////////////////////////////////////////////////////////////////////////////////

void TDiskInfoProviderConfig::Register(TRegistrar registrar)
{
    registrar.Parameter("disk_ids", &TThis::DiskIds)
        .Default();
    registrar.Parameter("yt_disk_prefix", &TThis::YtDiskPrefix)
        .Default("/yt");
}

////////////////////////////////////////////////////////////////////////////////

void TDiskManagerProxyConfig::Register(TRegistrar registrar)
{
    registrar.Parameter("disk_manager_address", &TThis::DiskManagerAddress)
        .Default("unix:/run/yandex-diskmanager/yandex-diskmanager.sock");
    registrar.Parameter("disk_manager_service_name", &TThis::DiskManagerServiceName)
        .Default("diskman.DiskManager");
    registrar.Parameter("request_timeout", &TThis::RequestTimeout)
        .Default(TDuration::Seconds(10));
}

////////////////////////////////////////////////////////////////////////////////

void TDiskManagerProxyDynamicConfig::Register(TRegistrar registrar)
{
    registrar.Parameter("request_timeout", &TThis::RequestTimeout)
        .Default();
}

////////////////////////////////////////////////////////////////////////////////

} // namespace NYT::NContainers
