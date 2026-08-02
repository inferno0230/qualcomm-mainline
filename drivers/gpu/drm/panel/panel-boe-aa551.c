// SPDX-License-Identifier: GPL-2.0-only
/*
 * Generated with linux-mdss-dsi-panel-driver-generator from vendor device tree.
 * Copyright (c) 2026 inferno0230 <mail@inferno0230.in>
 */

#include <linux/backlight.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/regulator/consumer.h>

#include <drm/display/drm_dsc.h>
#include <drm/display/drm_dsc_helper.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_modes.h>
#include <drm/drm_panel.h>
#include <drm/drm_probe_helper.h>

struct boe_aa551 {
	struct drm_panel panel;
	struct mipi_dsi_device *dsi;
	struct drm_dsc_config dsc;
	struct regulator_bulk_data *supplies;
	struct gpio_desc *enable_gpio;
	struct gpio_desc *reset_gpio;
};

static const struct regulator_bulk_data boe_aa551_supplies[] = {
	{ .supply = "vddio" },
	{ .supply = "vci" },
};

static inline
struct boe_aa551 *to_boe_aa551(struct drm_panel *panel)
{
	return container_of_const(panel, struct boe_aa551, panel);
}

static void boe_aa551_reset(struct boe_aa551 *ctx)
{
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	usleep_range(2000, 3000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	usleep_range(5000, 6000);
	gpiod_set_value_cansleep(ctx->reset_gpio, 0);
	msleep(25);
}

static int boe_aa551_on(struct boe_aa551 *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	ctx->dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x35, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x53, 0x20);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x06);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc6, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x08);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xed,
				     0xff, 0xff, 0xff, 0xf7, 0xff, 0xff, 0xbf,
				     0xff);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xee,
				     0xfe, 0xef, 0xc1, 0xe0, 0x00, 0xc0, 0x01,
				     0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd2, 0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd3, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x02);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf8, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x20);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb3, 0x50);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb5, 0x03);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x08);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc8, 0x62);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x31);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xa0, 0xf3);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x07);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8a, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8b, 0x11, 0xe0);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x81,
				     0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0x00,
				     0x00, 0xab, 0x30, 0x80, 0x0a, 0xdc, 0x04,
				     0xf0, 0x00, 0x14, 0x02, 0x78, 0x02, 0x78,
				     0x02, 0x00, 0x02, 0x57, 0x00, 0x20, 0x01,
				     0xf8, 0x00, 0x08, 0x00, 0x0d, 0x05, 0x7a,
				     0x04, 0x4f, 0x18, 0x00, 0x10, 0xe0, 0x07,
				     0x10, 0x20, 0x00, 0x06, 0x0f, 0x0f, 0x33,
				     0x0e, 0x1c, 0x2a, 0x38, 0x46, 0x54, 0x62,
				     0x69, 0x70, 0x77, 0x79, 0x7b, 0x7d, 0x7e,
				     0x02, 0x02, 0x22, 0x00, 0x2a, 0x40, 0x2a,
				     0xbe, 0x3a, 0xfc, 0x3a, 0xfa, 0x3a, 0xf8,
				     0x3b, 0x38, 0x3b, 0x78, 0x3b, 0xb6, 0x4b,
				     0xb6, 0x4b, 0xf4, 0x4b, 0xf4, 0x6c, 0x34,
				     0x84, 0x74, 0x74, 0x00, 0x00, 0x00, 0x00,
				     0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x20);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf2, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf5, 0x33);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf6, 0xb7);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf7, 0x98);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xf2, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x22);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd0,
				     0x00, 0xd6, 0x11, 0x10, 0x32, 0x14, 0x14);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd3,
				     0x00, 0xd6, 0x11, 0x10, 0x32, 0x14, 0x14);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd6,
				     0x00, 0xd6, 0x11, 0x10, 0x32, 0x14, 0x14);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd9,
				     0x00, 0xd6, 0x11, 0x10, 0x32, 0x14, 0x14);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xdc,
				     0x84, 0x94, 0xbb, 0x00, 0x20, 0xc4, 0xa1,
				     0xcc, 0x10);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xdd, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xde, 0xf1);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xdf, 0x80);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xe0,
				     0x02, 0x00, 0x5f, 0x21, 0x00, 0x3c, 0x28,
				     0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x1f);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x83, 0xdb, 0x7f);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x84, 0x2d, 0x7f);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x85, 0x5f, 0x79, 0x07);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x08);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd2, 0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd3, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x24);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x92, 0x8e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x23);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xcb, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x64);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x80, 0x00, 0x00, 0x4e, 0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x81, 0x00, 0x00, 0x4e, 0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x82, 0x00, 0x00, 0x4e, 0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x83, 0x00, 0x00, 0x4e, 0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x84, 0x00, 0x00, 0x4e, 0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x85, 0x00, 0x00, 0x4e, 0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x86, 0x00, 0x00, 0xcb, 0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x87, 0x00, 0x02, 0xd7, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x88, 0x00, 0x03, 0x31, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x89, 0x00, 0x03, 0xb8, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8a, 0x00, 0x04, 0x03, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8b, 0x00, 0x04, 0x36, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8c, 0x00, 0x04, 0xb0, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8d, 0x00, 0x04, 0xb0, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8e, 0x00, 0x04, 0xb0, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8f, 0x00, 0x04, 0xb0, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x90, 0x00, 0x04, 0xb0, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x91, 0x00, 0x04, 0xb0, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x92, 0x00, 0x04, 0xb0, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x93, 0x00, 0x04, 0xb0, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x94, 0x00, 0x04, 0xb0, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x95, 0x00, 0x04, 0xb0, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x96, 0x00, 0x04, 0xb0, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x97, 0x00, 0x04, 0xb0, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x98, 0x00, 0x04, 0xb0, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x23);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xcb, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x08);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc8, 0x62);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x82,
				     0x00, 0x00, 0x40, 0x0a, 0x00, 0x11, 0x08,
				     0x04, 0x00, 0x0f, 0x00, 0x00, 0x3e, 0xfc);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x83,
				     0x00, 0x00, 0xc0, 0x00, 0x10, 0x18, 0x08,
				     0x18, 0x00, 0x00, 0x00, 0x00, 0x3e, 0xfc);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x94, 0x09);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x98, 0x08);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x09);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8e, 0x08, 0x10);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb4, 0x04);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xee, 0x40);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x0a);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x82, 0x08, 0x08, 0x00, 0x40);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x83, 0x08, 0x18, 0x00, 0xc0);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x90, 0x08, 0x08, 0x00, 0x40);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x91, 0x08, 0x18, 0x00, 0xc0);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x9e, 0x20, 0x20, 0x00, 0xc0);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x9f, 0x20, 0x20, 0x00, 0xc0);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x0b);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x80,
				     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				     0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x82,
				     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				     0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x84,
				     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				     0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x86, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x87, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x88, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x89,
				     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				     0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8a,
				     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				     0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8b,
				     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				     0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x81,
				     0x08, 0x08, 0x08, 0x08, 0x18, 0x18, 0x18,
				     0x18);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x83,
				     0x08, 0x08, 0x08, 0x08, 0x18, 0x18, 0x18,
				     0x18);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x85,
				     0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
				     0x20);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8c, 0x10);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8d, 0x03);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8e, 0xbf);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x8f,
				     0x33, 0xbf, 0xbf, 0x33, 0xbf, 0xbf, 0x03,
				     0xbf);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x90,
				     0x00, 0x05, 0x05, 0x00, 0x05, 0x05, 0x00,
				     0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x91,
				     0x08, 0x08, 0x08, 0x08, 0x18, 0x18, 0x18,
				     0x18);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x93,
				     0x08, 0x00, 0x00, 0x00, 0x10);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x0d);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xcf, 0x84);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd0, 0x40);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd1, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd2, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd3, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd4, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd5, 0x17);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd6, 0x17);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd7, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd8, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x02);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xa6, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xa7, 0x40);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xa2, 0x20);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xa0,
				     0x07, 0x0b, 0x63, 0x00, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb0,
				     0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb1,
				     0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
				     0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb6, 0x80);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x20);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xbc, 0x32);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xbd, 0x34);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xbe, 0x56);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x4f);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x81, 0x02);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x80, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x2d);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x81, 0x02);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xce, 0x52);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x1f);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xaa, 0x04);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x4e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd4,
				     0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00,
				     0x80);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd1,
				     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x2d);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x80, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd0, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x4e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb1, 0x04);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x63);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xa0, 0x81);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x49);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x98, 0x08, 0x00, 0x8c, 0x02);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xa0,
				     0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
				     0x04, 0x04, 0x00, 0x04, 0x05, 0x04, 0x04,
				     0x03, 0x04);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xa4,
				     0x04, 0x01, 0x01, 0x00, 0x01, 0x00, 0x04,
				     0x04, 0x00, 0x04, 0x04, 0x04, 0x05, 0x04,
				     0x04, 0x04, 0x04);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xa5,
				     0x04, 0x04, 0x04, 0x04, 0xfa, 0x0f, 0x10,
				     0x04, 0x04, 0x00, 0x04, 0x09, 0x05, 0x04,
				     0x03, 0x04);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xa9,
				     0x00, 0x01, 0x01, 0x00, 0x01, 0x00, 0x04,
				     0x04, 0x00, 0x03, 0x04, 0x04, 0x05, 0x04,
				     0x04, 0x04, 0x04);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xaa,
				     0x04, 0x04, 0x04, 0x04, 0x08, 0x08, 0x04,
				     0x04, 0x04, 0x00, 0x04, 0x05, 0x04, 0x04,
				     0x04, 0x04);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xae,
				     0x02, 0x01, 0x01, 0x00, 0x01, 0x00, 0x04,
				     0x04, 0x00, 0x03, 0x04, 0x04, 0x05, 0x04,
				     0x04, 0x04, 0x04);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb0,
				     0x01, 0x01, 0x01, 0x01, 0x0b, 0x01, 0x16,
				     0x00, 0x05, 0x03, 0x35, 0x03, 0x15, 0x0f,
				     0x05, 0x20, 0x10, 0x23);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb1,
				     0x00, 0x00, 0x00, 0x01, 0x05, 0xfe, 0xfe,
				     0x05, 0x01, 0x01, 0x69, 0xfe, 0x37, 0x30,
				     0x24, 0x16, 0x08, 0x03);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb4,
				     0x03, 0x06, 0x06, 0x06, 0x06, 0x06, 0x14,
				     0x06, 0x05, 0x01, 0x24, 0x02, 0x13, 0x0e,
				     0x06, 0x20, 0x0f, 0x22);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb5,
				     0x1d, 0x13, 0x04, 0xff, 0x10, 0x0f, 0x0f,
				     0x0c, 0x02, 0x03, 0x74, 0x3c, 0x38, 0x2f,
				     0x26, 0x15, 0x0a, 0x04);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb8,
				     0x04, 0x04, 0x02, 0x08, 0x02, 0x01, 0x18,
				     0x07, 0x05, 0x03, 0x34, 0x16, 0x13, 0x0f,
				     0x08, 0x55, 0x28, 0x24);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xb9,
				     0x1d, 0x13, 0x07, 0x04, 0x30, 0x2b, 0x24,
				     0x05, 0x02, 0x01, 0x70, 0x3b, 0x33, 0x2b,
				     0x1f, 0x10, 0x05, 0x01);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x49);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xdc, 0x03);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x05);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x95, 0x66);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x1e);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc0, 0x94);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc1, 0x20);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc4, 0x04, 0x40);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc9, 0x40);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc2,
				     0xff, 0xff, 0xed, 0x61, 0x00, 0x62);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc5,
				     0x44, 0x00, 0x00, 0x44, 0x00, 0x00, 0x44,
				     0x00, 0x00, 0x43, 0x07, 0x73, 0x21, 0x75,
				     0xa7, 0x00, 0xc3, 0x00, 0x44, 0x00, 0x00,
				     0x44, 0x00, 0x00, 0x44, 0x00, 0x00, 0x44,
				     0x00, 0x00, 0x44, 0x00, 0x00, 0x44, 0x00,
				     0x00, 0x04, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x13);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xd1, 0x88);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x17);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xa0, 0xcc);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xae, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x24);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x80, 0x04);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x81, 0x40);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x10);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x83, 0x2b);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x1f);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x80, 0x08);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x96, 0x07);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x97, 0xe9);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x98, 0x02);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x99, 0x78);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x08);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xc8, 0x62);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x08, 0x38, 0x00);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0xff, 0x03, 0x08, 0x38, 0x02);
	mipi_dsi_dcs_write_seq_multi(&dsi_ctx, 0x88, 0x02, 0xc0, 0x00);
	mipi_dsi_dcs_exit_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 120);
	mipi_dsi_dcs_set_display_on_multi(&dsi_ctx);

	return dsi_ctx.accum_err;
}

