/*
 * Copyright (c) 2018 Prevas A/S
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <shell/shell.h>
#include <stdlib.h>
#include <adc.h>
#include <ctype.h>
#include <misc/util.h>

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <logging/log.h>
LOG_MODULE_REGISTER(adc_shell);

struct adc_hdl {
	char *device_name;
	struct adc_channel_cfg channel_config;
	u8_t resolution;
};

struct adc_hdl adc_list[] = {
#ifdef CONFIG_ADC_0
	{
		.device_name = DT_ADC_0_NAME,
		.channel_config = {
			.gain = ADC_GAIN_1,
			.reference = ADC_REF_INTERNAL,
			.acquisition_time = ADC_ACQ_TIME_DEFAULT,
			.channel_id = 0,
		},
		.resolution = 0,
	},
#endif
#ifdef CONFIG_ADC_1
	{
		.device_name = DT_ADC_1_NAME,
		.channel_config = {
			.gain = ADC_GAIN_1,
			.reference = ADC_REF_INTERNAL,
			.acquisition_time = ADC_ACQ_TIME_DEFAULT,
			.channel_id = 0,
		},
		.resolution = 0,
	},
#endif
};

struct args_index {
	u8_t adc;
	u8_t channel;
	u8_t conf;
	u8_t acq_unit;
};

struct args_number {
	u8_t help;
	u8_t channel;
	u8_t gain;
	u8_t ref;
	u8_t acq_time;
	u8_t resolution;
	u8_t read;
};

static const struct args_index args_indx = {
	.adc = 1,
	.channel = 2,
	.conf = 2,
	.acq_unit = 3,
};

static const struct args_number args_no = {
	.help = 1,
	.channel = 3,
	.gain = 3,
	.ref = 3,
	.acq_time = 4,
	.resolution = 3,
	.read = 3,
};

/** get_adc_from_list returns the number entry of the adc in the adc_list,
 * returns -ENODEV if it doesn't exist
 */
static int get_adc_from_list(char *name)
{
	int adc_idx;

	for (adc_idx = 0; adc_idx < ARRAY_SIZE(adc_list); adc_idx++) {
		if (!strcmp(name, adc_list[adc_idx].device_name)) {
			return adc_idx;
		}
	}
	return -ENODEV;
}

static int cmd_adc_channel(const struct shell *shell, size_t argc, char **argv)
{
	int retval = 0;
	struct device *adc_dev;
	int chosen_adc;

	if (argc != args_no.channel) {
		shell_fprintf(shell, SHELL_NORMAL,
				"Usage: channel <adc> <channel_id>\n");
		return 0;
	}

	chosen_adc = get_adc_from_list(argv[args_indx.adc]);
	if (chosen_adc < 0) {
		return -EINVAL;
	}

	adc_dev = device_get_binding(adc_list[chosen_adc].device_name);
	if (adc_dev == NULL) {
		shell_error(shell, "ADC device not found");
		return -ENODEV;
	}
	if (!isdigit(argv[args_indx.conf][0])) {
		return -EINVAL;
	}
	adc_list[chosen_adc].channel_config.channel_id =
		(u8_t)strtol(argv[args_indx.conf], NULL, 10);
	retval = adc_channel_setup(adc_dev,
			&adc_list[chosen_adc].channel_config);
	LOG_DBG("Channel setup returned %i\n", retval);
	return retval;
}

struct gain_string_to_enum {
	char *string;
	enum adc_gain gain;
};

static struct gain_string_to_enum gain_list[] = {
	{ .string = "1_6", .gain = ADC_GAIN_1_6 },
	{ .string = "1_5", .gain = ADC_GAIN_1_5 },
	{ .string = "1_4", .gain = ADC_GAIN_1_4 },
	{ .string = "1_3", .gain = ADC_GAIN_1_3 },
	{ .string = "1_2", .gain = ADC_GAIN_1_2 },
	{ .string = "2_3", .gain = ADC_GAIN_2_3 },
	{ .string = "1", .gain = ADC_GAIN_1 },
	{ .string = "2", .gain = ADC_GAIN_2 },
	{ .string = "3", .gain = ADC_GAIN_3 },
	{ .string = "4", .gain = ADC_GAIN_4 },
	{ .string = "8", .gain = ADC_GAIN_8 },
	{ .string = "16", .gain = ADC_GAIN_16 },
	{ .string = "32", .gain = ADC_GAIN_32 },
	{ .string = "64", .gain = ADC_GAIN_64 }
};

