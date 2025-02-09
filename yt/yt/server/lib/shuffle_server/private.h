#pragma once

#include <yt/yt/core/logging/log.h>

namespace NYT::NShuffleServer {

YT_DEFINE_GLOBAL(const NLogging::TLogger, ShuffleServiceLogger, "ShuffleService");

} // namespace NYT::NShuffleServer
