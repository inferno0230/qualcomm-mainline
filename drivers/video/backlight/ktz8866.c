// SPDX-License-Identifier: GPL-2.0-only
/*
 * Backlight driver for the Kinetic KTZ8866
 *
 * Copyright (C) 2022, 2023 Jianhua Lu <lujianhua000@gmail.com>
 */

#include <linux/backlight.h>
#include <linux/err.h>
#include <linux/gpio/consumer.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/regmap.h>

#define DEFAULT_BRIGHTNESS 1500
#define MAX_BRIGHTNESS 2047
#define REG_MAX 0x15

/* reg */
#define DEVICE_ID 0x01
#define BL_CFG1 0x02
#define BL_CFG2 0x03
#define BL_BRT_LSB 0x04
#define BL_BRT_MSB 0x05
#define BL_EN 0x08
#define LCD_BIAS_CFG1 0x09
#define LCD_BIAS_CFG2 0x0A
#define LCD_BIAS_CFG3 0x0B
#define LCD_BOOST_CFG 0x0C
#define OUTP_CFG 0x0D
#define OUTN_CFG 0x0E
#define FLAG 0x0F
#define BL_OPTION1 0x10
#define BL_OPTION2 0x11
#define PWM2DIG_LSBs 0x12
#define PWM2DIG_MSBs 0x13
#define BL_DIMMING 0x14
#define PWM_RAMP_TIME 0x15

/* definition */
#define BL_EN_BIT BIT(6)
#define LCD_BIAS_EN 0x9F
#define PWM_HYST 0x5

#define CURRENT_SINKS_MASK GENMASK(5, 0)
#define LOWER_BYTE GENMASK(2, 0)
#define HIGHER_BYTE GENMASK(10, 3)

struct ktz8866_slave {
	struct i2c_client *client;
	struct regmap *regmap;
};

struct ktz8866 {
	struct i2c_client *client;
	struct regmap *regmap;
	struct gpio_desc *enable_gpio;
	struct gpio_desc *enp_gpio;
	struct gpio_desc *enn_gpio;
	struct ktz8866_slave *slave;
	bool led_on;
};

/* For debug purpose */
static struct ktz8866 *priv;

static const struct regmap_config ktz8866_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = REG_MAX,
};

static inline void ktz8866_read(struct ktz8866 *ktz, unsigned int reg,
				unsigned int *val)
{
	regmap_read(ktz->regmap, reg, val);
}

static void ktz8866_write(struct ktz8866 *ktz, unsigned int reg,
			  unsigned int val)
{
	regmap_write(ktz->regmap, reg, val);

	if (ktz->slave)
		regmap_write(ktz->slave->regmap, reg, val);
}

static void ktz8866_ic_write_byte_dual(unsigned int reg,
				       unsigned int val)
{
	regmap_write(priv->regmap, reg, val);

	if (priv->slave)
		regmap_write(priv->slave->regmap, reg, val);
}

static void ktz8866_ic_write_byte_single(unsigned int reg,
					 unsigned int val)
{
	regmap_write(priv->regmap, reg, val);
}

static void ktz8866_update_bits(struct ktz8866 *ktz, unsigned int reg,
				unsigned int mask, unsigned int val)
{
	regmap_update_bits(ktz->regmap, reg, mask, val);

	if (ktz->slave)
		regmap_update_bits(ktz->slave->regmap, reg, mask, val);
}

static inline void
ktz8866_update_bits_single(struct ktz8866 *ktz,unsigned int reg,
			   unsigned int mask, unsigned int val)
{
	regmap_update_bits(ktz->regmap, reg, mask, val);
}

static void bl_ic_ktz8866_hw_en(bool enable)
{
	/* hw_en */
	gpiod_set_value_cansleep(priv->enable_gpio, enable ? 1 : 0);
	mdelay(1);
}

static void bl_ic_ktz8866_set_lcd_bias_by_gpio(bool enable)
{
	int val = enable ? 1 : 0;

	if (enable) {
		/* Dual LCD Bias Power registers */
		ktz8866_ic_write_byte_dual(0x0C, 0x30);/* LCD_BOOST_CFG */
		ktz8866_ic_write_byte_dual(0x0D, 0x28);/* OUTP_CFG，OUTP = 6V */
		ktz8866_ic_write_byte_dual(0x0E, 0x28);/* OUTN_CFG，OUTN = -6V */
		ktz8866_ic_write_byte_dual(0x09, 0x9f);/* Enable OUT OUTN */
		mdelay(1);
	}

	/* ENP: Enable pin for positive power (OUTP) */
	gpiod_set_value_cansleep(priv->enp_gpio, val);

	mdelay(1);

	/* ENN: Enable pin for negative power (OUTN) */
	gpiod_set_value_cansleep(priv->enn_gpio, val);
}

