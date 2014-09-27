/*
This file is part of the PyDAW project, Copyright PyDAW Team

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/


#ifndef PYDAW_H
#define	PYDAW_H

//Required for sched.h
#ifndef __USE_GNU
#define __USE_GNU
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define PYDAW_LOOP_MODE_OFF 0
#define PYDAW_LOOP_MODE_REGION 1

#define PYDAW_PLAYBACK_MODE_OFF 0
#define PYDAW_PLAYBACK_MODE_PLAY 1
#define PYDAW_PLAYBACK_MODE_REC 2

#define FADE_STATE_OFF 0
#define FADE_STATE_FADING 1
#define FADE_STATE_FADED 2
#define FADE_STATE_RETURNING 3

#define STATUS_NOT_PROCESSED 0
#define STATUS_PROCESSING 1
#define STATUS_PROCESSED 2

#define MAX_PLUGIN_COUNT 10
#define MAX_ROUTING_COUNT 4
#define MAX_PLUGIN_TOTAL_COUNT (MAX_PLUGIN_COUNT + MAX_ROUTING_COUNT)

#define PYDAW_MAX_ITEM_COUNT 5000
#define PYDAW_MAX_REGION_COUNT 300
#define PYDAW_MAX_EVENTS_PER_ITEM_COUNT 1024

#define PYDAW_AUDIO_INPUT_TRACK_COUNT 5
#define PYDAW_TRACK_COUNT_ALL 33

#define PYDAW_MAX_EVENT_BUFFER_SIZE 512  //This could probably be made smaller
#define PYDAW_MAX_REGION_SIZE 64
#define PYDAW_MIDI_NOTE_COUNT 128
#define PYDAW_MIDI_RECORD_BUFFER_LENGTH 600 //recording buffer for MIDI, in bars
#define PYDAW_MAX_WORK_ITEMS_PER_THREAD 128

#define PYDAW_VERSION "musikernel"

#define PYDAW_OSC_SEND_QUEUE_SIZE 256
#define PYDAW_OSC_MAX_MESSAGE_SIZE 65536

#define FRAMES_PER_BUFFER 4096

#define MAX_PLUGIN_POOL_COUNT 1000

#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "pydaw_files.h"
#include "pydaw_plugin_wrapper.h"
#include <sys/stat.h>
#include <sched.h>
#include <unistd.h>
#include <sndfile.h>
#include <time.h>
#include "../libmodsynth/lib/amp.h"
#include "../libmodsynth/lib/lmalloc.h"
#include "../libmodsynth/lib/peak_meter.h"
#include "../libmodsynth/modules/multifx/multifx3knob.h"
#include "../libmodsynth/modules/modulation/ramp_env.h"
#include "pydaw_audio_tracks.h"
#include "pydaw_audio_inputs.h"
#include "pydaw_audio_util.h"
#include "synth.h"
#include <lo/lo.h>
#include "midi_device.h"

#ifdef SCHED_DEADLINE

#define RT_SCHED SCHED_DEADLINE

#else

#define RT_SCHED SCHED_FIFO

#endif

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct
{
    int output_track;
    int on;
}t_pydaw_midi_routing;

typedef struct
{
    t_pydaw_midi_routing routes[32];
}t_pydaw_midi_routing_list;

typedef struct
{
    t_pydaw_seq_event * events[PYDAW_MAX_EVENTS_PER_ITEM_COUNT];
    int event_count;
    //Used to avoid reading the same event twice in an unsorted item.
    int rec_event_count;
    int uid;
}t_pyitem;

typedef struct
{
    //Refers to the index of items in the master item pool
    int item_indexes[PYDAW_TRACK_COUNT_ALL][PYDAW_MAX_REGION_SIZE];
    int uid;
    /*This flag is set to 1 if created during recording, signifying
     * that it requires a default name to be created for it*/
    int not_yet_saved;
    int region_length_bars;    //0 to use pydaw_data default
    int region_length_beats;    //0 to use pydaw_data default
    int bar_length;  //0 to use pydaw_data default
    int alternate_tempo;  //0 or 1, used as a boolean
    float tempo;
}t_pyregion;

typedef struct
{
    int track;
    int bar;
    float beat;
    int port;
    float val;
    int index;
    int plugin;
}t_pydaw_atm_point;

typedef struct
{
    t_pydaw_atm_point * points;
    int point_count;
}t_pydaw_atm_plugin;

typedef struct
{
    t_pydaw_atm_plugin plugins[MAX_PLUGIN_TOTAL_COUNT];
}t_pydaw_atm_track;

typedef struct
{
    t_pydaw_atm_track tracks[PYDAW_TRACK_COUNT_ALL];
}t_pydaw_atm_region;

typedef struct
{
    float a_knobs[3];
    int fx_type;
    fp_mf3_run func_ptr;
    t_mf3_multi * mf3;
}t_pydaw_per_audio_item_fx_item;

typedef struct
{
    int loaded[PYDAW_MAX_AUDIO_ITEM_COUNT];
    t_pydaw_per_audio_item_fx_item * items[PYDAW_MAX_AUDIO_ITEM_COUNT][8];
}t_pydaw_per_audio_item_fx_region;

typedef struct
{
    t_pyregion * regions[PYDAW_MAX_REGION_COUNT];
    t_pydaw_audio_items * audio_items[PYDAW_MAX_REGION_COUNT];
    t_pydaw_atm_region * regions_atm[PYDAW_MAX_REGION_COUNT];
    t_pydaw_per_audio_item_fx_region
            *per_audio_item_fx[PYDAW_MAX_REGION_COUNT];
    int default_bar_length;
}t_pysong;

typedef struct
{
    int output;
    int active;
}
t_pytrack_routing;

typedef struct
{
    int track_pool_sorted[PYDAW_TRACK_COUNT_ALL];
    int track_pool_sorted_count;
    t_pytrack_routing routes[PYDAW_TRACK_COUNT_ALL][MAX_ROUTING_COUNT];
}t_pydaw_routing_graph;

typedef struct
{
    int status __attribute__((aligned(16)));
    int solo;
    int mute;
    t_pydaw_seq_event * event_buffer;
    int period_event_index;
    t_pydaw_plugin * plugins[MAX_PLUGIN_TOTAL_COUNT];
    int track_num;
    //Only for busses, the count of plugins writing to the buffer
    int bus_count;
    /*This is reset to bus_count each cycle and the
     * bus track processed when count reaches 0*/
    int bus_counter __attribute__((aligned(16)));
    t_pkm_peak_meter * peak_meter;
    float ** buffers;
    int channels;
    pthread_spinlock_t lock;
    t_ramp_env * fade_env;
    int fade_state;
    /*When a note_on event is fired,
     * a sample number of when to release it is stored here*/
    long note_offs[PYDAW_MIDI_NOTE_COUNT];
    int item_event_index;
    char * osc_cursor_message;
    int * extern_midi_count;
    t_pydaw_seq_event * extern_midi;
    t_midi_device * midi_device;
}t_pytrack;

float MASTER_VOL __attribute__((aligned(16))) = 1.0f;

typedef struct
{
    //set from the audio device buffer size every time the main loop is called.
    int sample_count;
    float tempo;
    pthread_spinlock_t main_lock;
    t_pysong * pysong;
    //contains a reference to all track types, in order:  MIDI, Bus, Audio
    t_pytrack * track_pool_all[PYDAW_TRACK_COUNT_ALL];
    t_pydaw_routing_graph * routing_graph;
    t_pyaudio_input * audio_inputs[PYDAW_AUDIO_INPUT_TRACK_COUNT];
    int playback_mode;  //0 == Stop, 1 == Play, 2 == Rec
    int loop_mode;  //0 == Off, 1 == Bar, 2 == Region
    int overdub_mode;  //0 == Off, 1 == On
    //char * project_name;
    char * project_folder;
    char * item_folder;
    char * region_folder;
    char * region_audio_folder;
    char * region_atm_folder;
    char * audio_folder;
    char * audio_tmp_folder;
    char * samples_folder;
    char * samplegraph_folder;
    char * wav_pool_file;
    char * plugins_folder;
    char * tracks_folder;

    //only refers to the fractional position within the current bar.
    float playback_cursor;
    /*the increment per-period to iterate through 1 bar,
     * as determined by sample rate and tempo*/
    float playback_inc;
    int current_region; //the current region
    int current_bar; //the current bar(0 to 7), within the current region
    //int samples_per_bar;
    float sample_rate;
    /*The sample number of the exact point in the song,
     * 0 == bar0/region0, 44100 == 1 second in at 44.1khz.*/
    long current_sample;
    lo_server_thread serverThread;

    char * osc_url;
    //The number of samples per beat, for calculating length
    float samples_per_beat;

    t_pyitem * item_pool[PYDAW_MAX_ITEM_COUNT];

    int is_soloed;

    //For broadcasting to the threads that it's time to process the tracks
    pthread_cond_t * track_cond;
    //For preventing the main thread from continuing until the workers finish
    pthread_mutex_t * track_block_mutexes;
    pthread_t * track_worker_threads;
    int track_worker_thread_count;
    int * track_thread_quit_notifier;
    int * track_thread_is_finished;

    /*Used to artificially inject sleep into the loop to
     * prevent a race condition*/
    int is_offline_rendering;

    int default_region_length_bars;
    int default_region_length_beats;
    int default_bar_length;

    //Main-loop variables, prefixed with ml_
    float ml_sample_period_inc;
    float ml_sample_period_inc_beats;
    float ml_next_playback_cursor;
    float ml_current_period_beats;
    float ml_next_period_beats;
    //New additions, to eventually replace some of the older variables
    int ml_current_region;
    int ml_current_bar;
    float ml_current_beat;
    int ml_next_region;
    int ml_next_bar;
    float ml_next_beat;
    //1 if a new bar starts in this sample period, 0 otherwise
    int ml_starting_new_bar;
    /*0 if false, or 1 if the next bar loops.  Consumers of
     * this value should check for the ->loop_mode variable..*/
    int ml_is_looping;

    float ** input_buffers;
    int input_buffers_active;
    pthread_mutex_t audio_inputs_mutex;

    pthread_t audio_recording_thread;
    int audio_recording_quit_notifier __attribute__((aligned(16)));

    /*used to prevent new audio items from playing while
     * the existing are being faded out.*/
    int suppress_new_audio_items;
    t_wav_pool * wav_pool;
    t_wav_pool_item * ab_wav_item;
    t_pydaw_audio_item * ab_audio_item;
    int ab_mode;  //0 == off, 1 == on
    int is_ab_ing;  //Set this to self->ab_mode on playback
    t_wav_pool_item * preview_wav_item;
    t_pydaw_audio_item * preview_audio_item;
    float preview_start; //0.0f to 1.0f
    int is_previewing;  //Set this to self->ab_mode on playback
    float preview_amp_lin;
    int preview_max_sample_count;
    char * per_audio_item_fx_folder;
    lo_address uiTarget;
    char * osc_cursor_message;
    void * main_thread_args;
    int audio_glue_indexes[PYDAW_MAX_AUDIO_ITEM_COUNT];
    int midi_learn;  //0 to disable, 1 to enable sending CC events to the UI
    int osc_queue_index;
    char osc_queue_keys[PYDAW_OSC_SEND_QUEUE_SIZE][12];
    char osc_queue_vals[PYDAW_OSC_SEND_QUEUE_SIZE][PYDAW_OSC_MAX_MESSAGE_SIZE];
    pthread_t osc_queue_thread;
    int f_region_length_bars;
    long f_next_current_sample;
    //Threads must hold this to write OSC messages
    pthread_spinlock_t ui_spinlock;
    int wave_editor_cursor_count;
    t_pydaw_plugin * plugin_pool[MAX_PLUGIN_POOL_COUNT];
    t_midi_device_list * midi_devices;
    t_pydaw_midi_routing_list midi_routing;
}t_pydaw_data;

typedef struct
{
    t_pydaw_data * pydaw_data;
    int thread_num;
}t_pydaw_thread_args;

void g_pysong_get(t_pydaw_data*, int);
t_pytrack * g_pytrack_get(int, float);
t_pytrack_routing * g_pytrack_routing_get();
t_pydaw_routing_graph * g_pydaw_routing_graph_get(t_pydaw_data *);
void v_pytrack_routing_graph_free(t_pydaw_routing_graph*);
void v_pytrack_routing_set(t_pytrack_routing *, int);
void v_pytrack_routing_free(t_pytrack_routing *);
t_pyregion * g_pyregion_get(t_pydaw_data*, const int);
t_pydaw_atm_region * g_atm_region_get(t_pydaw_data*, int);
void v_atm_region_free(t_pydaw_atm_region*);
void g_pyitem_get(t_pydaw_data*, int);

t_pydaw_seq_event * g_pycc_get(int, float, float);
t_pydaw_seq_event * g_pypitchbend_get(float a_start, float a_value);
t_pydaw_seq_event * g_pynote_get(int a_note, int a_vel, float a_start,
                                 float a_length);
t_pydaw_data * g_pydaw_data_get(float, t_midi_device_list*);
int i_get_region_index_from_name(t_pydaw_data *, int);
void v_open_project(t_pydaw_data*, const char*, int);
void v_set_tempo(t_pydaw_data*,float);
void v_pydaw_set_is_soloed(t_pydaw_data * self);
void v_set_loop_mode(t_pydaw_data * self, int a_mode);
void v_set_playback_cursor(t_pydaw_data * self, int a_region,
                           int a_bar);
void v_pydaw_assert_memory_integrity(t_pydaw_data* self);
int i_get_song_index_from_region_uid(t_pydaw_data*, int);
void v_save_pysong_to_disk(t_pydaw_data * self);
void v_save_pyitem_to_disk(t_pydaw_data * self, int a_index);
void v_save_pyregion_to_disk(t_pydaw_data * self, int a_region_num);
void v_pydaw_set_plugin_index(t_pydaw_data * self, int a_track_num,
        int a_index, int a_plugin_index, int a_plugin_uid,
        int a_power, int a_lock);
void v_pydaw_update_track_send(t_pydaw_data * self, int a_lock);
inline void v_pydaw_update_ports(t_pydaw_plugin * a_plugin);
void * v_pydaw_worker_thread(void*);
void v_pydaw_init_worker_threads(t_pydaw_data*, int, int);
inline void v_pydaw_process_external_midi(t_pydaw_data * pydaw_data,
        t_pytrack * a_track, int sample_count);
inline void v_pydaw_run_main_loop(t_pydaw_data * pydaw_data, int sample_count,
        long f_next_current_sample, PYFX_Data *output0, PYFX_Data *output1,
        PYFX_Data **a_input_buffers);
void v_pydaw_offline_render(t_pydaw_data * self, int a_start_region,
        int a_start_bar, int a_end_region, int a_end_bar, char * a_file_out,
        int a_is_audio_glue);
void v_pydaw_print_benchmark(char * a_message, clock_t a_start);
inline void v_pydaw_audio_items_run(t_pydaw_data * self,
        int a_sample_count, float* a_output0,
        float* a_output1, int a_audio_track_num, int a_is_audio_glue);
void v_pydaw_update_audio_inputs(t_pydaw_data * self);
void * v_pydaw_audio_recording_thread(void* a_arg);
inline float v_pydaw_count_beats(t_pydaw_data * self,
        int a_start_region, int a_start_bar, float a_start_beat,
        int a_end_region, int a_end_bar, float a_end_beat);
t_pydaw_audio_items * v_audio_items_load_all(t_pydaw_data * self,
        int a_region_uid);

void v_pydaw_set_ab_mode(t_pydaw_data * self, int a_mode);
void v_pydaw_set_ab_file(t_pydaw_data * self, const char * a_file);
void v_pydaw_set_wave_editor_item(t_pydaw_data * self,
        const char * a_string);

t_pydaw_per_audio_item_fx_region * g_paif_region_get();
void v_paif_region_free(t_pydaw_per_audio_item_fx_region*);
t_pydaw_per_audio_item_fx_region * g_paif_region_open(t_pydaw_data*, int);
t_pydaw_per_audio_item_fx_item * g_paif_item_get(t_pydaw_data *);
void v_paif_set_control(t_pydaw_data *, int, int, int, float);

void v_pysong_free(t_pysong *);
inline void v_pydaw_process_note_offs(t_pydaw_data * self, int f_i);
inline void v_pydaw_process_midi(t_pydaw_data * self,
        int f_i, int sample_count);
void v_pydaw_zero_all_buffers(t_pydaw_data * self);
inline void v_pydaw_zero_buffer(float**, int);
void v_pydaw_panic(t_pydaw_data * self);
inline void v_queue_osc_message(char * a_key, char * a_val);

void v_pydaw_process_atm(
    t_pydaw_data * self, int f_track_num, int f_index, int sample_count);

void v_pydaw_set_midi_device(t_pydaw_data*, int, int, int);

#ifdef	__cplusplus
}
#endif

int ZERO = 0;
t_pydaw_data * pydaw_data;


inline float f_bpm_to_seconds_per_beat(float a_tempo)
{
    return (60.0f / a_tempo);
}

inline int i_pydaw_beat_count_to_samples(float a_beat_count, float a_tempo,
        float a_sr)
{
    float f_seconds = f_bpm_to_seconds_per_beat(a_tempo) * a_beat_count;
    return (int)(f_seconds * a_sr);
}

inline float f_pydaw_samples_to_beat_count(int a_sample_count, float a_tempo,
        float a_sr)
{
    float f_seconds_per_beat = f_bpm_to_seconds_per_beat(a_tempo);
    float f_seconds = (float)(a_sample_count) / a_sr;
    return f_seconds / f_seconds_per_beat;
}

void v_pydaw_reset_audio_item_read_heads(t_pydaw_data * self,
        int a_region, int a_start_bar)
{
    if(a_start_bar == 0)
    {
        return;  //no need to run because the audio loop will reset it all
    }

    if(!self->pysong->audio_items[a_region])
    {
        return;
    }

    t_pydaw_audio_items * f_audio_items = self->
            pysong->audio_items[a_region];

    int f_i = 0;
    float f_start_beats = (float)(a_start_bar * 4);

    while(f_i < PYDAW_MAX_AUDIO_ITEM_COUNT)
    {
        if(f_audio_items->items[f_i])
        {
            float f_start_beat =
                (float)(f_audio_items->items[f_i]->start_bar * 4) +
                f_audio_items->items[f_i]->start_beat;

            float f_end_beat =
                f_start_beat + f_pydaw_samples_to_beat_count(
                    (f_audio_items->items[f_i]->sample_end_offset -
                     f_audio_items->items[f_i]->sample_start_offset),
                    self->tempo,
                    self->sample_rate);

            if((f_start_beats > f_start_beat) && (f_start_beats < f_end_beat))
            {
                float f_beats_offset = (f_start_beats - f_start_beat);
                int f_sample_start = i_pydaw_beat_count_to_samples(
                        f_beats_offset,
                        self->tempo,
                        self->sample_rate);

                v_ifh_retrigger(
                        f_audio_items->items[f_i]->sample_read_head,
                        f_sample_start);

                v_adsr_retrigger(f_audio_items->items[f_i]->adsr);
            }
        }
        f_i++;
    }
}

/* void v_pydaw_zero_all_buffers(t_pydaw_data * self)
 */
