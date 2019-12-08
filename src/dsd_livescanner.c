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

#include <string.h>
#include "dsd.h"
#include "dsd_nocarrier.h"

void liveScanner(dsd_opts * opts, dsd_state * state)
{
#ifdef USE_PORTAUDIO
    if(opts->audio_in_type == 2)
    {
        PaError err = Pa_StartStream( opts->audio_in_pa_stream );
        if( err != paNoError )
        {
            fprintf( stderr, "An error occured while starting the portaudio input stream\n" );
            fprintf( stderr, "Error number: %d\n", err );
            fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
            return;
        }
    }
#endif
    while (1)
    {
        noCarrier(opts, state);
        state->synctype = getFrameSync(opts, state);
        // recalibrate center/umid/lmid
        state->center = ((state->max) + (state->min)) / 2;
        state->umid = (((state->max) - state->center) * 5 / 8) + state->center;
        state->lmid = (((state->min) - state->center) * 5 / 8) + state->center;
        while (state->synctype != -1)
        {
            processFrame(opts, state);

#ifdef TRACE_DSD
            state->debug_prefix = 'S';
#endif

            state->synctype = getFrameSync(opts, state);

#ifdef TRACE_DSD
            state->debug_prefix = '\0';
#endif

            // recalibrate center/umid/lmid
            state->center = ((state->max) + (state->min)) / 2;
            state->umid = (((state->max) - state->center) * 5 / 8)
                    + state->center;
            state->lmid = (((state->min) - state->center) * 5 / 8)
                    + state->center;
        }
    }
}




