/*
 * Copyright (C) 2015 Kaspar Schleiser <kaspar@schleiser.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 *
 */

/*
 * @ingroup auto_init_gnrc_netif
 * @{
 *
 * @file
 * @brief   Auto initialization for at86rf2xx network interfaces
 *
 * @author  Kaspar Schleiser <kaspar@schleiser.de>
 */

#ifdef MODULE_AT86RF2XX

#include "log.h"
#include "board.h"
#ifdef MODULE_GNRC_NETIF2
#include "net/gnrc/netif2/ieee802154.h"
#else
#include "net/gnrc/netdev.h"
#include "net/gnrc/netdev/ieee802154.h"
#endif
#include "net/gnrc/lwmac/lwmac.h"
#include "net/gnrc/lasmac/lasmac.h"
#include "net/gnrc.h"

#include "at86rf2xx.h"
#include "at86rf2xx_params.h"

/**
 * @brief   Define stack parameters for the MAC layer thread
 * @{
 */
#define AT86RF2XX_MAC_STACKSIZE     (THREAD_STACKSIZE_DEFAULT)
#ifndef AT86RF2XX_MAC_PRIO
#ifdef MODULE_GNRC_NETIF2
#define AT86RF2XX_MAC_PRIO          (GNRC_NETIF2_PRIO)
#else
#define AT86RF2XX_MAC_PRIO          (GNRC_NETDEV_MAC_PRIO)
#endif
#endif

#define AT86RF2XX_NUM (sizeof(at86rf2xx_params) / sizeof(at86rf2xx_params[0]))

static at86rf2xx_t at86rf2xx_devs[AT86RF2XX_NUM];
#ifndef MODULE_GNRC_NETIF2
static gnrc_netdev_t gnrc_adpt[AT86RF2XX_NUM];
#endif
static char _at86rf2xx_stacks[AT86RF2XX_NUM][AT86RF2XX_MAC_STACKSIZE];

void auto_init_at86rf2xx(void)
{
    for (unsigned i = 0; i < AT86RF2XX_NUM; i++) {
        LOG_DEBUG("[auto_init_netif] initializing at86rf2xx #%u\n", i);

        at86rf2xx_setup(&at86rf2xx_devs[i], &at86rf2xx_params[i]);
#ifdef MODULE_GNRC_NETIF2
        gnrc_netif2_ieee802154_create(_at86rf2xx_stacks[i],
                                      AT86RF2XX_MAC_STACKSIZE,
                                      AT86RF2XX_MAC_PRIO, "at86rf2xx",
                                      (netdev_t *)&at86rf2xx_devs[i]);
#else
        int res = gnrc_netdev_ieee802154_init(&gnrc_adpt[i],
                                              (netdev_ieee802154_t *)&at86rf2xx_devs[i]);

        if (res < 0) {
            LOG_ERROR("[auto_init_netif] error initializing at86rf2xx radio #%u\n", i);
        }
        else {
#ifdef MODULE_GNRC_LWMAC
            gnrc_lwmac_init(_at86rf2xx_stacks[i],
                            AT86RF2XX_MAC_STACKSIZE,
                            AT86RF2XX_MAC_PRIO,
                            "at86rf2xx-lwmac",
                            &gnrc_adpt[i]);
#else
#ifdef MODULE_GNRC_LASMAC
            gnrc_lasmac_init(_at86rf2xx_stacks[i],
                             AT86RF2XX_MAC_STACKSIZE,
                             AT86RF2XX_MAC_PRIO,
                             "at86rf2xx-lasmac",
                             &gnrc_adpt[i]);
#else
            gnrc_netdev_init(_at86rf2xx_stacks[i],
                             AT86RF2XX_MAC_STACKSIZE,
                             AT86RF2XX_MAC_PRIO,
                             "at86rf2xx",
                             &gnrc_adpt[i]);
#endif
#endif
        }
#endif
    }
}
#else
typedef int dont_be_pedantic;
#endif /* MODULE_AT86RF2XX */

/** @} */