void v_pydaw_zero_all_buffers(t_pydaw_data * self)
{
    int f_i = 0;
    float ** f_buff;
    while(f_i < PYDAW_TRACK_COUNT_ALL)
    {
        f_buff = self->track_pool_all[f_i]->buffers;
        v_pydaw_zero_buffer(f_buff, FRAMES_PER_BUFFER);
        f_i++;
    }
}

inline void v_pydaw_zero_buffer(float ** a_buffers, int a_count)
{
    int f_i2 = 0;

    while(f_i2 < a_count)
    {
        a_buffers[0][f_i2] = 0.0f;
        a_buffers[1][f_i2] = 0.0f;
        f_i2++;
    }
}

/* void v_pydaw_panic(t_pydaw_data * self)
 *
 */
void v_pydaw_panic(t_pydaw_data * self)
{
    int f_i = 0;
    int f_i2 = 0;
    t_pytrack * f_track;
    t_pydaw_plugin * f_plugin;

    while(f_i < PYDAW_TRACK_COUNT_ALL)
    {
        f_track = self->track_pool_all[f_i];

        f_i2 = 0;
        while(f_i2 < MAX_PLUGIN_TOTAL_COUNT)
        {
            f_plugin = f_track->plugins[f_i2];
            if(f_plugin && f_plugin->descriptor->panic)
            {
                f_plugin->descriptor->panic(f_plugin->PYFX_handle);
            }
            f_i2++;
        }

        f_i++;
    }

    v_pydaw_zero_all_buffers(self);
}

/*Function for passing to plugins that re-use the wav pool*/
t_wav_pool_item * g_pydaw_wavpool_item_get(int a_uid)
{
    return g_wav_pool_get_item_by_uid(pydaw_data->wav_pool, a_uid);
}

void v_pysong_free(t_pysong * a_pysong)
{
    int f_i = 0;
    while(f_i < PYDAW_MAX_REGION_COUNT)
    {
        if(a_pysong->audio_items[f_i])
        {
            v_pydaw_audio_items_free(a_pysong->audio_items[f_i]);
        }

        if(a_pysong->per_audio_item_fx[f_i])
        {
            v_paif_region_free(a_pysong->per_audio_item_fx[f_i]);
        }

        if(a_pysong->regions[f_i])
        {
            free(a_pysong->regions[f_i]);
        }

        f_i++;
    }
}

t_pydaw_per_audio_item_fx_region * g_paif_region_get()
{
    t_pydaw_per_audio_item_fx_region * f_result =
            (t_pydaw_per_audio_item_fx_region*)malloc(
            sizeof(t_pydaw_per_audio_item_fx_region));

    int f_i = 0;

    while(f_i < PYDAW_MAX_AUDIO_ITEM_COUNT)
    {
        f_result->loaded[f_i] = 0;
        int f_i2 = 0;
        while(f_i2 < 8)
        {
            f_result->items[f_i][f_i2] = 0;
            f_i2++;
        }
        f_i++;
    }

    return f_result;
}

void v_paif_region_free(t_pydaw_per_audio_item_fx_region * a_paif)
{
    int f_i = 0;
    while(f_i < PYDAW_MAX_AUDIO_ITEM_COUNT)
    {
        int f_i2 = 0;
        while(f_i2 < 8)
        {
            if(a_paif->items[f_i][f_i2])
            {
                v_mf3_free(a_paif->items[f_i][f_i2]->mf3);
                free(a_paif->items[f_i][f_i2]);
                a_paif->items[f_i][f_i2] = 0;
            }
            f_i2++;
        }
        f_i++;
    }
    free(a_paif);
}

/* t_pydaw_per_audio_item_fx_region *
 * g_paif_region_open(t_pydaw_data * self, int a_region_uid) */
t_pydaw_per_audio_item_fx_region * g_paif_region_open(
        t_pydaw_data * self, int a_region_uid)
{
    t_pydaw_per_audio_item_fx_region * f_result = g_paif_region_get();

    int f_i = 0;
    char f_temp[1024];
    sprintf(f_temp, "%s%i", self->per_audio_item_fx_folder,
            a_region_uid);
    if(i_pydaw_file_exists(f_temp))
    {
        t_2d_char_array * f_current_string =
                g_get_2d_array_from_file(f_temp, PYDAW_LARGE_STRING);
        while(f_i < PYDAW_MAX_AUDIO_ITEM_COUNT)
        {
            char * f_index_char = c_iterate_2d_char_array(f_current_string);
            if(f_current_string->eof)
            {
                free(f_index_char);
                break;
            }
            int f_index = atoi(f_index_char);
            free(f_index_char);

            f_result->loaded[f_index] = 1;

            int f_i2 = 0;

            while(f_i2 < 8)
            {
                f_result->items[f_index][f_i2] = g_paif_item_get(self);
                int f_i3 = 0;
                while(f_i3 < 3)
                {
                    char * f_knob_char =
                        c_iterate_2d_char_array(f_current_string);
                    float f_knob_val = atof(f_knob_char);
                    free(f_knob_char);
                    f_result->items[f_index][f_i2]->a_knobs[f_i3] = f_knob_val;
                    f_i3++;
                }
                char * f_type_char = c_iterate_2d_char_array(f_current_string);
                int f_type_val = atoi(f_type_char);
                free(f_type_char);
                f_result->items[f_index][f_i2]->fx_type = f_type_val;
                f_result->items[f_index][f_i2]->func_ptr =
                        g_mf3_get_function_pointer(f_type_val);
                v_mf3_set(f_result->items[f_index][f_i2]->mf3,
                        f_result->items[f_index][f_i2]->a_knobs[0],
                        f_result->items[f_index][f_i2]->a_knobs[1],
                        f_result->items[f_index][f_i2]->a_knobs[2]);
                f_i2++;
            }

            f_i++;
        }

        g_free_2d_char_array(f_current_string);
    }

    return f_result;
}

t_pydaw_per_audio_item_fx_item * g_paif_item_get(t_pydaw_data * self)
{
    t_pydaw_per_audio_item_fx_item * f_result =
            (t_pydaw_per_audio_item_fx_item*)malloc(
                sizeof(t_pydaw_per_audio_item_fx_item));

    int f_i = 0;
    while(f_i < 3)
    {
        f_result->a_knobs[f_i] = 64.0f;
        f_i++;
    }
    f_result->fx_type = 0;
    f_result->func_ptr = v_mf3_run_off;
    f_result->mf3 = g_mf3_get(self->sample_rate);

    return f_result;
}

void v_paif_set_control(t_pydaw_data * self, int a_region_uid,
        int a_item_index, int a_port, float a_val)
{
    int f_effect_index = a_port / 4;
    int f_control_index = a_port % 4;
    int f_song_index = i_get_song_index_from_region_uid(
        self, a_region_uid);

    if(!self->pysong->per_audio_item_fx[f_song_index])
    {
        t_pydaw_per_audio_item_fx_region * f_region = g_paif_region_get();

        pthread_spin_lock(&self->main_lock);
        self->pysong->per_audio_item_fx[f_song_index] = f_region;
        pthread_spin_unlock(&self->main_lock);
    }

    if(!self->pysong->
            per_audio_item_fx[f_song_index]->loaded[a_item_index])
    {
        t_pydaw_per_audio_item_fx_item * f_items[8];
        int f_i = 0;
        while(f_i < 8)
        {
            f_items[f_i] = g_paif_item_get(self);
            f_i++;
        }
        pthread_spin_lock(&self->main_lock);
        f_i = 0;
        while(f_i < 8)
        {
            self->pysong->per_audio_item_fx[f_song_index]->
                    items[a_item_index][f_i] = f_items[f_i];
            f_i++;
        }
        self->pysong->per_audio_item_fx[f_song_index]->
                loaded[a_item_index] = 1;
        pthread_spin_unlock(&self->main_lock);
    }

    pthread_spin_lock(&self->main_lock);

    if(f_control_index == 3)
    {
        int f_fx_index = (int)a_val;
        self->pysong->per_audio_item_fx[f_song_index]->
                items[a_item_index][f_effect_index]->fx_type = f_fx_index;
        self->pysong->per_audio_item_fx[f_song_index]->
                items[a_item_index][f_effect_index]->func_ptr =
                        g_mf3_get_function_pointer(f_fx_index);

        v_mf3_set(self->pysong->per_audio_item_fx[f_song_index]->
                items[a_item_index][f_effect_index]->mf3,
            self->pysong->per_audio_item_fx[f_song_index]->
                items[a_item_index][f_effect_index]->a_knobs[0],
            self->pysong->per_audio_item_fx[f_song_index]->
                items[a_item_index][f_effect_index]->a_knobs[1],
            self->pysong->per_audio_item_fx[f_song_index]->
                items[a_item_index][f_effect_index]->a_knobs[2]);
    }
    else
    {
        self->pysong->per_audio_item_fx[f_song_index]->
                items[a_item_index][f_effect_index]->
                        a_knobs[f_control_index] = a_val;

        v_mf3_set(self->pysong->per_audio_item_fx[f_song_index]->
                items[a_item_index][f_effect_index]->mf3,
            self->pysong->per_audio_item_fx[f_song_index]->
                items[a_item_index][f_effect_index]->a_knobs[0],
            self->pysong->per_audio_item_fx[f_song_index]->
                items[a_item_index][f_effect_index]->a_knobs[1],
            self->pysong->per_audio_item_fx[f_song_index]->
                items[a_item_index][f_effect_index]->a_knobs[2]);
    }

    pthread_spin_unlock(&self->main_lock);

}

/* Create a clock_t with clock() when beginning some work,
 * and use this function to print the completion time*/
void v_pydaw_print_benchmark(char * a_message, clock_t a_start)
{
    printf ( "\n\nCompleted %s in %f seconds\n", a_message,
            ( (double)clock() - a_start ) / CLOCKS_PER_SEC );
}


void * v_pydaw_osc_send_thread(void* a_arg)
{
    t_pydaw_data * self = (t_pydaw_data*)a_arg;

    int f_i = 0;

    char osc_queue_keys[PYDAW_OSC_SEND_QUEUE_SIZE][12];
    char * osc_queue_vals[PYDAW_OSC_SEND_QUEUE_SIZE];

    while(f_i < PYDAW_OSC_SEND_QUEUE_SIZE)
    {
        buffer_alloc((void**)&osc_queue_vals[f_i],
            sizeof(char) * PYDAW_OSC_MAX_MESSAGE_SIZE);
        f_i++;
    }

    char * f_tmp1 = NULL;
    buffer_alloc((void**)&f_tmp1, sizeof(char) * PYDAW_OSC_MAX_MESSAGE_SIZE);
    char * f_tmp2 = NULL;
    buffer_alloc((void**)&f_tmp2, sizeof(char) * PYDAW_OSC_MAX_MESSAGE_SIZE);
    char * f_msg = NULL;
    buffer_alloc((void**)&f_msg, sizeof(char) * PYDAW_OSC_MAX_MESSAGE_SIZE);

    f_tmp1[0] = '\0';
    f_tmp2[0] = '\0';
    f_msg[0] = '\0';

    int f_track_index;
    t_pkm_peak_meter * f_pkm;

    while(!self->audio_recording_quit_notifier)
    {
        f_i = 0;

        f_tmp1[0] = '\0';
        f_tmp2[0] = '\0';

        f_pkm = self->track_pool_all[0]->peak_meter;
        sprintf(f_tmp2, "%i:%f:%f", 0, f_pkm->value[0], f_pkm->value[1]);
        v_pkm_reset(f_pkm);

        while(f_i < self->routing_graph->track_pool_sorted_count)
        {
            f_track_index = self->routing_graph->track_pool_sorted[f_i];
            f_pkm = self->track_pool_all[f_track_index]->peak_meter;

            sprintf(f_tmp1, "|%i:%f:%f",
                f_track_index, f_pkm->value[0], f_pkm->value[1]);

            v_pkm_reset(f_pkm);

            strcat(f_tmp2, f_tmp1);

            f_i++;
        }

        v_queue_osc_message("peak", f_tmp2);

        f_tmp1[0] = '\0';
        f_tmp2[0] = '\0';

        if(self->playback_mode > 0)
        {
            if(self->is_ab_ing)
            {
                float f_frac =
                (float)(self->ab_audio_item->sample_read_head->whole_number) /
                (float)(self->ab_audio_item->wav_pool_item->length);

                sprintf(f_msg, "%f", f_frac);
                v_queue_osc_message("wec", f_msg);
            }
            else
            {
                if(!self->is_offline_rendering)
                {
                    sprintf(f_msg, "%i|%i|%f", self->ml_next_region,
                        self->ml_next_bar, self->ml_next_beat);
                    v_queue_osc_message("cur", f_msg);
                }
            }
        }

        if(self->osc_queue_index > 0)
        {
            f_i = 0;

            while(f_i < self->osc_queue_index)
            {
                strcpy(osc_queue_keys[f_i], self->osc_queue_keys[f_i]);
                strcpy(osc_queue_vals[f_i], self->osc_queue_vals[f_i]);
                f_i++;
            }

            pthread_spin_lock(&self->main_lock);

            //Now grab any that may have been written since the previous copy

            while(f_i < self->osc_queue_index)
            {
                strcpy(osc_queue_keys[f_i], self->osc_queue_keys[f_i]);
                strcpy(osc_queue_vals[f_i], self->osc_queue_vals[f_i]);
                f_i++;
            }

            int f_index = self->osc_queue_index;
            self->osc_queue_index = 0;

            pthread_spin_unlock(&self->main_lock);

            f_i = 0;

            f_tmp1[0] = '\0';

            while(f_i < f_index)
            {
                sprintf(f_tmp2, "%s|%s\n", osc_queue_keys[f_i],
                        osc_queue_vals[f_i]);
                strcat(f_tmp1, f_tmp2);
                f_i++;
            }

            if(!self->is_offline_rendering)
            {
                lo_send(self->uiTarget, "musikernel/ui_configure", "s", f_tmp1);
            }
        }

        usleep(20000);
    }

    printf("osc send thread exiting\n");

    return (void*)1;
}

#if defined(__amd64__) || defined(__i386__)
void cpuID(unsigned int i, unsigned int regs[4])
{
    asm volatile
      ("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
       : "a" (i), "c" (0));
    // ECX is set to zero for CPUID function 4
} __attribute__((optimize("-O0")))

char * uint_to_char(unsigned int a_input)
{
    char* bytes = (char*)malloc(sizeof(char) * 5);

    bytes[0] = a_input & 0xFF;
    bytes[1] = (a_input >> 8) & 0xFF;
    bytes[2] = (a_input >> 16) & 0xFF;
    bytes[3] = (a_input >> 24) & 0xFF;
    bytes[4] = '\0';

    return bytes;
} __attribute__((optimize("-O0")))

int i_cpu_has_hyperthreading()
{
    unsigned int regs[4];

    // Get vendor
    cpuID(0, regs);

    char cpuVendor[12];
    sprintf(cpuVendor, "%s%s%s", uint_to_char(regs[1]), uint_to_char(regs[3]),
            uint_to_char(regs[2]));

    // Get CPU features
    cpuID(1, regs);
    unsigned cpuFeatures = regs[3]; // EDX

    // Logical core count per CPU
    cpuID(1, regs);
    unsigned logical = (regs[1] >> 16) & 0xff; // EBX[23:16]
    unsigned cores = logical;

    if(!strcmp(cpuVendor, "GenuineIntel"))
    {
        printf("\nDetected Intel CPU, checking for hyperthreading.\n");
        // Get DCP cache info
        cpuID(4, regs);
        cores = ((regs[0] >> 26) & 0x3f) + 1; // EAX[31:26] + 1
        // Detect hyper-threads
        int hyperThreads = cpuFeatures & (1 << 28) && cores < logical;
        return hyperThreads;

    }
    /*else if(!strcmp(cpuVendor, "AuthenticAMD"))
    {
        return 0;
      // Get NC: Number of CPU cores - 1
      //cpuID(0x80000008, regs);
      //cores = ((unsigned)(regs[2] & 0xff)) + 1; // ECX[7:0] + 1
    }*/
    else
    {
        printf("Detected CPU vendor %s , assuming no hyper-threading.\n",
                cpuVendor);
        return 0;
    }
} __attribute__((optimize("-O0")))
#else
int i_cpu_has_hyperthreading()
{
    return 0;
}
#endif

void v_self_set_thread_affinity()
{
    pthread_attr_t threadAttr;
    struct sched_param param;
    param.__sched_priority = sched_get_priority_max(RT_SCHED);
    printf(" Attempting to set pthread_self to .__sched_priority = %i\n",
            param.__sched_priority);
    pthread_attr_init(&threadAttr);
    pthread_attr_setschedparam(&threadAttr, &param);
    pthread_attr_setstacksize(&threadAttr, 8388608);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setschedpolicy(&threadAttr, RT_SCHED);

    pthread_t f_self = pthread_self();
    pthread_setschedparam(f_self, RT_SCHED, &param);
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    pthread_setaffinity_np(f_self, sizeof(cpu_set_t), &cpuset);

    pthread_attr_destroy(&threadAttr);
}