static int cmd_adc_gain(const struct shell *shell, size_t argc, char **argv)
{
	int retval = -EINVAL;
	struct device *adc_dev;
	int chosen_adc;
	int i;

	if (argc != args_no.gain) {
		shell_fprintf(shell, SHELL_NORMAL,
				"Usage: gain <adc> <gain>.\n<gain> options (x_y = x/y):\n");
		for (i = 0; i < ARRAY_SIZE(gain_list); i++) {
			shell_fprintf(shell, SHELL_NORMAL, "%s\n",
					gain_list[i].string);
		}

		return 0;
	}
	chosen_adc = get_adc_from_list(argv[args_indx.adc]);
	if (chosen_adc < 0) {
		return -EINVAL;
	}

	adc_dev = device_get_binding(adc_list[chosen_adc].device_name);
	if (adc_dev == NULL) {
		shell_error(shell, "ADC device not found");
		return -ENODEV;
	}
	for (i = 0; i < ARRAY_SIZE(gain_list); i++) {
		if (!strcmp(argv[args_indx.conf], gain_list[i].string)) {
			adc_list[chosen_adc].channel_config.gain =
				gain_list[i].gain;
			retval = adc_channel_setup(adc_dev,
					&adc_list[chosen_adc].channel_config);
			LOG_DBG("Channel setup returned %i\n", retval);
			break;
		}
	}
	return retval;
}
static int cmd_adc_acq(const struct shell *shell, size_t argc, char **argv)
{
	int retval = 0;
	struct device *adc_dev;
	int chosen_adc;
	u16_t acq_time;

	if (argc != args_no.acq_time) {
		shell_fprintf(shell, SHELL_NORMAL,
				"Usage: acq_time <adc> <time> <unit>\nunits: us, ns, ticks\n");
		return 0;
	}

	chosen_adc = get_adc_from_list(argv[args_indx.adc]);
	if (chosen_adc < 0) {
		return -EINVAL;
	}

	adc_dev = device_get_binding(adc_list[chosen_adc].device_name);
	if (adc_dev == NULL) {
		shell_error(shell, "ADC device not found");
		return -ENODEV;
	}

	if (!isdigit(argv[args_indx.conf][0])) {
		return -EINVAL;
	}
	acq_time = (u16_t)strtol(argv[args_indx.conf], NULL, 10);
	if (!strcmp(argv[args_indx.acq_unit], "us")) {
		adc_list[chosen_adc].channel_config.acquisition_time =
			ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, acq_time);
	} else if (!strcmp(argv[args_indx.acq_unit], "ns")) {
		adc_list[chosen_adc].channel_config.acquisition_time =
			ADC_ACQ_TIME(ADC_ACQ_TIME_NANOSECONDS, acq_time);
	} else if (!strcmp(argv[args_indx.acq_unit], "ticks")) {
		adc_list[chosen_adc].channel_config.acquisition_time =
			ADC_ACQ_TIME(ADC_ACQ_TIME_TICKS, acq_time);
	} else {
		adc_list[chosen_adc].channel_config.acquisition_time =
			ADC_ACQ_TIME_DEFAULT;
	}
	retval = adc_channel_setup(adc_dev,
			&adc_list[chosen_adc].channel_config);
	LOG_DBG("Channel setup returned %i\n", retval);
	return retval;
}
static int cmd_adc_reso(const struct shell *shell, size_t argc, char **argv)
{
	int retval = 0;
	struct device *adc_dev;
	int chosen_adc;

	if (argc != args_no.resolution ||
			!isdigit((unsigned char)argv[args_indx.conf][0])) {
		shell_fprintf(shell, SHELL_NORMAL,
				"Usage: resolution <adc> <resolution>\n");
		return 0;
	}

	chosen_adc = get_adc_from_list(argv[args_indx.adc]);
	if (chosen_adc < 0) {
		return -EINVAL;
	}

	adc_dev = device_get_binding(adc_list[chosen_adc].device_name);
	if (adc_dev == NULL) {
		shell_error(shell, "ADC device not found");
		return -ENODEV;
	}
	if (!isdigit(argv[args_indx.conf][0])) {
		return -EINVAL;
	}
	adc_list[chosen_adc].resolution =
		(u8_t)strtol(argv[args_indx.conf], NULL, 10);
	retval = adc_channel_setup(adc_dev,
			&adc_list[chosen_adc].channel_config);
	return retval;
}

struct reference_string_to_enum {
	char *string;
	enum adc_reference reference;
};

static struct reference_string_to_enum reference_list[] = {
	{ .string = "VDD_1", .reference = ADC_REF_VDD_1 },
	{ .string = "VDD_1_2", .reference = ADC_REF_VDD_1_2 },
	{ .string = "VDD_1_3", .reference = ADC_REF_VDD_1_3 },
	{ .string = "VDD_1_4", .reference = ADC_REF_VDD_1_4 },
	{ .string = "INT", .reference = ADC_REF_INTERNAL },
	{ .string = "EXT0", .reference = ADC_REF_EXTERNAL0 },
	{ .string = "EXT1", .reference = ADC_REF_EXTERNAL1 }
};

