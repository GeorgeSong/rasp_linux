// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2020, Maxim Integrated

#include <linux/acpi.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <sound/tlv.h>
#include "max98050.h"
#include "maxim_ultrasound.h"

static struct max98050_priv *g_max98050 = NULL; 


/* =============== max98050_inf.c ================ */
int max_i2c_read(int addr, int *value)
{
	/* Add i2c read function of the target system here
	 * Return 0 if succeeded, return -1 if falied.
	 */
	
	if (g_max98050->regmap != NULL)
		regmap_read(g_max98050->regmap,
			addr, value);
	return 0;
}
int max_i2c_write(int addr, int value)
{
	/* Add i2c write function of the target system here
	 * Return 0 if succeeded, return -1 if falied.
	 */
	if (g_max98050->regmap != NULL)
		regmap_write(g_max98050->regmap,
			addr, value);
#ifdef ULTRASOUND_DEMO		
	usleep_range(5000, 6000);
#endif	
	return 0;
}


#define max_dbg_info(format, ...) \
    pr_info(format " %s %d", __VA_ARGS__, __FILE__, __LINE__)


/* =============== end ======================= */


/* =============== max98050.c ================ */
void max98050_set_volume_control(int volume)
{

}

void max98050_get_volume_control(int volume)
{

}

void max98050_set_dai_cfg(int cfg)
{

}

void max98050_get_dai_cfg(int cfg)
{

}


void max98050_set_mic_path(int mic_id)
{

}

void max98050_get_mic_path(int mic_id)
{

}

void max98050_shutdown(bool onoff)
{
	pr_info("[GEORGE] %s onoff : %d", __func__, onoff);
	max_i2c_write(MAX98050_R20FF_SYS_LVL_EN, onoff ? 1 : 0);
}
EXPORT_SYMBOL(max98050_shutdown);

void max98050_flt_config(bool onoff, int *coeff)
{
	max_i2c_write(MAX98050_R2000_SW_RESET, 0);
}

void max98050_probe(void)
{
	int rev_id;
	int val;

	pr_info("[STEVE] %s in", __func__);
	
	/* SW Reset */
	max_i2c_write(MAX98050_R2000_SW_RESET, 0);

	/* Get Revision ID of max98050 */
	max_i2c_read(MAX98050_R28FD_DEV_ID_MSB, &val);
	rev_id = (val << 8);
	max_i2c_read(MAX98050_R28FE_DEV_ID_LSB, &val);
	rev_id |= val;
	max_dbg_info("MAX980990 Device ID : %08x", rev_id);

	pr_info("[STEVE] %s out", __func__);	
}

/* =============== end ======================= */