void v_pydaw_init_worker_threads(t_pydaw_data * self,
        int a_thread_count, int a_set_thread_affinity)
{
    int f_cpu_count = sysconf( _SC_NPROCESSORS_ONLN );
    int f_cpu_core_inc = 1;
    int f_has_ht = i_cpu_has_hyperthreading();

    if(f_has_ht)
    {
        printf("\n\n#####################################################\n");
        printf("Detected Intel hyperthreading, dividing logical"
                " core count by 2.\n");
        printf("You should consider turning off hyperthreading in "
                "your PC's BIOS for best performance.\n");
        printf("#########################################################\n\n");
        f_cpu_count /= 2;
        f_cpu_core_inc = 2;
    }

    if(a_thread_count == 0)
    {
        self->track_worker_thread_count = f_cpu_count;

        if((self->track_worker_thread_count) > 4)
        {
            self->track_worker_thread_count = 4;
        }
        else if((self->track_worker_thread_count) == 4)
        {
            self->track_worker_thread_count = 3;
        }
        else if((self->track_worker_thread_count) <= 0)
        {
            self->track_worker_thread_count = 1;
        }
    }
    else
    {
        self->track_worker_thread_count = a_thread_count;
    }

    if(!f_has_ht &&
            ((self->track_worker_thread_count * 2) <= f_cpu_count))
    {
        f_cpu_core_inc = f_cpu_count /
                (self->track_worker_thread_count);

        if(f_cpu_core_inc < 2)
        {
            f_cpu_core_inc = 2;
        }
        else if(f_cpu_core_inc > 4)
        {
            f_cpu_core_inc = 4;
        }
    }

    printf("Spawning %i worker threads\n",
            self->track_worker_thread_count);

    self->track_block_mutexes =
            (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t) *
                (self->track_worker_thread_count));
    self->track_worker_threads =
            (pthread_t*)malloc(sizeof(pthread_t) *
                (self->track_worker_thread_count));

    lmalloc((void**)&self->track_thread_quit_notifier,
            (sizeof(int) * (self->track_worker_thread_count)));
    lmalloc((void**)&self->track_thread_is_finished,
            (sizeof(int) * (self->track_worker_thread_count)));

    self->track_cond =
            (pthread_cond_t*)malloc(sizeof(pthread_cond_t) *
                (self->track_worker_thread_count));

    pthread_attr_t threadAttr;
    struct sched_param param;
    param.__sched_priority = sched_get_priority_max(RT_SCHED); //90;
    printf(" Attempting to set .__sched_priority = %i\n",
            param.__sched_priority);
    pthread_attr_init(&threadAttr);
    pthread_attr_setschedparam(&threadAttr, &param);
    pthread_attr_setstacksize(&threadAttr, 8388608);
    pthread_attr_setdetachstate(&threadAttr, PTHREAD_CREATE_DETACHED);
    pthread_attr_setschedpolicy(&threadAttr, RT_SCHED);

    //pthread_t f_self = pthread_self();
    //pthread_setschedparam(f_self, RT_SCHED, &param);

    int f_cpu_core = 0;

    if(a_set_thread_affinity)
    {
        f_cpu_core += f_cpu_core_inc;

        if(f_cpu_core >= f_cpu_count)
        {
            f_cpu_core = 0;
        }
    }



    int f_i = 0;

    while(f_i < (self->track_worker_thread_count))
    {
        self->track_thread_is_finished[f_i] = 0;
        self->track_thread_quit_notifier[f_i] = 0;
        t_pydaw_thread_args * f_args =
                (t_pydaw_thread_args*)malloc(sizeof(t_pydaw_thread_args));
        f_args->pydaw_data = self;
        f_args->thread_num = f_i;

        if(f_i > 0)
        {
            //pthread_mutex_init(&self->track_cond_mutex[f_i], NULL);
            pthread_cond_init(&self->track_cond[f_i], NULL);
            pthread_mutex_init(&self->track_block_mutexes[f_i], NULL);
            pthread_create(&self->track_worker_threads[f_i],
                    &threadAttr, v_pydaw_worker_thread, (void*)f_args);

            if(a_set_thread_affinity)
            {
                cpu_set_t cpuset;
                CPU_ZERO(&cpuset);
                CPU_SET(f_cpu_core, &cpuset);
                pthread_setaffinity_np(self->track_worker_threads[f_i],
                        sizeof(cpu_set_t), &cpuset);
                //sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
                f_cpu_core += f_cpu_core_inc;
            }

            struct sched_param param2;
            int f_applied_policy = 0;
            pthread_getschedparam(self->track_worker_threads[f_i],
                &f_applied_policy, &param2);

            if(f_applied_policy == RT_SCHED)
            {
                printf("Scheduling successfully applied with priority %i\n ",
                        param2.__sched_priority);
            }
            else
            {
                printf("Scheduling was not successfully applied\n");
            }
        }
        else
        {
            self->main_thread_args = (void*)f_args;
        }
        f_i++;
    }

    pthread_attr_destroy(&threadAttr);
    self->audio_recording_quit_notifier = 0;


    pthread_attr_t auxThreadAttr;
    pthread_attr_init(&auxThreadAttr);
    pthread_attr_setdetachstate(&auxThreadAttr, PTHREAD_CREATE_DETACHED);

    /*The worker thread for flushing recorded audio from memory to disk*/
    /*No longer recording audio in PyDAW, but keeping the code here for
     * when I bring it back...*/
    /*pthread_create(&self->audio_recording_thread, &threadAttr,
        v_pydaw_audio_recording_thread, (void*)self);*/

    pthread_create(&self->osc_queue_thread, &auxThreadAttr,
            v_pydaw_osc_send_thread, (void*)self);
    pthread_attr_destroy(&auxThreadAttr);
}

inline void v_queue_osc_message(char * a_key, char * a_val)
{
    if(pydaw_data->osc_queue_index >= PYDAW_OSC_SEND_QUEUE_SIZE)
    {
        printf("Dropping OSC event to prevent buffer overrun:\n%s|%s\n\n",
                a_key, a_val);
    }
    else
    {
        pthread_spin_lock(&pydaw_data->ui_spinlock);
        sprintf(pydaw_data->osc_queue_keys[pydaw_data->osc_queue_index],
                "%s", a_key);
        sprintf(pydaw_data->osc_queue_vals[pydaw_data->osc_queue_index],
                "%s", a_val);
        pydaw_data->osc_queue_index += 1;
        pthread_spin_unlock(&pydaw_data->ui_spinlock);
    }
}

void v_pydaw_set_control_from_atm(
        t_pydaw_seq_event *event, t_pydaw_data * self,
        int a_plugin_uid, int a_track_num)
{
    t_pytrack * f_track = self->track_pool_all[a_track_num];

    if(!self->is_offline_rendering)
    {
        sprintf(
            f_track->osc_cursor_message, "%i|%i|%f",
            a_plugin_uid, event->port, event->value);
        v_queue_osc_message("pc", f_track->osc_cursor_message);
    }
}

void v_pydaw_set_control_from_cc(
        t_pydaw_seq_event *event, t_pydaw_data * self, int a_track_num)
{
    t_pytrack * f_track = self->track_pool_all[a_track_num];

    if(!self->is_offline_rendering)
    {
        sprintf(
            f_track->osc_cursor_message, "%i|%i|%i",
            a_track_num, event->param, (int)(event->value));
        v_queue_osc_message("cc", f_track->osc_cursor_message);
    }
}


inline void v_pydaw_set_bus_counters(t_pydaw_data * self)
{
    int f_i = 0;
    int f_i2;
    int f_global_track_num;

    while(f_i < PYDAW_TRACK_COUNT_ALL)
    {
        self->track_pool_all[f_i]->bus_count = 0;
        f_i++;
    }

    f_i = 0;

    while(f_i < PYDAW_TRACK_COUNT_ALL)
    {
        f_i2 = 0;
        while(f_i2 < MAX_ROUTING_COUNT)
        {
            if(self->routing_graph->routes[f_i][f_i2].active)
            {
                f_global_track_num =
                    self->routing_graph->routes[f_i][f_i2].output;
                self->track_pool_all[f_global_track_num]->bus_count += 1;
            }
            f_i2++;
        }
        f_i++;
    }

    f_i = 0;

    while(f_i < PYDAW_TRACK_COUNT_ALL)
    {
        self->track_pool_all[f_i]->bus_counter =
            self->track_pool_all[f_i]->bus_count;
        f_i++;
    }
}

inline void v_pydaw_sum_track_outputs(t_pydaw_data * self, t_pytrack * a_track)
{
    int f_bus_num;
    t_pytrack * f_bus;
    t_pytrack_routing * f_route;
    t_pydaw_plugin * f_plugin = 0;
    float ** f_buff;
    float ** f_track_buff = a_track->buffers;

    if((a_track->bus_count)
        ||
        ((!a_track->mute) && (!self->is_soloed))
        ||
        ((self->is_soloed) && (a_track->solo)))
    {
        if(a_track->fade_state == FADE_STATE_FADED)
        {
            a_track->fade_state = FADE_STATE_RETURNING;
            v_rmp_retrigger(a_track->fade_env, 0.1f, 1.0f);
        }
        else if(a_track->fade_state == FADE_STATE_FADING)
        {
            a_track->fade_env->output = 1.0f - a_track->fade_env->output;
            a_track->fade_state = FADE_STATE_RETURNING;
        }
    }
    else
    {
        if(a_track->fade_state == FADE_STATE_OFF)
        {
            a_track->fade_state = FADE_STATE_FADING;
            v_rmp_retrigger(a_track->fade_env, 0.1f, 1.0f);
        }
        else if(a_track->fade_state == FADE_STATE_RETURNING)
        {
            a_track->fade_env->output = 1.0f - a_track->fade_env->output;
            a_track->fade_state = FADE_STATE_FADING;
        }
    }

    int f_i2 = 0;

    if(a_track->fade_state == FADE_STATE_OFF)
    {

    }
    else if(a_track->fade_state == FADE_STATE_FADING)
    {
        while(f_i2 < self->sample_count)
        {
            f_rmp_run_ramp(a_track->fade_env);

            f_track_buff[0][f_i2] *= (1.0f - a_track->fade_env->output);
            f_track_buff[1][f_i2] *= (1.0f - a_track->fade_env->output);
            f_i2++;
        }

        if(a_track->fade_env->output >= 1.0f)
        {
            a_track->fade_state = FADE_STATE_FADED;
        }
    }
    else if(a_track->fade_state == FADE_STATE_RETURNING)
    {
        while(f_i2 < self->sample_count)
        {
            f_rmp_run_ramp(a_track->fade_env);
            f_track_buff[0][f_i2] *= a_track->fade_env->output;
            f_track_buff[1][f_i2] *= a_track->fade_env->output;
            f_i2++;
        }

        if(a_track->fade_env->output >= 1.0f)
        {
            a_track->fade_state = FADE_STATE_OFF;
        }
    }


    int f_i3 = 0;

    while(f_i3 < MAX_ROUTING_COUNT)
    {
        f_route = &self->routing_graph->routes[a_track->track_num][f_i3];

        if(!f_route->active)
        {
            f_i3++;
            continue;
        }

        f_bus_num = f_route->output;

        if(f_bus_num < 0)
        {
            f_i3++;
            continue;
        }

        int f_plugin_index = MAX_PLUGIN_COUNT + f_i3;

        if(a_track->plugins[f_plugin_index])
        {
            f_plugin = a_track->plugins[f_plugin_index];
        }
        else
        {
            f_plugin = 0;
        }

        f_bus = self->track_pool_all[f_bus_num];
        f_buff = f_bus->buffers;

        pthread_spin_lock(&f_bus->lock);

        if(a_track->fade_state != FADE_STATE_FADED)
        {
            if(f_plugin && f_plugin->power)
            {
                v_pydaw_process_atm(
                    self, a_track->track_num, f_plugin_index,
                    self->sample_count);
                f_plugin->descriptor->run_mixing(
                    f_plugin->PYFX_handle, self->sample_count,
                    f_buff, 2, a_track->event_buffer,
                    a_track->period_event_index,
                    f_plugin->atm_buffer, f_plugin->atm_count,
                    a_track->extern_midi, *a_track->extern_midi_count);
            }
            else
            {
                f_i2 = 0;
                while(f_i2 < self->sample_count)
                {
                    f_buff[0][f_i2] += f_track_buff[0][f_i2];
                    f_buff[1][f_i2] += f_track_buff[1][f_i2];
                    f_i2++;
                }
            }
        }
        f_bus->bus_counter -= 1;

        pthread_spin_unlock(&f_bus->lock);
        f_i3++;
    }
}

void * v_pydaw_audio_recording_thread(void* a_arg)
{
    t_pydaw_data * self = (t_pydaw_data*)(a_arg);
    char f_file_name[1024];

    sleep(3);

    while(1)
    {
        int f_flushed_buffer = 0;
        int f_did_something = 0;

        if(self->audio_recording_quit_notifier)
        {
            printf("audio recording thread exiting...\n");
            break;
        }

        pthread_mutex_lock(&self->audio_inputs_mutex);

        if(self->playback_mode == PYDAW_PLAYBACK_MODE_REC)
        {
            int f_i = 0;

            while(f_i < PYDAW_AUDIO_INPUT_TRACK_COUNT)
            {
                if((self->audio_inputs[f_i]->rec) &&
                    (self->audio_inputs[f_i]->
                        flush_last_buffer_pending))
                {
                    f_flushed_buffer = 1;
                    printf("Flushing record buffer of %i frames\n",
                            ((self->audio_inputs[f_i]->
                            buffer_iterator[(self->
                            audio_inputs[f_i]->buffer_to_flush)]) / 2));

                    sf_writef_float(self->audio_inputs[f_i]->sndfile,
                            self->audio_inputs[f_i]->
                            rec_buffers[(self->
                            audio_inputs[f_i]->buffer_to_flush)],
                            ((self->audio_inputs[f_i]->
                            buffer_iterator[(self->audio_inputs[f_i]->
                            buffer_to_flush)]) / 2) );

                    self->audio_inputs[f_i]->
                            flush_last_buffer_pending = 0;
                    self->audio_inputs[f_i]->
                            buffer_iterator[(self->audio_inputs[f_i]->
                            buffer_to_flush)] = 0;
                }

                f_i++;
            }
        }
        else
        {
            int f_i = 0;

            while(f_i < PYDAW_AUDIO_INPUT_TRACK_COUNT)
            {
                /*I guess the main mutex keeps this concurrent, as the
                 * set_playback_mode has to grab it before setting the
                 * recording_stopped flag, which means we won't wind up with
                 * half-a-buffer, even if this
                 * thread uses lockless techniques while running
                 * fast-and-loose with the data...
                 * TODO:  verify that this is safe...*/
                if(self->audio_inputs[f_i]->recording_stopped)
                {
                    f_did_something = 1;
                    sf_writef_float(self->audio_inputs[f_i]->sndfile,
                            self->audio_inputs[f_i]->rec_buffers[
                            (self->audio_inputs[f_i]->current_buffer)],
                            ((self->audio_inputs[f_i]->
                            buffer_iterator[(self->audio_inputs[f_i]->
                            current_buffer)]) / 2) );

                    sf_close(self->audio_inputs[f_i]->sndfile);
                    self->audio_inputs[f_i]->recording_stopped = 0;
                    self->audio_inputs[f_i]->sndfile = 0;
                }
                f_i++;
            }

            /*Re-create the sndfile if it no longer exists, that means the
             * UI has moved it from the tmp folder...*/
            f_i = 0;

            while(f_i < PYDAW_AUDIO_INPUT_TRACK_COUNT)
            {
                if(self->audio_inputs[f_i]->rec)
                {
                    f_did_something = 1;

                    sprintf(f_file_name, "%s%i.wav",
                            self->audio_tmp_folder, f_i);

                    if(!i_pydaw_file_exists(f_file_name))
                    {
                        v_pydaw_audio_input_record_set(
                                self->audio_inputs[f_i], f_file_name);
                    }
                }
                f_i++;
            }

        }

        pthread_mutex_unlock(&self->audio_inputs_mutex);

        if(!f_flushed_buffer || !f_did_something)
        {
            usleep(10000);
        }
    }

    return (void*)1;
}

inline void v_pydaw_process_track(t_pydaw_data * self, int a_global_track_num)
{
    t_pytrack * f_track = self->track_pool_all[a_global_track_num];
    t_pydaw_plugin * f_plugin;

    if(self->playback_mode > 0)
    {
        v_pydaw_process_midi(self, a_global_track_num, self->sample_count);
    }

    v_pydaw_process_external_midi(self, f_track, self->sample_count);

    v_pydaw_process_note_offs(self, a_global_track_num);

    if((f_track->bus_count) != 0)
    {
        while(1)
        {
            pthread_spin_lock(&f_track->lock);

            if((f_track->bus_counter) <= 0)
            {
                pthread_spin_unlock(&f_track->lock);
                break;
            }

            pthread_spin_unlock(&f_track->lock);
        }
        f_track->bus_counter = (f_track->bus_count);
    }

    v_pydaw_audio_items_run(self, self->sample_count,
        f_track->buffers[0], f_track->buffers[1],
        a_global_track_num, 0);

    int f_i = 0;

    while(f_i < MAX_PLUGIN_COUNT)
    {
        f_plugin = f_track->plugins[f_i];
        if(f_plugin && f_plugin->power)
        {
            v_pydaw_process_atm(
                self, a_global_track_num, f_i, self->sample_count);
            f_plugin->descriptor->run_replacing(
                f_plugin->PYFX_handle, self->sample_count,
                f_track->event_buffer, f_track->period_event_index,
                f_plugin->atm_buffer, f_plugin->atm_count,
                f_track->extern_midi, *f_track->extern_midi_count);
        }
        f_i++;
    }

    v_pydaw_sum_track_outputs(self, f_track);

    v_pkm_run(f_track->peak_meter, f_track->buffers[0],
        f_track->buffers[1], self->sample_count);

    v_pydaw_zero_buffer(f_track->buffers, self->sample_count);
}

inline void v_pydaw_process(t_pydaw_thread_args * f_args)
{
    t_pytrack * f_track;
    int f_track_index;
    t_pydaw_data * self = f_args->pydaw_data;
    int f_i = f_args->thread_num;

    while(f_i < self->routing_graph->track_pool_sorted_count)
    {
        f_track_index = self->routing_graph->track_pool_sorted[f_i];
        f_track = self->track_pool_all[f_track_index];

        if(f_track->status != STATUS_NOT_PROCESSED)
        {
            f_i++;
            continue;
        }

        pthread_spin_lock(&f_track->lock);

        if(f_track->status == STATUS_NOT_PROCESSED)
        {
            f_track->status = STATUS_PROCESSING;
        }
        else
        {
            pthread_spin_unlock(&f_track->lock);
            f_i++;
            continue;
        }

        pthread_spin_unlock(&f_track->lock);

        v_pydaw_process_track(self, f_track->track_num);

        //pthread_spin_lock(&f_track->lock);

        f_track->status = STATUS_PROCESSED;

        //pthread_spin_unlock(&f_track->lock);

        f_i++;
    }

    self->track_thread_is_finished[f_args->thread_num] = 1;
}


void * v_pydaw_worker_thread(void* a_arg)
{
    t_pydaw_thread_args * f_args = (t_pydaw_thread_args*)(a_arg);
    t_pydaw_data * self = f_args->pydaw_data;
    int f_thread_num = f_args->thread_num;
    pthread_cond_t * f_track_cond = &self->track_cond[f_thread_num];
    pthread_mutex_t * f_track_block_mutex =
        &self->track_block_mutexes[f_thread_num];

    while(1)
    {
        pthread_cond_wait(f_track_cond, f_track_block_mutex);

        if(self->track_thread_quit_notifier[f_thread_num])
        {
            self->track_thread_quit_notifier[f_thread_num] = 2;
            printf("Worker thread %i exiting...\n", f_thread_num);
            break;
        }

        v_pydaw_process(f_args);
    }

    return (void*)1;
}

