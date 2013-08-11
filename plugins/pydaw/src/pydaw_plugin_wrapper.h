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

#ifndef PYDAW_PLUGIN_H
#define	PYDAW_PLUGIN_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../../include/pydaw3/pydaw_plugin.h"
#include "pydaw.h"
#include <lo/lo.h>
#include <alsa/asoundlib.h>
#include <dlfcn.h>
#include <math.h>
#include <stdlib.h>

#include "../../lms_modulex/src/synth.c"
#include "../../euphoria/src/synth.c"
#include "../../way_v/src/synth.c"
#include "../../ray_v/src/synth.c"
    
#define PYDAW_MAX_BUFFER_SIZE 4096
    
//#define PYDAW_PLUGIN_MEMCHECK

PYFX_Data g_pydaw_get_port_default(const PYFX_Descriptor *plugin, int port, int sample_rate)
{
    PYFX_PortRangeHint hint = plugin->PortRangeHints[port];    
    assert(hint.DefaultValue <= hint.UpperBound && hint.DefaultValue >= hint.LowerBound );
    return hint.DefaultValue;
}

typedef struct st_pydaw_plugin
{
    //void * lib_handle;
        
    PYINST_Descriptor_Function descfn;
    
    PYFX_Handle PYFX_handle;
    
    const PYINST_Descriptor *descriptor;
    int    ins;
    int    outs;
    int    controlIns;
    int    controlOuts;    
    int    firstControlIn;                       /* the offset to translate instance control in # to global control in # */
    int    *pluginPortControlInNumbers;           /* maps instance LADSPA port # to global control in # */    
    PYINST_Program_Descriptor *pluginPrograms;
        
    lo_address       uiTarget;
    lo_address       uiSource;
    int              ui_initial_show_sent;    
    char            *ui_osc_control_path;
    char            *ui_osc_configure_path;
    char            *ui_osc_program_path;
    char            *ui_osc_quit_path;
    char            *ui_osc_rate_path;
    char            *ui_osc_show_path;
        
    float **pluginInputBuffers, **pluginOutputBuffers;

    float *pluginControlIns, *pluginControlOuts;
    int *pluginControlInPortNumbers;
    
    int * pluginPortUpdated;
    char euphoria_load[16384];
    int euphoria_load_set;
    //char euphoria_last_dir[512];
    //int euphoria_last_dir_set;    
    
    int showing_ui;  //Used to prevent a race condition where the UI can be shown twice
}t_pydaw_plugin;

#ifdef PYDAW_PLUGIN_MEMCHECK    
void v_pydaw_plugin_memcheck(t_pydaw_plugin * a_plugin);
#endif

void v_pydaw_set_control_from_cc(t_pydaw_plugin *instance, int controlIn, snd_seq_event_t *event, int a_ci_is_port)
{
    int port;
    if(a_ci_is_port)
    {
        port = controlIn;
    }
    else
    {
        port = instance->pluginControlInPortNumbers[controlIn];
    }
    
    //const PYFX_Descriptor *p = instance->descriptor->PYFX_Plugin;
    
    /*
    PYFX_PortRangeHintDescriptor d = p->PortRangeHints[port].HintDescriptor;

    PYFX_Data lb = p->PortRangeHints[port].LowerBound ;
    
    PYFX_Data ub = p->PortRangeHints[port].UpperBound ;
    */
    float value = (float)event->data.control.value;
    /*
    if (!PYFX_IS_HINT_BOUNDED_BELOW(d)) 
    {
	if (!PYFX_IS_HINT_BOUNDED_ABOVE(d)) {
            return;
	} else {
	    value = ub - 127.0f + value;
	}
    } 
    else 
    {
	if (!PYFX_IS_HINT_BOUNDED_ABOVE(d)) {
	    value = lb + value;
	} else {
            if (PYFX_IS_HINT_LOGARITHMIC(d) && lb > 0.0f && ub > 0.0f) {
		const float llb = logf(lb);
		const float lub = logf(ub);

		value = expf(llb + ((lub - llb) * value / 127.0f));
	    } else {
		value = lb + ((ub - lb) * value / 127.0f);
	    }
	}
    }
    if (PYFX_IS_HINT_INTEGER(d)) 
    {
        value = lrintf(value);
    }
    */

    instance->pluginControlIns[controlIn] = value;
    instance->pluginPortUpdated[controlIn] = 1;
    
#ifdef PYDAW_PLUGIN_MEMCHECK
    v_pydaw_plugin_memcheck(instance);
#endif
}


int v_pydaw_plugin_configure_handler(t_pydaw_plugin *instance, const char *key, const char *value)
{    
    char * message = 0;
        
    if(!strcmp(key, "load"))
    {
        instance->euphoria_load_set = 1;
        strcpy(instance->euphoria_load, value);
    }
    /*else if(!strcmp(key, "lastdir"))
    {
        instance->euphoria_last_dir_set = 1;
        strcpy(instance->euphoria_last_dir, value);
    }*/
    
    if (instance->descriptor->configure) 
    {
        message = instance->descriptor->configure(instance->PYFX_handle, key, value);
        if (message) 
        {
            printf("PyDAW: on configure '%s' '%s', plugin returned error '%s'\n",key, value, message);
            free(message);
        }    
    }
    
    return 0;
}