static struct reg_default max98050_reg[] = {
    {MAX98050_R2000_SW_RESET, 0x0},
    {MAX98050_R2001_INTR_RAW_1, 0x0},
    {MAX98050_R2002_INTR_RAW_2, 0x0},
    {MAX98050_R2003_INTR_STATE_1, 0x0},
    {MAX98050_R2004_INTR_STATE_2, 0x0},
    {MAX98050_R2005_INTR_FLAG_1, 0x0},
    {MAX98050_R2006_INTR_FLAG_2, 0x0},
    {MAX98050_R2007_INTR_EN_1, 0xC},
    {MAX98050_R2008_INTR_EN_2, 0x0},
    {MAX98050_R2009_INTR_CLR_1, 0x0},
    {MAX98050_R200A_INTR_CLR_2, 0x0},
    {MAX98050_R2012_IRQ_CTRL, 0x8},
    {MAX98050_R2013_IRQ_EN, 0x1},
    {MAX98050_R2014_I2C_SLAVE_CFG, 0x1},
    {MAX98050_R2020_PCM_CFG_1, 0x18},
    {MAX98050_R2021_PCM_CFG_2, 0x14},
    {MAX98050_R2022_PCM_CFG_3, 0x20},
    {MAX98050_R2024_PCM_SR_CFG, 0x0},
    {MAX98050_R2026_PCM_PLAY_CH_SRC, 0x0},
    {MAX98050_R2028_PCM_REC_CH1_SLOT_SEL, 0x0},
    {MAX98050_R2029_PCM_REC_CH2_SLOT_SEL, 0x0},
    {MAX98050_R202A_PCM_REC_CH3_SLOT_SEL, 0x0},
    {MAX98050_R202B_PCM_REC_CH4_SLOT_SEL, 0x0},
    {MAX98050_R2031_PCM_TX_HIZ_CTRL_1, 0x0},
    {MAX98050_R2032_PCM_TX_HIZ_CTRL_2, 0x0},
    {MAX98050_R2033_PCM_TX_HIZ_CTRL_3, 0x0},
    {MAX98050_R2034_PCM_TX_HIZ_CTRL_4, 0x0},
    {MAX98050_R2035_PCM_TX_HIZ_CTRL_5, 0x0},
    {MAX98050_R2036_PCM_TX_HIZ_CTRL_6, 0x0},
    {MAX98050_R2037_PCM_TX_HIZ_CTRL_7, 0x0},
    {MAX98050_R2038_PCM_TX_HIZ_CTRL_8, 0x0},
    {MAX98050_R203E_PCM_LOOP_EN, 0x0},
    {MAX98050_R203F_PCM_EN, 0x0},
    {MAX98050_R2060_PLAY_BLOCK_CFG_1, 0x0},
    {MAX98050_R2061_PLAY_BLOCK_CFG_2, 0x0},
    {MAX98050_R2062_PLAY_BQ_EN, 0x0},
    {MAX98050_R2063_PLAY_GAIN_CORRECT, 0x0},
    {MAX98050_R2064_PLAY_RAMP_RATE_CFG, 0x0},
    {MAX98050_R2065_PLAY_VOL_CTRL, 0x0},
    {MAX98050_R2066_PLAY_NG_THRESH, 0x0},
    {MAX98050_R2067_PLAY_NG_CTRL, 0x0},
    {MAX98050_R2068_PLAY_DAC_CFG, 0x0},
    {MAX98050_R2069_PLAY_AMP_CFG, 0x0},
    {MAX98050_R206A_PLAY_AMP_FALL_SLEW_RATE_CTRL, 0x0},
    {MAX98050_R206B_PLAY_DAC_RISE_SLEW_RATE_CTRL, 0x0},
    {MAX98050_R206C_PLAY_SSM_CTRL, 0x0},
    {MAX98050_R206E_PLAY_DIG_MUTE_CTRL, 0x0},
    {MAX98050_R206F_PLAY_EN, 0x0},
    {MAX98050_R2075_DMIC_CLK_DR_STRENGTH_CFG, 0x0},
    {MAX98050_R2076_DMIC_CLK_CFG, 0x0},
    {MAX98050_R2077_DMIC_EN, 0x0},
    {MAX98050_R2080_AMIC1_CFG_1, 0x0},
    {MAX98050_R2081_AMIC1_CFG_2, 0x0},
    {MAX98050_R2083_AMIC2_CFG_1, 0x0},
    {MAX98050_R2084_AMIC2_CFG_2, 0x0},
    {MAX98050_R2086_AMIC3_CFG_1, 0x0},
    {MAX98050_R2087_AMIC3_CFG_2, 0x0},
    {MAX98050_R208A_AMIC_PWR_DN_CTRL, 0x0},
    {MAX98050_R208B_AMIC_BIAS_CFG, 0x0},
    {MAX98050_R208C_AMIC_BIAS_OUTPUT_EN, 0x0},
    {MAX98050_R208D_AMIC_IN_CFG, 0x0},
    {MAX98050_R208E_AMIC_PWR_MODE_SEL, 0x0},
    {MAX98050_R208F_AMIC_EN, 0x0},
    {MAX98050_R20A0_REC_CH1_CFG_1, 0x0},
    {MAX98050_R20A1_REC_CH1_CFG_2, 0x0},
    {MAX98050_R20A2_REC_CH1_DIG_VOL_CTRL, 0x0},
    {MAX98050_R20A4_REC_CH2_CFG_1, 0x0},
    {MAX98050_R20A5_REC_CH2_CFG_2, 0x0},
    {MAX98050_R20A6_REC_CH2_DIG_VOL, 0x0},
    {MAX98050_R20A8_REC_CH3_CFG_1, 0x0},
    {MAX98050_R20A9_REC_CH3_CFG_2, 0x0},
    {MAX98050_R20AA_REC_CH3_DIG_VOL, 0x0},
    {MAX98050_R20AC_REC_CH4_CFG_1, 0x0},
    {MAX98050_R20AF_REC_CH_EN, 0x0},
    {MAX98050_R20B0_DIG_FLT_CH1_BANK_A, 0x0},
    {MAX98050_R20B1_DIG_FLT_CH1_BANK_A_VOL, 0x0},
    {MAX98050_R20B2_DIG_FLT_CH1_BANK_B, 0x0},
    {MAX98050_R20B3_DIG_FLT_CH1_BANK_B_VOL, 0x0},
    {MAX98050_R20B4_DIG_FLT_CH1_RAMP_CFG, 0x0},
    {MAX98050_R20B5_DIG_FLT_CH1_CFG, 0x0},
    {MAX98050_R20B8_DIG_FLT_CH1_DRC_ATTACK_CFG_1, 0x0},
    {MAX98050_R20B9_DIG_FLT_CH1_DRC_ATTACK_CFG_2, 0x0},
    {MAX98050_R20BA_DIG_FLT_CH1_DRC_REL_CFG_1, 0x0},
    {MAX98050_R20BB_DIG_FLT_CH1_DRC_REL_CFG_2, 0x0},
    {MAX98050_R20BC_DIG_FLT_CH1_DRC_THRESH, 0x0},
    {MAX98050_R20BD_DIG_FLT_CH1_DRC_EN, 0x0},
    {MAX98050_R20C0_DIG_FLT_CH2_BANK_A, 0x0},
    {MAX98050_R20C1_DIG_FLT_CH2_BANK_A_VOL, 0x0},
    {MAX98050_R20C2_DIG_FLT_CH2_BANK_B, 0x0},
    {MAX98050_R20C3_DIG_FLT_CH2_BANK_B_VOL, 0x0},
    {MAX98050_R20C4_DIG_FLT_CH2_RAMP_CFG, 0x0},
    {MAX98050_R20C5_DIG_FLT_CH2_CFG, 0x0},
    {MAX98050_R20C8_DIG_FLT_CH2_DRC_ATTACK_CFG_1, 0x0},
    {MAX98050_R20C9_DIG_FLT_CH2_DRC_ATTACK_CFG_2, 0x0},
    {MAX98050_R20CA_DIG_FLT_CH2_DRC_REL_CFG_1, 0x0},
    {MAX98050_R20CB_DIG_FLT_CH2_DRC_REL_CFG_2, 0x0},
    {MAX98050_R20CC_DIG_FLT_CH2_DRC_THRESH, 0x0},
    {MAX98050_R20CD_DIG_FLT_CH2_DRC_EN, 0x0},
    {MAX98050_R20D0_PLAY_COMP_BANK_A_BQ_EN, 0x0},
    {MAX98050_R20D1_PLAY_COMP_BANK_A_BQ_VOL, 0x0},
    {MAX98050_R20D2_PLAY_COMP_BANK_B_BQ_EN, 0x0},
    {MAX98050_R20D3_PLAY_COMP_BANK_B_BQ_VOL, 0x0},
    {MAX98050_R20D4_PLAY_COMP_RAMP_CFG, 0x0},
    {MAX98050_R20DC_DIG_FLT_CH_CFG, 0x0},
    {MAX98050_R20DD_DIG_FLT_BQ_BANK_SEL, 0x0},
    {MAX98050_R20DE_DIG_FLT_MUTE_CTRL, 0x0},
    {MAX98050_R20DF_DIG_FLT_EN, 0x0},
    {MAX98050_R20F1_CLK_MON_CFG, 0x0},
    {MAX98050_R20F2_CLK_MON_EN, 0x0},
    {MAX98050_R20F4_DATA_MON_CFG, 0x0},
    {MAX98050_R20F5_DATA_MON_EN, 0x0},
    {MAX98050_R20FE_SYS_LVL_CFG, 0x0},
    {MAX98050_R20FF_SYS_LVL_EN, 0x0},
    {MAX98050_R28FD_DEV_ID_MSB, 0x0},
    {MAX98050_R28FE_DEV_ID_LSB, 0x0},
    {MAX98050_R28FF_REV_ID, 0x0},
};