inline void v_pydaw_process_atm(
    t_pydaw_data * self, int f_track_num, int f_index, int sample_count)
{
    t_pytrack * f_track = self->track_pool_all[f_track_num];
    t_pydaw_plugin * f_plugin = f_track->plugins[f_index];
    int f_current_track_region = self->current_region;
    int f_current_track_bar = self->current_bar;
    float f_track_current_period_beats = self->ml_current_period_beats;
    float f_track_next_period_beats = self->ml_next_period_beats;
    float f_track_beats_offset = 0.0f;

    f_plugin->atm_count = 0;

    if((!self->overdub_mode) && (self->playback_mode == 2) &&
        (f_track->extern_midi))
    {
        return;
    }

    while(1)
    {
        if(self->pysong->regions_atm[f_current_track_region] &&
            self->pysong->regions_atm[f_current_track_region]->tracks[
                f_track_num].plugins[f_index].point_count)
        {
            t_pydaw_atm_plugin * f_current_item =
                &self->pysong->regions_atm[f_current_track_region]->tracks[
                    f_track_num].plugins[f_index];

            if((f_plugin->atm_pos) >= (f_current_item->point_count))
            {
                if(f_track_next_period_beats >= 4.0f)
                {
                    f_track_current_period_beats = 0.0f;
                    f_track_next_period_beats =
                        f_track_next_period_beats - 4.0f;
                    f_track_beats_offset =
                        ((self->ml_sample_period_inc) * 4.0f) -
                        f_track_next_period_beats;

                    f_plugin->atm_pos = 0;

                    f_current_track_bar++;

                    if(f_current_track_bar >= self->f_region_length_bars)
                    {
                        f_current_track_bar = 0;

                        if(self->loop_mode != PYDAW_LOOP_MODE_REGION)
                        {
                            f_current_track_region++;
                        }
                    }

                    continue;
                }
                else
                {
                    break;
                }
            }

            t_pydaw_atm_point * f_point =
                &f_current_item->points[f_plugin->atm_pos];

            if((f_point->bar < f_current_track_bar) ||
                ((f_point->bar == f_current_track_bar) &&
                (f_point->beat < f_track_current_period_beats)))
            {
                f_plugin->atm_pos += 1;
                continue;
            }


            if((f_point->bar == f_current_track_bar) &&
                (f_point->beat >= f_track_current_period_beats) &&
                (f_point->beat < f_track_next_period_beats))
            {
                t_pydaw_seq_event * f_buff_ev =
                    &f_plugin->atm_buffer[f_plugin->atm_count];

                int f_note_sample_offset = 0;
                float f_note_start_diff =
                    ((f_point->beat) - f_track_current_period_beats) +
                    f_track_beats_offset;
                float f_note_start_frac =
                    f_note_start_diff / self->ml_sample_period_inc_beats;
                f_note_sample_offset =
                    (int)(f_note_start_frac * (float)sample_count);

                if(f_plugin->uid == f_point->plugin)
                {
                    float f_val = f_atm_to_ctrl_val(
                        f_plugin->descriptor, f_point->port, f_point->val);
                    v_pydaw_ev_clear(f_buff_ev);
                    v_pydaw_ev_set_atm(f_buff_ev, f_point->port, f_val);
                    f_buff_ev->tick = f_note_sample_offset;
                    v_pydaw_set_control_from_atm(
                        f_buff_ev, self, f_plugin->pool_uid, f_track_num);
                    f_plugin->atm_count += 1;
                }
                f_plugin->atm_pos += 1;
            }
            else
            {
                break;
            }
        }
        else
        {
            if(f_track_next_period_beats >= 4.0f)
            {
                f_track_current_period_beats = 0.0f;
                f_track_next_period_beats =
                    f_track_next_period_beats - 4.0f;
                f_track_beats_offset =
                    ((self->ml_sample_period_inc) * 4.0f) -
                        f_track_next_period_beats;

                f_current_track_bar++;

                if(f_current_track_bar >= self->f_region_length_bars)
                {
                    f_current_track_bar = 0;
                    f_plugin->atm_pos = 0;

                    if(self->loop_mode != PYDAW_LOOP_MODE_REGION)
                    {
                        f_current_track_region++;
                    }
                }

                continue;
            }
            else
            {
                break;
            }
        }
    }
}

inline void v_pydaw_process_midi(t_pydaw_data * self, int f_i,
        int sample_count)
{
    t_pytrack * f_track = self->track_pool_all[f_i];
    f_track->period_event_index = 0;

    int f_current_track_region = self->current_region;
    int f_current_track_bar = self->current_bar;
    float f_track_current_period_beats =
        (self->ml_current_period_beats);
    float f_track_next_period_beats = (self->ml_next_period_beats);
    float f_track_beats_offset = 0.0f;

    if((!self->overdub_mode) && (self->playback_mode == 2) &&
        (f_track->extern_midi))
    {

    }
    else
    {
        while(1)
        {
            if((self->pysong->regions[f_current_track_region]) &&
                (self->pysong->regions[
                    f_current_track_region]->item_indexes[
                        f_i][f_current_track_bar] != -1))
            {
                t_pyitem * f_current_item =
                    self->item_pool[(self->pysong->regions[
                        f_current_track_region]->item_indexes[
                            f_i][f_current_track_bar])];

                if((f_track->item_event_index) >= (f_current_item->event_count))
                {
                    if(f_track_next_period_beats >= 4.0f)
                    {
                        f_track_current_period_beats = 0.0f;
                        f_track_next_period_beats -= 4.0f;
                        f_track_beats_offset =
                            ((self->ml_sample_period_inc) * 4.0f) -
                            f_track_next_period_beats;

                        f_track->item_event_index = 0;

                        f_current_track_bar++;

                        if(f_current_track_bar >= self->f_region_length_bars)
                        {
                            f_current_track_bar = 0;

                            if(self->loop_mode !=
                                    PYDAW_LOOP_MODE_REGION)
                            {
                                f_current_track_region++;
                            }
                        }

                        continue;
                    }
                    else
                    {
                        break;
                    }
                }

                t_pydaw_seq_event * f_event =
                    f_current_item->events[f_track->item_event_index];

                if((f_event->start >= f_track_current_period_beats) &&
                    (f_event->start < f_track_next_period_beats))
                {
                    if(f_event->type == PYDAW_EVENT_NOTEON)
                    {
                        int f_note_sample_offset = 0;
                        float f_note_start_diff =
                            f_event->start - f_track_current_period_beats +
                            f_track_beats_offset;
                        float f_note_start_frac = f_note_start_diff /
                                (self->ml_sample_period_inc_beats);
                        f_note_sample_offset =  (int)(f_note_start_frac *
                                ((float)sample_count));

                        if(f_track->note_offs[f_event->note]
                            >= (self->current_sample))
                        {
                            t_pydaw_seq_event * f_buff_ev;

                            /*There's already a note_off scheduled ahead of
                             * this one, process it immediately to avoid
                             * hung notes*/
                            f_buff_ev = &f_track->event_buffer[
                                f_track->period_event_index];
                            v_pydaw_ev_clear(f_buff_ev);

                            v_pydaw_ev_set_noteoff(f_buff_ev, 0,
                                    (f_event->note), 0);
                            f_buff_ev->tick = f_note_sample_offset;

                            f_track->period_event_index += 1;
                        }

                        t_pydaw_seq_event * f_buff_ev =
                            &f_track->event_buffer[f_track->period_event_index];

                        v_pydaw_ev_clear(f_buff_ev);

                        v_pydaw_ev_set_noteon(f_buff_ev, 0,
                                f_event->note, f_event->velocity);

                        f_buff_ev->tick = f_note_sample_offset;

                        f_track->period_event_index += 1;

                        f_track->note_offs[(f_event->note)] =
                            (self->current_sample) + ((int)(f_event->length *
                            self->samples_per_beat));
                    }
                    else if(f_event->type == PYDAW_EVENT_CONTROLLER)
                    {
                        int controller = f_event->param;

                        t_pydaw_seq_event * f_buff_ev =
                            &f_track->event_buffer[f_track->period_event_index];

                        int f_note_sample_offset = 0;

                        float f_note_start_diff =
                            ((f_event->start) - f_track_current_period_beats) +
                            f_track_beats_offset;
                        float f_note_start_frac = f_note_start_diff /
                            self->ml_sample_period_inc_beats;
                        f_note_sample_offset =
                            (int)(f_note_start_frac * ((float)sample_count));

                        v_pydaw_ev_clear(f_buff_ev);

                        v_pydaw_ev_set_controller(
                            f_buff_ev, 0, controller, f_event->value);

                        v_pydaw_set_control_from_cc(f_buff_ev, self, f_i);

                        f_buff_ev->tick = f_note_sample_offset;

                        f_track->period_event_index += 1;
                    }
                    else if(f_event->type == PYDAW_EVENT_PITCHBEND)
                    {
                        int f_note_sample_offset = 0;
                        float f_note_start_diff = ((f_event->start) -
                        f_track_current_period_beats) + f_track_beats_offset;
                        float f_note_start_frac = f_note_start_diff /
                            self->ml_sample_period_inc_beats;
                        f_note_sample_offset =  (int)(f_note_start_frac *
                            ((float)sample_count));

                        t_pydaw_seq_event * f_buff_ev =
                            &f_track->event_buffer[f_track->period_event_index];

                        v_pydaw_ev_clear(f_buff_ev);
                        v_pydaw_ev_set_pitchbend(f_buff_ev, 0, f_event->value);
                        f_buff_ev->tick = f_note_sample_offset;

                        f_track->period_event_index += 1;
                    }

                    f_track->item_event_index += 1;
                }
                else
                {
                    break;
                }
            }
            else
            {
                if(f_track_next_period_beats >= 4.0f)
                {
                    f_track_current_period_beats = 0.0f;
                    f_track_next_period_beats =
                        f_track_next_period_beats - 4.0f;
                    f_track_beats_offset = ((self->ml_sample_period_inc)
                        * 4.0f) - f_track_next_period_beats;

                    f_track->item_event_index = 0;

                    f_current_track_bar++;

                    if(f_current_track_bar >= self->f_region_length_bars)
                    {
                        f_current_track_bar = 0;

                        if(self->loop_mode != PYDAW_LOOP_MODE_REGION)
                        {
                            f_current_track_region++;
                        }
                    }

                    continue;
                }
                else
                {
                    break;
                }
            }
        }
    }
}

inline void v_pydaw_process_note_offs(t_pydaw_data * self, int f_i)
{
    t_pytrack * f_track = self->track_pool_all[f_i];

    int f_i2 = 0;

    while(f_i2 < PYDAW_MIDI_NOTE_COUNT)
    {
        if((f_track->note_offs[f_i2]) >= (self->current_sample) &&
           (f_track->note_offs[f_i2]) < self->f_next_current_sample)
        {
            t_pydaw_seq_event * f_event =
                &f_track->event_buffer[f_track->period_event_index];
            v_pydaw_ev_clear(f_event);

            v_pydaw_ev_set_noteoff(f_event, 0, f_i2, 0);
            f_event->tick = (f_track->note_offs[f_i2]) - (self->current_sample);
            f_track->period_event_index += 1;
            f_track->note_offs[f_i2] = -1;
        }
        f_i2++;
    }
}

inline void v_pydaw_process_external_midi(t_pydaw_data * self,
        t_pytrack * a_track, int sample_count)
{
    if(!a_track->midi_device)
    {
        return;
    }

    midiDeviceRead(a_track->midi_device, self->sample_rate, self->sample_count);

    int f_extern_midi_count = *a_track->extern_midi_count;
    t_pydaw_seq_event * events = a_track->extern_midi;

    assert(f_extern_midi_count < 200);

    int f_i2 = 0;

    char * f_osc_msg = a_track->osc_cursor_message;

    while(f_i2 < f_extern_midi_count)
    {
        if(events[f_i2].tick >= sample_count)
        {
            //Otherwise the event will be missed
            events[f_i2].tick = sample_count - 1;
        }

        if(events[f_i2].type == PYDAW_EVENT_NOTEON)
        {
            if(self->playback_mode == PYDAW_PLAYBACK_MODE_REC)
            {
                float f_beat = self->ml_current_period_beats +
                    f_pydaw_samples_to_beat_count(events[f_i2].tick,
                        self->tempo, self->sample_rate);

                sprintf(
                    f_osc_msg,
                    "on|%i|%i|%f|%i|%i|%i|%ld", self->current_region,
                    self->current_bar, f_beat, a_track->track_num,
                    events[f_i2].note,
                    events[f_i2].velocity,
                    self->current_sample + events[f_i2].tick);
                v_queue_osc_message("mrec", f_osc_msg);
            }

            sprintf(f_osc_msg, "1|%i", events[f_i2].note);
            v_queue_osc_message("ne", f_osc_msg);

        }
        else if(events[f_i2].type == PYDAW_EVENT_NOTEOFF)
        {
            a_track->period_event_index += 1;

            if(self->playback_mode == PYDAW_PLAYBACK_MODE_REC)
            {
                float f_beat = self->ml_current_period_beats +
                    f_pydaw_samples_to_beat_count(events[f_i2].tick,
                        self->tempo, self->sample_rate);

                sprintf(
                    f_osc_msg,
                    "off|%i|%i|%f|%i|%i|%ld", self->current_region,
                    self->current_bar, f_beat, a_track->track_num,
                    events[f_i2].note,
                    self->current_sample + events[f_i2].tick);
                v_queue_osc_message("mrec", f_osc_msg);
            }

            sprintf(f_osc_msg, "0|%i", events[f_i2].note);
            v_queue_osc_message("ne", f_osc_msg);
        }
        else if(events[f_i2].type == PYDAW_EVENT_PITCHBEND)
        {
            if(self->playback_mode == PYDAW_PLAYBACK_MODE_REC)
            {
                float f_beat = self->ml_current_period_beats +
                    f_pydaw_samples_to_beat_count(events[f_i2].tick,
                        self->tempo, self->sample_rate);

                sprintf(
                    f_osc_msg,
                    "pb|%i|%i|%f|%i|%f", self->current_region,
                    self->current_bar, f_beat,
                    a_track->track_num, events[f_i2].value);
                v_queue_osc_message("mrec", f_osc_msg);
            }
        }
        else if(events[f_i2].type == PYDAW_EVENT_CONTROLLER)
        {
            int controller = events[f_i2].param;

            if(self->midi_learn)
            {
                self->midi_learn = 0;
                sprintf(f_osc_msg, "%i", controller);
                v_queue_osc_message("ml", f_osc_msg);
            }

            /*float f_start =
                ((self->playback_cursor) +
                ((((float)(events[f_i2].tick)) / ((float)sample_count))
                * (self->playback_inc))) * 4.0f;*/
            v_pydaw_set_control_from_cc(
                &events[f_i2], self, a_track->track_num);

            a_track->period_event_index += 1;

            if(self->playback_mode == PYDAW_PLAYBACK_MODE_REC)
            {
                float f_beat =
                    self->ml_current_period_beats +
                    f_pydaw_samples_to_beat_count(
                        events[f_i2].tick, self->tempo, self->sample_rate);

                sprintf(f_osc_msg,
                    "cc|%i|%i|%f|%i|%i|%f",
                    self->current_region, self->current_bar, f_beat,
                    a_track->track_num, controller, events[f_i2].value);
                v_queue_osc_message("mrec", f_osc_msg);
            }
        }
        f_i2++;
    }
}


inline void v_pydaw_set_time_params(t_pydaw_data * self,
        int sample_count)
{
    self->ml_sample_period_inc =
        ((self->playback_inc) * ((float)(sample_count)));
    self->ml_sample_period_inc_beats =
        (self->ml_sample_period_inc) * 4.0f;
    self->ml_next_playback_cursor =
        (self->playback_cursor) + (self->ml_sample_period_inc);
    self->ml_current_period_beats =
        (self->playback_cursor) * 4.0f;
    self->ml_next_period_beats =
        (self->ml_next_playback_cursor) * 4.0f;

    self->ml_current_region = (self->current_region);
    self->ml_current_bar = (self->current_bar);
    self->ml_current_beat = (self->ml_current_period_beats);

    self->ml_next_bar = (self->current_bar);
    self->ml_next_region = (self->current_region);

    if((self->ml_next_period_beats) > 4.0f)  //Should it be >= ???
    {
        self->ml_starting_new_bar = 1;
        self->ml_next_beat =
            (self->ml_next_period_beats) - 4.0f;

        self->ml_next_bar = (self->current_bar) + 1;

        int f_region_length = 8;
        if(self->pysong->regions[(self->current_region)])
        {
            f_region_length =
                (self->pysong->regions[
                    (self->current_region)]->region_length_bars);
        }

        if(f_region_length == 0)
        {
            f_region_length = 8;
        }

        if((self->ml_next_bar) >= f_region_length)
        {
            self->ml_next_bar = 0;
            if(self->loop_mode != PYDAW_LOOP_MODE_REGION)
            {
                self->ml_next_region =
                    (self->ml_next_region) + 1;
            }
            else
            {
                self->ml_is_looping = 1;
            }
        }
    }
    else
    {
        self->ml_is_looping = 0;
        self->ml_starting_new_bar = 0;
        self->ml_next_region = self->current_region;
        self->ml_next_bar = self->current_bar;
        self->ml_next_beat = self->ml_next_period_beats;
    }
}

inline void v_pydaw_finish_time_params(t_pydaw_data * self,
        int a_region_length_bars)
{
    self->playback_cursor = (self->ml_next_playback_cursor);

    if((self->playback_cursor) >= 1.0f)
    {
        self->playback_cursor = (self->playback_cursor) - 1.0f;

        self->current_bar = (self->current_bar) + 1;

        if((self->current_bar) >= a_region_length_bars)
        {
            self->current_bar = 0;

            if(self->loop_mode != PYDAW_LOOP_MODE_REGION)
            {
                self->current_region =
                    (self->current_region) + 1;

                if((self->current_region) >= PYDAW_MAX_REGION_COUNT)
                {
                    self->playback_mode = 0;
                    self->current_region = 0;
                }
            }
            else if(self->playback_mode == PYDAW_PLAYBACK_MODE_REC)
            {
                float f_beat = self->ml_current_period_beats;

                sprintf(self->osc_cursor_message, "loop|%i|%i|%f|-1",
                    self->current_region, self->current_bar, f_beat);
                v_queue_osc_message("mrec", self->osc_cursor_message);
            }
        }
    }
}


inline void v_pydaw_run_wave_editor(t_pydaw_data * self,
        int sample_count, PYFX_Data *output0, PYFX_Data *output1)
{
    t_pydaw_plugin * f_plugin;
    int f_global_track_num = PYDAW_TRACK_COUNT_ALL - 1;
    t_pytrack * f_track = self->track_pool_all[f_global_track_num];
    int f_i = 0;

    while(f_i < sample_count)
    {
        if((self->ab_audio_item->sample_read_head->whole_number) >=
            (self->ab_audio_item->sample_end_offset))
        {
            output0[f_i] = 0.0f;
            output1[f_i] = 0.0f;
        }
        else
        {
            v_adsr_run_db(self->ab_audio_item->adsr);
            v_pydaw_audio_item_set_fade_vol(self->ab_audio_item);

            if(self->ab_wav_item->channels == 1)
            {
                float f_tmp_sample = f_cubic_interpolate_ptr_ifh(
                (self->ab_wav_item->samples[0]),
                (self->ab_audio_item->sample_read_head->
                    whole_number),
                (self->ab_audio_item->sample_read_head->fraction)) *
                (self->ab_audio_item->adsr->output) *
                (self->ab_audio_item->vol_linear) *
                (self->ab_audio_item->fade_vol);

                output0[f_i] = f_tmp_sample;
                output1[f_i] = f_tmp_sample;
            }
            else if(self->ab_wav_item->channels > 1)
            {
                output0[f_i] = f_cubic_interpolate_ptr_ifh(
                (self->ab_wav_item->samples[0]),
                (self->ab_audio_item->sample_read_head->
                    whole_number),
                (self->ab_audio_item->sample_read_head->fraction)) *
                (self->ab_audio_item->adsr->output) *
                (self->ab_audio_item->vol_linear) *
                (self->ab_audio_item->fade_vol);

                output1[f_i] = f_cubic_interpolate_ptr_ifh(
                (self->ab_wav_item->samples[1]),
                (self->ab_audio_item->sample_read_head->whole_number),
                (self->ab_audio_item->sample_read_head->fraction)) *
                (self->ab_audio_item->adsr->output) *
                (self->ab_audio_item->vol_linear) *
                (self->ab_audio_item->fade_vol);
            }

            v_ifh_run(self->ab_audio_item->sample_read_head,
                    self->ab_audio_item->ratio);

            if(self->playback_mode != PYDAW_PLAYBACK_MODE_PLAY &&
                self->ab_audio_item->adsr->stage < ADSR_STAGE_RELEASE)
            {
                v_adsr_release(self->ab_audio_item->adsr);
            }
        }
        f_i++;
    }

    float ** f_buff = f_track->buffers;

    f_i = 0;
    while(f_i < sample_count)
    {
        f_buff[0][f_i] = output0[f_i];
        f_buff[1][f_i] = output1[f_i];
        f_i++;
    }

    f_i = 0;
    while(f_i < MAX_PLUGIN_COUNT)
    {
        f_plugin = f_track->plugins[f_i];
        if(f_plugin)
        {
            f_plugin->descriptor->run_replacing(
                f_plugin->PYFX_handle, sample_count,
                f_track->event_buffer, f_track->period_event_index,
                f_plugin->atm_buffer, f_plugin->atm_count,
                f_track->extern_midi, *f_track->extern_midi_count);
        }
        f_i++;
    }

    f_i = 0;
    while(f_i < sample_count)
    {
        output0[f_i] = f_buff[0][f_i];
        output1[f_i] = f_buff[1][f_i];
        f_i++;
    }

    v_pkm_run(f_track->peak_meter, f_buff[0], f_buff[1],
        self->sample_count);
}