static int boe_aa551_off(struct boe_aa551 *ctx)
{
	struct mipi_dsi_multi_context dsi_ctx = { .dsi = ctx->dsi };

	ctx->dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	mipi_dsi_dcs_set_display_off_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 20);
	mipi_dsi_dcs_enter_sleep_mode_multi(&dsi_ctx);
	mipi_dsi_msleep(&dsi_ctx, 120);

	return dsi_ctx.accum_err;
}

static int boe_aa551_prepare(struct drm_panel *panel)
{
	struct boe_aa551 *ctx = to_boe_aa551(panel);
	struct device *dev = &ctx->dsi->dev;
	struct drm_dsc_picture_parameter_set pps;
	int ret;

	ret = regulator_bulk_enable(ARRAY_SIZE(boe_aa551_supplies), ctx->supplies);
	if (ret < 0) {
		dev_err(dev, "Failed to enable regulators: %d\n", ret);
		return ret;
	}

	gpiod_set_value_cansleep(ctx->enable_gpio, 1);
	usleep_range(10000, 11000);

	boe_aa551_reset(ctx);

	ret = boe_aa551_on(ctx);
	if (ret < 0) {
		dev_err(dev, "Failed to initialize panel: %d\n", ret);
		gpiod_set_value_cansleep(ctx->reset_gpio, 1);
		gpiod_set_value_cansleep(ctx->enable_gpio, 0);
		regulator_bulk_disable(ARRAY_SIZE(boe_aa551_supplies), ctx->supplies);
		return ret;
	}

	drm_dsc_pps_payload_pack(&pps, &ctx->dsc);

	ret = mipi_dsi_picture_parameter_set(ctx->dsi, &pps);
	if (ret < 0) {
		dev_err(panel->dev, "failed to transmit PPS: %d\n", ret);
		return ret;
	}

	ret = mipi_dsi_compression_mode(ctx->dsi, true);
	if (ret < 0) {
		dev_err(dev, "failed to enable compression mode: %d\n", ret);
		return ret;
	}

	msleep(28); /* TODO: Is this panel-dependent? */

	return 0;
}

