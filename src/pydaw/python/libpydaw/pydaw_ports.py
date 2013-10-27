# -*- coding: utf-8 -*-
"""
This file is part of the PyDAW project, Copyright PyDAW Team

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
"""

#Euphoria
EUPHORIA_FILES_STRING_DELIMITER = '|' #'|'
EUPHORIA_MAX_SAMPLE_COUNT = 32
#+1 to LMS_MAX_SAMPLE_COUNT, the highest index is for the preview sample
EUPHORIA_TOTAL_SAMPLE_COUNT = 33
#Total number of LFOs, ADSRs, other envelopes, etc... Used for the PolyFX mod matrix
EUPHORIA_MODULATOR_COUNT = 4
#How many modular PolyFX
EUPHORIA_MODULAR_POLYFX_COUNT = 4
#How many ports per PolyFX, 3 knobs and a combobox
EUPHORIA_PORTS_PER_MOD_EFFECT = 4
#How many knobs per PolyFX, 3 knobs
EUPHORIA_CONTROLS_PER_MOD_EFFECT = 3
#How many groups of effects. This will become useful when each sample has an "effects group" choice
#EDIT: This may or may not ever come to fruition with my strategy. Delete self and re-arrange everywhere it's used...
EUPHORIA_EFFECTS_GROUPS_COUNT = 1
#The number of mono_fx groups
EUPHORIA_MONO_FX_GROUPS_COUNT = EUPHORIA_MAX_SAMPLE_COUNT
EUPHORIA_MONO_FX_COUNT = 4
EUPHORIA_LABEL = "Euphoria"
EUPHORIA_OUTPUT_LEFT = 0
EUPHORIA_OUTPUT_RIGHT = 1
EUPHORIA_FIRST_CONTROL_PORT = 2
EUPHORIA_SELECTED_SAMPLE = 2
EUPHORIA_ATTACK = 3
EUPHORIA_DECAY = 4
EUPHORIA_SUSTAIN = 5
EUPHORIA_RELEASE = 6
EUPHORIA_FILTER_ATTACK = 7
EUPHORIA_FILTER_DECAY = 8
EUPHORIA_FILTER_SUSTAIN = 9
EUPHORIA_FILTER_RELEASE = 10
EUPHORIA_LFO_PITCH = 11
EUPHORIA_MASTER_VOLUME = 12
EUPHORIA_MASTER_GLIDE = 13
EUPHORIA_MASTER_PITCHBEND_AMT = 14
EUPHORIA_PITCH_ENV_TIME = 15
EUPHORIA_LFO_FREQ = 16
EUPHORIA_LFO_TYPE = 17
#From Modulex
EUPHORIA_FX0_KNOB0 = 18
EUPHORIA_FX0_KNOB1 = 19
EUPHORIA_FX0_KNOB2 = 20
EUPHORIA_FX0_COMBOBOX = 21
EUPHORIA_FX1_KNOB0 = 22
EUPHORIA_FX1_KNOB1 = 23
EUPHORIA_FX1_KNOB2 = 24
EUPHORIA_FX1_COMBOBOX = 25
EUPHORIA_FX2_KNOB0 = 26
EUPHORIA_FX2_KNOB1 = 27
EUPHORIA_FX2_KNOB2 = 28
EUPHORIA_FX2_COMBOBOX = 29
EUPHORIA_FX3_KNOB0 = 30
EUPHORIA_FX3_KNOB1 = 31
EUPHORIA_FX3_KNOB2 = 32
EUPHORIA_FX3_COMBOBOX = 33
#PolyFX Mod Matrix
EUPHORIA_PFXMATRIX_FIRST_PORT = 34
EUPHORIA_PFXMATRIX_GRP0DST0SRC0CTRL0 = 34
EUPHORIA_PFXMATRIX_GRP0DST0SRC0CTRL1 = 35
EUPHORIA_PFXMATRIX_GRP0DST0SRC0CTRL2 = 36
EUPHORIA_PFXMATRIX_GRP0DST0SRC1CTRL0 = 37
EUPHORIA_PFXMATRIX_GRP0DST0SRC1CTRL1 = 38
EUPHORIA_PFXMATRIX_GRP0DST0SRC1CTRL2 = 39
EUPHORIA_PFXMATRIX_GRP0DST0SRC2CTRL0 = 40
EUPHORIA_PFXMATRIX_GRP0DST0SRC2CTRL1 = 41
EUPHORIA_PFXMATRIX_GRP0DST0SRC2CTRL2 = 42
EUPHORIA_PFXMATRIX_GRP0DST0SRC3CTRL0 = 43
EUPHORIA_PFXMATRIX_GRP0DST0SRC3CTRL1 = 44
EUPHORIA_PFXMATRIX_GRP0DST0SRC3CTRL2 = 45
EUPHORIA_PFXMATRIX_GRP0DST1SRC0CTRL0 = 46
EUPHORIA_PFXMATRIX_GRP0DST1SRC0CTRL1 = 47
EUPHORIA_PFXMATRIX_GRP0DST1SRC0CTRL2 = 48
EUPHORIA_PFXMATRIX_GRP0DST1SRC1CTRL0 = 49
EUPHORIA_PFXMATRIX_GRP0DST1SRC1CTRL1 = 50
EUPHORIA_PFXMATRIX_GRP0DST1SRC1CTRL2 = 51
EUPHORIA_PFXMATRIX_GRP0DST1SRC2CTRL0 = 52
EUPHORIA_PFXMATRIX_GRP0DST1SRC2CTRL1 = 53
EUPHORIA_PFXMATRIX_GRP0DST1SRC2CTRL2 = 54
EUPHORIA_PFXMATRIX_GRP0DST1SRC3CTRL0 = 55
EUPHORIA_PFXMATRIX_GRP0DST1SRC3CTRL1 = 56
EUPHORIA_PFXMATRIX_GRP0DST1SRC3CTRL2 = 57
EUPHORIA_PFXMATRIX_GRP0DST2SRC0CTRL0 = 58
EUPHORIA_PFXMATRIX_GRP0DST2SRC0CTRL1 = 59
EUPHORIA_PFXMATRIX_GRP0DST2SRC0CTRL2 = 60
EUPHORIA_PFXMATRIX_GRP0DST2SRC1CTRL0 = 61
EUPHORIA_PFXMATRIX_GRP0DST2SRC1CTRL1 = 62
EUPHORIA_PFXMATRIX_GRP0DST2SRC1CTRL2 = 63
EUPHORIA_PFXMATRIX_GRP0DST2SRC2CTRL0 = 64
EUPHORIA_PFXMATRIX_GRP0DST2SRC2CTRL1 = 65
EUPHORIA_PFXMATRIX_GRP0DST2SRC2CTRL2 = 66
EUPHORIA_PFXMATRIX_GRP0DST2SRC3CTRL0 = 67
EUPHORIA_PFXMATRIX_GRP0DST2SRC3CTRL1 = 68
EUPHORIA_PFXMATRIX_GRP0DST2SRC3CTRL2 = 69
EUPHORIA_PFXMATRIX_GRP0DST3SRC0CTRL0 = 70
EUPHORIA_PFXMATRIX_GRP0DST3SRC0CTRL1 = 71
EUPHORIA_PFXMATRIX_GRP0DST3SRC0CTRL2 = 72
EUPHORIA_PFXMATRIX_GRP0DST3SRC1CTRL0 = 73
EUPHORIA_PFXMATRIX_GRP0DST3SRC1CTRL1 = 74
EUPHORIA_PFXMATRIX_GRP0DST3SRC1CTRL2 = 75
EUPHORIA_PFXMATRIX_GRP0DST3SRC2CTRL0 = 76
EUPHORIA_PFXMATRIX_GRP0DST3SRC2CTRL1 = 77
EUPHORIA_PFXMATRIX_GRP0DST3SRC2CTRL2 = 78
EUPHORIA_PFXMATRIX_GRP0DST3SRC3CTRL0 = 79
EUPHORIA_PFXMATRIX_GRP0DST3SRC3CTRL1 = 80
EUPHORIA_PFXMATRIX_GRP0DST3SRC3CTRL2 = 81
#End PolyFX Mod Matrix