static int cmd_adc_ref(const struct shell *shell, size_t argc, char **argv)
{
	int retval = 0;
	struct device *adc_dev;
	int chosen_adc;
	int i;

	if (argc != args_no.ref) {
		shell_fprintf(shell, SHELL_NORMAL,
				"Usage: ref <adc> <ref>\n<ref> options:\n");
		for (i = 0; i < ARRAY_SIZE(reference_list); i++) {
			shell_fprintf(shell, SHELL_NORMAL,
					"%s\n", reference_list[i].string);
		}
		return 0;
	}

	chosen_adc = get_adc_from_list(argv[args_indx.adc]);
	if (chosen_adc < 0) {
		return -EINVAL;
	}

	adc_dev = device_get_binding(adc_list[chosen_adc].device_name);
	if (adc_dev == NULL) {
		shell_error(shell, "ADC device not found");
		return -ENODEV;
	}
	for (i = 0; i < ARRAY_SIZE(reference_list); i++) {
		if (!strcmp(argv[args_indx.conf], reference_list[i].string)) {
			adc_list[chosen_adc].channel_config.reference =
				reference_list[i].reference;
			retval = adc_channel_setup(adc_dev,
					&adc_list[chosen_adc].channel_config);
			LOG_DBG("Channel setup returned %i\n", retval);
			break;
		}
	}
	retval = adc_channel_setup(adc_dev, &adc_list[chosen_adc].channel_config);
	return retval;
}
#define BUFFER_SIZE 1
static int cmd_adc_read(const struct shell *shell, size_t argc, char **argv)
{
	int retval = 0;
	int chosen_adc = -1;
	struct device *adc_dev;
	u16_t m_sample_buffer[BUFFER_SIZE];

	if (argc != args_no.read) {
		shell_fprintf(shell, SHELL_NORMAL,
				"Usage: read <adc> <channel>\n");
		return 0;
	}
	chosen_adc = get_adc_from_list(argv[args_indx.adc]);
	if (chosen_adc < 0) {
		return 0;
	}
	u8_t adc_channel_id = strtol(argv[args_indx.channel], NULL, 10);

	adc_dev = device_get_binding(adc_list[chosen_adc].device_name);
	if (adc_dev == NULL) {
		shell_error(shell, "ADC device not found");
		return -ENODEV;
	}
	adc_list[chosen_adc].channel_config.channel_id = adc_channel_id;
	const struct adc_sequence sequence = {
		.channels	=
			BIT(adc_list[chosen_adc].channel_config.channel_id),
		.buffer		= m_sample_buffer,
		.buffer_size	= sizeof(m_sample_buffer),
		.resolution	= adc_list[chosen_adc].resolution,
	};
	retval = adc_read(adc_dev, &sequence);
	if (retval >= 0) {
		shell_fprintf(shell, SHELL_NORMAL,
				"Read: %i\n", m_sample_buffer[0]);
	}
	return retval;
}

static int cmd_adc_print(const struct shell *shell, size_t argc, char **argv)
{
	int chosen_adc = -1;
	int i;
	char *gain = "1";
	char *ref = "INTERNAL";
	u16_t acq_time;
	u8_t channel_id;
	u8_t resolution;

	if (argc < 2) {
		shell_fprintf(shell, SHELL_NORMAL, "usage: print <adc>\n");
	}
	chosen_adc = get_adc_from_list(argv[args_indx.adc]);
	if (chosen_adc < 0) {
		return 0;
	}
	for (i = 0; i < ARRAY_SIZE(gain_list); i++) {
		if (gain_list[i].gain ==
				adc_list[chosen_adc].channel_config.gain) {
			gain = gain_list[i].string;
		}
	}
	for (i = 0; i < ARRAY_SIZE(reference_list); i++) {
		if (reference_list[i].reference ==
				adc_list[chosen_adc].channel_config.reference) {
			ref = reference_list[i].string;
		}
	}
	acq_time = adc_list[chosen_adc].channel_config.acquisition_time;
	channel_id = adc_list[chosen_adc].channel_config.channel_id;
	resolution = adc_list[chosen_adc].resolution;
	shell_fprintf(shell, SHELL_NORMAL, "%s:\n"
			"Gain: %s\n"
			"Reference: %s\n"
			"Acquisition Time: %u\n"
			"Channel ID: %u\n"
			"Resolution: %u\n",
			argv[args_indx.adc],
			gain,
			ref,
			acq_time,
			channel_id,
			resolution);
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_adc,
	/* Alphabetically sorted. */
	SHELL_CMD(acq_time, NULL, "Configure acquisition time", cmd_adc_acq),
	SHELL_CMD(channel_id, NULL, "Configure channel id", cmd_adc_channel),
	SHELL_CMD(gain, NULL, "Configure gain", cmd_adc_gain),
	SHELL_CMD(print, NULL, "Print current configuration", cmd_adc_print),
	SHELL_CMD(read, NULL, "Read adc value", cmd_adc_read),
	SHELL_CMD(reference, NULL, "Configure reference", cmd_adc_ref),
	SHELL_CMD(resolution, NULL, "Configure resolution", cmd_adc_reso),
	SHELL_SUBCMD_SET_END /* Array terminated. */
);

SHELL_CMD_REGISTER(adc, &sub_adc, "ADC commands", NULL);
