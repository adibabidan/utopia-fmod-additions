#include "fmod_studio.hpp"
#include <studio/fmod_event.h>
#include "fmod_server.h"

#include <callback/event_callbacks.h>

#include <variant/callable.hpp>
#include <variant/dictionary.hpp>

#include <stdio.h>
#include <stdlib.h>

namespace Callbacks {

    FMOD_RESULT F_CALL event_callback(FMOD_STUDIO_EVENT_CALLBACK_TYPE type, FMOD_STUDIO_EVENTINSTANCE* event, void* parameters) {
        auto* instance = reinterpret_cast<FMOD::Studio::EventInstance*>(event);
        godot::FmodEvent* event_instance;
        instance->getUserData((void**) &event_instance);
        if (event_instance) {
            godot::Dictionary dictionary;
            if (type == FMOD_STUDIO_EVENT_CALLBACK_CREATE_PROGRAMMER_SOUND) {
                const FMOD::Sound* sound {event_instance->get_programmer_callback_file()};

                auto* props { reinterpret_cast<FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES*>(parameters) };

                props->sound = (FMOD_SOUND*) sound;
                props->subsoundIndex = -1; // this is also hard coded and might need fixed if i ever do subsound stuff. i doubt it though

                return FMOD_OK;
            }
            if (type == FMOD_STUDIO_EVENT_CALLBACK_DESTROY_PROGRAMMER_SOUND) {
                auto* props { reinterpret_cast<FMOD_STUDIO_PROGRAMMER_SOUND_PROPERTIES*>(parameters) };
                auto* sound {(FMOD::Sound*) props->sound};

                ERROR_CHECK(sound->release());

                return FMOD_OK;
            }
            if (type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_MARKER) {
                auto* props { reinterpret_cast<FMOD_STUDIO_TIMELINE_MARKER_PROPERTIES*>(parameters) };
                dictionary["name"] = props->name;
                dictionary["position"] = props->position;
            }
            else if (type == FMOD_STUDIO_EVENT_CALLBACK_TIMELINE_BEAT) {
                auto* props { reinterpret_cast<FMOD_STUDIO_TIMELINE_BEAT_PROPERTIES*>(parameters) };
                dictionary["beat"] = props->beat;
                dictionary["bar"] = props->bar;
                dictionary["tempo"] = props->tempo;
                dictionary["time_signature_upper"] = props->timesignatureupper;
                dictionary["time_signature_lower"] = props->timesignaturelower;
                dictionary["position"] = props->position;
            }
            const godot::Callable& callback {event_instance->get_callback()};
            if (!callback.is_null() && callback.is_valid()) {
                godot::FmodServer::get_singleton()->add_callback(
                        {
                            type,
                            callback,
                            dictionary
                        }
                );
            }
        }

        return FMOD_OK;
    }

    FMOD_RESULT F_CALL pcm_read_callback(FMOD_SOUND* p_sound, void* data, unsigned int datalen) {
        FMOD::Sound* sound = (FMOD::Sound*) p_sound;
        godot::FmodFile* file;
        sound->getUserData((void**) &file);
        if(file){
            file->pop_from_buffer(data, datalen);
        }
        return FMOD_OK;
    }
}// namespace Callbacks
