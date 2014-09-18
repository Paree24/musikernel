/* -*- c-basic-offset: 4 -*-  vi:set ts=8 sts=4 sw=4: */
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


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>

#include <math.h>
#include <stdio.h>

#include "../../include/pydaw_plugin.h"

#include "libmodsynth.h"
#include "../../libmodsynth/lib/amp.h"
#include "../../libmodsynth/modules/filter/svf.h"

#include "../../libmodsynth/modules/delay/reverb.h"

#include "synth.h"

static void v_sfader_run(
    PYFX_Handle, int, t_pydaw_seq_event *, int, t_pydaw_seq_event *, int,
    t_pydaw_seq_event *, int);

PYFX_Descriptor *sfader_PYFX_descriptor(int index);


static void v_sfader_cleanup(PYFX_Handle instance)
{
    free(instance);
}

static void v_sfader_set_cc_map(PYFX_Handle instance, char * a_msg)
{
    t_sfader *plugin = (t_sfader *)instance;
    v_generic_cc_map_set(&plugin->cc_map, a_msg);
}

static void v_sfader_panic(PYFX_Handle instance)
{
    t_sfader *plugin = (t_sfader*)instance;
}

static void v_sfader_on_stop(PYFX_Handle instance)
{
    t_sfader *plugin = (t_sfader*)instance;
}

static void v_sfader_connect_buffer(PYFX_Handle instance, int a_index,
        float * DataLocation)
{
    t_sfader *plugin = (t_sfader*)instance;

    switch(a_index)
    {
        case 0:
            plugin->output0 = DataLocation;
            break;
        case 1:
            plugin->output1 = DataLocation;
            break;
        default:
            assert(0);
            break;
    }
}

static void v_sfader_connect_port(PYFX_Handle instance, int port,
        PYFX_Data * data)
{
    t_sfader *plugin;

    plugin = (t_sfader *) instance;

    switch (port)
    {
        case SFADER_VOL_SLIDER: plugin->vol_slider = data; break;
    }
}

static PYFX_Handle g_sfader_instantiate(PYFX_Descriptor * descriptor,
        int s_rate, fp_get_wavpool_item_from_host a_host_wavpool_func,
        int a_plugin_uid, fp_queue_message a_queue_func)
{
    t_sfader *plugin_data;
    lmalloc((void**)&plugin_data, sizeof(t_sfader));

    plugin_data->descriptor = descriptor;
    plugin_data->fs = s_rate;
    plugin_data->plugin_uid = a_plugin_uid;
    plugin_data->queue_func = a_queue_func;

    plugin_data->mono_modules =
            v_sfader_mono_init(plugin_data->fs, plugin_data->plugin_uid);

    plugin_data->port_table = g_pydaw_get_port_table(
        (void**)plugin_data, descriptor);

    v_cc_map_init(&plugin_data->cc_map);

    return (PYFX_Handle) plugin_data;
}

static void v_sfader_load(PYFX_Handle instance,
        PYFX_Descriptor * Descriptor, char * a_file_path)
{
    t_sfader *plugin_data = (t_sfader*)instance;
    pydaw_generic_file_loader(instance, Descriptor,
        a_file_path, plugin_data->port_table, &plugin_data->cc_map);
}

static void v_sfader_set_port_value(PYFX_Handle Instance,
        int a_port, float a_value)
{
    t_sfader *plugin_data = (t_sfader*)Instance;
    plugin_data->port_table[a_port] = a_value;
}

static void v_sfader_process_midi_event(
    t_sfader * plugin_data, t_pydaw_seq_event * a_event)
{

    if (a_event->type == PYDAW_EVENT_CONTROLLER)
    {
        assert(a_event->param >= 1 && a_event->param < 128);

        plugin_data->midi_event_types[plugin_data->midi_event_count] =
                PYDAW_EVENT_CONTROLLER;
        plugin_data->midi_event_ticks[plugin_data->midi_event_count] =
                a_event->tick;
        plugin_data->midi_event_ports[plugin_data->midi_event_count] =
                a_event->param;
        plugin_data->midi_event_values[plugin_data->midi_event_count] =
                a_event->value;

        plugin_data->midi_event_count++;
    }
}