inline void v_pydaw_run_engine(t_pydaw_data * self, int sample_count,
        long f_next_current_sample, PYFX_Data *output0, PYFX_Data *output1,
        PYFX_Data **a_input_buffers)
{
    self->sample_count = sample_count;
    self->input_buffers = a_input_buffers;
    self->f_next_current_sample = f_next_current_sample;

    if((self->playback_mode) > 0)
    {
        v_pydaw_set_time_params(self, sample_count);

        self->f_region_length_bars =
                self->default_region_length_bars;
        //float f_bar_length = (float)(self->default_bar_length);

        if(self->pysong->regions[(self->current_region)])
        {
            if(self->pysong->regions[
                (self->current_region)]->bar_length)
            {
                //f_bar_length = (float)(self->pysong->regions[
                        //(self->current_region)]->bar_length);
            }

            if(self->pysong->regions[(self->current_region)]->
                    region_length_bars)
            {
                self->f_region_length_bars =
                    self->pysong->regions[
                        (self->current_region)]->region_length_bars;

                if(self->pysong->regions[
                        (self->current_region)]->region_length_beats)
                {
                    self->f_region_length_bars++;

                    if((self->current_bar) == (self->
                            f_region_length_bars - 1))
                    {
                        //f_bar_length = (float)(self->pysong->regions[
                        //    (self->current_region)]->
                        //    region_length_beats);
                    }
                }
            }
        }
    }

    int f_i = 0;
    while(f_i < PYDAW_TRACK_COUNT_ALL)
    {
        self->track_pool_all[f_i]->status = STATUS_NOT_PROCESSED;
        f_i++;
    }
    //notify the worker threads
    f_i = 1;
    while(f_i < self->track_worker_thread_count)
    {
        self->track_thread_is_finished[f_i] = 0;
        pthread_mutex_lock(&self->track_block_mutexes[f_i]);
        pthread_cond_broadcast(&self->track_cond[f_i]);
        pthread_mutex_unlock(&self->track_block_mutexes[f_i]);
        f_i++;
    }

    v_pydaw_process((t_pydaw_thread_args*)self->main_thread_args);

    //prep the master channel

    if(self->playback_mode > 0)
    {
        v_pydaw_process_midi(self, 0, self->sample_count);
    }

    t_pytrack * f_master_track = self->track_pool_all[0];

    v_pydaw_process_external_midi(self, f_master_track, self->sample_count);

    v_pydaw_process_note_offs(pydaw_data, 0);

    float ** f_master_buff = f_master_track->buffers;

    v_pydaw_audio_items_run(self, self->sample_count,
        f_master_buff[0], f_master_buff[1], 0, 0);

    t_pydaw_plugin * f_plugin;

    //wait for the other threads to finish

    f_i = 1;

    while(f_i < (self->track_worker_thread_count))
    {
        if(self->track_thread_is_finished[f_i] == 0)
        {
            continue;  //spin until it is finished...
        }

        f_i++;
    }

    //Run the master channels effects

    f_i = 0;
    while(f_i < MAX_PLUGIN_COUNT)
    {
        f_plugin = f_master_track->plugins[f_i];
        if(f_plugin)
        {
            v_pydaw_process_atm(
                self, 0, f_i, sample_count);
            f_plugin->descriptor->run_replacing(
                f_plugin->PYFX_handle, sample_count,
                    f_master_track->event_buffer,
                    f_master_track->period_event_index,
                    f_plugin->atm_buffer, f_plugin->atm_count,
                    f_master_track->extern_midi,
                    *f_master_track->extern_midi_count);
        }
        f_i++;
    }

    f_i = 0;
    while(f_i < sample_count)
    {
        output0[f_i] = f_master_buff[0][f_i];
        output1[f_i] = f_master_buff[1][f_i];
        f_i++;
    }

    f_master_track->bus_counter = (f_master_track->bus_count);

    v_pkm_run(f_master_track->peak_meter,
        f_master_buff[0], f_master_buff[1], sample_count);


    v_pydaw_zero_buffer(f_master_buff, sample_count);

    if((self->playback_mode) > 0)
    {
        v_pydaw_finish_time_params(self,
                self->f_region_length_bars);
    }
}

inline void v_pydaw_run_main_loop(t_pydaw_data * self, int sample_count,
        long f_next_current_sample, PYFX_Data *output0, PYFX_Data *output1,
        PYFX_Data **a_input_buffers)
{
    if(self->is_ab_ing)
    {
        v_pydaw_run_wave_editor(self, sample_count, output0, output1);
    }
    else
    {
        v_pydaw_run_engine(
            self, sample_count, f_next_current_sample,
            output0, output1, a_input_buffers);
    }

    if(self->is_previewing)
    {
        int f_i = 0;
        while(f_i < sample_count)
        {
            if((self->preview_audio_item->sample_read_head->whole_number) >=
                (self->preview_wav_item->length))
            {
                self->is_previewing = 0;
                break;
            }
            else
            {
                v_adsr_run_db(self->preview_audio_item->adsr);
                if(self->preview_wav_item->channels == 1)
                {
                    float f_tmp_sample = f_cubic_interpolate_ptr_ifh(
                    (self->preview_wav_item->samples[0]),
                    (self->preview_audio_item->sample_read_head->whole_number),
                    (self->preview_audio_item->sample_read_head->fraction)) *
                    (self->preview_audio_item->adsr->output) *
                    (self->preview_amp_lin); // *
                    //(self->preview_audio_item->fade_vol);

                    output0[f_i] = f_tmp_sample;
                    output1[f_i] = f_tmp_sample;
                }
                else if(self->preview_wav_item->channels > 1)
                {
                    output0[f_i] = f_cubic_interpolate_ptr_ifh(
                    (self->preview_wav_item->samples[0]),
                    (self->preview_audio_item->sample_read_head->whole_number),
                    (self->preview_audio_item->sample_read_head->fraction)) *
                    (self->preview_audio_item->adsr->output) *
                    (self->preview_amp_lin); // *
                    //(self->preview_audio_item->fade_vol);

                    output1[f_i] = f_cubic_interpolate_ptr_ifh(
                    (self->preview_wav_item->samples[1]),
                    (self->preview_audio_item->sample_read_head->whole_number),
                    (self->preview_audio_item->sample_read_head->fraction)) *
                    (self->preview_audio_item->adsr->output) *
                    (self->preview_amp_lin); // *
                    //(self->preview_audio_item->fade_vol);
                }

                v_ifh_run(self->preview_audio_item->sample_read_head,
                        self->preview_audio_item->ratio);

                if((self->preview_audio_item->sample_read_head->whole_number)
                    >=  (self->preview_max_sample_count))
                {
                    v_adsr_release(self->preview_audio_item->adsr);
                }

                if(self->preview_audio_item->adsr->stage == ADSR_STAGE_OFF)
                {
                    self->is_previewing = 0;
                    break;
                }
            }
            f_i++;
        }
    }

    if(!self->is_offline_rendering && MASTER_VOL != 1.0f)
    {
        int f_i = 0;
        while(f_i < sample_count)
        {
            output0[f_i] *= MASTER_VOL;
            output1[f_i] *= MASTER_VOL;
            f_i++;
        }
    }
}



inline void v_pydaw_audio_items_run(t_pydaw_data * self,
        int a_sample_count, float* a_output0,
        float* a_output1, int a_audio_track_num, int a_is_audio_glue)
{
    if(!a_is_audio_glue &&
      (!self->pysong->audio_items[self->current_region] ||
      !self->pysong->audio_items[
        self->current_region]->index_counts[a_audio_track_num])
      && (!self->pysong->audio_items[self->ml_next_region] ||
        !self->pysong->audio_items[
          self->ml_next_region]->index_counts[a_audio_track_num]))
    {
        return;
    }

    int f_i6 = 0;
    int f_region_count = 1;

    if(self->ml_current_region != self->ml_next_region ||
            self->ml_is_looping)
    {
        f_region_count = 2;
    }

    int f_adjusted_sample_count = a_sample_count;
    int f_start_sample = 0;

    while(f_i6 < f_region_count)
    {
        float f_adjusted_song_pos_beats;
        float f_adjusted_next_song_pos_beats;
        int f_current_region = self->ml_current_region;

        f_adjusted_song_pos_beats = v_pydaw_count_beats(self,
                self->ml_current_region, 0, 0.0f,
                self->ml_current_region, self->ml_current_bar,
                self->ml_current_beat);

        if(f_region_count == 2)
        {
            if(f_i6 == 0)
            {
                if(!self->pysong->audio_items[f_current_region])
                {
                    f_i6++;
                    continue;
                }

                if(self->pysong->regions[
                        (self->current_region)]->
                        region_length_bars == 0)
                {
                    f_adjusted_next_song_pos_beats = 32.0f;
                }
                else
                {
                    f_adjusted_next_song_pos_beats =
                            (float)(self->pysong->regions[
                            (self->current_region)]->
                            region_length_bars * 4);
                }

                float test1 = (int)(f_adjusted_next_song_pos_beats);
                float test2 = test1 - f_adjusted_song_pos_beats;
                float test3 = (test2 /
                    (self->ml_sample_period_inc_beats)) *
                    ((float)(a_sample_count));
                f_adjusted_sample_count = (int)test3;
                assert(f_adjusted_sample_count < a_sample_count);

            }
            else
            {
                f_start_sample = f_adjusted_sample_count;
                f_adjusted_sample_count = a_sample_count;
                // - f_adjusted_sample_count;

                f_current_region = self->ml_next_region;

                if(!self->pysong->audio_items[f_current_region])
                {
                    break;
                }

                f_adjusted_song_pos_beats = 0.0f;
                f_adjusted_next_song_pos_beats = self->ml_next_beat;
            }
        }
        else
        {
            if(!self->pysong->audio_items[f_current_region])
            {
                break;
            }

            f_adjusted_next_song_pos_beats = v_pydaw_count_beats(self,
                    self->ml_current_region, 0, 0.0f,
                    self->ml_next_region,
                    self->ml_next_bar, self->ml_next_beat);
        }

        t_pydaw_per_audio_item_fx_region * f_paif_region =
            self->pysong->per_audio_item_fx[(f_current_region)];

        t_pydaw_audio_items * f_region =
            self->pysong->audio_items[f_current_region];

        int f_i = 0;
        int f_index_pos = 0;

        while(a_is_audio_glue ||
            f_index_pos < f_region->index_counts[a_audio_track_num])
        {
            if(!a_is_audio_glue)
            {
                f_i = f_region->indexes[a_audio_track_num][f_index_pos];
                f_index_pos++;
            }
            else
            {
                if(f_i >= PYDAW_MAX_AUDIO_ITEM_COUNT)
                {
                    break;
                }
            }

            if(f_region->items[f_i] == 0)
            {
                f_i++;
                continue;
            }

            t_pydaw_audio_item * f_audio_item = f_region->items[f_i];

            if(self->suppress_new_audio_items &&
                ((f_audio_item->adsr->stage) == ADSR_STAGE_OFF))
            {
                f_i++;
                continue;
            }

            if(a_is_audio_glue  && !self->audio_glue_indexes[f_i])
            {
                f_i++;
                continue;
            }

            if(self->playback_mode == PYDAW_PLAYBACK_MODE_OFF &&
                f_audio_item->adsr->stage < ADSR_STAGE_RELEASE)
            {
                v_adsr_release(self->pysong->audio_items[
                            f_current_region]->items[f_i]->adsr);
            }

            if(a_is_audio_glue ||
            f_audio_item->audio_track_output == a_audio_track_num)
            {
                if((f_audio_item->adjusted_start_beat) >=
                        f_adjusted_next_song_pos_beats)
                {
                    f_i++;
                    continue;
                }

                int f_i2 = f_start_sample;

                if(((f_audio_item->adjusted_start_beat) >=
                        f_adjusted_song_pos_beats) &&
                    ((f_audio_item->adjusted_start_beat) <
                        f_adjusted_next_song_pos_beats))
                {
                    if(f_audio_item->is_reversed)
                    {
                        v_ifh_retrigger(f_audio_item->sample_read_head,
                            f_audio_item->sample_end_offset);
                    }
                    else
                    {
                        v_ifh_retrigger(f_audio_item->sample_read_head,
                            f_audio_item->sample_start_offset);
                    }

                    v_svf_reset(f_audio_item->lp_filter);

                    v_adsr_retrigger(f_audio_item->adsr);

                    float f_diff = (f_adjusted_next_song_pos_beats -
                        f_adjusted_song_pos_beats);
                    float f_distance = f_adjusted_next_song_pos_beats -
                        (f_audio_item->adjusted_start_beat);

                    f_i2 = f_adjusted_sample_count - (int)((f_distance /
                            f_diff) * ((float)(f_adjusted_sample_count -
                            f_start_sample)));

                    if(f_i2 < 0)
                    {
                        f_i2 = 0;
                    }
                    else if(f_i2 >= f_adjusted_sample_count)
                    {
                        f_i2 = f_adjusted_sample_count - 1;
                    }
                }

                if((f_audio_item->adsr->stage) != ADSR_STAGE_OFF)
                {
                    while((f_i2 < f_adjusted_sample_count) &&
                        (((!f_audio_item->is_reversed) &&
                        ((f_audio_item->sample_read_head->whole_number) <
                        (f_audio_item->sample_end_offset)))
                            ||
                        ((f_audio_item->is_reversed) &&
                        ((f_audio_item->sample_read_head->whole_number) >
                        (f_audio_item->sample_start_offset)))
                        ))
                    {
                        assert(f_i2 < a_sample_count);
                        v_pydaw_audio_item_set_fade_vol(f_audio_item);

                        if(f_audio_item->wav_pool_item->channels == 1)
                        {
                            float f_tmp_sample0 = f_cubic_interpolate_ptr_ifh(
                            (f_audio_item->wav_pool_item->samples[0]),
                            (f_audio_item->sample_read_head->whole_number),
                            (f_audio_item->sample_read_head->fraction)) *
                            (f_audio_item->adsr->output) *
                            (f_audio_item->vol_linear) *
                            (f_audio_item->fade_vol);

                            float f_tmp_sample1 = f_tmp_sample0;

                            if(self->pysong->per_audio_item_fx[
                                    (f_current_region)])
                            {
                                if(self->pysong->per_audio_item_fx[
                                        (f_current_region)]->loaded[f_i])
                                {
                                    int f_i3 = 0;
                                    while(f_i3 < 8)
                                    {
                                        self->pysong->per_audio_item_fx[
                                        (f_current_region)]->items[f_i][f_i3]->
                                        func_ptr(
                                        self->pysong->per_audio_item_fx[
                                        (f_current_region)]->items[f_i][f_i3]->
                                        mf3,
                                        f_tmp_sample0, f_tmp_sample1);
                                        f_tmp_sample0 =
                                            self->pysong->
                                            per_audio_item_fx[
                                                (f_current_region)]->items[f_i][
                                                f_i3]->mf3->output0;
                                        f_tmp_sample1 =
                                            self->pysong->
                                            per_audio_item_fx[
                                            (f_current_region)]->items[f_i][
                                            f_i3]->mf3->output1;
                                        f_i3++;
                                    }
                                }
                            }

                            a_output0[f_i2] += f_tmp_sample0;
                            a_output1[f_i2] += f_tmp_sample1;
                        }
                        else if(f_audio_item->wav_pool_item->channels == 2)
                        {
                            assert(f_audio_item->sample_read_head->whole_number
                                    <=
                                f_audio_item->wav_pool_item->length);

                            assert(f_audio_item->sample_read_head->whole_number
                                    >=
                                PYDAW_AUDIO_ITEM_PADDING_DIV2);

                            float f_tmp_sample0 = f_cubic_interpolate_ptr_ifh(
                            f_audio_item->wav_pool_item->samples[0],
                            f_audio_item->sample_read_head->whole_number,
                            f_audio_item->sample_read_head->fraction) *
                            f_audio_item->adsr->output *
                            f_audio_item->vol_linear *
                            f_audio_item->fade_vol;

                            float f_tmp_sample1 = f_cubic_interpolate_ptr_ifh(
                            f_audio_item->wav_pool_item->samples[1],
                            f_audio_item->sample_read_head->whole_number,
                            f_audio_item->sample_read_head->fraction) *
                            f_audio_item->adsr->output *
                            f_audio_item->vol_linear * f_audio_item->fade_vol;

                            if(f_paif_region)
                            {
                                if(f_paif_region->loaded[f_i])
                                {
                                    t_pydaw_per_audio_item_fx_item *
                                        f_paif_item;
                                    int f_i3 = 0;
                                    while(f_i3 < 8)
                                    {
                                        f_paif_item =
                                            f_paif_region->items[f_i][f_i3];
                                        f_paif_item->func_ptr(
                                            f_paif_item->mf3,
                                            f_tmp_sample0, f_tmp_sample1);
                                        f_tmp_sample0 =
                                            f_paif_item->mf3->output0;
                                        f_tmp_sample1 =
                                            f_paif_item->mf3->output1;
                                        f_i3++;
                                    }
                                }
                            }

                            a_output0[f_i2] += f_tmp_sample0;
                            a_output1[f_i2] += f_tmp_sample1;

                        }
                        else
                        {
                            // TODO:  Catch this during load and
                            // do something then...
                            printf(
                                "Error: v_pydaw_pysong->audio_items"
                                "[f_current_region]_run, invalid number "
                                "of channels %i\n",
                                f_audio_item->wav_pool_item->channels);
                        }

                        if(f_audio_item->is_reversed)
                        {
                            v_ifh_run_reverse(f_audio_item->sample_read_head,
                                f_audio_item->ratio);

                            if(f_audio_item->sample_read_head->whole_number
                                    < PYDAW_AUDIO_ITEM_PADDING_DIV2)
                            {
                                f_audio_item->adsr->stage = ADSR_STAGE_OFF;
                            }
                        }
                        else
                        {
                            v_ifh_run(f_audio_item->sample_read_head,
                                f_audio_item->ratio);

                            if(f_audio_item->sample_read_head->whole_number >=
                                f_audio_item->wav_pool_item->length - 1)
                            {
                                f_audio_item->adsr->stage = ADSR_STAGE_OFF;
                            }
                        }


                        if(f_audio_item->adsr->stage == ADSR_STAGE_OFF)
                        {
                            break;
                        }

                        v_adsr_run_db(f_audio_item->adsr);

                        f_i2++;
                    }//while < sample count
                }  //if stage
            } //if this track_num
            f_i++;
        } //while < audio item count
        f_i6++;
    } //region count loop
    return;
}