static int boe_aa551_unprepare(struct drm_panel *panel)
{
	struct boe_aa551 *ctx = to_boe_aa551(panel);
	struct device *dev = &ctx->dsi->dev;
	int ret;

	ret = boe_aa551_off(ctx);
	if (ret < 0)
		dev_err(dev, "Failed to un-initialize panel: %d\n", ret);

	gpiod_set_value_cansleep(ctx->reset_gpio, 1);
	gpiod_set_value_cansleep(ctx->enable_gpio, 0);
	regulator_bulk_disable(ARRAY_SIZE(boe_aa551_supplies), ctx->supplies);

	return 0;
}

static const struct drm_display_mode boe_aa551_mode = {
	.clock = (1264 + 26 + 2 + 26) * (2780 + 22 + 2 + 42) * 120 / 1000,
	.hdisplay = 1264,
	.hsync_start = 1264 + 26,
	.hsync_end = 1264 + 26 + 2,
	.htotal = 1264 + 26 + 2 + 26,
	.vdisplay = 2780,
	.vsync_start = 2780 + 22,
	.vsync_end = 2780 + 22 + 2,
	.vtotal = 2780 + 22 + 2 + 42,
	.width_mm = 71,
	.height_mm = 157,
	.type = DRM_MODE_TYPE_DRIVER,
};