static void v_sfader_run(
        PYFX_Handle instance, int sample_count,
        t_pydaw_seq_event *events, int event_count,
        t_pydaw_seq_event *atm_events, int atm_event_count,
        t_pydaw_seq_event *ext_events, int ext_event_count)
{
    t_sfader *plugin_data = (t_sfader*)instance;

    int event_pos = 0;
    int midi_event_pos = 0;
    plugin_data->midi_event_count = 0;

    while (event_pos < event_count)
    {
        v_sfader_process_midi_event(plugin_data, &events[event_pos]);
        event_pos++;
    }

    int f_i = 0;

    v_plugin_event_queue_reset(&plugin_data->atm_queue);

    while(f_i < atm_event_count)
    {
        v_plugin_event_queue_add(
            &plugin_data->atm_queue, atm_events[f_i].type,
            atm_events[f_i].tick, atm_events[f_i].value, atm_events[f_i].port);
        f_i++;
    }

    f_i = 0;

    while(f_i < ext_event_count)
    {
        v_sfader_process_midi_event(plugin_data, &ext_events[f_i]);
        f_i++;
    }

    f_i = 0;

    while((f_i) < sample_count)
    {
        while(midi_event_pos < plugin_data->midi_event_count &&
                plugin_data->midi_event_ticks[midi_event_pos] ==
                f_i)
        {
            if(plugin_data->midi_event_types[midi_event_pos] ==
                    PYDAW_EVENT_CONTROLLER)
            {
                v_cc_map_translate(
                    &plugin_data->cc_map, plugin_data->descriptor,
                    plugin_data->port_table,
                    plugin_data->midi_event_ports[midi_event_pos],
                    plugin_data->midi_event_values[midi_event_pos]);
            }
            midi_event_pos++;
        }

        v_plugin_event_queue_atm_set(
            &plugin_data->atm_queue, f_i,
            plugin_data->port_table);

        v_sml_run(plugin_data->mono_modules->volume_smoother,
            (*plugin_data->vol_slider));

        if(plugin_data->mono_modules->volume_smoother->last_value != 0.0f ||
        (*plugin_data->vol_slider != 0.0f))
        {

            plugin_data->mono_modules->vol_linear =
                f_db_to_linear_fast(
                (plugin_data->mono_modules->volume_smoother->last_value));

            plugin_data->output0[f_i] *=
                    (plugin_data->mono_modules->vol_linear);
            plugin_data->output1[f_i] *=
                    (plugin_data->mono_modules->vol_linear);


        }

        f_i++;
    }

}

PYFX_Descriptor *sfader_PYFX_descriptor(int index)
{
    PYFX_Descriptor *f_result =
            pydaw_get_pyfx_descriptor(123456, "Modulex", SFADER_COUNT);

    pydaw_set_pyfx_port(f_result, SFADER_VOL_SLIDER, 0.0f, -50.0f, 0.0f);

    f_result->cleanup = v_sfader_cleanup;
    f_result->connect_port = v_sfader_connect_port;
    f_result->connect_buffer = v_sfader_connect_buffer;
    f_result->instantiate = g_sfader_instantiate;
    f_result->panic = v_sfader_panic;
    f_result->load = v_sfader_load;
    f_result->set_port_value = v_sfader_set_port_value;
    f_result->set_cc_map = v_sfader_set_cc_map;

    f_result->PYINST_API_Version = 1;
    f_result->configure = NULL;
    f_result->run_synth = v_sfader_run;
    f_result->on_stop = v_sfader_on_stop;
    f_result->offline_render_prep = NULL;

    return f_result;
}




/*
void v_sfader_destructor()
{
    if (f_result) {
	free((PYFX_PortDescriptor *) f_result->PortDescriptors);
	free((char **) f_result->PortNames);
	free((PYFX_PortRangeHint *) f_result->PortRangeHints);
	free(f_result);
    }
    if (LMSDDescriptor) {
	free(LMSDDescriptor);
    }
}
*/