static const struct snd_soc_dapm_widget max98050_dapm_widgets[] = {
	SND_SOC_DAPM_OUTPUT("BE_OUT"),
};

static bool max98050_readable_register(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case MAX98050_R2000_SW_RESET ... MAX98050_R28FF_REV_ID:
		return true;
	default:
		return false;
	}
};

static bool max98050_volatile_reg(struct device *dev, unsigned int reg)
{
	switch (reg) {
	case MAX98050_R2000_SW_RESET ... MAX98050_R28FF_REV_ID:
		return true;
	default:
		return false;
	}
}

static const struct snd_kcontrol_new max98050_snd_controls[] = {

};

static const struct snd_soc_dapm_route max98050_audio_map[] = {
	/* Plabyack */
};

static int max98050_dai_set_fmt(struct snd_soc_dai *codec_dai, unsigned int fmt)
{
	return 0;
}

static int max98050_dai_hw_params(struct snd_pcm_substream *substream,
	struct snd_pcm_hw_params *params,
	struct snd_soc_dai *dai)
{
	return 0;
}

#define MAX98050_RATES SNDRV_PCM_RATE_8000_96000

#define MAX98050_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | \
	SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32_LE)
	
static const struct snd_soc_dai_ops max98050_dai_ops = {
	.set_fmt = max98050_dai_set_fmt,
	.hw_params = max98050_dai_hw_params,	
};

