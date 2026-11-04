#include "bj_cache_stats.h"

#include <stdint.h>

#if !defined(CONFIG_BLUEJOULE_CACHE_STATS)

void bj_cache_stats_start(void) {}
void bj_cache_stats_stop(void) {}
void bj_cache_stats_print(void) {}

#else

#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/sys_io.h>

#if (defined(CONFIG_SOC_NRF54L15_CPUAPP) || \
     defined(CONFIG_SOC_NRF54L10_CPUAPP) || \
     defined(CONFIG_SOC_NRF54L05_CPUAPP) || \
     defined(CONFIG_SOC_SERIES_NRF54LX))

#define NRF54_ICACHE_BASE              0xE0082000u

#define NRF54_ICACHE_PROF_ENABLE       0x414u
#define NRF54_ICACHE_PROF_CLEAR        0x418u
#define NRF54_ICACHE_PROF_HIT          0x41Cu
#define NRF54_ICACHE_PROF_MISS         0x420u
#define NRF54_ICACHE_PROF_LMISS        0x424u
#define NRF54_ICACHE_PROF_READS        0x428u
#define NRF54_ICACHE_PROF_WRITES       0x42Cu

#define NRF54_ICACHE_REG(offset)       (NRF54_ICACHE_BASE + (offset))

static uint32_t bj_cache_read(uint32_t offset)
{
    return sys_read32(NRF54_ICACHE_REG(offset));
}

static void bj_cache_write(uint32_t offset, uint32_t value)
{
    sys_write32(value, NRF54_ICACHE_REG(offset));
}

void bj_cache_stats_start(void)
{
    bj_cache_write(NRF54_ICACHE_PROF_ENABLE, 0);
    bj_cache_write(NRF54_ICACHE_PROF_CLEAR, 1);
    bj_cache_write(NRF54_ICACHE_PROF_CLEAR, 0);
    bj_cache_write(NRF54_ICACHE_PROF_ENABLE, 1);
}

void bj_cache_stats_stop(void)
{
    bj_cache_write(NRF54_ICACHE_PROF_ENABLE, 0);
}

void bj_cache_stats_print(void)
{
    uint32_t reads = bj_cache_read(NRF54_ICACHE_PROF_READS);
    uint32_t writes = bj_cache_read(NRF54_ICACHE_PROF_WRITES);
    uint32_t hit = bj_cache_read(NRF54_ICACHE_PROF_HIT);
    uint32_t miss = bj_cache_read(NRF54_ICACHE_PROF_MISS);
    uint32_t lmiss = bj_cache_read(NRF54_ICACHE_PROF_LMISS);
    uint32_t total = hit + miss;
    uint32_t miss_per_mille = total ? ((miss * 1000u) / total) : 0;

    printk("*** cache stats: reads = %u, writes = %u, hit = %u, miss = %u, lmiss = %u, miss = %u.%u%%\n",
           reads, writes, hit, miss, lmiss,
           miss_per_mille / 10u, miss_per_mille % 10u);
}

#elif (defined(CONFIG_SOC_NRF52832) || \
       defined(CONFIG_SOC_NRF52833) || \
       defined(CONFIG_SOC_SERIES_NRF52X))

#define NRF52_NVMC_BASE                0x4001E000u

#define NRF52_NVMC_ICACHECNF           0x540u
#define NRF52_NVMC_IHIT                0x548u
#define NRF52_NVMC_IMISS               0x54Cu

#define NRF52_NVMC_ICACHECNF_CACHEPROFEN_Msk 0x00000100u

#define NRF52_NVMC_REG(offset)         (NRF52_NVMC_BASE + (offset))

static uint32_t bj_cache_read(uint32_t offset)
{
    return sys_read32(NRF52_NVMC_REG(offset));
}

static void bj_cache_write(uint32_t offset, uint32_t value)
{
    sys_write32(value, NRF52_NVMC_REG(offset));
}

void bj_cache_stats_start(void)
{
    uint32_t icachecnf = bj_cache_read(NRF52_NVMC_ICACHECNF);

    bj_cache_write(NRF52_NVMC_ICACHECNF, icachecnf | NRF52_NVMC_ICACHECNF_CACHEPROFEN_Msk);
    bj_cache_write(NRF52_NVMC_IHIT, 0);
    bj_cache_write(NRF52_NVMC_IMISS, 0);
}

void bj_cache_stats_stop(void)
{
    uint32_t icachecnf = bj_cache_read(NRF52_NVMC_ICACHECNF);

    bj_cache_write(NRF52_NVMC_ICACHECNF, icachecnf & ~NRF52_NVMC_ICACHECNF_CACHEPROFEN_Msk);
}

void bj_cache_stats_print(void)
{
    uint32_t hit = bj_cache_read(NRF52_NVMC_IHIT);
    uint32_t miss = bj_cache_read(NRF52_NVMC_IMISS);
    uint32_t reads = hit + miss;
    uint32_t miss_per_mille = reads ? ((miss * 1000u) / reads) : 0;

    printk("*** cache stats: reads = %u, hit = %u, miss = %u, miss = %u.%u%%\n",
           reads, hit, miss,
           miss_per_mille / 10u, miss_per_mille % 10u);
}

#else

void bj_cache_stats_start(void)
{
}

void bj_cache_stats_stop(void)
{
}

void bj_cache_stats_print(void)
{
    printk("*** no cache stats\n");
}

#endif

#endif