t_pydaw_plugin * g_pydaw_plugin_get(int a_sample_rate, int a_index)
{
    t_pydaw_plugin * f_result = (t_pydaw_plugin*)malloc(sizeof(t_pydaw_plugin));  //TODO: posix_memalign instead...
    
    switch(a_index)
    {
        case -1:
            f_result->descfn = (PYINST_Descriptor_Function)modulex_PYINST_descriptor;
            //f_result->lib_handle = dlopen("/usr/lib/dssi/lms_modulex.so", RTLD_NOW | RTLD_LOCAL);                  
            break;
        case 1:
            f_result->descfn = (PYINST_Descriptor_Function)euphoria_PYINST_descriptor;
            //f_result->lib_handle = dlopen("/usr/lib/dssi/euphoria.so", RTLD_NOW | RTLD_LOCAL);                  
            break;
        case 2:
            f_result->descfn = (PYINST_Descriptor_Function)rayv_PYINST_descriptor;
            //f_result->lib_handle = dlopen("/usr/lib/dssi/ray_v.so", RTLD_NOW | RTLD_LOCAL);                  
            break;
        case 3:
            f_result->descfn = (PYINST_Descriptor_Function)wayv_PYINST_descriptor;
            //f_result->lib_handle = dlopen("/usr/lib/dssi/way_v.so", RTLD_NOW | RTLD_LOCAL);                  
            break;
    }
    
    //f_result->euphoria_last_dir_set = 0;
    f_result->euphoria_load_set = 0;
        
    f_result->pluginPrograms = NULL;    
    f_result->uiTarget = NULL;
    f_result->uiSource = NULL;
    f_result->ui_initial_show_sent = 0;
    f_result->ui_osc_control_path = NULL;
    f_result->ui_osc_configure_path = NULL;
    f_result->ui_osc_program_path = NULL;
    f_result->ui_osc_quit_path = NULL;
    f_result->ui_osc_rate_path = NULL;
    f_result->ui_osc_show_path = NULL;
    
    f_result->showing_ui = 0;
        
    //f_result->descfn = (PYINST_Descriptor_Function)dlsym(f_result->lib_handle, "PYINST_descriptor");
    
    f_result->descriptor = f_result->descfn(0);
    
    f_result->ins = 0;
    f_result->outs = 0;
    f_result->controlIns = 0;
    f_result->controlOuts = 0;

    int j;
    
    for (j = 0; j < f_result->descriptor->PYFX_Plugin->PortCount; j++) 
    {
        PYFX_PortDescriptor pod = f_result->descriptor->PYFX_Plugin->PortDescriptors[j];

        if (PYFX_IS_PORT_AUDIO(pod)) 
        {
            if (PYFX_IS_PORT_INPUT(pod)) ++f_result->ins;
            else if (PYFX_IS_PORT_OUTPUT(pod)) ++f_result->outs;
        } 
        else if (PYFX_IS_PORT_CONTROL(pod)) 
        {
            if (PYFX_IS_PORT_INPUT(pod)) 
            {
                ++f_result->controlIns;
            }
            else if (PYFX_IS_PORT_OUTPUT(pod)) 
            {
                ++f_result->controlOuts;
            }
        }
    }
   
    /* finish up new plugin */
    f_result->pluginPortControlInNumbers =
            (int*)malloc(f_result->descriptor->PYFX_Plugin->PortCount *
                          sizeof(int));

    /*printf("f_result->in %i\n", f_result->ins);
    printf("f_result->outs %i\n", f_result->outs);
    printf("f_result->controlIns %i\n", f_result->controlIns);
    printf("f_result->controlOuts %i\n", f_result->controlOuts);*/
    //f_result->inputPorts = (jack_port_t **)malloc(f_result->insTotal * sizeof(jack_port_t *));
    f_result->pluginInputBuffers = (float**)malloc((f_result->ins) * sizeof(float*));
    f_result->pluginControlIns = (float*)calloc(f_result->controlIns, sizeof(float));
    //f_result->pluginControlInInstances = (d3h_instance_t **)malloc(f_result->controlInsTotal * sizeof(d3h_instance_t *));
    f_result->pluginControlInPortNumbers = (int*)malloc(f_result->controlIns * sizeof(int));
    f_result->pluginPortUpdated = (int*)malloc(f_result->controlIns * sizeof(int));
    //f_result->outputPorts = (jack_port_t **)malloc(f_result->outsTotal * sizeof(jack_port_t *));
    f_result->pluginOutputBuffers = (float**)malloc((f_result->outs) * sizeof(float*));
    f_result->pluginControlOuts = (float *)calloc(f_result->controlOuts, sizeof(float));
    
    //TODO:  Count ins and outs from the loop at line 1142.  Or just rely on that we already know it
    
    f_result->PYFX_handle = f_result->descriptor->PYFX_Plugin->instantiate(f_result->descriptor->PYFX_Plugin, a_sample_rate);
        
    f_result->firstControlIn = 0;
    
    int in, out, controlIn, controlOut;
    
    in = out = controlIn = controlOut = 0;
    
    for (j = 0; j < f_result->descriptor->PYFX_Plugin->PortCount; j++) 
    {
        PYFX_PortDescriptor pod =
            f_result->descriptor->PYFX_Plugin->PortDescriptors[j];

        f_result->pluginPortControlInNumbers[j] = -1;

        if (PYFX_IS_PORT_AUDIO(pod)) {

            if (PYFX_IS_PORT_INPUT(pod)) 
            {
                if(posix_memalign((void**)(&f_result->pluginInputBuffers[in]), 16, (sizeof(float) * 8192)) != 0)
                {
                    return 0;
                }
                
                int f_i = 0;
                while(f_i < 8192)
                {
                    f_result->pluginInputBuffers[in][f_i] = 0.0f;
                    f_i++;
                }

                f_result->descriptor->PYFX_Plugin->connect_port(f_result->PYFX_handle, j, f_result->pluginInputBuffers[in]);                                
                in++;
            } 
            else if (PYFX_IS_PORT_OUTPUT(pod)) 
            {
                if(posix_memalign((void**)(&f_result->pluginOutputBuffers[out]), 16, (sizeof(float) * 8192)) != 0)
                {
                    return 0;
                }                
                
                int f_i = 0;
                while(f_i < 8192)
                {
                    f_result->pluginOutputBuffers[out][f_i] = 0.0f;
                    f_i++;
                }

                f_result->descriptor->PYFX_Plugin->connect_port(f_result->PYFX_handle, j, f_result->pluginOutputBuffers[out]);                
                out++;
            }

        } 
        else if (PYFX_IS_PORT_CONTROL(pod)) 
        {
            if (PYFX_IS_PORT_INPUT(pod)) {                
                //f_result->pluginControlInInstances[controlIn] = instance;                 
                f_result->pluginControlInPortNumbers[controlIn] = j;
                f_result->pluginPortControlInNumbers[j] = controlIn;

                f_result->pluginControlIns[controlIn] = g_pydaw_get_port_default
                    (f_result->descriptor->PYFX_Plugin, j, a_sample_rate);

                f_result->descriptor->PYFX_Plugin->connect_port
                    (f_result->PYFX_handle, j, &f_result->pluginControlIns[controlIn++]);

            } else if (PYFX_IS_PORT_OUTPUT(pod)) {
                f_result->descriptor->PYFX_Plugin->connect_port
                    (f_result->PYFX_handle, j, &f_result->pluginControlOuts[controlOut++]);
            }
        }
    }
    int f_i;
    for (f_i = 0; f_i < f_result->controlIns; f_i++) 
    {
        f_result->pluginPortUpdated[f_i] = 0;
    }
    
    f_result->descriptor->PYFX_Plugin->activate(f_result->PYFX_handle);
    
#ifdef PYDAW_PLUGIN_MEMCHECK
    v_pydaw_plugin_memcheck(f_result);
#endif
    
    return f_result;
}