"""This is the last control port, + 1, for zero-based iteration"""
EUPHORIA_LAST_REGULAR_CONTROL_PORT = 82
"""The first port to use when enumerating the ports for mod_matrix controls. All of the mod_matrix ports should be sequential,
* any additional ports should prepend self port number"""
EUPHORIA_FIRST_SAMPLE_TABLE_PORT = 82
"""The range of ports for sample pitch"""
EUPHORIA_SAMPLE_PITCH_PORT_RANGE_MIN = EUPHORIA_FIRST_SAMPLE_TABLE_PORT
EUPHORIA_SAMPLE_PITCH_PORT_RANGE_MAX = (EUPHORIA_SAMPLE_PITCH_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
"""The range of ports for the low note to play a sample on"""
EUPHORIA_PLAY_PITCH_LOW_PORT_RANGE_MIN = EUPHORIA_SAMPLE_PITCH_PORT_RANGE_MAX
EUPHORIA_PLAY_PITCH_LOW_PORT_RANGE_MAX = (EUPHORIA_PLAY_PITCH_LOW_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
"""The range of ports for the high note to play a sample on"""
EUPHORIA_PLAY_PITCH_HIGH_PORT_RANGE_MIN = EUPHORIA_PLAY_PITCH_LOW_PORT_RANGE_MAX
EUPHORIA_PLAY_PITCH_HIGH_PORT_RANGE_MAX = (EUPHORIA_PLAY_PITCH_HIGH_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
"""The range of ports for sample volume"""
EUPHORIA_SAMPLE_VOLUME_PORT_RANGE_MIN = EUPHORIA_PLAY_PITCH_HIGH_PORT_RANGE_MAX
EUPHORIA_SAMPLE_VOLUME_PORT_RANGE_MAX = (EUPHORIA_SAMPLE_VOLUME_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
EUPHORIA_SAMPLE_START_PORT_RANGE_MIN = EUPHORIA_SAMPLE_VOLUME_PORT_RANGE_MAX
EUPHORIA_SAMPLE_START_PORT_RANGE_MAX = (EUPHORIA_SAMPLE_START_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
EUPHORIA_SAMPLE_END_PORT_RANGE_MIN = EUPHORIA_SAMPLE_START_PORT_RANGE_MAX
EUPHORIA_SAMPLE_END_PORT_RANGE_MAX = (EUPHORIA_SAMPLE_END_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
EUPHORIA_SAMPLE_VEL_SENS_PORT_RANGE_MIN = EUPHORIA_SAMPLE_END_PORT_RANGE_MAX
EUPHORIA_SAMPLE_VEL_SENS_PORT_RANGE_MAX = (EUPHORIA_SAMPLE_VEL_SENS_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
EUPHORIA_SAMPLE_VEL_LOW_PORT_RANGE_MIN = EUPHORIA_SAMPLE_VEL_SENS_PORT_RANGE_MAX
EUPHORIA_SAMPLE_VEL_LOW_PORT_RANGE_MAX = (EUPHORIA_SAMPLE_VEL_LOW_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
EUPHORIA_SAMPLE_VEL_HIGH_PORT_RANGE_MIN = EUPHORIA_SAMPLE_VEL_LOW_PORT_RANGE_MAX
EUPHORIA_SAMPLE_VEL_HIGH_PORT_RANGE_MAX = (EUPHORIA_SAMPLE_VEL_HIGH_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
EUPHORIA_PITCH_PORT_RANGE_MIN = EUPHORIA_SAMPLE_VEL_HIGH_PORT_RANGE_MAX
EUPHORIA_PITCH_PORT_RANGE_MAX = (EUPHORIA_PITCH_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
EUPHORIA_TUNE_PORT_RANGE_MIN = EUPHORIA_PITCH_PORT_RANGE_MAX
EUPHORIA_TUNE_PORT_RANGE_MAX = (EUPHORIA_TUNE_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
EUPHORIA_SAMPLE_INTERPOLATION_MODE_PORT_RANGE_MIN = EUPHORIA_TUNE_PORT_RANGE_MAX
EUPHORIA_SAMPLE_INTERPOLATION_MODE_PORT_RANGE_MAX = (EUPHORIA_SAMPLE_INTERPOLATION_MODE_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
EUPHORIA_SAMPLE_LOOP_START_PORT_RANGE_MIN = EUPHORIA_SAMPLE_INTERPOLATION_MODE_PORT_RANGE_MAX
EUPHORIA_SAMPLE_LOOP_START_PORT_RANGE_MAX = (EUPHORIA_SAMPLE_LOOP_START_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
EUPHORIA_SAMPLE_LOOP_END_PORT_RANGE_MIN = EUPHORIA_SAMPLE_LOOP_START_PORT_RANGE_MAX
EUPHORIA_SAMPLE_LOOP_END_PORT_RANGE_MAX = (EUPHORIA_SAMPLE_LOOP_END_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
EUPHORIA_SAMPLE_LOOP_MODE_PORT_RANGE_MIN = EUPHORIA_SAMPLE_LOOP_END_PORT_RANGE_MAX
EUPHORIA_SAMPLE_LOOP_MODE_PORT_RANGE_MAX = (EUPHORIA_SAMPLE_LOOP_MODE_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
#MonoFX0
EUPHORIA_MONO_FX0_KNOB0_PORT_RANGE_MIN = EUPHORIA_SAMPLE_LOOP_MODE_PORT_RANGE_MAX
EUPHORIA_MONO_FX0_KNOB0_PORT_RANGE_MAX = (EUPHORIA_MONO_FX0_KNOB0_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
EUPHORIA_MONO_FX0_KNOB1_PORT_RANGE_MIN = EUPHORIA_MONO_FX0_KNOB0_PORT_RANGE_MAX
EUPHORIA_MONO_FX0_KNOB1_PORT_RANGE_MAX = (EUPHORIA_MONO_FX0_KNOB1_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
EUPHORIA_MONO_FX0_KNOB2_PORT_RANGE_MIN = EUPHORIA_MONO_FX0_KNOB1_PORT_RANGE_MAX
EUPHORIA_MONO_FX0_KNOB2_PORT_RANGE_MAX = (EUPHORIA_MONO_FX0_KNOB2_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
EUPHORIA_MONO_FX0_COMBOBOX_PORT_RANGE_MIN = EUPHORIA_MONO_FX0_KNOB2_PORT_RANGE_MAX
EUPHORIA_MONO_FX0_COMBOBOX_PORT_RANGE_MAX = (EUPHORIA_MONO_FX0_COMBOBOX_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
#MonoFX1
EUPHORIA_MONO_FX1_KNOB0_PORT_RANGE_MIN = EUPHORIA_MONO_FX0_COMBOBOX_PORT_RANGE_MAX
EUPHORIA_MONO_FX1_KNOB0_PORT_RANGE_MAX = (EUPHORIA_MONO_FX1_KNOB0_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
EUPHORIA_MONO_FX1_KNOB1_PORT_RANGE_MIN = EUPHORIA_MONO_FX1_KNOB0_PORT_RANGE_MAX
EUPHORIA_MONO_FX1_KNOB1_PORT_RANGE_MAX = (EUPHORIA_MONO_FX1_KNOB1_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
EUPHORIA_MONO_FX1_KNOB2_PORT_RANGE_MIN = EUPHORIA_MONO_FX1_KNOB1_PORT_RANGE_MAX
EUPHORIA_MONO_FX1_KNOB2_PORT_RANGE_MAX = (EUPHORIA_MONO_FX1_KNOB2_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
EUPHORIA_MONO_FX1_COMBOBOX_PORT_RANGE_MIN = EUPHORIA_MONO_FX1_KNOB2_PORT_RANGE_MAX
EUPHORIA_MONO_FX1_COMBOBOX_PORT_RANGE_MAX = (EUPHORIA_MONO_FX1_COMBOBOX_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
#MonoFX2
EUPHORIA_MONO_FX2_KNOB0_PORT_RANGE_MIN = EUPHORIA_MONO_FX1_COMBOBOX_PORT_RANGE_MAX
EUPHORIA_MONO_FX2_KNOB0_PORT_RANGE_MAX = (EUPHORIA_MONO_FX2_KNOB0_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
EUPHORIA_MONO_FX2_KNOB1_PORT_RANGE_MIN = EUPHORIA_MONO_FX2_KNOB0_PORT_RANGE_MAX
EUPHORIA_MONO_FX2_KNOB1_PORT_RANGE_MAX = (EUPHORIA_MONO_FX2_KNOB1_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
EUPHORIA_MONO_FX2_KNOB2_PORT_RANGE_MIN = EUPHORIA_MONO_FX2_KNOB1_PORT_RANGE_MAX
EUPHORIA_MONO_FX2_KNOB2_PORT_RANGE_MAX = (EUPHORIA_MONO_FX2_KNOB2_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
EUPHORIA_MONO_FX2_COMBOBOX_PORT_RANGE_MIN = EUPHORIA_MONO_FX2_KNOB2_PORT_RANGE_MAX
EUPHORIA_MONO_FX2_COMBOBOX_PORT_RANGE_MAX = (EUPHORIA_MONO_FX2_COMBOBOX_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
#MonoFX3
EUPHORIA_MONO_FX3_KNOB0_PORT_RANGE_MIN = EUPHORIA_MONO_FX2_COMBOBOX_PORT_RANGE_MAX
EUPHORIA_MONO_FX3_KNOB0_PORT_RANGE_MAX = (EUPHORIA_MONO_FX3_KNOB0_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
EUPHORIA_MONO_FX3_KNOB1_PORT_RANGE_MIN = EUPHORIA_MONO_FX3_KNOB0_PORT_RANGE_MAX
EUPHORIA_MONO_FX3_KNOB1_PORT_RANGE_MAX = (EUPHORIA_MONO_FX3_KNOB1_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
EUPHORIA_MONO_FX3_KNOB2_PORT_RANGE_MIN = EUPHORIA_MONO_FX3_KNOB1_PORT_RANGE_MAX
EUPHORIA_MONO_FX3_KNOB2_PORT_RANGE_MAX = (EUPHORIA_MONO_FX3_KNOB2_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
EUPHORIA_MONO_FX3_COMBOBOX_PORT_RANGE_MIN = EUPHORIA_MONO_FX3_KNOB2_PORT_RANGE_MAX
EUPHORIA_MONO_FX3_COMBOBOX_PORT_RANGE_MAX = (EUPHORIA_MONO_FX3_COMBOBOX_PORT_RANGE_MIN + EUPHORIA_MONO_FX_GROUPS_COUNT)
#Sample FX Group
EUPHORIA_SAMPLE_MONO_FX_GROUP_PORT_RANGE_MIN = EUPHORIA_MONO_FX3_COMBOBOX_PORT_RANGE_MAX
EUPHORIA_SAMPLE_MONO_FX_GROUP_PORT_RANGE_MAX = (EUPHORIA_SAMPLE_MONO_FX_GROUP_PORT_RANGE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
#Noise amp
EUPHORIA_NOISE_AMP_MIN = EUPHORIA_SAMPLE_MONO_FX_GROUP_PORT_RANGE_MAX
EUPHORIA_NOISE_AMP_MAX = (EUPHORIA_NOISE_AMP_MIN + EUPHORIA_MAX_SAMPLE_COUNT)
#Noise type
EUPHORIA_NOISE_TYPE_MIN = EUPHORIA_NOISE_AMP_MAX
EUPHORIA_NOISE_TYPE_MAX = (EUPHORIA_NOISE_TYPE_MIN + EUPHORIA_MAX_SAMPLE_COUNT)

EUPHORIA_PORT_COUNT = EUPHORIA_NOISE_TYPE_MAX

#Modulex

MODULEX_INPUT0 = 0
MODULEX_INPUT1 = 1
MODULEX_OUTPUT0 = 2
MODULEX_OUTPUT1 = 3
MODULEX_FIRST_CONTROL_PORT = 4
MODULEX_FX0_KNOB0 = 4
MODULEX_FX0_KNOB1 = 5
MODULEX_FX0_KNOB2 = 6
MODULEX_FX0_COMBOBOX = 7
MODULEX_FX1_KNOB0 = 8
MODULEX_FX1_KNOB1 = 9
MODULEX_FX1_KNOB2 = 10
MODULEX_FX1_COMBOBOX = 11
MODULEX_FX2_KNOB0 = 12
MODULEX_FX2_KNOB1 = 13
MODULEX_FX2_KNOB2 = 14
MODULEX_FX2_COMBOBOX = 15
MODULEX_FX3_KNOB0 = 16
MODULEX_FX3_KNOB1 = 17
MODULEX_FX3_KNOB2 = 18
MODULEX_FX3_COMBOBOX = 19
MODULEX_FX4_KNOB0 = 20
MODULEX_FX4_KNOB1 = 21
MODULEX_FX4_KNOB2 = 22
MODULEX_FX4_COMBOBOX = 23
MODULEX_FX5_KNOB0 = 24
MODULEX_FX5_KNOB1 = 25
MODULEX_FX5_KNOB2 = 26
MODULEX_FX5_COMBOBOX = 27
MODULEX_FX6_KNOB0 = 28
MODULEX_FX6_KNOB1 = 29
MODULEX_FX6_KNOB2 = 30
MODULEX_FX6_COMBOBOX = 31
MODULEX_FX7_KNOB0 = 32
MODULEX_FX7_KNOB1 = 33
MODULEX_FX7_KNOB2 = 34
MODULEX_FX7_COMBOBOX = 35
MODULEX_DELAY_TIME = 36
MODULEX_FEEDBACK = 37
MODULEX_DRY = 38
MODULEX_WET = 39
MODULEX_DUCK = 40
MODULEX_CUTOFF = 41
MODULEX_STEREO = 42
MODULEX_VOL_SLIDER = 43
MODULEX_REVERB_TIME = 44
MODULEX_REVERB_WET = 45
MODULEX_REVERB_COLOR = 46
"""This is the last control port"""
MODULEX_LAST_CONTROL_PORT = 46
MODULEX_COUNT = 47 #must be 1 + highest value above CHANGE THIS IF YOU ADD OR TAKE AWAY ANYTHING


#Way-V


#Total number of LFOs, ADSRs, other envelopes, etc... Used for the PolyFX mod matrix
WAYV_MODULATOR_COUNT = 4
#How many modular PolyFX
WAYV_MODULAR_POLYFX_COUNT = 4
#How many ports per PolyFX, 3 knobs and a combobox
WAYV_PORTS_PER_MOD_EFFECT = 4
#How many knobs per PolyFX, 3 knobs
WAYV_CONTROLS_PER_MOD_EFFECT = 3
WAYV_EFFECTS_GROUPS_COUNT = 1
WAYV_OUTPUT0 = 0
WAYV_OUTPUT1 = 1
WAYV_FIRST_CONTROL_PORT = 2
WAYV_ATTACK_MAIN = 2
WAYV_DECAY_MAIN = 3
WAYV_SUSTAIN_MAIN = 4
WAYV_RELEASE_MAIN = 5
WAYV_NOISE_AMP = 6
WAYV_OSC1_TYPE = 7
WAYV_OSC1_PITCH = 8
WAYV_OSC1_TUNE = 9
WAYV_OSC1_VOLUME = 10
WAYV_OSC2_TYPE = 11
WAYV_OSC2_PITCH = 12
WAYV_OSC2_TUNE = 13
WAYV_OSC2_VOLUME = 14
WAYV_MASTER_VOLUME = 15
WAYV_OSC1_UNISON_VOICES = 16
WAYV_OSC1_UNISON_SPREAD = 17
WAYV_MASTER_GLIDE = 18
WAYV_MASTER_PITCHBEND_AMT = 19
WAYV_ATTACK1 = 20
WAYV_DECAY1 = 21
WAYV_SUSTAIN1 = 22
WAYV_RELEASE1 = 23
WAYV_ATTACK2 = 24
WAYV_DECAY2 = 25
WAYV_SUSTAIN2 = 26
WAYV_RELEASE2 = 27
WAYV_ATTACK_PFX1 = 28
WAYV_DECAY_PFX1 = 29
WAYV_SUSTAIN_PFX1 = 30
WAYV_RELEASE_PFX1 = 31
WAYV_ATTACK_PFX2 = 32
WAYV_DECAY_PFX2 = 33
WAYV_SUSTAIN_PFX2 = 34
WAYV_RELEASE_PFX2 = 35
LMS_NOISE_TYPE = 36
WAYV_RAMP_ENV_TIME = 37
WAYV_LFO_FREQ = 38
WAYV_LFO_TYPE = 39
WAYV_FX0_KNOB0 = 40
WAYV_FX0_KNOB1 = 41
WAYV_FX0_KNOB2 = 42
WAYV_FX0_COMBOBOX = 43
WAYV_FX1_KNOB0 = 44
WAYV_FX1_KNOB1 = 45
WAYV_FX1_KNOB2 = 46
WAYV_FX1_COMBOBOX = 47
WAYV_FX2_KNOB0 = 48
WAYV_FX2_KNOB1 = 49
WAYV_FX2_KNOB2 = 50
WAYV_FX2_COMBOBOX = 51
WAYV_FX3_KNOB0 = 52
WAYV_FX3_KNOB1 = 53
WAYV_FX3_KNOB2 = 54
WAYV_FX3_COMBOBOX = 55
#PolyFX Mod Matrix
WAVV_PFXMATRIX_FIRST_PORT = 56
WAVV_PFXMATRIX_GRP0DST0SRC0CTRL0 = 56
WAVV_PFXMATRIX_GRP0DST0SRC0CTRL1 = 57
WAVV_PFXMATRIX_GRP0DST0SRC0CTRL2 = 58
WAVV_PFXMATRIX_GRP0DST0SRC1CTRL0 = 59
WAVV_PFXMATRIX_GRP0DST0SRC1CTRL1 = 60
WAVV_PFXMATRIX_GRP0DST0SRC1CTRL2 = 61
WAVV_PFXMATRIX_GRP0DST0SRC2CTRL0 = 62
WAVV_PFXMATRIX_GRP0DST0SRC2CTRL1 = 63
WAVV_PFXMATRIX_GRP0DST0SRC2CTRL2 = 64
WAVV_PFXMATRIX_GRP0DST0SRC3CTRL0 = 65
WAVV_PFXMATRIX_GRP0DST0SRC3CTRL1 = 66
WAVV_PFXMATRIX_GRP0DST0SRC3CTRL2 = 67
WAVV_PFXMATRIX_GRP0DST1SRC0CTRL0 = 68
WAVV_PFXMATRIX_GRP0DST1SRC0CTRL1 = 69
WAVV_PFXMATRIX_GRP0DST1SRC0CTRL2 = 70
WAVV_PFXMATRIX_GRP0DST1SRC1CTRL0 = 71
WAVV_PFXMATRIX_GRP0DST1SRC1CTRL1 = 72
WAVV_PFXMATRIX_GRP0DST1SRC1CTRL2 = 73
WAVV_PFXMATRIX_GRP0DST1SRC2CTRL0 = 74
WAVV_PFXMATRIX_GRP0DST1SRC2CTRL1 = 75
WAVV_PFXMATRIX_GRP0DST1SRC2CTRL2 = 76
WAVV_PFXMATRIX_GRP0DST1SRC3CTRL0 = 77
WAVV_PFXMATRIX_GRP0DST1SRC3CTRL1 = 78
WAVV_PFXMATRIX_GRP0DST1SRC3CTRL2 = 79
WAVV_PFXMATRIX_GRP0DST2SRC0CTRL0 = 80
WAVV_PFXMATRIX_GRP0DST2SRC0CTRL1 = 81
WAVV_PFXMATRIX_GRP0DST2SRC0CTRL2 = 82
WAVV_PFXMATRIX_GRP0DST2SRC1CTRL0 = 83
WAVV_PFXMATRIX_GRP0DST2SRC1CTRL1 = 84
WAVV_PFXMATRIX_GRP0DST2SRC1CTRL2 = 85
WAVV_PFXMATRIX_GRP0DST2SRC2CTRL0 = 86
WAVV_PFXMATRIX_GRP0DST2SRC2CTRL1 = 87
WAVV_PFXMATRIX_GRP0DST2SRC2CTRL2 = 88
WAVV_PFXMATRIX_GRP0DST2SRC3CTRL0 = 89
WAVV_PFXMATRIX_GRP0DST2SRC3CTRL1 = 90
WAVV_PFXMATRIX_GRP0DST2SRC3CTRL2 = 91
WAVV_PFXMATRIX_GRP0DST3SRC0CTRL0 = 92
WAVV_PFXMATRIX_GRP0DST3SRC0CTRL1 = 93
WAVV_PFXMATRIX_GRP0DST3SRC0CTRL2 = 94
WAVV_PFXMATRIX_GRP0DST3SRC1CTRL0 = 95
WAVV_PFXMATRIX_GRP0DST3SRC1CTRL1 = 96
WAVV_PFXMATRIX_GRP0DST3SRC1CTRL2 = 97
WAVV_PFXMATRIX_GRP0DST3SRC2CTRL0 = 98
WAVV_PFXMATRIX_GRP0DST3SRC2CTRL1 = 99
WAVV_PFXMATRIX_GRP0DST3SRC2CTRL2 = 100
WAVV_PFXMATRIX_GRP0DST3SRC3CTRL0 = 101
WAVV_PFXMATRIX_GRP0DST3SRC3CTRL1 = 102
WAVV_PFXMATRIX_GRP0DST3SRC3CTRL2 = 103
#End PolyFX Mod Matrix
WAYV_ADSR1_CHECKBOX = 105
WAYV_ADSR2_CHECKBOX = 106
WAYV_LFO_AMP = 107
WAYV_LFO_PITCH = 108
WAYV_PITCH_ENV_AMT = 109
WAYV_OSC2_UNISON_VOICES = 110
WAYV_OSC2_UNISON_SPREAD = 111
WAYV_LFO_AMOUNT = 112
WAYV_OSC3_TYPE = 113
WAYV_OSC3_PITCH = 114
WAYV_OSC3_TUNE = 115
WAYV_OSC3_VOLUME = 116
WAYV_OSC3_UNISON_VOICES = 117
WAYV_OSC3_UNISON_SPREAD = 118
WAYV_OSC1_FM1 = 119
WAYV_OSC1_FM2 = 120
WAYV_OSC1_FM3 = 121
WAYV_OSC2_FM1 = 122
WAYV_OSC2_FM2 = 123
WAYV_OSC2_FM3 = 124
WAYV_OSC3_FM1 = 125
WAYV_OSC3_FM2 = 126
WAYV_OSC3_FM3 = 127
WAYV_ATTACK3 = 128
WAYV_DECAY3 = 129
WAYV_SUSTAIN3 = 130
WAYV_RELEASE3 = 131
WAYV_ADSR3_CHECKBOX = 132
WAYV_LAST_CONTROL_PORT = 132
WAYV_COUNT = 133 #must be 1 + highest value above CHANGE THIS IF YOU ADD OR TAKE AWAY ANYTHING
WAYV_POLYPHONY = 16


#Ray-V

RAYV_OUTPUT0 = 0
RAYV_OUTPUT1 = 1
RAYV_FIRST_CONTROL_PORT = 2
RAYV_ATTACK = 2
RAYV_DECAY = 3
RAYV_SUSTAIN = 4
RAYV_RELEASE = 5
RAYV_TIMBRE = 6
RAYV_RES = 7
RAYV_DIST = 8
RAYV_FILTER_ATTACK = 9
RAYV_FILTER_DECAY = 10
RAYV_FILTER_SUSTAIN = 11
RAYV_FILTER_RELEASE = 12
RAYV_NOISE_AMP = 13
RAYV_FILTER_ENV_AMT = 14
RAYV_DIST_WET = 15
RAYV_OSC1_TYPE = 16
RAYV_OSC1_PITCH = 17
RAYV_OSC1_TUNE = 18
RAYV_OSC1_VOLUME = 19
RAYV_OSC2_TYPE = 20
RAYV_OSC2_PITCH = 21
RAYV_OSC2_TUNE = 22
RAYV_OSC2_VOLUME = 23
RAYV_MASTER_VOLUME = 24
RAYV_MASTER_UNISON_VOICES = 25
RAYV_MASTER_UNISON_SPREAD = 26
RAYV_MASTER_GLIDE = 27
RAYV_MASTER_PITCHBEND_AMT = 28
RAYV_PITCH_ENV_TIME = 29
RAYV_PITCH_ENV_AMT = 30
RAYV_LFO_FREQ = 31
RAYV_LFO_TYPE = 32
RAYV_LFO_AMP = 33
RAYV_LFO_PITCH = 34
RAYV_LFO_FILTER = 35
RAYV_OSC_HARD_SYNC = 36
RAYV_LAST_CONTROL_PORT = 36
RAYV_POLYPHONY = 16
RAYV_COUNT = 37 #must be 1 + highest value above CHANGE THIS IF YOU ADD OR TAKE AWAY ANYTHING


#Port maps

WAYV_PORT_MAP = \
[("Master Attack", "2", "2", "10.0", "100.0"),
("Master Decay", "3", "2", "10.0", "100.0"),
("Master Sustain", "4", "0", "-30.0", "0.0"),
("Master Release", "5", "2", "10.0", "200.0"),
("Noise Amp", "6", "0", "-60.0", "0.0"),
("Master Glide", "18", "2", "0.0", "200.0"),
("Osc1 Attack", "20", "2", "10.0", "100.0"),
("Osc1 Decay", "21", "2", "10.0", "100.0"),
("Osc1 Sustain", "22", "0", "-30.0", "0.0"),
("Osc1 Release", "23", "2", "10.0", "200.0"),
("Osc2 Attack", "24", "2", "10.0", "100.0"),
("Osc2 Decay", "25", "2", "10.0", "100.0"),
("Osc2 Sustain", "26", "0", "-30.0", "0.0"),
("Osc2 Release", "27", "2", "10.0", "200.0"),
("ADSR1 Attack", "28", "2", "10.0", "100.0"),
("ADSR1 Decay", "29", "2", "10.0", "100.0"),
("ADSR1 Sustain", "30", "0", "-60.0", "0.0"),
("ADSR1 Release", "31", "2", "10.0", "200.0"),
("ADSR2 Attack", "32", "2", "10.0", "100.0"),
("ADSR2 Sustain", "34", "2", "0.0", "100.0"),
("ADSR2 Release", "35", "2", "10.0", "200.0"),
("Pitch Env Time", "37", "2", "0.0", "200.0"),
("LFO Freq", "38", "2", "10.0", "1600.0"),
("FX0 Knob0", "40", "0", "0.0", "127.0"),
("FX0 Knob1", "41", "0", "0.0", "127.0"),
("FX0 Knob2", "42", "0", "0.0", "127.0"),
("FX1 Knob0", "44", "0", "0.0", "127.0"),
("FX1 Knob1", "45", "0", "0.0", "127.0"),
("FX1 Knob2", "46", "0", "0.0", "127.0"),
("FX2 Knob0", "48", "0", "0.0", "127.0"),
("FX2 Knob1", "49", "0", "0.0", "127.0"),
("FX2 Knob2", "50", "0", "0.0", "127.0"),
("FX3 Knob0", "52", "0", "0.0", "127.0"),
("FX3 Knob1", "53", "0", "0.0", "127.0"),
("FX3 Knob2", "54", "0", "0.0", "127.0"),
("LFO Amp", "107", "0", "-24.0", "24.0"),
("LFO Pitch", "108", "0", "-36.0", "36.0"),
("Pitch Env Amt", "109", "0", "-36.0", "36.0"),
("LFO Amount", "112", "2", "0.0", "100.0"),
("Osc1 FM1", "119", "0", "0.0", "100.0"),
("Osc1 FM2", "120", "0", "0.0", "100.0"),
("Osc1 FM3", "121", "0", "0.0", "100.0"),
("Osc2 FM1", "122", "0", "0.0", "100.0"),
("Osc2 FM2", "123", "0", "0.0", "100.0"),
("Osc2 FM3", "124", "0", "0.0", "100.0"),
("Osc1 FM1", "125", "0", "0.0", "100.0"),
("Osc1 FM2", "126", "0", "0.0", "100.0"),
("Osc1 FM3", "127", "0", "0.0", "100.0"),
("Osc3 Attack", "128", "2", "10.0", "100.0"),
("Osc3 Decay", "129", "2", "10.0", "100.0"),
("Osc3 Sustain", "130", "0", "-30.0", "0.0"),
("Osc3 Release time", "131", "2", "10.0", "200.0")]

RAYV_PORT_MAP = \
[("Attack time (s)", "2", "2", "10.0", "100.0"),
("Decay time (s)", "3", "2", "10.0", "100.0"),
("Sustain level (%)", "4", "0", "-60.0", "0.0"),
("Release time (s)", "5", "2", "10.0", "200.0"),
("Filter Cutoff", "6", "1", "20.0", "124.0"),
("Res", "7", "0", "-30.0", "0.0"),
("Dist", "8", "0", "-6.0", "36.0"),
("Attack time (s) filter", "9", "2", "10.0", "100.0"),
("Decay time (s) filter", "10", "2", "10.0", "100.0"),
("Sustain level (%) filter", "11", "2", "0.0", "100.0"),
("Release time (s) filter", "12", "2", "10.0", "200.0"),
("Noise Amp", "13", "0", "-60.0", "0.0"),
("Filter Env Amt", "14", "0", "-36.0", "36.0"),
("Dist Wet", "15", "2", "0.0", "100.0"),
("Master Glide", "27", "2", "0.0", "200.0"),
("Pitch Env Time", "29", "2", "0.0", "200.0"),
("Pitch Env Amt", "30", "0", "-36.0", "36.0"),
("LFO Freq", "31", "2", "10.0", "400.0"),
("LFO Amp", "33", "0", "-24.0", "24.0"),
("LFO Pitch", "34", "0", "-36.0", "36.0"),
("LFO Filter", "35", "0", "-48.0", "48.0")]


MODULEX_PORT_MAP = \
[("FX0 Knob0", "4", "0", "0.0", "127.0"),
("FX0 Knob1", "5", "0", "0.0", "127.0"),
("FX0 Knob2", "6", "0", "0.0", "127.0"),
("FX1 Knob0", "8", "0", "0.0", "127.0"),
("FX1 Knob1", "9", "0", "0.0", "127.0"),
("FX1 Knob2", "10", "0", "0.0", "127.0"),
("FX2 Knob0", "12", "0", "0.0", "127.0"),
("FX2 Knob1", "13", "0", "0.0", "127.0"),
("FX2 Knob2", "14", "0", "0.0", "127.0"),
("FX3 Knob0", "16", "0", "0.0", "127.0"),
("FX3 Knob1", "17", "0", "0.0", "127.0"),
("FX3 Knob2", "18", "0", "0.0", "127.0"),
("FX4 Knob0", "20", "0", "0.0", "127.0"),
("FX4 Knob1", "21", "0", "0.0", "127.0"),
("FX4 Knob2", "22", "0", "0.0", "127.0"),
("FX5 Knob0", "24", "0", "0.0", "127.0"),
("FX5 Knob1", "25", "0", "0.0", "127.0"),
("FX5 Knob2", "26", "0", "0.0", "127.0"),
("FX6 Knob0", "28", "0", "0.0", "127.0"),
("FX6 Knob1", "29", "0", "0.0", "127.0"),
("FX6 Knob2", "30", "0", "0.0", "127.0"),
("FX7 Knob0", "32", "0", "0.0", "127.0"),
("FX7 Knob1", "33", "0", "0.0", "127.0"),
("FX7 Knob2", "34", "0", "0.0", "127.0"),
("Delay Feedback", "37", "0", "-15.0", "0.0"),
("Delay Dry", "38", "0", "-30.0", "0.0"),
("Delay Wet", "39", "0", "-30.0", "0.0"),
("Delay Duck", "40", "0", "-40.0", "0.0"),
("Delay LP Cutoff", "41", "0", "40.0", "118.0"),
("Volume Slider", "43", "0", "-50.0", "0.0"),
("Reverb Wet", "45", "0", "0.0", "100.0"),
("Reverb Color", "46", "0", "0.0", "100.0")]


EUPHORIA_PORT_MAP = \
[("Master Attack", "3", "2", "10.0", "100.0"),
("Master Decay", "4", "2", "10.0", "100.0"),
("Master Sustain", "5", "0", "-60.0", "0.0"),
("Master Release", "6", "2", "10.0", "200.0"),
("ADSR2 Attack", "7", "2", "10.0", "100.0"),
("ADSR2 Decay", "8", "2", "10.0", "100.0"),
("ADSR2 Sustain", "9", "2", "0.0", "100.0"),
("ADSR2 Release", "10", "2", "10.0", "200.0"),
("Noise Amp", "11", "0", "-60.0", "0.0"),
("Master Glide", "13", "2", "0.0", "200.0"),
("Pitch Env Time", "15", "2", "0.0", "200.0"),
("LFO Freq", "16", "2", "10.0", "1600.0"),
("FX0 Knob0", "18", "0", "0.0", "127.0"),
("FX0 Knob1", "19", "0", "0.0", "127.0"),
("FX0 Knob2", "20", "0", "0.0", "127.0"),
("FX1 Knob0", "22", "0", "0.0", "127.0"),
("FX1 Knob1", "23", "0", "0.0", "127.0"),
("FX1 Knob2", "24", "0", "0.0", "127.0"),
("FX2 Knob0", "26", "0", "0.0", "127.0"),
("FX2 Knob1", "27", "0", "0.0", "127.0"),
("FX2 Knob2", "28", "0", "0.0", "127.0"),
("FX3 Knob0", "30", "0", "0.0", "127.0"),
("FX3 Knob1", "31", "0", "0.0", "127.0"),
("FX3 Knob2", "32", "0", "0.0", "127.0"),
("LFO Pitch", "83", "0", "-36.0", "36.0"),
("Mono FX0 Knob0 Group 1", "564", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 2", "565", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 3", "566", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 4", "567", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 5", "568", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 6", "569", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 7", "570", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 8", "571", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 9", "572", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 10", "573", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 11", "574", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 12", "575", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 13", "576", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 14", "577", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 15", "578", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 16", "579", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 17", "580", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 18", "581", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 19", "582", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 20", "583", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 21", "584", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 22", "585", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 23", "586", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 24", "587", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 25", "588", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 26", "589", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 27", "590", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 28", "591", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 29", "592", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 30", "593", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 31", "594", "0", "0.0", "127.0"),
("Mono FX0 Knob0 Group 32", "595", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 1", "596", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 2", "597", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 3", "598", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 4", "599", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 5", "600", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 6", "601", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 7", "602", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 8", "603", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 9", "604", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 10", "605", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 11", "606", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 12", "607", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 13", "608", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 14", "609", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 15", "610", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 16", "611", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 17", "612", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 18", "613", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 19", "614", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 20", "615", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 21", "616", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 22", "617", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 23", "618", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 24", "619", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 25", "620", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 26", "621", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 27", "622", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 28", "623", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 29", "624", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 30", "625", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 31", "626", "0", "0.0", "127.0"),
("Mono FX0 Knob1 Group 32", "627", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 1", "628", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 2", "629", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 3", "630", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 4", "631", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 5", "632", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 6", "633", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 7", "634", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 8", "635", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 9", "636", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 10", "637", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 11", "638", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 12", "639", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 13", "640", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 14", "641", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 15", "642", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 16", "643", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 17", "644", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 18", "645", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 19", "646", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 20", "647", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 21", "648", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 22", "649", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 23", "650", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 24", "651", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 25", "652", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 26", "653", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 27", "654", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 28", "655", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 29", "656", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 30", "657", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 31", "658", "0", "0.0", "127.0"),
("Mono FX0 Knob2 Group 32", "659", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 1", "692", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 2", "693", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 3", "694", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 4", "695", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 5", "696", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 6", "697", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 7", "698", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 8", "699", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 9", "700", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 10", "701", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 11", "702", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 12", "703", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 13", "704", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 14", "705", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 15", "706", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 16", "707", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 17", "708", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 18", "709", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 19", "710", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 20", "711", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 21", "712", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 22", "713", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 23", "714", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 24", "715", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 25", "716", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 26", "717", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 27", "718", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 28", "719", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 29", "720", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 30", "721", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 31", "722", "0", "0.0", "127.0"),
("Mono FX1 Knob0 Group 32", "723", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 1", "724", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 2", "725", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 3", "726", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 4", "727", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 5", "728", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 6", "729", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 7", "730", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 8", "731", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 9", "732", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 10", "733", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 11", "734", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 12", "735", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 13", "736", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 14", "737", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 15", "738", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 16", "739", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 17", "740", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 18", "741", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 19", "742", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 20", "743", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 21", "744", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 22", "745", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 23", "746", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 24", "747", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 25", "748", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 26", "749", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 27", "750", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 28", "751", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 29", "752", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 30", "753", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 31", "754", "0", "0.0", "127.0"),
("Mono FX1 Knob1 Group 32", "755", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 1", "756", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 2", "757", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 3", "758", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 4", "759", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 5", "760", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 6", "761", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 7", "762", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 8", "763", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 9", "764", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 10", "765", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 11", "766", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 12", "767", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 13", "768", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 14", "769", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 15", "770", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 16", "771", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 17", "772", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 18", "773", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 19", "774", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 20", "775", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 21", "776", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 22", "777", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 23", "778", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 24", "779", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 25", "780", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 26", "781", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 27", "782", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 28", "783", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 29", "784", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 30", "785", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 31", "786", "0", "0.0", "127.0"),
("Mono FX1 Knob2 Group 32", "787", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 1", "820", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 2", "821", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 3", "822", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 4", "823", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 5", "824", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 6", "825", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 7", "826", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 8", "827", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 9", "828", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 10", "829", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 11", "830", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 12", "831", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 13", "832", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 14", "833", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 15", "834", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 16", "835", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 17", "836", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 18", "837", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 19", "838", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 20", "839", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 21", "840", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 22", "841", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 23", "842", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 24", "843", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 25", "844", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 26", "845", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 27", "846", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 28", "847", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 29", "848", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 30", "849", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 31", "850", "0", "0.0", "127.0"),
("Mono FX2 Knob0 Group 32", "851", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 1", "852", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 2", "853", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 3", "854", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 4", "855", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 5", "856", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 6", "857", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 7", "858", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 8", "859", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 9", "860", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 10", "861", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 11", "862", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 12", "863", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 13", "864", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 14", "865", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 15", "866", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 16", "867", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 17", "868", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 18", "869", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 19", "870", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 20", "871", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 21", "872", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 22", "873", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 23", "874", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 24", "875", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 25", "876", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 26", "877", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 27", "878", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 28", "879", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 29", "880", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 30", "881", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 31", "882", "0", "0.0", "127.0"),
("Mono FX2 Knob1 Group 32", "883", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 1", "884", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 2", "885", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 3", "886", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 4", "887", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 5", "888", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 6", "889", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 7", "890", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 8", "891", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 9", "892", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 10", "893", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 11", "894", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 12", "895", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 13", "896", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 14", "897", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 15", "898", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 16", "899", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 17", "900", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 18", "901", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 19", "902", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 20", "903", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 21", "904", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 22", "905", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 23", "906", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 24", "907", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 25", "908", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 26", "909", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 27", "910", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 28", "911", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 29", "912", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 30", "913", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 31", "914", "0", "0.0", "127.0"),
("Mono FX2 Knob2 Group 32", "915", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 1", "948", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 2", "949", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 3", "950", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 4", "951", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 5", "952", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 6", "953", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 7", "954", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 8", "955", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 9", "956", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 10", "957", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 11", "958", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 12", "959", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 13", "960", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 14", "961", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 15", "962", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 16", "963", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 17", "964", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 18", "965", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 19", "966", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 20", "967", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 21", "968", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 22", "969", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 23", "970", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 24", "971", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 25", "972", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 26", "973", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 27", "974", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 28", "975", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 29", "976", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 30", "977", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 31", "978", "0", "0.0", "127.0"),
("Mono FX3 Knob0 Group 32", "979", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 1", "980", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 2", "981", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 3", "982", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 4", "983", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 5", "984", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 6", "985", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 7", "986", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 8", "987", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 9", "988", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 10", "989", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 11", "990", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 12", "991", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 13", "992", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 14", "993", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 15", "994", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 16", "995", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 17", "996", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 18", "997", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 19", "998", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 20", "999", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 21", "1000", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 22", "1001", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 23", "1002", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 24", "1003", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 25", "1004", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 26", "1005", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 27", "1006", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 28", "1007", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 29", "1008", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 30", "1009", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 31", "1010", "0", "0.0", "127.0"),
("Mono FX3 Knob1 Group 32", "1011", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 1", "1012", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 2", "1013", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 3", "1014", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 4", "1015", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 5", "1016", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 6", "1017", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 7", "1018", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 8", "1019", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 9", "1020", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 10", "1021", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 11", "1022", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 12", "1023", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 13", "1024", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 14", "1025", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 15", "1026", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 16", "1027", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 17", "1028", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 18", "1029", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 19", "1030", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 20", "1031", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 21", "1032", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 22", "1033", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 23", "1034", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 24", "1035", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 25", "1036", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 26", "1037", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 27", "1038", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 28", "1039", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 29", "1040", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 30", "1041", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 31", "1042", "0", "0.0", "127.0"),
("Mono FX3 Knob2 Group 32", "1043", "0", "0.0", "127.0")]