t_pydaw_seq_event * g_pynote_get(int a_note, int a_vel,
        float a_start, float a_length)
{
    t_pydaw_seq_event * f_result =
        (t_pydaw_seq_event*)malloc(sizeof(t_pydaw_seq_event));
    f_result->type = PYDAW_EVENT_NOTEON;
    f_result->length = a_length;
    f_result->note = a_note;
    f_result->start = a_start;
    f_result->velocity = a_vel;

    return f_result;
}

t_pydaw_seq_event * g_pycc_get(int a_cc_num, float a_cc_val, float a_start)
{
    t_pydaw_seq_event * f_result =
        (t_pydaw_seq_event*)malloc(sizeof(t_pydaw_seq_event));
    f_result->type = PYDAW_EVENT_CONTROLLER;
    f_result->param = a_cc_num;
    f_result->value = a_cc_val;
    f_result->start = a_start;

    return f_result;
}

t_pydaw_seq_event * g_pypitchbend_get(float a_start, float a_value)
{
    t_pydaw_seq_event * f_result =
        (t_pydaw_seq_event*)malloc(sizeof(t_pydaw_seq_event));
    f_result->type = PYDAW_EVENT_PITCHBEND;
    f_result->start = a_start;
    f_result->value = a_value;

    return f_result;
}

void g_pysong_get(t_pydaw_data* self, int a_lock)
{
    t_pysong * f_result = (t_pysong*)malloc(sizeof(t_pysong));

    int f_i = 0;

    while(f_i < PYDAW_MAX_REGION_COUNT)
    {
        f_result->regions[f_i] = 0;
        f_result->regions_atm[f_i] = 0;
        f_result->audio_items[f_i] = 0;
        f_result->per_audio_item_fx[f_i] = 0;
        f_i++;
    }

    char f_full_path[2048];
    sprintf(f_full_path, "%s/projects/edmnext/song.txt",
        self->project_folder);

    if(i_pydaw_file_exists(f_full_path))
    {
        f_i = 0;

        t_2d_char_array * f_current_string =
            g_get_2d_array_from_file(f_full_path, PYDAW_LARGE_STRING);

        while(f_i < PYDAW_MAX_REGION_COUNT)
        {
            char * f_pos_char = c_iterate_2d_char_array(f_current_string);
            if(f_current_string->eof)
            {
                break;
            }
            int f_pos = atoi(f_pos_char);
            char * f_region_char = c_iterate_2d_char_array(f_current_string);
            int f_uid = atoi(f_region_char);
            f_result->regions[f_pos] = g_pyregion_get(self, f_uid);
            f_result->regions[f_pos]->uid = f_uid;
            f_result->regions_atm[f_pos] = g_atm_region_get(self, f_uid);
            //v_pydaw_audio_items_free(self->audio_items);
            f_result->audio_items[f_pos] =
                v_audio_items_load_all(self, f_uid);
            f_result->per_audio_item_fx[f_pos] =
                g_paif_region_open(self, f_uid);

            free(f_pos_char);
            free(f_region_char);
            f_i++;
        }

        g_free_2d_char_array(f_current_string);
    }

    t_pysong * f_old = self->pysong;

    if(a_lock)
    {
        pthread_spin_lock(&self->main_lock);
    }

    self->pysong = f_result;

    if(a_lock)
    {
        pthread_spin_unlock(&self->main_lock);
    }

    if(f_old)
    {
        v_pysong_free(f_old);
    }

}


int i_get_song_index_from_region_uid(t_pydaw_data* self, int a_uid)
{
    int f_i = 0;

    while(f_i < PYDAW_MAX_REGION_COUNT)
    {
        if(self->pysong->regions[f_i])
        {
            if(a_uid == self->pysong->regions[f_i]->uid)
            {
                return f_i;
            }
        }
        f_i++;
    }
    return -1;
}

t_pydaw_atm_region * g_atm_region_get(t_pydaw_data * self, int a_uid)
{
    t_pydaw_atm_region * f_result = 0;

    char f_file[1024] = "\0";
    sprintf(f_file, "%s%i", self->region_atm_folder, a_uid);

    if(i_pydaw_file_exists(f_file))
    {
        printf("g_atm_region_get: loading a_file: \"%s\"\n", f_file);

        lmalloc((void**)&f_result, sizeof(t_pydaw_atm_region));

        int f_i = 0;

        while(f_i < PYDAW_TRACK_COUNT_ALL)
        {
            int f_i2 = 0;
            while(f_i2 < MAX_PLUGIN_TOTAL_COUNT)
            {
                f_result->tracks[f_i].plugins[f_i2].point_count = 0;
                f_result->tracks[f_i].plugins[f_i2].points = 0;
                f_i2++;
            }
            f_i++;
        }

        t_2d_char_array * f_current_string = g_get_2d_array_from_file(f_file,
            PYDAW_XLARGE_STRING); //TODO:  1MB big enough???

        int f_pos = 0;

        while(1)
        {
            char * f_first = c_iterate_2d_char_array(f_current_string);
            if(f_current_string->eof)
            {
                free(f_first);
                break;
            }

            if(f_first[0] == 'p')
            {
                free(f_first);
                char * f_track_char = c_iterate_2d_char_array(f_current_string);
                int f_track = atoi(f_track_char);
                free(f_track_char);

                char * f_index_char = c_iterate_2d_char_array(f_current_string);
                int f_index = atoi(f_index_char);
                free(f_index_char);

                char * f_count_char = c_iterate_2d_char_array(f_current_string);
                int f_count = atoi(f_count_char);
                free(f_count_char);

                assert(f_count >= 1 && f_count < 100000);  //sanity check

                f_result->tracks[f_track].plugins[f_index].point_count =
                    f_count;
                lmalloc(
                    (void**)&f_result->tracks[f_track].plugins[f_index].points,
                    sizeof(t_pydaw_atm_point) * f_count);
                f_pos = 0;
            }
            else
            {
                int f_track = atoi(f_first);
                free(f_first);

                char * f_bar_char = c_iterate_2d_char_array(f_current_string);
                int f_bar = atoi(f_bar_char);
                free(f_bar_char);

                char * f_beat_char = c_iterate_2d_char_array(f_current_string);
                float f_beat = atof(f_beat_char);
                free(f_beat_char);

                char * f_port_char = c_iterate_2d_char_array(f_current_string);
                int f_port = atoi(f_port_char);
                free(f_port_char);

                char * f_val_char = c_iterate_2d_char_array(f_current_string);
                float f_val = atof(f_val_char);
                free(f_val_char);

                char * f_index_char = c_iterate_2d_char_array(f_current_string);
                int f_index = atoi(f_index_char);
                free(f_index_char);

                char * f_plugin_char =
                    c_iterate_2d_char_array(f_current_string);
                int f_plugin = atoi(f_plugin_char);
                free(f_plugin_char);

                assert(f_pos <
                    f_result->tracks[f_track].plugins[f_index].point_count);

                assert(f_result->tracks[f_track].plugins[f_index].points);

                t_pydaw_atm_point * f_point =
                    &f_result->tracks[f_track].plugins[f_index].points[f_pos];
                f_point->track = f_track;

                f_point->track = f_track;
                f_point->bar = f_bar;
                f_point->beat = f_beat;
                f_point->port = f_port;
                f_point->val = f_val;
                f_point->index = f_index;
                f_point->plugin = f_plugin;

                f_pos++;
            }
        }

        g_free_2d_char_array(f_current_string);
    }

    return f_result;
}

void v_atm_region_free(t_pydaw_atm_region * self)
{
    int f_i = 0;
    while(f_i < PYDAW_TRACK_COUNT_ALL)
    {
        int f_i2 = 0;
        while(f_i2 < MAX_PLUGIN_TOTAL_COUNT)
        {
            if(self->tracks[f_i].plugins[f_i2].point_count)
            {
                free(self->tracks[f_i].plugins[f_i2].points);
            }
            f_i2++;
        }
        f_i++;
    }
    free(self);
}

t_pyregion * g_pyregion_get(t_pydaw_data* self, int a_uid)
{
    t_pyregion * f_result = (t_pyregion*)malloc(sizeof(t_pyregion));

    f_result->alternate_tempo = 0;
    f_result->tempo = 128.0f;
    f_result->region_length_bars = 0;
    f_result->region_length_beats = 0;
    f_result->bar_length = 0;
    f_result->uid = a_uid;
    f_result->not_yet_saved = 0;

    int f_i = 0;
    int f_i2 = 0;

    while(f_i < PYDAW_TRACK_COUNT_ALL)
    {
        f_i2 = 0;
        while(f_i2 < PYDAW_MAX_REGION_SIZE)
        {
            f_result->item_indexes[f_i][f_i2] = -1;
            f_i2++;
        }
        f_i++;
    }


    char f_full_path[PYDAW_TINY_STRING];
    sprintf(f_full_path, "%s%i", self->region_folder, a_uid);

    t_2d_char_array * f_current_string =
        g_get_2d_array_from_file(f_full_path, PYDAW_LARGE_STRING);

    f_i = 0;

    while(f_i < 264)
    {
        char * f_y_char = c_iterate_2d_char_array(f_current_string);
        if(f_current_string->eof)
        {
            free(f_y_char);
            break;
        }

        if(!strcmp("L", f_y_char))
        {
            free(f_y_char);
            char * f_bars_char = c_iterate_2d_char_array(f_current_string);
            int f_bars = atoi(f_bars_char);
            f_result->region_length_bars = f_bars;
            free(f_bars_char);

            char * f_beats_char = c_iterate_2d_char_array(f_current_string);
            int f_beats = atoi(f_beats_char);
            f_result->region_length_beats = f_beats;
            free(f_beats_char);
            continue;
        }
        if(!strcmp("T", f_y_char))  //per-region tempo, not yet implemented
        {
            free(f_y_char);
            char * f_tempo_char = c_iterate_2d_char_array(f_current_string);
            f_result->alternate_tempo = 1;
            f_result->tempo = atof(f_tempo_char);
            free(f_tempo_char);

            char * f_null_char = c_iterate_2d_char_array(f_current_string);
            free(f_null_char);
            continue;
        }
        //per-region bar length in beats, not yet implemented
        if(!strcmp("B", f_y_char))
        {
            free(f_y_char);
            char * f_len_char = c_iterate_2d_char_array(f_current_string);
            f_result->bar_length = atoi(f_len_char);
            free(f_len_char);

            char * f_null_char = c_iterate_2d_char_array(f_current_string);
            free(f_null_char);
            continue;
        }

        int f_y = atoi(f_y_char);
        free(f_y_char);

        char * f_x_char = c_iterate_2d_char_array(f_current_string);
        int f_x = atoi(f_x_char);
        free(f_x_char);

        char * f_item_uid = c_iterate_2d_char_array(f_current_string);
        assert(f_y < PYDAW_TRACK_COUNT_ALL);
        assert(f_x < PYDAW_MAX_REGION_SIZE);
        f_result->item_indexes[f_y][f_x] = atoi(f_item_uid);
        assert(self->item_pool[f_result->item_indexes[f_y][f_x]]);
        free(f_item_uid);

        f_i++;
    }

    g_free_2d_char_array(f_current_string);

    //v_pydaw_assert_memory_integrity(self);

    return f_result;
}


void g_pyitem_get(t_pydaw_data* self, int a_uid)
{
    t_pyitem * f_result;
    lmalloc((void**)&f_result, sizeof(t_pyitem));

    f_result->event_count = 0;
    f_result->uid = a_uid;

    char f_full_path[512];
    sprintf(f_full_path, "%s%i", self->item_folder, a_uid);

    t_2d_char_array * f_current_string = g_get_2d_array_from_file(f_full_path,
            PYDAW_LARGE_STRING);

    int f_i = 0;

    while(f_i < PYDAW_MAX_EVENTS_PER_ITEM_COUNT)
    {
        char * f_type = c_iterate_2d_char_array(f_current_string);

        if(f_current_string->eof)
        {
            break;
        }

        char * f_start = c_iterate_2d_char_array(f_current_string);

        if(!strcmp(f_type, "n"))  //note
        {
            char * f_length = c_iterate_2d_char_array(f_current_string);
            char * f_note = c_iterate_2d_char_array(f_current_string);
            char * f_vel = c_iterate_2d_char_array(f_current_string);
            assert((f_result->event_count) < PYDAW_MAX_EVENTS_PER_ITEM_COUNT);
            f_result->events[(f_result->event_count)] = g_pynote_get(
                    atoi(f_note), atoi(f_vel), atof(f_start), atof(f_length));
            f_result->event_count = (f_result->event_count) + 1;

            free(f_length);
            free(f_note);
            free(f_vel);
        }
        else if(!strcmp(f_type, "c")) //cc
        {
            char * f_cc_num = c_iterate_2d_char_array(f_current_string);
            char * f_cc_val = c_iterate_2d_char_array(f_current_string);

            f_result->events[(f_result->event_count)] = g_pycc_get(
                    atoi(f_cc_num), atof(f_cc_val), atof(f_start));
            f_result->event_count = (f_result->event_count) + 1;

            free(f_cc_num);
            free(f_cc_val);
        }
        else if(!strcmp(f_type, "p")) //pitchbend
        {
            char * f_pb_val_char = c_iterate_2d_char_array(f_current_string);
            float f_pb_val = atof(f_pb_val_char) * 8192.0f;

            f_result->events[(f_result->event_count)] = g_pypitchbend_get(
                    atof(f_start), f_pb_val);
            f_result->event_count = (f_result->event_count) + 1;

            free(f_pb_val_char);
        }
        else
        {
            printf("Invalid event type %s\n", f_type);
        }

        free(f_start);
        free(f_type);
        f_i++;
    }

    f_result->rec_event_count = f_result->event_count;

    g_free_2d_char_array(f_current_string);

    if(self->item_pool[a_uid])
    {
        free(self->item_pool[a_uid]);
    }

    self->item_pool[a_uid] = f_result;
}

t_pytrack * g_pytrack_get(int a_track_num, float a_sr)
{
    int f_i = 0;

    t_pytrack * f_result;
    lmalloc((void**)&f_result, sizeof(t_pytrack));

    f_result->track_num = a_track_num;
    f_result->channels = 2;
    f_result->extern_midi = 0;
    f_result->extern_midi_count = &ZERO;
    f_result->midi_device = 0;

    pthread_spin_init(&f_result->lock, 0);

    lmalloc((void**)&f_result->buffers, (sizeof(float*) * f_result->channels));

    while(f_i < f_result->channels)
    {
        buffer_alloc((void**)&f_result->buffers[f_i],
            (sizeof(float) * FRAMES_PER_BUFFER));
        f_i++;
    }

    v_pydaw_zero_buffer(f_result->buffers, FRAMES_PER_BUFFER);

    f_result->mute = 0;
    f_result->solo = 0;
    f_result->event_buffer = (t_pydaw_seq_event*)malloc(
            sizeof(t_pydaw_seq_event) * PYDAW_MAX_EVENT_BUFFER_SIZE);
    f_result->bus_count = 0;
    f_result->bus_counter = 0;

    f_i = 0;

    while(f_i < PYDAW_MAX_EVENT_BUFFER_SIZE)
    {
        v_pydaw_ev_clear(&f_result->event_buffer[f_i]);
        f_i++;
    }

    f_i = 0;
    while(f_i < MAX_PLUGIN_TOTAL_COUNT)
    {
        f_result->plugins[f_i] = 0;
        f_i++;
    }

    f_i = 0;

    while(f_i < PYDAW_MIDI_NOTE_COUNT)
    {
        f_result->note_offs[f_i] = -1;
        f_i++;
    }

    f_result->period_event_index = 0;

    f_result->peak_meter = g_pkm_get();

    f_result->fade_env = g_rmp_get_ramp_env(a_sr);
    v_rmp_set_time(f_result->fade_env, 0.03f);
    f_result->fade_state = 0;

    f_result->osc_cursor_message = (char*)malloc(sizeof(char) * 1024);

    f_result->status = STATUS_NOT_PROCESSED;

    return f_result;
}


void pydaw_osc_error(int num, const char *msg, const char *path)
{
    fprintf(stderr, "PyDAW: liblo server error %d in path %s: %s\n",
	    num, path, msg);
}

t_pydaw_data * g_pydaw_data_get(float a_sr, t_midi_device_list * a_midi_devices)
{
    t_pydaw_data * f_result = (t_pydaw_data*)malloc(sizeof(t_pydaw_data));

    pthread_spin_init(&f_result->main_lock, 0);
    pthread_mutex_init(&f_result->audio_inputs_mutex, NULL);

    pthread_spin_init(&f_result->ui_spinlock, 0);

    f_result->midi_devices = a_midi_devices;
    f_result->midi_learn = 0;
    f_result->sample_rate = a_sr;
    f_result->current_sample = 0;
    f_result->current_bar = 0;
    f_result->current_region = 0;
    f_result->playback_cursor = 0.0f;
    f_result->playback_inc = 0.0f;

    f_result->osc_queue_index = 0;

    f_result->overdub_mode = 0;
    f_result->loop_mode = 0;
    f_result->item_folder = (char*)malloc(sizeof(char) * 1024);
    f_result->project_folder = (char*)malloc(sizeof(char) * 1024);
    f_result->region_folder = (char*)malloc(sizeof(char) * 1024);
    f_result->audio_folder = (char*)malloc(sizeof(char) * 1024);
    f_result->audio_tmp_folder = (char*)malloc(sizeof(char) * 1024);
    f_result->plugins_folder = (char*)malloc(sizeof(char) * 1024);
    f_result->samples_folder = (char*)malloc(sizeof(char) * 1024);
    f_result->wav_pool_file = (char*)malloc(sizeof(char) * 1024);
    f_result->region_audio_folder = (char*)malloc(sizeof(char) * 1024);
    f_result->region_atm_folder = (char*)malloc(sizeof(char) * 1024);
    f_result->per_audio_item_fx_folder = (char*)malloc(sizeof(char) * 1024);
    f_result->tracks_folder = (char*)malloc(sizeof(char) * 1024);

    f_result->osc_cursor_message = (char*)malloc(sizeof(char) * 1024);

    f_result->playback_mode = 0;
    f_result->pysong = NULL;
    f_result->is_soloed = 0;
    f_result->suppress_new_audio_items = 0;

    f_result->ml_current_period_beats = 0.0f;
    f_result->ml_next_period_beats = 0.0f;
    f_result->ml_next_playback_cursor = 0.0f;
    f_result->ml_sample_period_inc = 0.0f;
    f_result->ml_sample_period_inc_beats = 0.0f;

    f_result->ml_current_region = 0;
    f_result->ml_next_region = 0;
    f_result->ml_next_bar = 0;
    f_result->ml_next_beat = 0.0;
    f_result->ml_starting_new_bar = 0;
    f_result->ml_is_looping = 0;

    f_result->is_offline_rendering = 0;

    f_result->default_region_length_bars = 8;
    f_result->default_region_length_beats = 0;
    f_result->default_bar_length = 4;

    f_result->input_buffers_active = 0;

    f_result->wav_pool = g_wav_pool_get(a_sr);
    f_result->ab_wav_item = 0;
    f_result->ab_audio_item = g_pydaw_audio_item_get(f_result->sample_rate);
    f_result->ab_mode = 0;
    f_result->is_ab_ing = 0;
    f_result->preview_wav_item = 0;
    f_result->preview_audio_item =
            g_pydaw_audio_item_get(f_result->sample_rate);
    f_result->preview_start = 0.0f;
    f_result->preview_amp_lin = 1.0f;
    f_result->is_previewing = 0;
    f_result->preview_max_sample_count = ((int)(a_sr)) * 30;
    f_result->routing_graph = 0;

    int f_i = 0;
    int f_track_total = 0;

    while(f_i < PYDAW_AUDIO_INPUT_TRACK_COUNT)
    {
        f_result->audio_inputs[f_i] = g_pyaudio_input_get(a_sr);
        f_result->audio_inputs[f_i]->input_port[0] = f_i * 2;
        f_result->audio_inputs[f_i]->input_port[1] = (f_i * 2) + 1;
        f_i++;
    }

    f_i = 0;

    while(f_i < PYDAW_TRACK_COUNT_ALL)
    {
        f_result->track_pool_all[f_track_total] = g_pytrack_get(f_i, a_sr);
        f_i++;
        f_track_total++;
    }

    f_i = 0;

    while(f_i < PYDAW_MAX_AUDIO_ITEM_COUNT)
    {
        f_result->audio_glue_indexes[f_i] = 0;
        f_i++;
    }

    f_i = 0;

    while(f_i < PYDAW_MAX_ITEM_COUNT)
    {
        f_result->item_pool[f_i] = 0;
        f_i++;
    }

    f_i = 0;
    while(f_i < MAX_PLUGIN_POOL_COUNT)
    {
        f_result->plugin_pool[f_i] = 0;
        f_i++;
    }

    /* Create OSC thread */
    char *tmp;

    char osc_path_tmp[1024];

    f_result->serverThread = lo_server_thread_new(NULL, pydaw_osc_error);
    snprintf(osc_path_tmp, 31, "/dssi/pydaw_plugins");
    tmp = lo_server_thread_get_url(f_result->serverThread);
    f_result->osc_url = (char *)malloc(strlen(tmp) + strlen(osc_path_tmp));
    sprintf(f_result->osc_url, "%s%s", tmp, osc_path_tmp + 1);
    free(tmp);

    f_result->uiTarget = lo_address_new_from_url("osc.udp://localhost:30321/");

    return f_result;
}