static void bl_ic_ktz8866_enable(bool enable)
{
	if (priv->led_on == enable)
		return;

	/*
	 * TODO: take hw_en and bias en into account here, so we can fully shut
	 * down the device when backlight off
	 */
	if (enable) {
		/* LED Backlight Driver regisers */
		/* BL_CFG1: OVP=34.0V, linear BLED, PWM Disabled */
		ktz8866_ic_write_byte_dual(0x02, 0xda);
		/* BL_OPTION2: inductance 4.7uH, BL_CURRENT_LIMIT 2.5A */
		ktz8866_ic_write_byte_dual(0x11, 0x37);
		/* enable backlight and 4 / 6 backlihgt regions */
		ktz8866_update_bits(priv, 0x08, BL_EN_BIT | CURRENT_SINKS_MASK,
				    BL_EN_BIT | GENMASK(3, 0));
	} else {
		ktz8866_ic_write_byte_dual(0x08, 0x00);
	}

	priv->led_on = enable;
}

static int ktz8866_backlight_update_status(struct backlight_device *backlight_dev)
{
	struct ktz8866 *ktz = bl_get_data(backlight_dev);
	unsigned int brightness = backlight_get_brightness(backlight_dev);

	if (brightness)
		bl_ic_ktz8866_enable(true);

	ktz8866_write(ktz, BL_BRT_LSB, FIELD_GET(LOWER_BYTE, brightness));
	ktz8866_write(ktz, BL_BRT_MSB, FIELD_GET(HIGHER_BYTE, brightness));

	if (!brightness)
		bl_ic_ktz8866_enable(false);

	return 0;
}

static const struct backlight_ops ktz8866_backlight_ops = {
	.options = BL_CORE_SUSPENDRESUME,
	.update_status = ktz8866_backlight_update_status,
};

static void ktz8866_init(struct ktz8866 *ktz)
{
	bl_ic_ktz8866_hw_en(true);
	bl_ic_ktz8866_set_lcd_bias_by_gpio(true);
}

static int ktz8866_slave_register(struct ktz8866 *ktz)
{
	struct device *dev = &ktz->client->dev;
	struct ktz8866_slave *slave;
	struct i2c_client *client;
	struct device_node *np;

	np = of_parse_phandle(dev->of_node, "secondary", 0);
	if (!np)
		return 0;

	client = of_find_i2c_device_by_node(np);
	of_node_put(np);
	if (!client)
		return 0;

	slave = devm_kzalloc(dev, sizeof(*slave), GFP_KERNEL);
	if (!slave)
		return -ENOMEM;

	slave->client = client;
	slave->regmap = regmap_init_i2c(client, &ktz8866_regmap_config);
	if (IS_ERR(slave->regmap))
		return dev_err_probe(&client->dev, PTR_ERR(slave->regmap),
				     "failed to init regmap\n");

	put_device(&client->dev);

	ktz->slave = slave;

	return 0;
}

static void reg_dump(struct ktz8866 *ktz, char *buf)
{
	unsigned int val;
	int i, cur;

	for (cur = i = 0; i <= REG_MAX; ++i) {
		regmap_read(ktz->regmap, i, &val);
		cur += sprintf(buf + cur, "master[%02x]: %02x", i, val);
		if (ktz->slave) {
			regmap_read(ktz->slave->regmap, i, &val);
			cur += sprintf(buf + cur, ", slave[%02x]: %02x", i, val);
		}
		cur += sprintf(buf + cur, "\n");
	}
}

static char debug_buf[4096];
static ssize_t reg_dump_show(struct device *dev,/* &i2c_client->dev */
			     struct device_attribute *attr,
			     char *buf)
{
	struct backlight_device *bd = dev_get_drvdata(dev);
	struct ktz8866 *ktz = bl_get_data(bd);

	reg_dump(ktz, debug_buf);

	return sysfs_emit(buf, "%s\n", debug_buf);
}