static int boe_aa551_get_modes(struct drm_panel *panel,
			       struct drm_connector *connector)
{
	return drm_connector_helper_get_modes_fixed(connector, &boe_aa551_mode);
}

static const struct drm_panel_funcs boe_aa551_panel_funcs = {
	.prepare = boe_aa551_prepare,
	.unprepare = boe_aa551_unprepare,
	.get_modes = boe_aa551_get_modes,
};

static int boe_aa551_bl_update_status(struct backlight_device *bl)
{
	struct mipi_dsi_device *dsi = bl_get_data(bl);
	u16 brightness = backlight_get_brightness(bl);
	int ret;

	dsi->mode_flags &= ~MIPI_DSI_MODE_LPM;

	ret = mipi_dsi_dcs_set_display_brightness_large(dsi, brightness);
	if (ret < 0)
		return ret;

	dsi->mode_flags |= MIPI_DSI_MODE_LPM;

	return 0;
}

static const struct backlight_ops boe_aa551_bl_ops = {
	.update_status = boe_aa551_bl_update_status,
};

static struct backlight_device *
boe_aa551_create_backlight(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	const struct backlight_properties props = {
		.type = BACKLIGHT_RAW,
		.brightness = 4094,
		.max_brightness = 4094,
	};

	return devm_backlight_device_register(dev, dev_name(dev), dev, dsi,
					      &boe_aa551_bl_ops, &props);
}

