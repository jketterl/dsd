/*
 * Copyright (C) 2010 DSD Author
 * GPG Key ID: 0x3F1D7FD0 (74EF 430D F7F2 0A48 FCE6  F630 FAA2 635D 3F1D 7FD0)
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND ISC DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS.  IN NO EVENT SHALL ISC BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include "dsd_cleanupexit.h"
#include "dsd_nocarrier.h"
#include "dsd.h"

void cleanupAndExit(dsd_opts * opts, dsd_state * state)
{
    noCarrier(opts, state);
    if (opts->wav_out_f != NULL)
    {
        closeWavOutFile(opts, state);
    }

#ifdef USE_PORTAUDIO
    if((opts->audio_in_type == 2) || (opts->audio_out_type == 2))
    {
        fprintf(stderr, "Terminating portaudio.\n");
        PaError err = paNoError;
        if(opts->audio_in_pa_stream != NULL)
        {
            err = Pa_CloseStream( opts->audio_in_pa_stream );
            if( err != paNoError )
            {
                fprintf( stderr, "An error occured while closing the portaudio input stream\n" );
                fprintf( stderr, "Error number: %d\n", err );
                fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
            }
        }
        if(opts->audio_out_pa_stream != NULL)
        {
            err = Pa_IsStreamActive( opts->audio_out_pa_stream );
            if(err == 1)
            err = Pa_StopStream( opts->audio_out_pa_stream );
            if( err != paNoError )
            {
                fprintf( stderr, "An error occured while closing the portaudio output stream\n" );
                fprintf( stderr, "Error number: %d\n", err );
                fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
            }
            err = Pa_CloseStream( opts->audio_out_pa_stream );
            if( err != paNoError )
            {
                fprintf( stderr, "An error occured while closing the portaudio output stream\n" );
                fprintf( stderr, "Error number: %d\n", err );
                fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
            }
        }
        err = Pa_Terminate();
        if( err != paNoError )
        {
            fprintf( stderr, "An error occured while terminating portaudio\n" );
            fprintf( stderr, "Error number: %d\n", err );
            fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
        }
    }
#endif

    fprintf(stderr, "\n");
    fprintf(stderr, "Total audio errors: %i\n", state->debug_audio_errors);
    fprintf(stderr, "Total header errors: %i\n", state->debug_header_errors);
    fprintf(stderr, "Total irrecoverable header errors: %i\n",
            state->debug_header_critical_errors);

    //debug_print_heuristics(&(state->p25_heuristics));

    fprintf(stderr, "\n");
    fprintf(stderr, "+P25 BER estimate: %.2f%%\n",
            get_P25_BER_estimate(&state->p25_heuristics));
    fprintf(stderr, "-P25 BER estimate: %.2f%%\n",
            get_P25_BER_estimate(&state->inv_p25_heuristics));
    fprintf(stderr, "\n");

#ifdef TRACE_DSD
    if (state->debug_label_file != NULL)
    {
        fclose(state->debug_label_file);
        state->debug_label_file = NULL;
    }
    if(state->debug_label_dibit_file != NULL)
    {
        fclose(state->debug_label_dibit_file);
        state->debug_label_dibit_file = NULL;
    }
    if(state->debug_label_imbe_file != NULL)
    {
        fclose(state->debug_label_imbe_file);
        state->debug_label_imbe_file = NULL;
    }
#endif

    fprintf(stderr, "Exiting.\n");
    exit(0);
}