static ssize_t reg_dump_store(struct device *dev,
			      struct device_attribute *attr,
			      const char *buf, size_t size)
{
	int ret, val, reg, dual;

	ret = sscanf(buf, "%x %x %d", &reg, &val, &dual);
	if (ret != 3)
		return -EINVAL;

	pr_err("write %x to %x, dual ? %s", val, reg, dual ? "true" : "false");
	if (dual)
		ktz8866_ic_write_byte_dual(reg, val);
	else
		ktz8866_ic_write_byte_single(reg, val);

	return size;
}

static DEVICE_ATTR_RW(reg_dump);

static struct attribute *ktz8866_reg_attrs[] = {
	&dev_attr_reg_dump.attr,
	NULL,
};
ATTRIBUTE_GROUPS(ktz8866_reg);

static int ktz8866_probe(struct i2c_client *client)
{
	struct backlight_device *backlight_dev;
	struct backlight_properties props = {};
	struct ktz8866 *ktz;
	int ret = 0;

	ktz = devm_kzalloc(&client->dev, sizeof(*ktz), GFP_KERNEL);
	if (!ktz)
		return -ENOMEM;

	ktz->client = client;
	ktz->regmap = devm_regmap_init_i2c(client, &ktz8866_regmap_config);
	if (IS_ERR(ktz->regmap))
		return dev_err_probe(&client->dev, PTR_ERR(ktz->regmap), "failed to init regmap\n");

	ktz->enable_gpio = devm_gpiod_get_optional(&client->dev, "enable", GPIOD_OUT_HIGH);
	if (IS_ERR(ktz->enable_gpio))
		return PTR_ERR(ktz->enable_gpio);

	ktz->enp_gpio = devm_gpiod_get_optional(&client->dev, "enp", GPIOD_OUT_HIGH);
	if (IS_ERR(ktz->enp_gpio))
		return PTR_ERR(ktz->enp_gpio);

	ktz->enn_gpio = devm_gpiod_get_optional(&client->dev, "enn", GPIOD_OUT_HIGH);
	if (IS_ERR(ktz->enn_gpio))
		return PTR_ERR(ktz->enn_gpio);

	props.type = BACKLIGHT_RAW;
	props.max_brightness = MAX_BRIGHTNESS;
	props.brightness = DEFAULT_BRIGHTNESS;
	props.scale = BACKLIGHT_SCALE_LINEAR;

	backlight_dev = devm_backlight_device_register(&client->dev, "ktz8866-backlight",
					&client->dev, ktz, &ktz8866_backlight_ops, &props);
	if (IS_ERR(backlight_dev))
		return dev_err_probe(&client->dev, PTR_ERR(backlight_dev),
				"failed to register backlight device\n");

	ret = ktz8866_slave_register(ktz);
	if (ret)
		return dev_err_probe(&client->dev, ret,
				     "failed to register slave\n");

	i2c_set_clientdata(client, backlight_dev);

	priv = ktz;
	ktz8866_init(ktz);

	dev_warn(&client->dev, "initialized\n");

	return 0;
}

static void ktz8866_remove(struct i2c_client *client)
{
	struct backlight_device *bd = i2c_get_clientdata(client);
	struct ktz8866 *ktz = bl_get_data(bd);

	bd->props.brightness = 0;
	backlight_update_status(bd);

	regmap_exit(ktz->slave->regmap);
}

static const struct i2c_device_id ktz8866_ids[] = {
	{ "ktz8866" },
	{}
};
MODULE_DEVICE_TABLE(i2c, ktz8866_ids);

static const struct of_device_id ktz8866_match_table[] = {
	{
		.compatible = "kinetic,ktz8866",
	},
	{},
};
MODULE_DEVICE_TABLE(of, ktz8866_match_table);

static struct i2c_driver ktz8866_driver = {
	.driver = {
		.name = "ktz8866",
		.of_match_table = ktz8866_match_table,
		.dev_groups = ktz8866_reg_groups,
	},
	.probe = ktz8866_probe,
	.remove = ktz8866_remove,
	.id_table = ktz8866_ids,
};

module_i2c_driver(ktz8866_driver);

MODULE_DESCRIPTION("Kinetic KTZ8866 Backlight Driver");
MODULE_AUTHOR("Jianhua Lu <lujianhua000@gmail.com>");
MODULE_LICENSE("GPL");