static int boe_aa551_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct boe_aa551 *ctx;
	int ret;

	ctx = devm_drm_panel_alloc(dev, struct boe_aa551, panel,
				   &boe_aa551_panel_funcs,
				   DRM_MODE_CONNECTOR_DSI);
	if (IS_ERR(ctx))
		return PTR_ERR(ctx);

	ret = devm_regulator_bulk_get_const(dev,
					    ARRAY_SIZE(boe_aa551_supplies),
					    boe_aa551_supplies,
					    &ctx->supplies);
	if (ret < 0)
		return ret;

	ctx->enable_gpio = devm_gpiod_get(dev, "enable", GPIOD_OUT_LOW);
	if (IS_ERR(ctx->enable_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->enable_gpio),
				     "Failed to get enable-gpios\n");

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio))
		return dev_err_probe(dev, PTR_ERR(ctx->reset_gpio),
				     "Failed to get reset-gpios\n");

	ctx->dsi = dsi;
	mipi_dsi_set_drvdata(dsi, ctx);

	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_NO_EOT_PACKET |
			  MIPI_DSI_CLOCK_NON_CONTINUOUS;

	ctx->panel.prepare_prev_first = true;

	ctx->panel.backlight = boe_aa551_create_backlight(dsi);
	if (IS_ERR(ctx->panel.backlight))
		return dev_err_probe(dev, PTR_ERR(ctx->panel.backlight),
				     "Failed to create backlight\n");

	drm_panel_add(&ctx->panel);

	/* This panel only supports DSC; unconditionally enable it */
	dsi->dsc = &ctx->dsc;

	ctx->dsc.dsc_version_major = 1;
	ctx->dsc.dsc_version_minor = 2;

	ctx->dsc.slice_height = 20;
	ctx->dsc.slice_width = 632;
	ctx->dsc.slice_count = 1264 / ctx->dsc.slice_width;
	ctx->dsc.bits_per_component = 10;
	ctx->dsc.bits_per_pixel = 8 << 4; /* 4 fractional bits */
	ctx->dsc.block_pred_enable = true;

	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		drm_panel_remove(&ctx->panel);
		return dev_err_probe(dev, ret, "Failed to attach to DSI host\n");
	}

	return 0;
}

static void boe_aa551_remove(struct mipi_dsi_device *dsi)
{
	struct boe_aa551 *ctx = mipi_dsi_get_drvdata(dsi);
	int ret;

	ret = mipi_dsi_detach(dsi);
	if (ret < 0)
		dev_err(&dsi->dev, "Failed to detach from DSI host: %d\n", ret);

	drm_panel_remove(&ctx->panel);
}

static const struct of_device_id boe_aa551_of_match[] = {
	{ .compatible = "boe,aa551-p3-a0004" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, boe_aa551_of_match);

static struct mipi_dsi_driver boe_aa551_driver = {
	.probe = boe_aa551_probe,
	.remove = boe_aa551_remove,
	.driver = {
		.name = "panel-boe-aa551",
		.of_match_table = boe_aa551_of_match,
	},
};
module_mipi_dsi_driver(boe_aa551_driver);

MODULE_AUTHOR("inferno0230 <mail@inferno0230.in>");
MODULE_DESCRIPTION("BOE AA551 P3 A0004 MIPI-DSI OLED panel");
MODULE_LICENSE("GPL");