void v_pydaw_activate_osc_thread(t_pydaw_data * self,
        lo_method_handler osc_message_handler)
{
    lo_server_thread_add_method(self->serverThread, NULL, NULL,
            osc_message_handler, NULL);
    lo_server_thread_start(self->serverThread);
}

void v_pydaw_open_track(t_pydaw_data * self, int a_index)
{
    char f_file_name[1024];
    sprintf(f_file_name, "%s/%i",
        self->tracks_folder, a_index);

    if(i_pydaw_file_exists(f_file_name))
    {
        printf("v_pydaw_open_tracks:  File exists %s , loading\n", f_file_name);

        t_2d_char_array * f_2d_array = g_get_2d_array_from_file(f_file_name,
                PYDAW_LARGE_STRING);

        while(1)
        {
            char * f_first_str = c_iterate_2d_char_array(f_2d_array);

            if(f_2d_array->eof)
            {
                free(f_first_str);
                break;
            }

            if(f_first_str[0] == 'p')  //plugin
            {
                char * f_index_str = c_iterate_2d_char_array(f_2d_array);
                char * f_plugin_index_str = c_iterate_2d_char_array(f_2d_array);
                char * f_plugin_uid_str = c_iterate_2d_char_array(f_2d_array);
                char * f_mute_str = c_iterate_2d_char_array(f_2d_array);
                char * f_solo_str = c_iterate_2d_char_array(f_2d_array);
                char * f_power_str = c_iterate_2d_char_array(f_2d_array);

                int f_index = atoi(f_index_str);
                free(f_index_str);
                int f_plugin_index = atoi(f_plugin_index_str);
                free(f_plugin_index_str);
                int f_plugin_uid = atoi(f_plugin_uid_str);
                free(f_plugin_uid_str);
                //int f_mute = atoi(f_mute_str);
                free(f_mute_str);
                //int f_solo = atoi(f_solo_str);
                free(f_solo_str);
                int f_power = atoi(f_power_str);
                free(f_power_str);

                v_pydaw_set_plugin_index(self, a_index, f_index,
                    f_plugin_index, f_plugin_uid, f_power, 0);
                //TODO:  Mute, solo and power
            }
            else
            {
                printf("Invalid track identifier '%s'\n", f_first_str);
                assert(0);
            }

            free(f_first_str);
        }
    }
    else
    {
        printf("%s does not exist, not loading anything for track %i\n",
            f_file_name, a_index);
    }
}


void v_pydaw_open_tracks(t_pydaw_data * self)
{
    char f_file_name[1024];
    sprintf(f_file_name, "%s/projects/edmnext/tracks.txt",
        self->project_folder);

    if(i_pydaw_file_exists(f_file_name))
    {
        printf("v_pydaw_open_tracks:  File exists %s , loading\n", f_file_name);

        t_2d_char_array * f_2d_array = g_get_2d_array_from_file(f_file_name,
                PYDAW_LARGE_STRING);

        while(1)
        {
            char * f_track_index_str = c_iterate_2d_char_array(f_2d_array);

            if(f_2d_array->eof)
            {
                free(f_track_index_str);
                break;
            }

            char * f_solo_str = c_iterate_2d_char_array(f_2d_array);
            char * f_mute_str = c_iterate_2d_char_array(f_2d_array);
            char * f_track_pos = c_iterate_2d_char_array(f_2d_array);  //ignored
            char * f_name_str = c_iterate_2d_char_array(f_2d_array); //ignored
            free(f_track_pos);
            free(f_name_str);

            int f_track_index = atoi(f_track_index_str);
            free(f_track_index_str);
            assert(f_track_index >= 0 && f_track_index < PYDAW_TRACK_COUNT_ALL);

            int f_solo = atoi(f_solo_str);
            free(f_solo_str);
            assert(f_solo == 0 || f_solo == 1);

            int f_mute = atoi(f_mute_str);
            free(f_mute_str);
            assert(f_mute == 0 || f_mute == 1);

            v_pydaw_open_track(self, f_track_index);

            self->track_pool_all[f_track_index]->solo = f_solo;
            self->track_pool_all[f_track_index]->mute = f_mute;
        }

        g_free_2d_char_array(f_2d_array);
    }
    else   //ensure everything is closed...
    {
        int f_i = 0;

        while(f_i < PYDAW_TRACK_COUNT_ALL)
        {
            self->track_pool_all[f_i]->solo = 0;
            self->track_pool_all[f_i]->mute = 0;
            v_pydaw_open_track(self, f_i);
            f_i++;
        }
    }
}

void v_open_project(t_pydaw_data* self, const char* a_project_folder,
        int a_first_load)
{
    clock_t f_start = clock();

    sprintf(self->project_folder, "%s", a_project_folder);
    sprintf(self->item_folder, "%s/projects/edmnext/items/",
        self->project_folder);
    sprintf(self->region_folder, "%s/projects/edmnext/regions/",
        self->project_folder);
    sprintf(self->region_audio_folder, "%s/projects/edmnext/regions_audio/",
        self->project_folder);
    sprintf(self->region_atm_folder, "%s/projects/edmnext/regions_atm/",
        self->project_folder);
    sprintf(self->audio_folder, "%s/audio/files", self->project_folder);
    sprintf(self->audio_tmp_folder, "%s/audio/files/tmp/",
        self->project_folder);
    sprintf(self->plugins_folder, "%s/projects/edmnext/plugins/",
        self->project_folder);
    sprintf(self->samples_folder, "%s/audio/samples",
        self->project_folder);  //No trailing slash on this one
    sprintf(self->wav_pool->samples_folder, "%s", self->samples_folder);
    sprintf(self->wav_pool_file, "%s/audio/wavs.txt",
        self->project_folder);
    sprintf(self->per_audio_item_fx_folder,
        "%s/projects/edmnext/audio_per_item_fx/", self->project_folder);
    sprintf(self->tracks_folder, "%s/projects/edmnext/tracks",
        self->project_folder);

    int f_i = 0;

    while(f_i < PYDAW_MAX_ITEM_COUNT)
    {
        if(self->item_pool[f_i])
        {
            free(self->item_pool[f_i]);
            self->item_pool[f_i] = 0;
        }
        f_i++;
    }

    char f_song_file[1024];
    sprintf(f_song_file,
        "%s/projects/edmnext/song.txt", self->project_folder);

    struct stat f_proj_stat;
    stat((self->project_folder), &f_proj_stat);
    struct stat f_item_stat;
    stat((self->item_folder), &f_item_stat);
    struct stat f_reg_stat;
    stat((self->region_folder), &f_reg_stat);
    struct stat f_song_file_stat;
    stat(f_song_file, &f_song_file_stat);

    if(a_first_load && i_pydaw_file_exists(self->wav_pool_file))
    {
        v_wav_pool_add_items(self->wav_pool,
                self->wav_pool_file);
    }

    //TODO:  This should be moved to a separate function
    char f_transport_file[1024];
    sprintf(f_transport_file, "%s/projects/edmnext/transport.txt",
            self->project_folder);

    if(i_pydaw_file_exists(f_transport_file))
    {
        printf("v_open_project:  Found transport file, setting tempo\n");

        t_2d_char_array * f_2d_array = g_get_2d_array_from_file(
                f_transport_file, PYDAW_LARGE_STRING);
        char * f_tempo_str = c_iterate_2d_char_array(f_2d_array);
        float f_tempo = atof(f_tempo_str);
        free(f_tempo_str);

        assert(f_tempo > 30.0f && f_tempo < 300.0f);
        v_set_tempo(self, f_tempo);
        g_free_2d_char_array(f_2d_array);
    }
    else  //No transport file, set default tempo
    {
        printf("No transport file found, defaulting to 128.0 BPM\n");
        v_set_tempo(self, 128.0f);
    }

    if(S_ISDIR(f_proj_stat.st_mode) &&
        S_ISDIR(f_item_stat.st_mode) &&
        S_ISDIR(f_reg_stat.st_mode) &&
        S_ISREG(f_song_file_stat.st_mode))
    {
        t_dir_list * f_item_dir_list =
                g_get_dir_list(self->item_folder);
        f_i = 0;

        while(f_i < f_item_dir_list->dir_count)
        {
            g_pyitem_get(self, atoi(f_item_dir_list->dir_list[f_i]));
            f_i++;
        }

        g_pysong_get(self, 0);

        if(a_first_load)
        {
            v_pydaw_open_tracks(self);
        }
    }
    else
    {
        printf("Song file and project directory structure not found, not "
                "loading project.  This is to be expected if launching PyDAW "
                "for the first time\n");
        //Loads empty...  TODO:  Make this a separate function for getting an
        //empty pysong or loading a file into one...
        g_pysong_get(self, 0);
    }

    v_pydaw_update_track_send(self, 0);

    //v_pydaw_update_audio_inputs(self);

    v_pydaw_set_is_soloed(self);

    v_pydaw_print_benchmark("v_open_project", f_start);
}

/* void v_set_playback_mode(t_pydaw_data * self,
 * int a_mode, //
 * int a_region, //The region index to start playback on
 * int a_bar) //The bar index (with a_region) to start playback on
 */
void v_set_playback_mode(t_pydaw_data * self, int a_mode,
        int a_region, int a_bar, int a_lock)
{
    switch(a_mode)
    {
        case 0: //stop
        {
            int f_i = 0;
            t_pytrack * f_track;

            if(a_lock)
            {
                pthread_spin_lock(&self->main_lock);
            }

            self->suppress_new_audio_items = 1;
            //Fade out the playing audio tracks
            if(self->pysong->audio_items[self->current_region])
            {
                while(f_i < PYDAW_MAX_AUDIO_ITEM_COUNT)
                {
                    if(self->pysong->audio_items[
                            self->current_region]->items[f_i])
                    {
                        v_adsr_release(self->pysong->audio_items[
                            self->current_region]->items[f_i]->adsr);
                    }
                    f_i++;
                }
            }

            self->suppress_new_audio_items = 0;
            self->playback_mode = a_mode;

            f_i = 0;

            t_pydaw_plugin * f_plugin;

            while(f_i < PYDAW_TRACK_COUNT_ALL)
            {
                int f_i2 = 0;
                f_track = self->track_pool_all[f_i];

                f_track->period_event_index = 0;

                while(f_i2 < MAX_PLUGIN_TOTAL_COUNT)
                {
                    f_plugin = f_track->plugins[f_i2];
                    if(f_plugin)
                    {
                        f_plugin->descriptor->on_stop(f_plugin->PYFX_handle);
                    }
                    f_i2++;
                }

                f_track->item_event_index = 0;

                f_i++;
            }

            if(a_lock)
            {
                pthread_spin_unlock(&self->main_lock);
            }

        }
            break;
        case 1:  //play
        {
            if(a_lock)
            {
                pthread_spin_lock(&self->main_lock);
            }

            v_set_playback_cursor(self, a_region, a_bar);
            if(self->ab_wav_item)
            {
                self->is_ab_ing = self->ab_mode;
                if(self->is_ab_ing)
                {
                    v_ifh_retrigger(
                        self->ab_audio_item->sample_read_head,
                        self->ab_audio_item->sample_start_offset);
                    v_adsr_retrigger(self->ab_audio_item->adsr);
                    v_svf_reset(self->ab_audio_item->lp_filter);
                }
            }

            self->playback_mode = a_mode;

            if(a_lock)
            {
                pthread_spin_unlock(&self->main_lock);
            }

            break;
        }
        case 2:  //record
            if(self->playback_mode == PYDAW_PLAYBACK_MODE_REC)
            {
                return;
            }
            if(a_lock)
            {
                pthread_spin_lock(&self->main_lock);
            }

            v_set_playback_cursor(self, a_region, a_bar);

            self->playback_mode = a_mode;

            if(a_lock)
            {
                pthread_spin_unlock(&self->main_lock);
            }
            break;
    }

}


/*Load/Reload samples from file...*/
t_pydaw_audio_items * v_audio_items_load_all(t_pydaw_data * self,
        int a_region_uid)
{
    t_pydaw_audio_items * f_result =
            g_pydaw_audio_items_get(self->sample_rate);
    char f_file[1024] = "\0";
    sprintf(f_file, "%s%i", self->region_audio_folder, a_region_uid);

    if(i_pydaw_file_exists(f_file))
    {
        printf("v_audio_items_load_all: loading a_file: \"%s\"\n", f_file);
        int f_i = 0;

        t_2d_char_array * f_current_string = g_get_2d_array_from_file(f_file,
                PYDAW_LARGE_STRING);

        while(f_i < PYDAW_MAX_AUDIO_ITEM_COUNT)
        {
            t_pydaw_audio_item * f_new =
                    g_audio_item_load_single(self->sample_rate,
                    f_current_string, 0, self->wav_pool, 0);
            if(!f_new)  //EOF'd...
            {
                break;
            }

            int f_global_index = f_new->audio_track_output;

            f_result->indexes[f_global_index][
                f_result->index_counts[f_global_index]] = f_new->index;
            f_result->index_counts[f_global_index] += 1;
            f_result->items[f_new->index] = f_new;
            f_i++;
        }

        g_free_2d_char_array(f_current_string);
    }
    else
    {
        printf("Error:  v_audio_items_load_all:  a_file: \"%s\" "
                "does not exist\n", f_file);
        assert(0);
    }

    return f_result;
}


void v_set_playback_cursor(t_pydaw_data * self, int a_region, int a_bar)
{
    self->current_bar = a_bar;
    self->current_region = a_region;
    self->playback_cursor = 0.0f;

    v_pydaw_reset_audio_item_read_heads(self, a_region, a_bar);

    int f_i = 0;

    while(f_i < PYDAW_TRACK_COUNT_ALL)
    {
        self->track_pool_all[f_i]->item_event_index = 0;
        if((self->is_soloed && !self->track_pool_all[f_i]->solo) ||
            (self->track_pool_all[f_i]->mute))
        {
            self->track_pool_all[f_i]->fade_state = FADE_STATE_FADED;
        }
        f_i++;
    }

    f_i = 0;

    while(f_i < MAX_PLUGIN_TOTAL_COUNT)
    {
        if(self->plugin_pool[f_i])
        {
            self->plugin_pool[f_i]->atm_pos = 0;
        }
        f_i++;
    }
}

void v_pydaw_set_is_soloed(t_pydaw_data * self)
{
    int f_i = 0;
    self->is_soloed = 0;

    while(f_i < PYDAW_TRACK_COUNT_ALL)
    {
        if(self->track_pool_all[f_i]->solo)
        {
            self->is_soloed = 1;
            break;
        }
        f_i++;
    }
}

void v_set_loop_mode(t_pydaw_data * self, int a_mode)
{
    self->loop_mode = a_mode;
}

void v_set_tempo(t_pydaw_data * self, float a_tempo)
{
    self->tempo = a_tempo;
    self->playback_inc = ( (1.0f/(self->sample_rate)) /
            (60.0f/(a_tempo * 0.25f)) );
    self->samples_per_beat = (self->sample_rate) /
            (a_tempo/60.0f);
}

void v_pydaw_update_audio_inputs(t_pydaw_data * self)
{
    char f_inputs_file[1024];
    sprintf(f_inputs_file, "%s/projects/edmnext/input.txt",
        self->project_folder);

    if(i_pydaw_file_exists(f_inputs_file))
    {
        int f_i = 0;
        t_2d_char_array * f_2d_array = g_get_2d_array_from_file(f_inputs_file,
                PYDAW_LARGE_STRING);

        pthread_mutex_lock(&self->audio_inputs_mutex);
        while(f_i < PYDAW_AUDIO_INPUT_TRACK_COUNT)
        {
            char * f_index_str = c_iterate_2d_char_array(f_2d_array);

            if(!strcmp(f_index_str, "\\"))
            {
                free(f_index_str);
                break;
            }

            int f_index = atoi(f_index_str);
            free(f_index_str);

            char * f_rec_str = c_iterate_2d_char_array(f_2d_array);
            int f_rec = atoi(f_rec_str);
            free(f_rec_str);

            char * f_vol_str = c_iterate_2d_char_array(f_2d_array);
            int f_vol = atoi(f_vol_str);
            free(f_vol_str);

            char * f_out_str = c_iterate_2d_char_array(f_2d_array);
            int f_out = atoi(f_out_str);
            free(f_out_str);

            char * f_in_str = c_iterate_2d_char_array(f_2d_array);
            free(f_in_str);  //Not used yet

            self->audio_inputs[f_index]->rec = f_rec;
            self->audio_inputs[f_index]->output_track = f_out;

            self->audio_inputs[f_index]->vol = f_vol;
            self->audio_inputs[f_index]->vol_linear =
                f_db_to_linear_fast(f_vol);

            char f_tmp_file_name[128];

            sprintf(f_tmp_file_name, "%s%i.wav", self->audio_tmp_folder,
                    f_index);

            v_pydaw_audio_input_record_set(self->audio_inputs[f_index],
                    f_tmp_file_name);

            f_i++;
        }
        pthread_mutex_unlock(&self->audio_inputs_mutex);
        g_free_2d_char_array(f_2d_array);
    }
    else
    {
        printf("%s not found, setting default values\n", f_inputs_file);
        pthread_mutex_lock(&self->audio_inputs_mutex);
        int f_i = 0;
        while(f_i < PYDAW_AUDIO_INPUT_TRACK_COUNT)
        {
            self->audio_inputs[f_i]->rec = 0;
            self->audio_inputs[f_i]->output_track = 0;

            self->audio_inputs[f_i]->vol = 0.0f;
            self->audio_inputs[f_i]->vol_linear = 1.0f;
            f_i++;
        }
        pthread_mutex_unlock(&self->audio_inputs_mutex);
    }
}