static struct snd_soc_dai_driver max98050_dai[] = {
	{
		.name = "max98050-aif1",
		.playback = {
			.stream_name = "HiFi Playback",
			.channels_min = 1,
			.channels_max = 2,
			.rates = MAX98050_RATES,
			.formats = MAX98050_FORMATS,
		},
		.capture = {
			.stream_name = "HiFi Capture",
			.channels_min = 1,
			.channels_max = 2,
			.rates = MAX98050_RATES,
			.formats = MAX98050_FORMATS,
		},
		.ops = &max98050_dai_ops,
	}
};

static int max98050_component_probe(struct snd_soc_component *component)
{
	struct max98050_priv *max98050 = snd_soc_component_get_drvdata(component);
    max98050->component = component;
    
	pr_info("[STEVE] %s ", __func__);
	
	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int max98050_suspend(struct device *dev)
{
	struct max98050_priv *max98050 = dev_get_drvdata(dev);

	regcache_cache_only(max98050->regmap, true);
	regcache_mark_dirty(max98050->regmap);
	return 0;
}
static int max98050_resume(struct device *dev)
{
	struct max98050_priv *max98050 = dev_get_drvdata(dev);

	regcache_cache_only(max98050->regmap, false);
	regcache_sync(max98050->regmap);
	return 0;
}
#endif

static const struct dev_pm_ops max98050_pm = {
	SET_SYSTEM_SLEEP_PM_OPS(max98050_suspend, max98050_resume)
};

static const struct snd_soc_component_driver soc_codec_dev_max98050 = {
	.probe			= max98050_component_probe,
	.controls		= max98050_snd_controls,
	.num_controls		= ARRAY_SIZE(max98050_snd_controls),
	.dapm_widgets		= max98050_dapm_widgets,
	.num_dapm_widgets	= ARRAY_SIZE(max98050_dapm_widgets),
	.dapm_routes		= max98050_audio_map,
	.num_dapm_routes	= ARRAY_SIZE(max98050_audio_map),
	.idle_bias_on		= 1,
	.use_pmdown_time	= 1,
	.endianness		= 1,
	.non_legacy_dai_naming	= 1,
};

static const struct regmap_config max98050_regmap = {
	.reg_bits         = 16,
	.val_bits         = 8,
	.max_register     = MAX98050_R28FF_REV_ID,
	.reg_defaults     = max98050_reg,
	.readable_reg     = max98050_readable_register,
	.volatile_reg     = max98050_volatile_reg,	
	.num_reg_defaults = ARRAY_SIZE(max98050_reg),
	.cache_type       = REGCACHE_RBTREE,
};

#ifdef ULTRASOUND_DEMO
/* BCLKs per LRCLK */
static const int max98050_bclk_sel_table[] = {
	32, 48, 64, 96, 128, 192, 256, 384, 512,
};

static int MAX98050_get_bclk_sel(int bclk)
{
	int i;
	/* match BCLKs per LRCLK */
	for (i = 0; i < ARRAY_SIZE(max98050_bclk_sel_table); i++) {
		if (max98050_bclk_sel_table[i] == bclk)
			return i + 2;
	}
	return 0;
}

static int MAX98050_set_clock(int chan_sz)
{
	/* BCLK/LRCLK ratio calculation */
	int blr_clk_ratio = 2 * chan_sz;
	int value;

	/* BCLK configuration */
	value = MAX98050_get_bclk_sel(blr_clk_ratio);
	pr_info("[GEORGE] %s value:%d", __func__, value);
	if (!value) {
		pr_info("[GEORGE] format unsupported %d\n", chan_sz);
		return -EINVAL;
	}
	
	max_i2c_write(MAX98050_R2021_PCM_CFG_2, value);
		
	pr_info("[GEORGE] %s out", __func__);
	return 0;
}

static int max98050_hw_param(int prams_rate, int params_format)
{
	unsigned int sampling_rate = 0;
	unsigned int chan_sz = 0;
	
	/* pcm mode configuration */
	switch (params_format) {
	case 16:
		chan_sz = MAX98050_PCM_MODE_CFG_CHANSZ_16;
		break;
	case 24:
		chan_sz = MAX98050_PCM_MODE_CFG_CHANSZ_24;
		break;
	case 32:
		chan_sz = MAX98050_PCM_MODE_CFG_CHANSZ_32;
		break;
	default:
		pr_info("[GEORGE] format unsupported %d\n", params_format);
		goto err;
	}
	
	max_i2c_write(MAX98050_R2020_PCM_CFG_1, 0x18);
	
	/* sampling rate configuration */
	switch (prams_rate) {
	case 8000:
		sampling_rate = MAX98050_PCM_SR_8000;
		break;
	case 11025:
		sampling_rate = MAX98050_PCM_SR_11025;
		break;
	case 12000:
		sampling_rate = MAX98050_PCM_SR_12000;
		break;
	case 16000:
		sampling_rate = MAX98050_PCM_SR_16000;
		break;
	case 22050:
		sampling_rate = MAX98050_PCM_SR_22050;
		break;
	case 24000:
		sampling_rate = MAX98050_PCM_SR_24000;
		break;
	case 32000:
		sampling_rate = MAX98050_PCM_SR_32000;
		break;
	case 44100:
		sampling_rate = MAX98050_PCM_SR_44100;
		break;
	case 48000:
		sampling_rate = MAX98050_PCM_SR_48000;
		break;
	case 88200:
		sampling_rate = MAX98050_PCM_SR_88200;
		break;
	case 96000:
		sampling_rate = MAX98050_PCM_SR_96000;
		break;
	default:
		pr_info("[GEORGE] rate %d not supported\n", prams_rate);
		goto err;
	}	
	
	sampling_rate |= (sampling_rate) << MAX98050_IVADC_SR_SHIFT;
	max_i2c_write(MAX98050_R2024_PCM_SR_CFG, sampling_rate);
	
	MAX98050_set_clock(params_format);
		
	return 0;

err:
	pr_err("[GEORGE] %s out error", __func__);
	return -EINVAL;	
}

int max98050_ultrasound_init_params(int prams_rate, int params_format)
{	
	int ret = 0;

	pr_info("[GEORGE] %s in", __func__);
	
	max_i2c_write(MAX98050_R20FF_SYS_LVL_EN, 0);
	max_i2c_write(MAX98050_R2000_SW_RESET, 1);
	
	max98050_hw_param(prams_rate, params_format);
	
	//max_i2c_write(MAX98050_R2020_PCM_CFG_1, 0x18);
	//max_i2c_write(MAX98050_R2021_PCM_CFG_2, 0x04);
	max_i2c_write(MAX98050_R2022_PCM_CFG_3, 0x20);
	//max_i2c_write(MAX98050_R2024_PCM_SR_CFG, 0xaa);
	max_i2c_write(MAX98050_R2026_PCM_PLAY_CH_SRC, 0x0);
	max_i2c_write(MAX98050_R2028_PCM_REC_CH1_SLOT_SEL, 0x1);
	max_i2c_write(MAX98050_R2029_PCM_REC_CH2_SLOT_SEL, 0x1);
	max_i2c_write(MAX98050_R202A_PCM_REC_CH3_SLOT_SEL, 0x1);
	
	if(prams_rate > 48000)
		max_i2c_write(MAX98050_R203D_HIDDEN, 0x1);
	
	max_i2c_write(MAX98050_R203F_PCM_EN, 0x3);
	max_i2c_write(MAX98050_R2040_MILLI_IF_REC_SYNC, 0x0);
	max_i2c_write(MAX98050_R2041_MILLI_IF_CLK_MON_CFG, 0x0);
	max_i2c_write(MAX98050_R2042_MILLI_IF_CLK_MON_EN, 0x0);
	max_i2c_write(MAX98050_R2043_MILLI_IF_CH_EN, 0x0);
	max_i2c_write(MAX98050_R2044_MILLI_IF_MODE_EN, 0x0);
	max_i2c_write(MAX98050_R2060_PLAY_BLOCK_CFG_1, 0x11);
	max_i2c_write(MAX98050_R2061_PLAY_BLOCK_CFG_2, 0x20);
	max_i2c_write(MAX98050_R2069_PLAY_AMP_CFG, 0x20);
	max_i2c_write(MAX98050_R206A_PLAY_AMP_FALL_SLEW_RATE_CTRL, 0x2);
	max_i2c_write(MAX98050_R206B_PLAY_DAC_RISE_SLEW_RATE_CTRL, 0x2);
	max_i2c_write(MAX98050_R206E_PLAY_DIG_MUTE_CTRL, 0x0);
	max_i2c_write(MAX98050_R2080_AMIC1_CFG_1, 0x13);
	max_i2c_write(MAX98050_R2081_AMIC1_CFG_2, 0x3);
	max_i2c_write(MAX98050_R2082_HIDDEN, 0x5);
	max_i2c_write(MAX98050_R2083_AMIC2_CFG_1, 0x13);
	max_i2c_write(MAX98050_R2084_AMIC2_CFG_2, 0x3);
	max_i2c_write(MAX98050_R2086_AMIC3_CFG_1, 0x13);
	max_i2c_write(MAX98050_R2087_AMIC3_CFG_2, 0x3);
	max_i2c_write(MAX98050_R208A_AMIC_PWR_DN_CTRL, 0x0);
	max_i2c_write(MAX98050_R208B_AMIC_BIAS_CFG, 0x17); /* MIC BIAS */
	max_i2c_write(MAX98050_R208C_AMIC_BIAS_OUTPUT_EN, 0x7);
	max_i2c_write(MAX98050_R208F_AMIC_EN, 0x7);
	//max_i2c_write(MAX98050_R20A1_REC_CH1_CFG_2, 0x60); /* DIGITAL GAIN 12dB*/
	//max_i2c_write(MAX98050_R20A5_REC_CH2_CFG_2, 0x60); /* DIGITAL GAIN 12dB*/
	max_i2c_write(MAX98050_R20A1_REC_CH1_CFG_2, 0xF8); /* DIGITAL GAIN AMIC 1 31dB*/
	max_i2c_write(MAX98050_R20A5_REC_CH2_CFG_2, 0xF8); /* DIGITAL GAIN AMIC 2 31dB*/
	max_i2c_write(MAX98050_R20AF_REC_CH_EN, 0x7);
	max_i2c_write(MAX98050_R20B0_DIG_FLT_CH1_BANK_A, 0x0);
	max_i2c_write(MAX98050_R20B1_DIG_FLT_CH1_BANK_A_VOL, 0x0);
	max_i2c_write(MAX98050_R20B2_DIG_FLT_CH1_BANK_B, 0x0);
	max_i2c_write(MAX98050_R20B3_DIG_FLT_CH1_BANK_B_VOL, 0x0);
	max_i2c_write(MAX98050_R20B4_DIG_FLT_CH1_RAMP_CFG, 0x0);
	max_i2c_write(MAX98050_R20B5_DIG_FLT_CH1_CFG, 0x3);
	max_i2c_write(MAX98050_R20B8_DIG_FLT_CH1_DRC_ATTACK_CFG_1, 0x0);
	max_i2c_write(MAX98050_R20B9_DIG_FLT_CH1_DRC_ATTACK_CFG_2, 0x0);
	max_i2c_write(MAX98050_R20BA_DIG_FLT_CH1_DRC_REL_CFG_1, 0x0);
	max_i2c_write(MAX98050_R20BB_DIG_FLT_CH1_DRC_REL_CFG_2, 0x0);
	max_i2c_write(MAX98050_R20BC_DIG_FLT_CH1_DRC_THRESH, 0x1);
	max_i2c_write(MAX98050_R20BD_DIG_FLT_CH1_DRC_EN, 0x0);
	max_i2c_write(MAX98050_R20C0_DIG_FLT_CH2_BANK_A, 0x0);
	max_i2c_write(MAX98050_R20C1_DIG_FLT_CH2_BANK_A_VOL, 0x0);
	max_i2c_write(MAX98050_R20C2_DIG_FLT_CH2_BANK_B, 0x0);
	max_i2c_write(MAX98050_R20C3_DIG_FLT_CH2_BANK_B_VOL, 0x0);
	max_i2c_write(MAX98050_R20C3_DIG_FLT_CH2_BANK_B_VOL, 0x0);
	max_i2c_write(MAX98050_R20C4_DIG_FLT_CH2_RAMP_CFG, 0xF);
	max_i2c_write(MAX98050_R20C5_DIG_FLT_CH2_CFG, 0x3);
	max_i2c_write(MAX98050_R20C8_DIG_FLT_CH2_DRC_ATTACK_CFG_1, 0x0);
	max_i2c_write(MAX98050_R20C9_DIG_FLT_CH2_DRC_ATTACK_CFG_2, 0x0);
	max_i2c_write(MAX98050_R20CA_DIG_FLT_CH2_DRC_REL_CFG_1, 0x0);
	max_i2c_write(MAX98050_R20CB_DIG_FLT_CH2_DRC_REL_CFG_2, 0x0);
	max_i2c_write(MAX98050_R20CC_DIG_FLT_CH2_DRC_THRESH, 0x1);
	max_i2c_write(MAX98050_R20CD_DIG_FLT_CH2_DRC_EN, 0x0);
	max_i2c_write(MAX98050_R20D0_PLAY_COMP_BANK_A_BQ_EN, 0x0);
	max_i2c_write(MAX98050_R20D1_PLAY_COMP_BANK_A_BQ_VOL, 0x0);
	max_i2c_write(MAX98050_R20D2_PLAY_COMP_BANK_B_BQ_EN, 0x0);
	max_i2c_write(MAX98050_R20D3_PLAY_COMP_BANK_B_BQ_VOL, 0x0);
	max_i2c_write(MAX98050_R20D4_PLAY_COMP_RAMP_CFG, 0xF);
	max_i2c_write(MAX98050_R20DA_HIDDEN, 0x0);
	max_i2c_write(MAX98050_R20DB_HIDDEN, 0x0);
	max_i2c_write(MAX98050_R20DB_HIDDEN, 0x0);
	max_i2c_write(MAX98050_R20DC_DIG_FLT_CH_CFG, 0x0);
	max_i2c_write(MAX98050_R20DD_DIG_FLT_BQ_BANK_SEL, 0x0);
	max_i2c_write(MAX98050_R20DE_DIG_FLT_MUTE_CTRL, 0x3);
	
	if(prams_rate > 48000)
		max_i2c_write(MAX98050_R20DF_DIG_FLT_EN, 0x3);
	
	max_i2c_write(MAX98050_R20F1_CLK_MON_CFG, 0x1);
	max_i2c_write(MAX98050_R20F2_CLK_MON_EN, 0x1);
	max_i2c_write(MAX98050_R20F4_DATA_MON_CFG, 0x0);
	max_i2c_write(MAX98050_R20F5_DATA_MON_EN, 0x0);
	max_i2c_write(MAX98050_R20FE_SYS_LVL_CFG, 0x0);
	max_i2c_write(MAX98050_R20FF_SYS_LVL_EN, 0x1);

	pr_info("[GEORGE] %s out", __func__);
	
	return ret;
}
EXPORT_SYMBOL(max98050_ultrasound_init_params);
#endif

static int max98050_i2c_probe(struct i2c_client *i2c,
	const struct i2c_device_id *id)
{

	int ret = 0;
	int reg = 0;
	struct max98050_priv *max98050 = NULL;

	max98050 = devm_kzalloc(&i2c->dev,
		sizeof(*max98050), GFP_KERNEL);

	pr_info("[STEVE] %s in", __func__);
	
	if (!max98050) {
		ret = -ENOMEM;
		return ret;
	}
	i2c_set_clientdata(i2c, max98050);

	/* regmap initialization */
	max98050->regmap
		= devm_regmap_init_i2c(i2c, &max98050_regmap);
	if (IS_ERR(max98050->regmap)) {
		ret = PTR_ERR(max98050->regmap);
		dev_err(&i2c->dev,
			"Failed to allocate regmap: %d\n", ret);
		return ret;
	}

	/* Check Revision ID */
	ret = regmap_read(max98050->regmap,
		MAX98050_R28FF_REV_ID, &reg);
	if (ret < 0) {
		dev_err(&i2c->dev,
			"Failed to read: 0x%02X\n", MAX98050_R28FF_REV_ID);
		return ret;
	}
	dev_info(&i2c->dev, "MAX98050 revisionID: 0x%02X\n", reg);

	/* codec registeration */
	ret = devm_snd_soc_register_component(&i2c->dev, &soc_codec_dev_max98050,
		max98050_dai, ARRAY_SIZE(max98050_dai));
	if (ret < 0)
		dev_err(&i2c->dev, "Failed to register codec: %d\n", ret);

    g_max98050 = max98050;

	/* set Ultrasound hw parameters */
#ifdef ULTRASOUND_DEMO
	max98050_ultrasound_init_params(96000, 32);
	if (ret < 0) {
		dev_err(&i2c->dev,
			"Failed to wrtie for ultrasound hw param [%d]\n", ret);
		return ret;
	}
	dev_info(&i2c->dev, "MAX98050 set ultrasound hw params successfully [%d]\n", ret);
#endif

	pr_info("[STEVE] %s out", __func__);
	return ret;
}

static const struct i2c_device_id max98050_i2c_id[] = {
	{ "max98050", 0},
	{ },
};

MODULE_DEVICE_TABLE(i2c, max98050_i2c_id);

#if defined(CONFIG_OF)
static const struct of_device_id max98050_of_match[] = {
	{ .compatible = "maxim,max98050", },
	{ }
};
MODULE_DEVICE_TABLE(of, max98050_of_match);
#endif

#ifdef CONFIG_ACPI
static const struct acpi_device_id max98050_acpi_match[] = {
	{ "MX98050", 0 },
	{},
};
MODULE_DEVICE_TABLE(acpi, max98050_acpi_match);
#endif

static struct i2c_driver max98050_i2c_driver = {
	.driver = {
		.name = "max98050",
		.of_match_table = of_match_ptr(max98050_of_match),
		.acpi_match_table = ACPI_PTR(max98050_acpi_match),
		.pm = &max98050_pm,
	},
	.probe  = max98050_i2c_probe,
	.id_table = max98050_i2c_id,
};

module_i2c_driver(max98050_i2c_driver)

MODULE_DESCRIPTION("ALSA SoC MAX98050 driver");
MODULE_AUTHOR("STEVE Lee <STEVEs.lee@maximintegrated.com>");
MODULE_LICENSE("GPL");