void v_free_pydaw_plugin(t_pydaw_plugin * a_plugin)
{    
    if(a_plugin)
    {
        if (a_plugin->uiTarget) {
        lo_send(a_plugin->uiTarget, a_plugin->ui_osc_quit_path, "");
        lo_address_free(a_plugin->uiTarget);
        a_plugin->uiTarget = NULL;
        }

        if (a_plugin->uiSource) {
            lo_address_free(a_plugin->uiSource);
            a_plugin->uiSource = NULL;
        }

        if (a_plugin->descriptor->PYFX_Plugin->deactivate) 
        {
            a_plugin->descriptor->PYFX_Plugin->deactivate(a_plugin->PYFX_handle);
        }

        if (a_plugin->descriptor->PYFX_Plugin->cleanup) 
        {
            a_plugin->descriptor->PYFX_Plugin->cleanup(a_plugin->PYFX_handle);
        }
        
#ifdef PYDAW_PLUGIN_MEMCHECK
    v_pydaw_plugin_memcheck(a_plugin);
#endif

        free(a_plugin);
    }
    else
    {
        printf("Error, attempted to free NULL plugin with v_free_pydaw_plugin()\n");
    }
}

void v_run_plugin(t_pydaw_plugin * a_plugin, int a_sample_count, snd_seq_event_t * a_event_buffer, 
        int a_event_count)
{
    a_plugin->descriptor->run_synth(a_plugin->PYFX_handle, a_sample_count, a_event_buffer, a_event_count);

#ifdef PYDAW_PLUGIN_MEMCHECK
    v_pydaw_plugin_memcheck(a_plugin);
#endif
}

#ifdef PYDAW_PLUGIN_MEMCHECK
/*Check for known symptoms of memory corruption*/
void v_pydaw_plugin_memcheck(t_pydaw_plugin * a_plugin)
{
    if(a_plugin)
    {
        assert((a_plugin->euphoria_load_set == 0) || (a_plugin->euphoria_load_set == 1));
    }
}
#endif
#ifdef	__cplusplus
}
#endif

#endif	/* PYDAW_PLUGIN_H */

