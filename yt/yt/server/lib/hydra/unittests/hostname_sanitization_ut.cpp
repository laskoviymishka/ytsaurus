#include <yt/yt/server/lib/hydra/helpers.h>

#include <yt/yt/core/test_framework/framework.h>

namespace NYT::NHydra {
namespace {

////////////////////////////////////////////////////////////////////////////////

TEST(TLocalHostNameSanitizer, SingleDataCenterSanitizing)
{
    THashSet<TString> clusterPeers = {
        "m001-cluster-vla.vla.yp-c.yandex.net",
        "m002-cluster-vla.vla.yp-c.yandex.net",
        "m010-cluster-vla.vla.yp-c.yandex.net",
        "m042-cluster-vla.vla.yp-c.yandex.net",
        "m038-cluster-vla.vla.yp-c.yandex.net",
        "m723-cluster-vla.vla.yp-c.yandex.net",
        "m991-cluster-vla.vla.yp-c.yandex.net",
    };

    THashSet<TString> peers = {
        "mc091-cluster-vla.vla.yp-c.yandex.net",
        "clock01-cluster.vla.yp-c.yandex.net",
        "sas7-4539-proxy-cluster.sas.yp-c.yandex.net",
        "sas7-4614-spare-01e-rpc-cluster.sas.yp-c.yandex.net",
        "abc4-2666-bigb-179-tab-sen-v.abc.yp-c.yandex.net",
        "sas7-3822-click-002-tab-cluster.sas.yp-c.yandex.net",
    };

    for (const auto& peerAddress : clusterPeers) {
        auto sanitizedHost = SanitizeLocalHostName(clusterPeers, peerAddress);
        EXPECT_TRUE(sanitizedHost);
        EXPECT_EQ("m*-cluster-vla.vla.yp-c.yandex.net", sanitizedHost->ToStringBuf());
    }

    for (const auto& peerAddress : peers) {
        auto sanitizedHost = SanitizeLocalHostName(clusterPeers, peerAddress);
        EXPECT_FALSE(sanitizedHost);
    }
}

TEST(TLocalHostNameSanitizer, CrossDataCenterSanitizing)
{
    THashSet<TString> clusterPeers = {
        "m001-cluster.vla.yp-c.yandex.net",
        "m002-cluster.vlx.yp-c.yandex.net",
        "m010-cluster.man.yp-c.yandex.net",
        "m042-cluster.sas.yp-c.yandex.net",
        "m101-cluster.iva.yp-c.yandex.net",
        "m552-cluster.klg.yp-c.yandex.net",
        "m029-cluster.col.yp-c.yandex.net",
        "m723-cluster.myt.yp-c.yandex.net",
    };

    for (const auto& peerAddress : clusterPeers) {
        auto sanitizedHost = SanitizeLocalHostName(clusterPeers, peerAddress);
        EXPECT_TRUE(sanitizedHost);
        EXPECT_EQ("m*.yp-c.yandex.net", sanitizedHost->ToStringBuf());
    }
}

TEST(TLocalHostNameSanitizer, DifferentHostNameLengths)
{
    THashSet<TString> clusterPeers = {
        "eu-north1-c-4ct13-11b.hw.company.yt",
        "eu-north1-c-4ct14-15b.hw.company.yt",
        "eu-north1-c-4ct4-11a.hw.company.yt",
    };

    for (const auto& peerAddress : clusterPeers) {
        auto sanitizedHost = SanitizeLocalHostName(clusterPeers, peerAddress);
        EXPECT_TRUE(sanitizedHost);
        EXPECT_EQ("eu-north1-c-4ct*.hw.company.yt", sanitizedHost->ToStringBuf());
    }
}

TEST(TLocalHostNameSanitizer, PeerEqualToCommonPrefix)
{
    THashSet<TString> clusterPeers = {
        "abc.cX.cZ",
        "abc.cX",
        "abc.cX.dZ",
    };

    for (const auto& peerAddress : clusterPeers) {
        auto sanitizedHost = SanitizeLocalHostName(clusterPeers, peerAddress);
        EXPECT_TRUE(sanitizedHost);
        EXPECT_EQ("abc.cX*", sanitizedHost->ToStringBuf());
    }
}

TEST(TLocalHostNameSanitizer, PeerEqualToCommonSuffix)
{
    THashSet<TString> clusterPeers = {
        "abc.cX.cZ",
        "cX.cZ",
        "e.cX.cZ",
    };

    for (const auto& peerAddress : clusterPeers) {
        auto sanitizedHost = SanitizeLocalHostName(clusterPeers, peerAddress);
        EXPECT_TRUE(sanitizedHost);
        EXPECT_EQ("*cX.cZ", sanitizedHost->ToStringBuf());
    }
}

TEST(TLocalHostNameSanitizer, NoCommonParts)
{
    THashSet<TString> clusterPeers = {
        "a",
        "b",
        "c",
    };

    for (const auto& peerAddress : clusterPeers) {
        auto sanitizedHost = SanitizeLocalHostName(clusterPeers, peerAddress);
        EXPECT_TRUE(sanitizedHost);
        EXPECT_EQ("*", sanitizedHost->ToStringBuf());
    }
}

TEST(TLocalHostNameSanitizer, EqualPeers)
{
    THashSet<TString> clusterPeers = {
        "abc",
        "abc",
    };

    for (const auto& peerAddress : clusterPeers) {
        auto sanitizedHost = SanitizeLocalHostName(clusterPeers, peerAddress);
        EXPECT_TRUE(sanitizedHost);
        EXPECT_EQ("abc", sanitizedHost->ToStringBuf());
    }
}

TEST(TLocalHostNameSanitizer, PrefixAndSuffixOverlap)
{
    THashSet<TString> clusterPeers = {
        "abc.cX.bc.cZ",
        "abc.cZ",
    };

    for (const auto& peerAddress : clusterPeers) {
        auto sanitizedHost = SanitizeLocalHostName(clusterPeers, peerAddress);
        EXPECT_TRUE(sanitizedHost);
        EXPECT_EQ("abc.c*Z", sanitizedHost->ToStringBuf());
    }
}

////////////////////////////////////////////////////////////////////////////////

} // namespace
} // namespace NYT::NHydra