/*Count the number of beats between 2 points in time...*/
inline float v_pydaw_count_beats(t_pydaw_data * self,
        int a_start_region, int a_start_bar, float a_start_beat,
        int a_end_region, int a_end_bar, float a_end_beat)
{
    int f_bar_count = a_end_bar - a_start_bar;

    int f_i = a_start_region;
    int f_beat_total = 0;

    while(f_i < a_end_region)
    {
        if((self->pysong->regions[f_i]) &&
                (self->pysong->regions[f_i]->region_length_bars))
        {
            f_beat_total +=
                    self->pysong->regions[f_i]->region_length_bars * 4;
        }
        else
        {
            f_beat_total += (8 * 4);
        }
        f_i++;
    }

    f_beat_total += f_bar_count * 4;

    return ((float)(f_beat_total)) + (a_end_beat - a_start_beat);
}

void v_pydaw_offline_render_prep(t_pydaw_data * self)
{
    printf("Warming up plugins for offline rendering...\n");
    int f_i = 0;
    t_pytrack * f_track;
    t_pydaw_plugin * f_plugin;

    while(f_i < PYDAW_TRACK_COUNT_ALL)
    {
        f_track = self->track_pool_all[f_i];
        int f_i2 = 0;
        while(f_i2 < MAX_PLUGIN_TOTAL_COUNT)
        {
            f_plugin = f_track->plugins[f_i2];
            if(f_plugin && f_plugin->descriptor->offline_render_prep)
            {
                f_plugin->descriptor->offline_render_prep(
                    f_plugin->PYFX_handle);
            }
            f_i2++;
        }
        f_i++;
    }
    printf("Finished warming up plugins\n");
}

void v_pydaw_offline_render(t_pydaw_data * self, int a_start_region,
        int a_start_bar, int a_end_region,
        int a_end_bar, char * a_file_out, int a_is_audio_glue)
{
    pthread_spin_lock(&self->main_lock);
    self->is_offline_rendering = 1;
    pthread_spin_unlock(&self->main_lock);

    self->input_buffers_active = 0;
    int f_ab_old = self->ab_mode;
    self->ab_mode = 0;

    int f_bar_count = a_end_bar - a_start_bar;

    int f_i = a_start_region;
    int f_beat_total = 0;

    while(f_i < a_end_region)
    {
        if((self->pysong->regions[f_i]) &&
                (self->pysong->regions[f_i]->region_length_bars))
        {
            f_beat_total +=
                    self->pysong->regions[f_i]->region_length_bars * 4;
        }
        else
        {
            f_beat_total += (8 * 4);
        }
        f_i++;
    }

    f_beat_total += f_bar_count * 4;

    float f_sample_count =
        self->samples_per_beat * ((float)f_beat_total);
    long f_sample_count_long = (((long)f_sample_count) * 2) + 6000000;

    long f_size = 0;
    long f_block_size = (self->sample_count);

    float * f_output = (float*)malloc(sizeof(float) * (f_block_size * 2));

    long f_next_sample_block = 0;
    float * f_buffer0 = (float*)malloc(sizeof(float) * f_block_size);
    float * f_buffer1 = (float*)malloc(sizeof(float) * f_block_size);

    //We must set it back afterwards, or the UI will be wrong...
    int f_old_loop_mode = self->loop_mode;
    v_set_loop_mode(self, PYDAW_LOOP_MODE_OFF);

    v_set_playback_mode(self, PYDAW_PLAYBACK_MODE_PLAY,
            a_start_region, a_start_bar, 0);

    printf("\nOpening SNDFILE with sample rate %f\n",
            self->sample_rate);

    SF_INFO f_sf_info;
    f_sf_info.channels = 2;
    f_sf_info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
    f_sf_info.samplerate = (int)(self->sample_rate);

    SNDFILE * f_sndfile = sf_open(a_file_out, SFM_WRITE, &f_sf_info);

    printf("\nSuccessfully opened SNDFILE\n\n");

    clock_t f_start = clock();

    int f_current_bar = 999;  //For printing the current region/bar

    while(((self->current_region) < a_end_region) ||
            ((self->current_bar) < a_end_bar))
    {
        if(self->current_bar != f_current_bar)
        {
            f_current_bar = self->current_bar;
            printf("%i:%i\n", self->current_region,
                    self->current_bar);
        }

        f_i = 0;
        f_size = 0;

        while(f_i < f_block_size)
        {
            f_buffer0[f_i] = 0.0f;
            f_buffer1[f_i] = 0.0f;
            f_i++;
        }

        f_next_sample_block = (self->current_sample) + f_block_size;

        if(a_is_audio_glue)
        {
            v_pydaw_set_time_params(self, f_block_size);
            v_pydaw_audio_items_run(self, f_block_size, f_buffer0,
                    f_buffer1, -1, 1);
            v_pydaw_finish_time_params(self, 999999);
        }
        else
        {
            v_pydaw_run_main_loop(self, f_block_size,
                f_next_sample_block, f_buffer0, f_buffer1, 0);
        }

        self->current_sample = f_next_sample_block;

        f_i = 0;
        /*Interleave the samples...*/
        while(f_i < f_block_size)
        {
            f_output[f_size] = f_buffer0[f_i];
            f_size++;
            f_output[f_size] = f_buffer1[f_i];
            f_size++;
            f_i++;
        }

        sf_writef_float(f_sndfile, f_output, f_block_size);
    }

    v_pydaw_print_benchmark("v_pydaw_offline_render ", f_start);
    printf("f_size = %ld\nf_sample_count_long = %ld\n(f_sample_count_long"
            " - f_size) = %ld\n",
            f_size, f_sample_count_long, (f_sample_count_long - f_size));

    v_set_playback_mode(self, PYDAW_PLAYBACK_MODE_OFF, a_start_region,
            a_start_bar, 0);
    v_set_loop_mode(self, f_old_loop_mode);

    sf_close(f_sndfile);

    free(f_buffer0);
    free(f_buffer1);
    free(f_output);

    char f_tmp_finished[1024];

    sprintf(f_tmp_finished, "%s.finished", a_file_out);

    v_pydaw_write_to_file(f_tmp_finished, "finished");

    v_pydaw_panic(self);  //ensure all notes are off before returning

    pthread_spin_lock(&self->main_lock);
    self->is_offline_rendering = 0;
    pthread_spin_unlock(&self->main_lock);

    self->ab_mode = f_ab_old;
} __attribute__((optimize("-O0")))

void v_pydaw_we_export(t_pydaw_data * self, const char * a_file_out)
{
    pthread_spin_lock(&self->main_lock);
    self->is_offline_rendering = 1;
    pthread_spin_unlock(&self->main_lock);

    self->input_buffers_active = 0;

    long f_size = 0;
    long f_block_size = (self->sample_count);

    float * f_output = (float*)malloc(sizeof(float) * (f_block_size * 2));

    float * f_buffer0 = (float*)malloc(sizeof(float) * f_block_size);
    float * f_buffer1 = (float*)malloc(sizeof(float) * f_block_size);

    int f_old_ab_mode = self->ab_mode;
    self->ab_mode = 1;

    v_set_playback_mode(self, PYDAW_PLAYBACK_MODE_PLAY,
            self->current_region, self->current_bar, 0);

    printf("\nOpening SNDFILE with sample rate %f\n",
            self->sample_rate);

    SF_INFO f_sf_info;
    f_sf_info.channels = 2;
    f_sf_info.format = SF_FORMAT_WAV | SF_FORMAT_FLOAT;
    f_sf_info.samplerate = (int)(self->sample_rate);

    SNDFILE * f_sndfile = sf_open(a_file_out, SFM_WRITE, &f_sf_info);

    printf("\nSuccessfully opened SNDFILE\n\n");

    clock_t f_start = clock();

    while((self->ab_audio_item->sample_read_head->whole_number) <
            (self->ab_audio_item->sample_end_offset))
    {
        int f_i = 0;
        f_size = 0;

        while(f_i < f_block_size)
        {
            f_buffer0[f_i] = 0.0f;
            f_buffer1[f_i] = 0.0f;
            f_i++;
        }

        v_pydaw_run_wave_editor(self, f_block_size,
                f_buffer0, f_buffer1);

        f_i = 0;
        /*Interleave the samples...*/
        while(f_i < f_block_size)
        {
            f_output[f_size] = f_buffer0[f_i];
            f_size++;
            f_output[f_size] = f_buffer1[f_i];
            f_size++;
            f_i++;
        }

        sf_writef_float(f_sndfile, f_output, f_block_size);
    }

    v_pydaw_print_benchmark("v_pydaw_offline_render ", f_start);
    printf("f_size = %ld\n", f_size);

    v_set_playback_mode(self, PYDAW_PLAYBACK_MODE_OFF,
            self->current_region, self->current_bar, 0);

    sf_close(f_sndfile);

    free(f_buffer0);
    free(f_buffer1);
    free(f_output);

    char f_tmp_finished[1024];

    sprintf(f_tmp_finished, "%s.finished", a_file_out);

    v_pydaw_write_to_file(f_tmp_finished, "finished");

    pthread_spin_lock(&self->main_lock);
    self->is_offline_rendering = 0;
    pthread_spin_unlock(&self->main_lock);

    self->ab_mode = f_old_ab_mode;
}

void v_pydaw_set_ab_mode(t_pydaw_data * self, int a_mode)
{
    pthread_spin_lock(&self->main_lock);

    self->ab_mode = a_mode;

    if(!a_mode)
    {
        self->is_ab_ing = 0;
    }

    pthread_spin_unlock(&self->main_lock);
}

void v_pydaw_set_ab_file(t_pydaw_data * self, const char * a_file)
{
    t_wav_pool_item * f_result = g_wav_pool_item_get(0, a_file,
            self->sample_rate);

    if(i_wav_pool_item_load(f_result))
    {
        pthread_spin_lock(&self->main_lock);

        t_wav_pool_item * f_old = self->ab_wav_item;
        self->ab_wav_item = f_result;

        if(!f_result)
        {
            self->ab_mode = 0;
        }

        self->ab_audio_item->ratio =
                self->ab_wav_item->ratio_orig;

        pthread_spin_unlock(&self->main_lock);

        if(f_old)
        {
            v_wav_pool_item_free(f_old);
        }
    }
    else
    {
        printf("i_wav_pool_item_load failed in v_pydaw_set_ab_file\n");
    }
}


void v_pydaw_set_wave_editor_item(t_pydaw_data * self,
        const char * a_val)
{
    t_2d_char_array * f_current_string = g_get_2d_array(PYDAW_MEDIUM_STRING);
    sprintf(f_current_string->array, "%s", a_val);
    t_pydaw_audio_item * f_old = self->ab_audio_item;
    t_pydaw_audio_item * f_result = g_audio_item_load_single(
            self->sample_rate, f_current_string, 0, 0,
            self->ab_wav_item);

    pthread_spin_lock(&self->main_lock);
    self->ab_audio_item = f_result;
    pthread_spin_unlock(&self->main_lock);

    g_free_2d_char_array(f_current_string);
    if(f_old)
    {
        v_pydaw_audio_item_free(f_old);
    }
}

void v_pydaw_set_preview_file(t_pydaw_data * self, const char * a_file)
{
    t_wav_pool_item * f_result = g_wav_pool_item_get(0, a_file,
            self->sample_rate);

    if(f_result)
    {
        if(i_wav_pool_item_load(f_result))
        {
            t_wav_pool_item * f_old = self->preview_wav_item;

            pthread_spin_lock(&self->main_lock);

            self->preview_wav_item = f_result;

            self->preview_audio_item->ratio =
                    self->preview_wav_item->ratio_orig;

            self->is_previewing = 1;

            v_ifh_retrigger(self->preview_audio_item->sample_read_head,
                    PYDAW_AUDIO_ITEM_PADDING_DIV2);
            v_adsr_retrigger(self->preview_audio_item->adsr);

            pthread_spin_unlock(&self->main_lock);

            if(f_old)
            {
                v_wav_pool_item_free(f_old);
            }
        }
        else
        {
            printf("i_wav_pool_item_load(f_result) failed in "
                    "v_pydaw_set_preview_file\n");
        }
    }
    else
    {
        self->is_previewing = 0;
        printf("g_wav_pool_item_get returned zero. could not load "
                "preview item.\n");
    }
}


void v_pydaw_set_plugin_index(t_pydaw_data * self, int a_track_num,
        int a_index, int a_plugin_index, int a_plugin_uid,
        int a_power, int a_lock)
{
    t_pytrack * f_track = self->track_pool_all[a_track_num];
    t_pydaw_plugin * f_plugin = 0;

    if(a_plugin_index)
    {
        f_plugin = self->plugin_pool[a_plugin_uid];

        if(!f_plugin)
        {
            f_plugin = g_pydaw_plugin_get((int)(self->sample_rate),
                    a_plugin_index, g_pydaw_wavpool_item_get,
                    a_plugin_uid, v_queue_osc_message);
            self->plugin_pool[a_plugin_uid] = f_plugin;
            f_plugin->descriptor->connect_buffer(
                f_plugin->PYFX_handle, 0, f_track->buffers[0]);
            f_plugin->descriptor->connect_buffer(
                f_plugin->PYFX_handle, 1, f_track->buffers[1]);

            char f_file_name[1024];
            sprintf(f_file_name, "%s%i", self->plugins_folder, a_plugin_uid);

            if(i_pydaw_file_exists(f_file_name))
            {
                printf("v_pydaw_open_plugin:  plugin exists %s , loading\n",
                    f_file_name);

                f_plugin->descriptor->load(f_plugin->PYFX_handle,
                    f_plugin->descriptor, f_file_name);
            }
        }
    }

    if(f_plugin)
    {
        f_plugin->power = a_power;
    }

    if(a_lock)
    {
        pthread_spin_lock(&self->main_lock);
    }

    f_track->plugins[a_index] = f_plugin;

    if(a_lock)
    {
        pthread_spin_unlock(&self->main_lock);
    }
}


void v_pydaw_update_track_send(t_pydaw_data * self, int a_lock)
{
    t_pydaw_routing_graph * f_graph = g_pydaw_routing_graph_get(self);
    t_pydaw_routing_graph * f_old = self->routing_graph;

    if(a_lock)
    {
        pthread_spin_lock(&self->main_lock);
    }

    self->routing_graph = f_graph;

    if(a_lock)
    {
        pthread_spin_unlock(&self->main_lock);
    }

    if(f_old)
    {
        v_pytrack_routing_graph_free(f_old);
    }
}

t_pytrack_routing * g_pytrack_routing_get()
{
    t_pytrack_routing * f_result;
    lmalloc((void**)&f_result, sizeof(t_pytrack_routing));
    return f_result;
}

void v_pytrack_routing_set(t_pytrack_routing * self, int a_output)
{
    self->output = a_output;

    if(a_output >= 0)
    {
        self->active = 1;
    }
    else
    {
        self->active = 0;
    }
}

void v_pytrack_routing_free(t_pytrack_routing * self)
{
    free(self);
}

void v_pytrack_routing_graph_free(t_pydaw_routing_graph * self)
{
    free(self);
}

t_pydaw_routing_graph * g_pydaw_routing_graph_get(t_pydaw_data * self)
{
    t_pydaw_routing_graph * f_result;
    lmalloc((void**)&f_result, sizeof(t_pydaw_routing_graph));

    int f_i = 0;
    while(f_i < PYDAW_TRACK_COUNT_ALL)
    {
        f_result->track_pool_sorted[f_i] = 0;

        int f_i2 = 0;
        while(f_i2 < MAX_ROUTING_COUNT)
        {
            f_result->routes[f_i][f_i2].active = 0;
            f_i2++;
        }

        f_i++;
    }

    f_result->track_pool_sorted_count = 0;

    char f_tmp[1024];
    sprintf(f_tmp, "%s/projects/edmnext/routing.txt", self->project_folder);

    if(i_pydaw_file_exists(f_tmp))
    {
        t_2d_char_array * f_2d_array = g_get_2d_array_from_file(
        f_tmp, PYDAW_LARGE_STRING);
        while(1)
        {
            char * f_identifier_str = c_iterate_2d_char_array(f_2d_array);
            if(f_2d_array->eof)
            {
                free(f_identifier_str);
                break;
            }

            if(f_identifier_str[0] == 't')
            {
                char * f_track_str = c_iterate_2d_char_array(f_2d_array);
                int f_track_num = atoi(f_track_str);
                free(f_track_str);

                char * f_index_str = c_iterate_2d_char_array(f_2d_array);
                int f_index = atoi(f_index_str);
                free(f_index_str);

                f_result->track_pool_sorted[f_index] = f_track_num;

            }
            else if(f_identifier_str[0] == 's')
            {
                char * f_track_str = c_iterate_2d_char_array(f_2d_array);
                int f_track_num = atoi(f_track_str);
                free(f_track_str);

                char * f_index_str = c_iterate_2d_char_array(f_2d_array);
                int f_index = atoi(f_index_str);
                free(f_index_str);

                char * f_output_str = c_iterate_2d_char_array(f_2d_array);
                int f_output = atoi(f_output_str);
                free(f_output_str);

                v_pytrack_routing_set(
                    &f_result->routes[f_track_num][f_index], f_output);
            }
            else if(f_identifier_str[0] == 'c')
            {
                char * f_count_str = c_iterate_2d_char_array(f_2d_array);
                int f_count = atoi(f_count_str);
                f_result->track_pool_sorted_count = f_count;
            }
            else
            {
                assert(0);
            }

            free(f_identifier_str);
        }
        g_free_2d_char_array(f_2d_array);
    }

    return f_result;
}

void v_pydaw_set_midi_device(
    t_pydaw_data *self, int a_on, int a_device, int a_output)
{
    /* Interim logic to get a minimum viable product working
     * TODO:  Make it modular and able to support multiple devices
     */
    t_pydaw_midi_routing_list * f_list = &self->midi_routing;
    t_pydaw_midi_routing * f_route = &f_list->routes[a_device];
    t_pytrack * f_track_old = self->track_pool_all[f_route->output_track];
    t_pytrack * f_track_new = self->track_pool_all[a_output];

    if(!f_route->on ||
        f_route->output_track != a_output)
    {
        f_track_old->extern_midi = 0;
        f_track_old->extern_midi_count = &ZERO;
        f_track_old->midi_device = 0;
    }

    f_route->on = a_on;
    f_route->output_track = a_output;

    if(f_route->on && self->midi_devices->devices[a_device].loaded)
    {
        f_track_new->midi_device = &self->midi_devices->devices[a_device];
        f_track_new->extern_midi =
            self->midi_devices->devices[a_device].instanceEventBuffers;
        f_track_new->extern_midi_count =
            &self->midi_devices->devices[a_device].instanceEventCounts;
    }
    else
    {
        f_track_new->extern_midi = 0;
        f_track_new->extern_midi_count = &ZERO;
        f_track_new->midi_device = 0;
    }
}

#endif	/* PYDAW_H */

